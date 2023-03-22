#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    int cents_given;
    do
    {
        float money_given = get_float("Change Owed: ");     // Amount of money owed.
        cents_given = round(money_given * 100);             // Converts money given.
    }
    while (cents_given <= 0);

    int first = cents_given / 25;
    int second = (cents_given % 25) / 10;
    int third = ((cents_given % 25) % 10) / 5;
    int fourth = ((cents_given % 25) % 10) % 5;

    printf("%d\n", first + second + third + fourth);
}