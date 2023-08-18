#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

//word counter for size
int word_counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //hash word to get index
    int idx = hash(word);

    //create cursor
    node *cursor = table[idx];

    //navigate through the linked list at the correct index, searching for the word
    while (cursor != NULL)
    {
        if (strcmp(word, cursor -> word) == 0)
        {
            return true;
        }
        cursor = cursor -> next; //jumping to next node
    }

    //return false if while loop ended
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    //return (toupper(word[1]) - 65) + (toupper(word[strlen(word) - 1]) - 65);
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //open the dictionary file
    FILE *dict = fopen(dictionary, "r"); //open dictionary in reading mode
    if (dict == NULL)
    {
        return false;
    }

    //iterate over each word in the dictionary, place it in a new node and place the node inside the hash table
    int idx; //index of the hash table
    char word[LENGTH + 1]; //array to store the word
    while (fscanf(dict, "%s", word) != EOF)
    {
        //create new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        //place word into node
        strcpy(new_node -> word, word);

        //hash word to get index
        idx = hash(word);

        //place node in hash table
        new_node -> next = table[idx];
        table[idx] = new_node;

        //increase the word counter
        word_counter++;
    }

    //close the file
    fclose(dict);

    //return true at the end
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // size is stored in word counter variable
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //free all nodes iterating over them
    for (int i = 0; i < N; i++)
    {
        //create cursor
        node *cursor = table[i];

        //iterate over each linked list
        while (cursor != NULL)
        {
            //temporary variable to not lose the nodes
            node *tmp = cursor;
            cursor = cursor -> next; //jump to next node
            free(tmp); //and free the previous one
        }

        //return true if freeing was completed
        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}