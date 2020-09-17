#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256
#define TABLE_SIZE 5

//hash that deals with collisions using external chaining

typedef struct person
{
    char name[MAX_NAME];
    int age;
    struct person *next;
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
            printf("\t%i\t", i);
            person *tmp = hash_table[i];
            while(tmp != NULL){
                printf("%s - ", tmp->name);
                tmp = tmp -> next;
            }
            printf("\n");
        }
    }
    printf("/* END OF TABLE */\n");
}

bool hash_table_insert(person *p)
{
    if(p == NULL) return false;
    int index = hash(p->name);
    p->next = hash_table[index];
    hash_table[index] = p;

    return true;
}

//find a person in the table by their name
person *hash_table_lookup(char *name)
{
    int index = hash(name);
    person *tmp = hash_table[index];
    while (tmp != NULL && strcmp(tmp->name, name) !=0)
    {
        tmp = tmp->next;
    }
    return tmp;
}

person *hash_table_delete(char *name)
{
    int index = hash(name);
    person *tmp = hash_table[index];
    person *prev = NULL;
    while (tmp != NULL && strcmp(tmp->name, name) !=0)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    if (tmp == NULL) return NULL;
    if (prev == NULL) {
        //deleting the head
        hash_table[index] = tmp->next;
    } else {
        prev->next = tmp->next;
    }
    return tmp;
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