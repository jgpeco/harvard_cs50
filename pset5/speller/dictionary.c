// Implements a dictionary's functionality
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
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
const unsigned int N = 1000;

// Hash table
node *table[N];

//number of words in dictionary
int sizeOfDic = 0;

//function to initiate the hash table
void initiHashTable()
{
    for(int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    //every bucket of the hash table is now null
}

//just for error checking
void printTable()
{
    printf("/* START OF TABLE */\n");
    for(int i = 0; i < N; i++)
    {
        if(table[i] == NULL)
        {
            printf("\t%i\t---\n", i);
        } else {
            printf("\t%i\t", i);
            node *tmp = table[i];
            while(tmp != NULL){
                printf("%s - ", tmp->word);
                tmp = tmp -> next;
            }
            printf("\n");
        }
    }
    printf("/* END OF TABLE */\n");
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return sizeOfDic;
}



// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word);

    //following the linked list
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }
    return false;

}

// Hashes word to a number - I'm using the djib2 hash function, credits to the author  Dan Bernstein
unsigned int hash(const char *word)
{
        // printf("Starting a hash loop of the word: %s\n", word);
        unsigned int hash = 5381;
        //assgining first value to c, it is the value of the first char of word
        int c = tolower(*word);
        // printf("intial value of c is %d\n", c);

        while (*word!=0)
        {
            //iterates through all the chars in the word and adds a 'random' value to them
            hash = hash * 33 + c;
            c = *word++;
            c = tolower(c);
            // printf("in loop value of c is %d\n", c);
        }
        // printf("hash value is %d\n", hash);

        return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //opening dicitonary
    FILE *file = fopen(dictionary, "r");
    if(file == NULL)
    {
        printf("Could not open dictionary\n");
        return false;
    }


    char *word = malloc(LENGTH);
    //looping through dictionary and inserting words in hashtable

        while(fscanf(file, "%s", word) != EOF)
        {
            //removing the newline char
            int l = strlen(word);
            if(l && word[l-1] == '\n') word[l-1] = '\0';

            //inserting the word of dictionary inside of node and in the hashtable
            node *item = malloc(sizeof(node));
            if(item == NULL)
            {
                return false;
            }
            strcpy(item->word, word);

            //inserting word in table - find index of item via hash, points the node next to the table index and assign the value of item to the table index
             if(item == NULL) return false;
             int index = hash(item->word);
             item->next = table[index];
             table[index] = item;
             sizeOfDic++;
        }

    fclose(file);
    free(word);

    return true;
}



// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    int index = 0;
    while(index < N)
    {
        node *head = table[index];
        node *cursor = head;

        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        index++;
    }


    return true;
}
