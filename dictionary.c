// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// number of characters in dictionary (26 + apost)
#define ALPHABET_SIZE (27)

// Represents a trie node 
typedef struct trienode
{
    struct trienode *children[ALPHABET_SIZE];
    bool isEnd;
}
trienode;

// Function prototype
trienode *makenode(void);
void freeup(trienode *ptree);

// Global variables

// Root of trienode tree
trienode *root;

// Number of words loaded in dictionary
int dictionarysize = 0;

// Returns true if word is in dictionary else false
bool check(const char *checkword)
{
    trienode *cursor;
    int len, index;
    
    cursor = root;
    len = strlen(checkword);
    for(int level = 0; level < len; level++)
    {
        
        index = (int) tolower(checkword[level]) - 'a'; if (index < 0) index = ALPHABET_SIZE - 1;
        if (!cursor->children[index])
        {
            return false;
        }
        cursor = cursor->children[index];
    }
    if (!cursor)
    {
        return false;
    }
    else
    {
        return cursor->isEnd;
    }
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *pdictionary;
    char newword[LENGTH + 1];
    trienode *newnode, *cursor;
    
    dictionarysize = 0;

    // setup root
    root = makenode();

    //open dictionary
    pdictionary = fopen(dictionary, "r");
    if (pdictionary == NULL)
    {
        return false;
    }
    
    while (fscanf(pdictionary, "%s", newword) != EOF)
    {
        cursor = root;
        int len = strlen(newword);
        for (int level = 0; level < len; level++)
        {
            int index = (int) newword[level] - 'a'; if (index < 0) index = ALPHABET_SIZE - 1;
            if (!cursor->children[index])
            {
                cursor->children[index] = makenode();
                if (!cursor->children[index])
                {
                    return false;               // malloc failed
                }
            }
            cursor = cursor->children[index];
        }
        // final node is end of word
        cursor->isEnd = true;
        dictionarysize++;
    }
    printf("   *** loaded %i words\n", dictionarysize);
    fclose(pdictionary);
    return true;
}

trienode *makenode(void)
{
    trienode *newnode;

    newnode = (trienode *) malloc(sizeof(trienode));
    if (newnode)
    {
        newnode->isEnd = false;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            newnode->children[i] = NULL;
        }
    }
    return newnode;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionarysize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    freeup(root);
    return true;
}

// Recursive called by unload
void freeup(trienode *ptree)
{
    if (!ptree)
    {
        return;
    }
    for (int index = 0; index < ALPHABET_SIZE; index++)
    {
        if (ptree->children[index])
        {
            freeup(ptree->children[index]);
        }
    }
    free(ptree);
    return;
}