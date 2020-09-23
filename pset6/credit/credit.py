from sys import exit
from cs50 import get_string

def main():
    number = get_string('Number: ')
    #if the card pass the cardCheck with luhns algorithm, categorize and print
    if(cardCheck(number)):
        card = cardCategory(number)
        print(f'{card}')
        exit(0)
    else:
        print('INVALID')
        exit(0)
    #something wrong happens:
    exit(1)

def luhn(n):
    #create array with numbers
    nArr = list(n)

    #multiply every other digit by 2, starting from second to last
    everyDigits = ''
    for i in range(len(nArr)-2, -1, -2):
        digit = int(nArr[i]) * 2
        everyDigits += str(digit)

    #add the products digits together
    everySum = 0;
    everyArr = list(everyDigits)
    for i in range(len(everyArr)):
        everySum += int(everyArr[i])

    #sum of digits that weren't muplied
    otherSum = 0
    for i in range(len(nArr)-1, -1, -2):
        otherSum += int(nArr[i])

    #add sums
    result = everySum + otherSum

    #check validity
    if(result%10 == 0):
        return True

    return False

def cardCategory(n):
    type = ''
    if(len(n) == 15 and (n.startswith('34') or n.startswith('37'))):
        type = 'AMEX'
    elif(len(n) == 16 and (n.startswith('51') or n.startswith('52') or n.startswith('53') or n.startswith('54') or n.startswith('55'))):
        type = 'MASTERCARD'
    elif((len(n) == 14 or len(n) == 16) and (n.startswith('4'))):
        type = 'VISA'
    else:
        type = 'INVALID'

    return type


def cardCheck(n):

    #checks the length of the card
    if(len(n) < 13 or len(n) == 14 or len(n) > 16):
        return False

    #checks if the card is valid and categorize it
    if(luhn(n)):
        return True

    #if the luhn function returns false, the card is invalid
    return False




main()


