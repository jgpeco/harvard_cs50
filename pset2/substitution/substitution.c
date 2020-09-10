#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string encrypt(string plaintext, string key);
int checkForDuplicate(string key, int length);

int main(int argc, string argv[])
{
    if(argc != 2){
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1]; //key to encrypt message
    int keyLength = strlen(key);//key length

    if(keyLength != 26){
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for(int i = 0; i < keyLength; i++){//check to see if all key chars are alphabetical
        if(isalpha(key[i]) == 0){
            printf("Key must only contain alphabetical characters.\n");
            return 1;
        }
    }

    if(checkForDuplicate(key, keyLength) == 1){
        printf("Key must not contain duplicate characters.\n");
        return 1;
    }


    string plaintext = get_string("plaintext: ");
    string ciphertext = encrypt(plaintext, key);

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

string encrypt(string plaintext, string key){
    int plainLength = strlen(plaintext);
    int keyLength = strlen(key);

    int charCode;
    int stopLoop;
    int n;

    for(int i = 0; i < plainLength; i++){
        charCode = 65; //charcode will keep track of the compariso between arrays, it starts at 65, which is A, and we will loop it until it matches the char of the text the user provided
        stopLoop = 0; //this variable will stop the loop if we find the match between charcode and the char of the user array
        n = 0; //this will increment the value of the key array, so it is up to date with the charCode


        if(islower(plaintext[i]) > 0){
            charCode = 97; //if it is lowercase, the count should start at 97
        }

        if(isalpha(plaintext[i]) == 0){
            //if it is not an alpha char, this will jump the next loop and start in the next array element
            stopLoop = 1;
        }

        for(int j = 0; stopLoop < 1; j++){//makes a loop through the encrypt key.
            if(plaintext[i] == charCode){//in every element of the array, we are checking if the plaintext matches the charCode of that number
                plaintext[i] = key[n];//if it do, we trasnfer the value of the key array to the plain text one

                //plaintext is lower, key is upper
                if((charCode > 90 && key[n] <= 90)){
                    plaintext[i]+=32;
                }

                //plaintext is upper, key is lower
                if((charCode <= 90 && key[n] > 90)){
                    plaintext[i]-=32;
                }

                stopLoop = 1; //this stops the loop

            } else { //if the charCode of that element in the key array doesn't matches the user char, we update the numbers and keep goin
                charCode++;
                n++;
            }
        }
    }

    return plaintext;

}

int checkForDuplicate(string key, int length){

        for(int i = 0; i < length; i++){
            //looping through the strings chars
            for(int j = i+1; j < length; j++){
                //comparing the char to the rest of the string, if there is a duplicate char the loop returns 1
                if(key[i] == key[j]){
                    return 1;
                }
            }
        }
        return 0;
 }
