#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256
#define TABLE_SIZE 10

//simple hash that doesn't deal with collisions

typedef struct
{
    char name[MAX_NAME];
    int age;
} person;

person *hash_table[TABLE_SIZE];

//map the names through different locations in the hash table
unsigned int hash(char *name)
{
    int length = strlen(name);
    unsigned int hash_value = 0;
    for(int i = 0; i < length; i++)
    {
        hash_value += name[i];
        hash_value = hash_value * name[i] % TABLE_SIZE;
    }
    return hash_value;
}

void init_hash_table() {
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hash_table[i] = NULL;
    }
    //table is empty
}

void print_table()
{
    printf("/* START OF TABLE */\n");
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        if(hash_table[i] == NULL)
        {
            printf("\t%i\t---\n", i);
        } else {
            printf("\t%i\t%s\n", i, hash_table[i]->name);
        }
    }
    printf("/* END OF TABLE */\n");
}

bool hash_table_insert(person *p)
{
    if(p == NULL) return false;
    int index = hash(p->name);
    if(hash_table[index] != NULL)
    {
        return false;
    }
    hash_table[index] = p;
    return true;
}

//find a person in the table by their name
person *hash_table_lookup(char *name)
{
    int index = hash(name);
    if(hash_table[index] != NULL && strcmp(hash_table[index]->name, name) == 0)
    {
        return hash_table[index];
    } else {
        return NULL;
    }
}

person *hash_table_delete(char *name)
{
    int index = hash(name);
    if(hash_table[index] != NULL && strcmp(hash_table[index]->name, name) == 0)
    {
        person *tmp = hash_table[index];
        hash_table[index] = NULL;
        return tmp;
    } else {
        return NULL;
    }
}

int main()
{

    init_hash_table();
    print_table();

    person peco = {.name="peco", .age=2};
    person napa = {.name="napa", .age=32};
    person tobi = {.name="tobi", .age=21};

    hash_table_insert(&peco);
    hash_table_insert(&napa);
    hash_table_insert(&tobi);

    print_table();

    //cant find person who is not on the table
    person *tmp = hash_table_lookup("rive");
    if(tmp == NULL)
    {
        printf("Not found!\n");
    } else {
        printf("Found %s\n", tmp->name);
    }

    //can find person who is on the table
    tmp = hash_table_lookup("tobi");
    if(tmp == NULL)
    {
        printf("Not found!\n");
    } else {
        printf("Found %s\n", tmp->name);
    }

    hash_table_delete("tobi");
    //after deletion, can't find person anymore
    tmp = hash_table_lookup("tobi");
    if(tmp == NULL)
    {
        printf("Not found!\n");
    } else {
        printf("Found %s\n", tmp->name);
    }

    print_table();



    return 0;
}