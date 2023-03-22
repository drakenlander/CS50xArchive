#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
int main(int argc, string argv[])
{
    if (argc == 2)
    {
        for (int j = 0, m = strlen(argv[1]); j < m; j++)
        {
            if (isdigit(argv[1][j]))
            {
            }
            else
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            int lowerx = plaintext[i] - 97;
            int bruh1 = ((lowerx + atoi(argv[1])) % 26);
            printf("%c", bruh1 + 97);
        }
        else if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            int upperx = plaintext[i] - 65;
            int bruh2 = ((upperx + atoi(argv[1])) % 26);
            printf("%c", bruh2 + 65);
        }
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}