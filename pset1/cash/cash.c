#include <cs50.h>
#include <stdio.h>
#include <math.h>

int changeCounter(int change);

int main(void) {
    float change;
    do {
        change = get_float("Change owed: ");
    } while (change < 0);

    int cents = round(change * 100);

    int total = changeCounter(cents);

    printf("%i\n", total);
}

int changeCounter(int change){
    int totalChange = 0;


    while(change >= 25){
    	change-=25;
    	totalChange++;
    }


    while(change >= 10){
    	change-=10;
    	totalChange++;
    }



    while(change >= 5){
    	change-=5;
    	totalChange++;
    }


    while(change >= 1){
    	change-=1;
    	totalChange++;
    }

    return totalChange;
}
