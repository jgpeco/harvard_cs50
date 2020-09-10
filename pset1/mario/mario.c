#include <cs50.h>
#include <stdio.h>

void createPyramid(int n);
void printItem(int itemCounter, int itemLimit, string item);

int main(void){
    // get user input
    int height;
    do {
        height = get_int("Height: ");
    } while(height <= 0 || height > 8);

    //call function to create the mario blocks
    createPyramid(height);
}

void createPyramid(int n){
    //will loop throught the number the user gave and print one line at time
    for(int i = 0; i < n; i++){
        //items to print
        string space = " ";
        string block = "#";
        //variables to count the printfs we need
        int spacesCounter = 0;
        int leftBlocksCounter = 0;
    	int rightBlocksCounter = 0;
    	//variables to limit the number of prints
    	int spaceLimit = n-1-i;
    	int blockLimit = i+1;
        //calling function to print spaces
        printItem(spacesCounter, spaceLimit, space);
        //printing the first block
        printItem(leftBlocksCounter, blockLimit, block);
        //divisor of blocks
        printf("  ");
        //printing the last block
        printItem(rightBlocksCounter, blockLimit, block);
        printf("\n");
    }
}

void printItem(int itemCounter, int itemLimit, string item){
        //printing one item for each iteration
        	while(itemCounter < itemLimit){
            printf("%s", item);
            itemCounter++;
        }
}



