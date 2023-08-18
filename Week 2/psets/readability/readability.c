#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

//functions prototypes
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int text_length;

int main(void)
{
    //prompt the user
    string text = get_string("Text: ");

    //string lenght
    text_length = strlen(text);

    //calculate the letters
    int letters = count_letters(text);

    //calculate the words (no spaces)
    int words = count_words(text);

    //calculate the sentences (no punctuation marks)
    int sentences = count_sentences(text);

    //determine and print out the grade
    //Coleman-Liau formula
    float index = index = 0.0588 * (((float)letters * 100) / (float)words) - 0.296 * (((float)sentences * 100) / (float)words) - 15.8;

    //printing result
    if ((int) round(index) < 1)
    {
        printf("Before Grade 1\n");
    }
    else if ((int) round(index) > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}

int count_letters(string text)
{
    int cnt_letters = 0;

    for (int i = 0; i < text_length; i++)
    {
        if ((text[i] > 64 && text[i] < 91) || (text[i] > 96 && text[i] < 123))
        {
            cnt_letters++;
        }
    }
    return cnt_letters;
}

int count_words(string text)
{
    int cnt_spaces = 0;

    for (int i = 0; i < text_length; i++)
    {
        if (text[i] == 32)
        {
            cnt_spaces++;
        }
    }
    return cnt_spaces + 1;
}

int count_sentences(string text)
{
    int cnt_marks = 0;

    for (int i = 0; i < text_length; i++)
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            cnt_marks++;
        }
    }
    return cnt_marks;
}