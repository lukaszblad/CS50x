#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

string lower_key(string key);
string upper_key(string key);

int main(int argc, string argv[])
{
    //KEY VALIDATION
    //checking the number of arguments
    if (argc <= 1 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //string length variable
    int key_len = strlen(argv[1]);

    //checking length of the key
    if (key_len != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //checking if key contains only letters
    for (int i = 0; i < key_len; i++)
    {
        if (lower_key(argv[1])[i] < 97 || lower_key(argv[1])[i] > 122)
        {
            printf("Key must only contain alphabetic characters\n");
            return 1;
        }
    }

    //checking if letters in the key are unique
    int key_letters[26]; //array for each letter ASCII code

    for (int i = 0; i < key_len; i++)
    {
        //putting letter ASCII code in array
        key_letters[i] = (int) lower_key(argv[1])[i];

        //checking for doubled characters
        for (int j = 0; j < i; j++)
            if (key_letters[j] == key_letters[i])
            {
                printf("Key must not contain repeated characters\n");
                return 1;
            }
    }

    //PROMPTING THE USER FOR STRING TO ENCRYPT
    string text = get_string("plaintext:  ");

    //ENCRYPTING AND PRINTING THE STRING
    printf("ciphertext: ");
    for (int i = 0; i < strlen(text); i++)
    {
        //checking if letter lower or upper
        if (text[i] >= 65 && text[i] <= 90) //if character upper
        {
            printf("%c", upper_key(argv[1])[text[i] - 65]);
        }
        else if (text[i] >= 97 && text[i] <= 122)
        {
            printf("%c", lower_key(argv[1])[text[i] - 97]);
        }
        else
        {
            printf("%c", text[i]);
        }
    }
    printf("\n");
}

//lower_key function
string lower_key(string key)
{
    string lower_key = key;
    for (int i = 0; i < 26; i++)
    {
        lower_key[i] = tolower(key[i]);
    }
    return lower_key;
}

//upper key function
string upper_key(string key)
{
    string upper_key = key;
    for (int i = 0; i < 26; i++)
    {
        upper_key[i] = toupper(key[i]);
    }
    return upper_key;
}