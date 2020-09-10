#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int countLetters(string text);
int countWords(string text);
int countSentences(string text);
int findGrade(int words, int letters, int sentences);


int main(void){
    string text = get_string("Text: ");

    int words = countWords(text);
    int letters = countLetters(text);
    int sentences = countSentences(text);
    int grade = findGrade(words, letters, sentences);

    if (grade < 1){
        printf("Before Grade 1\n");
    } else if (grade >= 16){
        printf("Grade 16+\n");
    } else {
        printf("Grade %i\n", grade);
    }

}

int countLetters(string text){
    int letters = 0;

    for(int i = 0, n = strlen(text); i < n; i++){
        if(isalnum(text[i])){
            letters++;
        }
    }

    return letters;
}

int countWords(string text){
    int words = 1; //considering that the last word doesn't have a space, it just has a end dot, we should start the count in 1

    for(int i = 0, n = strlen(text); i < n; i++){
        if(isspace(text[i])){
            words++;
        }
    }

    return words;
}

int countSentences(string text){
    int sentences = 0; //considering that the last word doesn't have a space, it just has a end dot, we should start the count in 1

    for(int i = 0, n = strlen(text); i < n; i++){
        if((text[i] == 33) || (text[i] == 46) || (text[i] == 63)){
            sentences++;
        }
    }

    return sentences;
}

int findGrade(int words, int letters, int sentences){
    //applying the algorithm
    float l = (100 * (float) letters) / (float) words;
    float s = (100 * (float) sentences) / (float) words;

    float index = (0.0588 * l) - (0.296 * s) - 15.8;

    return round(index);
}