// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 2000000;

// Hash table
node *table[N];

// Stores number of words in the dictionary.
int wordcount = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int len = strlen(word); // Stores the length of each word so that we can iterate individually.
    char lcword[len + 1];
    for (int i = 0; i < len; i++) // For every character in a word...
    {
        lcword[i] = tolower(word[i]); // Copy from the OG one to the new one as a lowercase character.
    }
    lcword[len] = '\0'; // The last bit (+ 1) is NUL.
    int bucket = hash(lcword); // Variable stores the index returned by 'hash'.
    node *cursor = table[bucket]; // Sets the temp. node to this index in the hashtable.
    while (cursor != NULL) // As long as 'cursor' is pointing to the next node in this index...
    {
        if (strcasecmp(lcword, cursor->word) != 0)
        {
            cursor = cursor->next; // Goes to the next word if they're not the same.
        }
        else
        {
            return true; // Otherwise it returns true.
        }
    }
    return false; // If it reaches a NULL value, it is the end of the list and the word was not found.
}

// Hashes word to a number
unsigned int hash(const char *word) // taken from 'https://stackoverflow.com/questions/14409466/simple-hash-functions'
{
    unsigned int counter;
    unsigned int hashAddress = 0;
    for (counter = 0; word[counter] != '\0'; counter++)
    {
        hashAddress = word[counter] + (hashAddress << 6) + (hashAddress << 16) - hashAddress;
    }
    return hashAddress % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r"); // Opens the dictionary given.
    if (file == NULL) // If it didn't open anything, returns false.
    {
        return false;
    }
    char word[LENGTH + 1]; // The array in which every read word will be put into.
    while (fscanf(file, "%s", word) != EOF) // Reads every word in the dict. until it reaches the EOF.
    {
        wordcount++; // Counts everytime a word was found.
        node *n = malloc(sizeof(node)); // Creates a new node for every word.
        if (n == NULL) // If there's not enough memory it returns false.
        {
            unload();
            return false;
        }
        else
        {
            strcpy(n->word, word); // Copies every word read into the first element of our node.
            int dictindex = hash(n->word); // Stores the value returned by our hash function into an integer.
            n->next = table[dictindex]; // ->next points to the first element in the list.
            table[dictindex] = n; // The new first element is now our node.
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount; // Returns the total number of words, which is counted in the 'load' function.
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++) // Will repeat 2000000 times.
    {
        node *cursor = table[i]; // Iterates through every single bucket.
        while (cursor != NULL) // Until it reaches the end.
        {
            node *tmp = cursor; // Temp. node points at whatever 'cursor' is pointing at.
            cursor = cursor->next; // Updates 'cursor' to point at the next node.
            free(tmp); // Deletes the node.
        }
        free(cursor); // Deletes the very last one.
    }
    return true;
}
