// Implements a dictionary's functionality
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];

//globsl variable used to count the amount of words loaded into the load function
unsigned int count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //takes the incoming word to change it into a lowercase one
    char *lower = toLower(word);
    //gets the hash value for the lowercase letter to match to the lowercase word in the dictionary
    int hash_dict = hash(lower);
    //cursor points to the bucket the word was stored in
    node *cursor = table[hash_dict];

    while (cursor != NULL)
    {
        //compares the two words to see if they are a match, else the program traverses the linked list until it finds the matching word
        if (strcasecmp(cursor->word, lower) == 0)
        {
            return true;
        }
        else
        cursor = cursor->next;
    }
    //returns false if the words didn't match
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;  //hash * 33 + c
    }

    return hash % N;
    //hash function used in this code: djb2 by dan bernstein
    //website the hash function djb2 was obtained from: http://www.cse.yorku.ca/~oz/hash.html
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictonary
    FILE *file = fopen(dictionary, "r");

    //stops the program if a file wasn't loaded
    if (file == NULL)
    {
        printf("no such file.\n");
        return false;
    }

    //char array to store the word(s) into from the loaded dictionary
    char buffer[LENGTH + 1];

    //stores the word from "file" into buffer
    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        //copies the word in buffer into n->word
        strcpy(n->word, buffer);
        n->next = NULL;
        //stops the program if there isn't enough space allocated
        if (n == NULL)
        {
            printf("Not enough allocated memory");
            fclose(file);
            free(n);
            return false;
        }
        else
        {
            //gets a hash value for the word stored in n
            int index = hash(n->word);
            //n->next points to the head of the linked list at a particular bucket in the hash table
            n->next = table[index];
            //n becomes the head of the linked list in the hash table
            table[index] = n;
            //global count variable is updated
            count++;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //the global count variable is returned to speller.c
    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //the unload function will go through every bucket in the hash table
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        //tmp is the same as cursor so that the memory at that location isn't lost in the upcoming steps
        node *tmp = cursor;
        while ((tmp = cursor) != NULL)
        {
            //cursor points to the next value
            cursor = cursor->next;
            //the value tmp is pointing to is frees the memory
            free(tmp);
            //tmp equals cursos again to repeat the process until both cursor and tmp equal NULL (or simply put, reaches the end of the linked list)
            tmp = cursor;
        }
    }
    return true;
}

char *toLower(const char *word)
{
    static char lower[LENGTH];
    //copies the word into the locally declared char variable
    strcpy(lower, word);
    
    //the for loop goes through every character in the lower variable, changing its ASCII value from an uppercase letter to a lowercase letter
    for (int i = 0; i <= strlen(lower); i++)
    {
        if (lower[i] >= 65 && lower[i] <= 90)
            lower[i] = lower[i] + 32;
    }
    return lower;
}