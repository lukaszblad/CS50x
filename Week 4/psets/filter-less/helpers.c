#include "helpers.h"

//rounding function accepts float, return rounded int
int round_num(float num)
{
    float tmp = num - (int)num;

    int rounded_num;

    if (tmp < 0.5)
    {
        rounded_num = (int) num;
        return rounded_num;
    }
    else
    {
        rounded_num = (int) num + 1;
        return rounded_num;
    }
}

//value cap function
int value_cap(int clr_value)
{
    if (clr_value > 255)
    {
        return 255;
    }
    else if (clr_value < 0)
    {
        return 0;
    }
    else
    {
        return clr_value;
    }
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float clr_avg;

    for (int i = 0; i < height; i++) //row selection
    {
        for (int j = 0; j < width; j++) //column selection
        {
            clr_avg = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / (float) 3; //color average

            //rounding
            int rounded_value = round_num(clr_avg);

            //turning to grayscale
            image[i][j].rgbtRed = rounded_value;
            image[i][j].rgbtGreen = rounded_value;
            image[i][j].rgbtBlue = rounded_value;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int red_channel;
    int green_channel;
    int blue_channel;

    for (int i = 0; i < height; i++) //row selection
    {
        for (int j = 0; j < width; j++) //column selection
        {

            //calculating sepia equivalent
            red_channel = round_num(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            green_channel = round_num(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            blue_channel = round_num(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);

            //applying sepia filter
            image[i][j].rgbtRed = value_cap(red_channel);
            image[i][j].rgbtGreen = value_cap(green_channel);
            image[i][j].rgbtBlue = value_cap(blue_channel);

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            tmp[i][j] = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    float sum_red;
    float sum_green;
    float sum_blue;
    int denominator;

    for (int i = 0; i < height; i++) //selecting row
    {
        for (int j = 0; j < width; j++) //selecting column
        {
            //resetting the sum for each channel
            sum_red = 0.0;
            sum_green = 0.0;
            sum_blue = 0.0;
            denominator = 0;

            for (int v = i - 1; v <= i + 1; v++) //selecting the row of blur box
            {
                for (int h = j - 1; h <= j + 1; h++) //selecting the column of blur box
                {
                    if ((v >= 0 && v < height) && (h >= 0 && h < width)) //filtering out pixels outside boundaries
                    {
                        sum_red += image[v][h].rgbtRed;
                        sum_green += image[v][h].rgbtGreen;
                        sum_blue += image[v][h].rgbtBlue;
                        denominator++;
                    }
                }
            }
            //assigning average colour for each pixel in temporary array
            tmp[i][j].rgbtRed = round_num(sum_red / (float) denominator);
            tmp[i][j].rgbtGreen = round_num(sum_green / (float) denominator);
            tmp[i][j].rgbtBlue = round_num(sum_blue / (float) denominator);
        }
    }

    //copying the values from temporary array to the original image
    for (int i = 0; i < height; i++) // selecting row
    {
        for (int j = 0; j < width; j++) //selecting column
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}