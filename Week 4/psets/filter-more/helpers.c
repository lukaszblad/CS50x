#include "helpers.h"
#include <math.h>

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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    int gy;
    int gx;

    int r_channel_x;
    int r_channel_y;
    int g_channel_x;
    int g_channel_y;
    int b_channel_x;
    int b_channel_y;

    for (int i = 0; i < height; i++) //selecting row
    {
        for (int j = 0; j < width; j++) //selecting column
        {
            //resetting metrics
            r_channel_x = 0;
            r_channel_y = 0;
            g_channel_x = 0;
            g_channel_y = 0;
            b_channel_x = 0;
            b_channel_y = 0;

            for (int v = i - 1; v <= i + 1; v++) //selecting box row
            {
                for (int h = j - 1; h <= j + 1; h++) //selecting box column
                {
                    //calculating gx and gy coefficients
                    gx = (h - j) + ((h - j) * (((v - i) * (v - i)) * -1 + 1));
                    gy = (v - i) + ((v - i) * (((h - j) * (h - j)) * -1 + 1));

                    //calculating metrics for each channel
                    if ((v >= 0 && v < height) && (h >= 0 && h < width)) //filtering out pixels outside boundaries
                    {
                        r_channel_x += gx * image[v][h].rgbtRed;
                        r_channel_y += gy * image[v][h].rgbtRed;

                        g_channel_x += gx * image[v][h].rgbtGreen;
                        g_channel_y += gy * image[v][h].rgbtGreen;

                        b_channel_x += gx * image[v][h].rgbtBlue;
                        b_channel_y += gy * image[v][h].rgbtBlue;
                    }
                }
            }

            //applying new channel value on temp array
            tmp[i][j].rgbtRed = value_cap(round_num((float) sqrt((r_channel_x * r_channel_x) + (r_channel_y * (float) r_channel_y))));
            tmp[i][j].rgbtGreen = value_cap(round_num((float) sqrt((g_channel_x * g_channel_x) + (g_channel_y * (float) g_channel_y))));
            tmp[i][j].rgbtBlue = value_cap(round_num((float) sqrt((b_channel_x * b_channel_x) + (b_channel_y * (float) b_channel_y))));
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