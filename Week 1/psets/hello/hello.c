#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //prompting the user
    string name = get_string("What's your name? ");

    //printing the output
    printf("hello, %s\n", name);
}