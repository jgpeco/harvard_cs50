#include <cs50.h>
#include <stdio.h>

int main(void){
    string name = get_string("What's your name, little fella?\n");
    printf("Hello, %s!\n", name);

}