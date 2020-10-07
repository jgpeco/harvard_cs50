import csv
import random
from cs50 import SQL
from sys import argv, exit

def main():
    if(len(argv) != 2):
        print('The correct usage is: [name of program][file.csv]')
        exit(1)

    open('students.db', 'w').close()
    db = SQL('sqlite:///students.db')
    db.execute('CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)')

    #variables to use in loop
    fullName = ''
    house = ''
    birth  = 0
    studentId = 0

    with open(argv[1], 'r') as csvfile:
        reader = csv.DictReader(csvfile)
        for char in reader:
            tempName = []
            firstName = ''
            middleName= (None,)
            lastName = ''

            fullName = char['name']
            house = char['house']
            birth = int(char['birth'])

            tempName = fullName.split()
            if(len(tempName) == 2):
                firstName = tempName[0]
                lastName = tempName[1]
            else:
                firstName = tempName[0]
                middleName = tempName[1]
                lastName = tempName[2]

            db.execute('INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)', firstName, middleName, lastName, house, birth)


main()


