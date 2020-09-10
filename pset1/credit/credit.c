#include <cs50.h>
#include <stdio.h>

int longLength(long value);
string findCompany(long cardNumber);
void isCardValid(long cardNumber);

int main(void){
    //get user input
    long creditCard = get_long("Number: ");

    string company = findCompany(creditCard);
    isCardValid(creditCard);

}

string findCompany(long cardNumber){
    string company;
    int cardLength = longLength(cardNumber);
    int cardArray[cardLength];

    //transformin the card number into array
    for(int i = cardLength; i >= 0; i--){
        cardArray[i] = cardNumber % 10;
        cardNumber /= 10;
    }
    //making checks to see if the number falls into AMEX, VISA or MASTER. if not, invalid.
    if(cardArray[1] == 4 && (cardLength == 13 || cardLength == 16)){
        company = "VISA";
    } else if(cardLength == 15 && ((cardArray[1] == 3 && cardArray[2] == 4) || (cardArray[1] == 3 && cardArray[2] == 7))){
        company = "AMEX";
    } else if
            (
            	(
            	(cardLength == 16) &&
            	(cardArray[1] == 5)
            	) &&
                            (
                            (cardArray[2] == 1) ||
                            (cardArray[2] == 2) ||
                            (cardArray[2] == 3) ||
                            (cardArray[2] == 4) ||
                            (cardArray[2] == 5)
                            )
            )
     {
        company = "MASTERCARD";
     }  else {
        company = "INVALID";
    }

    return company;
}

//function to discover the length of a number
int longLength(long value){
	int length = 1;
	//divides the value by 10 and, if it is still greater than nine, add one number to the length
	while(value > 9){
		length++;
		value /= 10;
	}

	return length;
}



//function to discover if card number is valid with the luhs algorithm
void isCardValid(long cardNumber){
    int sumDigits = 0;

    int cardLength = longLength(cardNumber);
    int cardArray[cardLength];
    int otherDigitsArray[(cardLength/2)];

    //transformim the card number into array
    for(int i = cardLength; i >= 0; i--){
        cardArray[i] = cardNumber % 10;
        cardNumber /= 10;
    }

    //multiplying other digits by two and inserting in other digits array
    for(int i = (cardLength-1); i > 0; i -= 2){
        printf("No index %i temos o valor %i\n", i, cardArray[i]);
        otherDigitsArray[i] = (cardArray[i] * 2);
        printf("cardarray[i]: %i\n", cardArray[i]);
        printf("otherdigitsarra[i]: %i\n", otherDigitsArray[i]);
    }


}