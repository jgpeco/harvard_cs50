from cs50 import SQL
from sys import argv, exit

def main():
    if(len(argv) > 2):
        print('The usage of the program is [program][house name]')
        exit(1)

    house = argv[1]
    db = SQL('sqlite:///students.db')
    allStudents = db.execute('SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first', house)
    for student in allStudents:
        if(student['middle'] is None):
             print(f"{student['first']} {student['last']}, born {student['birth']}")
        else:
            print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")


main()
