import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # Store the username of the user.
    username = db.execute("SELECT username FROM users WHERE id = :uid", uid=int(session['user_id']))[0]["username"]

    # List info. from 'transactions'.
    stocks = db.execute(
        "SELECT symbol, shares, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0", user_id=session["user_id"])

    # TOTAL
    total_sum = []

    # Iterate over the stocks list to append the faulty information needed.
    for stock in stocks:
        symbol = str(stock["symbol"])
        shares = int(stock["total_shares"])
        price = lookup(symbol)["price"]
        total = shares * price
        stock["price"] = usd(price)
        stock["total"] = usd(total)
        total_sum.append(float(total))

    cash_available = db.execute("SELECT cash FROM users WHERE username = :username", username=username)[0]["cash"]
    cash_total = sum(total_sum) + cash_available

    return render_template("index.html", stocks=stocks, cash_available=usd(cash_available), cash_total=usd(cash_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")

    else:
        symbolquery = request.form.get("symbol")
        symbolresult = lookup(symbolquery)
        sharesquery = int(request.form.get("shares"))

        if not symbolquery:  # If no shares.
            return apology("must provide symbol", 400)

        elif symbolresult == None:  # If symbol exists in db.
            return apology("invalid symbol", 400)

        elif sharesquery <= 0:
            return apology("must provide positive int", 400)

        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])     # Check user's cash.

        cash_remaining = rows[0]["cash"]
        pps = symbolresult["price"]
        total_price = pps * sharesquery

        if total_price > cash_remaining:     # If not enough cash.
            return apology("can't afford", 400)

        db.execute("UPDATE users SET cash = cash - :price WHERE id = :user_id", price=total_price, user_id=session["user_id"])
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)", user_id=session["user_id"],
                   symbol=symbolquery, shares=sharesquery, price=pps, time=datetime.now())   # Records every transaction.

        flash("Bought!")

        return redirect("/")


@app.route("/history")
@login_required
def history():
    transactions = db.execute(
        "SELECT symbol, shares, price, time FROM transactions WHERE user_id = :user_id ORDER BY time ASC", user_id=session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")    # Prompts for input.

    else:
        query = request.form.get("symbol")  # The question is the input.
        result = lookup(query)

        if result == None:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", result=result)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    else:
        if not request.form.get("username"):    # If there's no username.
            return apology("must provide username", 400)

        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))    # Find the input in the db, store it in 'rows'.

        if len(rows) != 0:  # If it exists...
            return apology("username already exists", 400)

        elif not request.form.get("password"):  # No password.
            return apology("must provide password and/or confirmation", 400)

        elif not request.form.get("confirmation"):  # No confirmation.
            return apology("must provide password and/or confirmation", 400)

        elif request.form.get("password") != request.form.get("confirmation"):  # PW and CFRM are not the same.
            return apology("passwords do not match", 400)

        user_hash = generate_password_hash(request.form.get("password"))

        # Make new row for new username.
        new_user_id = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), user_hash)

        session["user_id"] = new_user_id    # Remember LOGIN.

        flash("Registered!")

        return redirect("/")


@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    if request.method == "GET":
        return render_template("change.html")

    else:
        if not request.form.get("password") or not request.form.get("newpassword") or not request.form.get("confirmation"):
            return apology("must provide input required", 400)

        users = db.execute("SELECT username, hash FROM users WHERE id=:uid", uid=session["user_id"])

        oldhash = generate_password_hash(request.form.get("password"))
        newhash = generate_password_hash(request.form.get("newpassword"))

        if len(users) != 1 or not check_password_hash(users[0]["hash"], request.form.get("password")):
            return apology("invalid password", 400)

        if request.form.get("newpassword") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        users = db.execute("UPDATE users SET hash = :newhash WHERE id = :uid", newhash=newhash, uid=session["user_id"])

        flash("Changed!")

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        stocks = db.execute("SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0",
                            user_id=session["user_id"])

        return render_template("sell.html", stocks=stocks)

    else:
        quote = lookup(request.form.get("symbol"))

        if quote == None:
            return apology("invalid symbol", 400)

        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be positive int", 403)

        if shares <= 0:
            return apology("can't sell less than 0", 403)

        stock = db.execute("SELECT SUM(shares) AS total_shares FROM transactions WHERE user_id = :user_id AND symbol = :symbol GROUP BY symbol",
                           user_id=session["user_id"], symbol=request.form.get("symbol"))

        if len(stock) != 1 or stock[0]["total_shares"] <= 0 or stock[0]["total_shares"] < shares:
            return apology("you can't sell less than 0 or more than you own", 400)

        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

        cash_remaining = rows[0]["cash"]
        pps = quote["price"]
        total_price = pps * shares

        db.execute("UPDATE users SET cash = cash + :price WHERE id = :user_id", price=total_price, user_id=session["user_id"])
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)",
                   user_id=session["user_id"], symbol=request.form.get("symbol"), shares=-shares, price=pps, time=datetime.now())

        flash("Sold!")

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
