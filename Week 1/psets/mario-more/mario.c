#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //requesting input
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    //printing rows
    for (int n = 0; n < height; n++)
    {
        //left side
        //printing spaces
        for (int s = height - 1; s > n; s--)
        {
            printf(" ");
        }
        //printing bricks
        for (int i = 0; i <= n; i++)
        {
            printf("#");
        }

        //printing distance in the middle
        printf("  ");

        //right side
        for (int i = 0; i <= n; i++)
        {
            printf("#");
        }
        printf("\n");
    }
}