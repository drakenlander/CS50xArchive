#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)    // Comments for style50.
    {
        for (int j = 0; j < width; j++) // More comments.
        {
            float avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.000);    // And yet.
            image[i][j].rgbtRed = avg;      // A bunch more.
            image[i][j].rgbtGreen = avg;    // How many does it need?
            image[i][j].rgbtBlue = avg;     // Many more.
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sepiared = ((0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue));
            int sepiared_final = round(sepiared);
            if (sepiared_final > 255)
            {
                sepiared_final = 255;
            }
            float sepiagreen = ((0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) + (0.168 * image[i][j].rgbtBlue));
            int sepiagreen_final = round(sepiagreen);
            if (sepiagreen_final > 255)
            {
                sepiagreen_final = 255;
            }
            float sepiablue = ((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue));
            int sepiablue_final = round(sepiablue);
            if (sepiablue_final > 255)
            {
                sepiablue_final = 255;
            }
            image[i][j].rgbtRed = sepiared_final;
            image[i][j].rgbtGreen = sepiagreen_final;
            image[i][j].rgbtBlue = sepiablue_final;
        }
    }
    return;     // Comments.
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int temp[3];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp[0] = image[i][j].rgbtRed;
            temp[1] = image[i][j].rgbtGreen;
            temp[2] = image[i][j].rgbtBlue;
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1].rgbtRed = temp[0];
            image[i][width - j - 1].rgbtGreen = temp[1];
            image[i][width - j - 1].rgbtBlue = temp[2];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image0[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image0[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0) //topleft
            {
                int red = round((float)(image0[0][0].rgbtRed + image0[0][1].rgbtRed + image0[1][1].rgbtRed + image0[1][0].rgbtRed) / 4.0);
                int green = round((float)(image0[0][0].rgbtGreen + image0[0][1].rgbtGreen + image0[1][1].rgbtGreen + image0[1][0].rgbtGreen) / 4.0);
                int blue = round((float)(image0[0][0].rgbtBlue + image0[0][1].rgbtBlue + image0[1][1].rgbtBlue + image0[1][0].rgbtBlue) / 4.0);
                image[0][0].rgbtRed = red;
                image[0][0].rgbtGreen = green;
                image[0][0].rgbtBlue = blue;
            }
            else if (i == 0 && j == width - 1) //topright
            {
                int red = round((float)(image0[0][width - 1].rgbtRed + image0[0][width - 2].rgbtRed + image0[1][width - 2].rgbtRed + image0[1][width
                                        - 1].rgbtRed)
                                / 4.0);
                int green = round((float)(image0[0][width - 1].rgbtGreen + image0[0][width - 2].rgbtGreen + image0[1][width - 2].rgbtGreen +
                                          image0[1][width - 1].rgbtGreen)
                                  / 4.0);
                int blue = round((float)(image0[0][width - 1].rgbtBlue + image0[0][width - 2].rgbtBlue + image0[1][width - 2].rgbtBlue +
                                         image0[1][width - 1].rgbtBlue)
                                 / 4.0);
                image[0][width - 1].rgbtRed = red;
                image[0][width - 1].rgbtGreen = green;
                image[0][width - 1].rgbtBlue = blue;
            }
            else if (i == height - 1 && j == 0) //bottomleft
            {
                int red = round((float)(image0[height - 1][0].rgbtRed + image0[height - 2][0].rgbtRed + image0[height - 2][1].rgbtRed +
                                        image0[height - 1][1].rgbtRed)
                                / 4.0);
                int green = round((float)(image0[height - 1][0].rgbtGreen + image0[height - 2][0].rgbtGreen + image0[height - 2][1].rgbtGreen
                                          + image0[height - 1][1].rgbtGreen)
                                  / 4.0);
                int blue = round((float)(image0[height - 1][0].rgbtBlue + image0[height - 2][0].rgbtBlue + image0[height - 2][1].rgbtBlue +
                                         image0[height - 1][1].rgbtBlue)
                                 / 4.0);
                image[height - 1][0].rgbtRed = red;
                image[height - 1][0].rgbtGreen = green;
                image[height - 1][0].rgbtBlue = blue;
            }
            else if (i == height - 1 && j == width - 1) //bottomright
            {
                int red = round((float)(image0[height - 1][width - 1].rgbtRed + image0[height - 2][width - 1].rgbtRed + image0[height - 2][width -
                                        2].rgbtRed
                                        + image0[height - 1][width - 2].rgbtRed) / 4.0);
                int green = round((float)(image0[height - 1][width - 1].rgbtGreen + image0[height - 2][width - 1].rgbtGreen + image0[height -
                                          2][width - 2].rgbtGreen
                                          + image0[height - 1][width - 2].rgbtGreen) / 4.0);
                int blue = round((float)(image0[height - 1][width - 1].rgbtBlue + image0[height - 2][width - 1].rgbtBlue + image0[height - 2][width
                                         - 2].rgbtBlue
                                         + image0[height - 1][width - 2].rgbtBlue) / 4.0);
                image[height - 1][width - 1].rgbtRed = red;
                image[height - 1][width - 1].rgbtGreen = green;
                image[height - 1][width - 1].rgbtBlue = blue;
            }
            else if (i == 0 && j != 0 && j != width - 1) //top
            {
                int red = round((float)(image0[i][j].rgbtRed + image0[i][j - 1].rgbtRed + image0[i + 1][j - 1].rgbtRed
                                        + image0[i + 1][j].rgbtRed + image0[i + 1][j + 1].rgbtRed + image0[i][j + 1].rgbtRed) / 6.0);
                int green = round((float)(image0[i][j].rgbtGreen + image0[i][j - 1].rgbtGreen + image0[i + 1][j - 1].rgbtGreen
                                          + image0[i + 1][j].rgbtGreen + image0[i + 1][j + 1].rgbtGreen + image0[i][j + 1].rgbtGreen) / 6.0);
                int blue = round((float)(image0[i][j].rgbtBlue + image0[i][j - 1].rgbtBlue + image0[i + 1][j - 1].rgbtBlue
                                         + image0[i + 1][j].rgbtBlue + image0[i + 1][j + 1].rgbtBlue + image0[i][j + 1].rgbtBlue) / 6.0);
                image[i][j].rgbtRed = red;
                image[i][j].rgbtGreen = green;
                image[i][j].rgbtBlue = blue;
            }
            else if (i != 0 && i != height - 1 && j == 0) //left
            {
                int red = round((float)(image0[i][j].rgbtRed + image0[i - 1][j].rgbtRed + image0[i - 1][j + 1].rgbtRed
                                        + image0[i][j + 1].rgbtRed + image0[i + 1][j + 1].rgbtRed + image0[i + 1][j].rgbtRed) / 6.0);
                int green = round((float)(image0[i][j].rgbtGreen + image0[i - 1][j].rgbtGreen + image0[i - 1][j + 1].rgbtGreen
                                          + image0[i][j + 1].rgbtGreen + image0[i + 1][j + 1].rgbtGreen + image0[i + 1][j].rgbtGreen) / 6.0);
                int blue = round((float)(image0[i][j].rgbtBlue + image0[i - 1][j].rgbtBlue + image0[i - 1][j + 1].rgbtBlue
                                         + image0[i][j + 1].rgbtBlue + image0[i + 1][j + 1].rgbtBlue + image0[i + 1][j].rgbtBlue) / 6.0);
                image[i][j].rgbtRed = red;
                image[i][j].rgbtGreen = green;
                image[i][j].rgbtBlue = blue;
            }
            else if (i != 0 && i != height - 1 && j == width - 1) //right
            {
                int red = round((float)(image0[i][j].rgbtRed + image0[i - 1][j].rgbtRed + image0[i - 1][j - 1].rgbtRed + image0[i][j - 1].rgbtRed
                                        + image0[i + 1][j - 1].rgbtRed + image0[i + 1][j].rgbtRed) / 6.0);
                int green = round((float)(image0[i][j].rgbtGreen + image0[i - 1][j].rgbtGreen + image0[i - 1][j - 1].rgbtGreen + image0[i][j -
                                          1].rgbtGreen
                                          + image0[i + 1][j - 1].rgbtGreen + image0[i + 1][j].rgbtGreen) / 6.0);
                int blue = round((float)(image0[i][j].rgbtBlue + image0[i - 1][j].rgbtBlue + image0[i - 1][j - 1].rgbtBlue + image0[i][j -
                                         1].rgbtBlue
                                         + image0[i + 1][j - 1].rgbtBlue + image0[i + 1][j].rgbtBlue) / 6.0);
                image[i][j].rgbtRed = red;
                image[i][j].rgbtGreen = green;
                image[i][j].rgbtBlue = blue;
            }
            else if (i == height - 1 && j != 0 && j != width - 1) //bottom
            {
                int red = round((float)(image0[i][j].rgbtRed + image0[i][j - 1].rgbtRed + image0[i - 1][j - 1].rgbtRed + image0[i - 1][j].rgbtRed
                                        + image0[i - 1][j + 1].rgbtRed + image0[i][j + 1].rgbtRed) / 6.0);
                int green = round((float)(image0[i][j].rgbtGreen + image0[i][j - 1].rgbtGreen + image0[i - 1][j - 1].rgbtGreen + image0[i -
                                          1][j].rgbtGreen
                                          + image0[i - 1][j + 1].rgbtGreen + image0[i][j + 1].rgbtGreen) / 6.0);
                int blue = round((float)(image0[i][j].rgbtBlue + image0[i][j - 1].rgbtBlue + image0[i - 1][j - 1].rgbtBlue + image0[i -
                                         1][j].rgbtBlue
                                         + image0[i - 1][j + 1].rgbtBlue + image0[i][j + 1].rgbtBlue) / 6.0);
                image[i][j].rgbtRed = red;
                image[i][j].rgbtGreen = green;
                image[i][j].rgbtBlue = blue;
            }
            else
            {
                int red = round((float)(image0[i][j].rgbtRed + image0[i - 1][j - 1].rgbtRed + image0[i - 1][j].rgbtRed + image0[i - 1][j +
                                        1].rgbtRed
                                        + image0[i][j - 1].rgbtRed + image0[i][j + 1].rgbtRed + image0[i + 1][j - 1].rgbtRed + image0[i + 1][j].rgbtRed + image0[i + 1][j
                                                +
                                                1].rgbtRed)
                                / 9.0);
                int green = round((float)(image0[i][j].rgbtGreen + image0[i - 1][j - 1].rgbtGreen + image0[i - 1][j].rgbtGreen + image0[i - 1][j +
                                          1].rgbtGreen
                                          + image0[i][j - 1].rgbtGreen + image0[i][j + 1].rgbtGreen + image0[i + 1][j - 1].rgbtGreen + image0[i + 1][j].rgbtGreen + image0
                                          [i + 1][j + 1].rgbtGreen)
                                  / 9.0);
                int blue = round((float)(image0[i][j].rgbtBlue + image0[i - 1][j - 1].rgbtBlue + image0[i - 1][j].rgbtBlue + image0[i - 1][j +
                                         1].rgbtBlue
                                         + image0[i][j - 1].rgbtBlue + image0[i][j + 1].rgbtBlue + image0[i + 1][j - 1].rgbtBlue + image0[i + 1][j].rgbtBlue + image0[i +

                                                 1][j + 1].rgbtBlue)
                                 / 9.0);
                image[i][j].rgbtRed = red;
                image[i][j].rgbtGreen = green;
                image[i][j].rgbtBlue = blue;
            }   // .
        }       //.
    }
    return; //COMMENTS.
}