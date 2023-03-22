#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if (argc == 2) //Checks if the initial input given consists in running the code and the key.
    {
        for (int j = 0, m = strlen(argv[1]); j < m; j++) //Checks for every charater in the given key.
        {
            if (isdigit(argv[1][j])) //Confirms that every character in the given key is a digit.
            {
            }
            else //Otherwise, an error is given.
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
    }
    else //If there is no key or more than a single key, an error is given.
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    string plaintext = get_string("plaintext: "); //Asks the user for a string to encrypt.
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++) //Checks every character in the input given.
    {
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z') //For lowercase characters.
        {
            int lowerx = plaintext[i] - 97; //Turns every letter into a number from 0 - 25.
            int bruh1 = ((lowerx + atoi(argv[1])) % 26); //The formula is used to calculate the new value of the character.
            printf("%c", bruh1 + 97); //Turns this value into ASCII code and prints it as a 'char'.
        }
        else if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') //For uppercase letters.
        {
            int upperx = plaintext[i] - 65; //Turns every letter into a number from 0 - 25.
            int bruh2 = ((upperx + atoi(argv[1])) % 26); //The formul is used to calculate the new value of the character.
            printf("%c", bruh2 + 65); //Turns this value into ASCII code and prints it as a 'char'.
        }
        else //For the rest of the characters, such as '.', ',', ' ', etc.
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}