#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256
#define TABLE_SIZE 5
#define DELETED_NODE (person*)(0xFFFFFFFFFFFFFFFFUL)

//hash that deals with collisions using open adressing

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
        } else if(hash_table[i] == DELETED_NODE) {
            printf("\t%i\t---<deleted>\n", i);
        }
        else {
            printf("\t%i\t%s\n", i, hash_table[i]->name);
        }
    }
    printf("/* END OF TABLE */\n");
}

bool hash_table_insert(person *p)
{
    if(p == NULL) return false;
    int index = hash(p->name);
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        int try = (i + index) % TABLE_SIZE;
        if(hash_table[try] == NULL || hash_table[try] == DELETED_NODE)
        {
            hash_table[try] = p;
            return true;
        }
    }
    return false;
}

//find a person in the table by their name
person *hash_table_lookup(char *name)
{
    int index = hash(name);
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        int try = (i + index) % TABLE_SIZE;
        if(hash_table[try] == NULL){
            return false;
        }
        if(hash_table[try] == DELETED_NODE) continue;
        if(strcmp(hash_table[try]->name, name) == 0)
            {
                return hash_table[try];
            }
    }
    //didn't find the person
    return NULL;
}

person *hash_table_delete(char *name)
{
    int index = hash(name);
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        int try = (i + index) % TABLE_SIZE;
        if(hash_table[try] == NULL){
            return false;
        }
        if(hash_table[try] == DELETED_NODE) continue;
        if(hash_table[try] != NULL && strcmp(hash_table[try]->name, name) == 0)
            {
                person *tmp = hash_table[try];
                hash_table[try] = DELETED_NODE;
                return tmp;
            }
    }
    //didn't find the person
    return NULL;
}

int main()
{

    init_hash_table();
    print_table();

    person male = {.name="male", .age=15};
    person peco = {.name="peco", .age=17};
    person napa = {.name="napa", .age=32};
    person tobi = {.name="tobi", .age=21};
    person rive = {.name="rive", .age=45};
    person teteu = {.name="teteu", .age=28};
    person dodo = {.name="dodo", .age=19};
    person zezo = {.name="zezo", .age=15};

    hash_table_insert(&peco);
    hash_table_insert(&napa);
    hash_table_insert(&tobi);
    hash_table_insert(&male);
    hash_table_insert(&rive);
    hash_table_insert(&teteu);
    hash_table_insert(&dodo);
    hash_table_insert(&zezo);

    print_table();

    hash_table_delete("rive");

    print_table();

    return 0;
}