#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: "); //Query
    }
    while (height > 8 || height < 1); //Condition (The process repeats itself and 'do' asks for a different answer if the int. given is higher than 8 or lower than 1.)

    for (int i = 0; i < height; i++) //"i" (number of rows ADDED) will increment until it MATCHES the quantity given by the user - 1 (height - 1).
    {
        for (int j = height - 1; j >= i; j--)
        {
            printf(" "); //SPACES PER ROW
        }
        for (int j = 0; j <= i; j++) //"j" (number of hashes) is never null and can only ever match 1-8 (<= i); it increments until so.
        {
            printf("#"); //HASHES PER ROW
        }
        printf("\n"); //ROWS ADDED
    }
}