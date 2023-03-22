#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
int main(void)
{
    float letters = 0; //Quantity of Letters in the text.
    float words = 1; //Quantity of Words in the text.
    float sentences = 0; //Quantity of Sentences in the text.
    string txt = get_string("Text: "); //Input.
    for (int i = 0, n = strlen(txt); i < n; i++) //The code below will repeat itself until the variable 'i' matches 'n'.
    {
        if ((txt[i] >= 'a' && txt[i] <= 'z') || (txt[i] >= 'A' && txt[i] <= 'Z')) //If the character 'i' is between A and Z.
        {
            letters++; //'letters' will increment by 1.
        }
        if (txt[i] == ' ') //If the character 'i' is a space (' ')...
        {
            words++; //'words' will increment by 1.
        }
        if (((txt[i] == '.') || (txt[i] == '!')) || (txt[i] == '?')) //If the character 'i' equals either a '.' or a '!' or a '?'...
        {
            sentences++; //'sentences' will increment by 1.
        }
    }
    float decimal_grade = (0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8);
    //'decimal_grade' stores the result as a FLOAT.
    int grade = round(decimal_grade); //'grade' equals 'decimal_grade', rounded and as an integer.

    if (grade >= 1 && grade <= 16) //If the result ('grade') is between 1 and 16...
    {
        printf("Grade %i\n", grade); //The output will be 'Grade 'x''.
    }
    if (grade < 1) //If the result ('grade') is lower than 1...
    {
        printf("Before Grade 1\n"); //The output will be 'Before Grade 1'.
    }
    if (grade > 16) //If the result ('grade') is higher than 16...
    {
        printf("Grade 16+\n"); //The output will be 'Grade 16+'.
    }
}