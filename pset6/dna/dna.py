import csv
from sys import argv, exit

def main():
    if(len(argv) != 3):
        print('Usage: python dna.py data.csv sequence.txt')
        exit(1)

    #variables to hold the data from the database
    data = [] #holds all the data from database
    sequence = '' #holds the sequence of dna
    strs = [] #holds the type of strs we are comparing
    strsCount = [] #holds the longest substring of strs in the sequence
    numberRows = 0 #number of people in database
    result = 'No match' #matched person with sequence of dna

    #opening database
    with open(argv[1], newline='') as database:
        reader = csv.reader(database)
        for row in reader:
            numberRows += 1
            data.append(row)

    #oepning sequence txt and retrieving values of dna sequences
    with open(argv[2], 'r') as sequenceFile:
        sequence = sequenceFile.readline().rstrip()

    #storin values of the database (this approach dynamically allocates the data and doesnt get the name)
    for dataStr in range(1, len(data[0])):
        strs.append(data[0][dataStr])


    # checking for STRs and calculating the longest string of them
    for i in strs:
        matchTotal = 0
        #first match in string
        index = sequence.find(i)
        matchCount = 1
        sizestr = len(i)
        while(index < len(sequence)):
            #checking for continuous match sequences
            while(index+sizestr < len(sequence) and sequence[(index+sizestr):(index+sizestr+sizestr)] == i):
                matchCount += 1
                index += sizestr
            #updatin the value of longest string of matchs
            if(matchCount > matchTotal):
                matchTotal = matchCount
            matchCount = 0
            index += 1
        strsCount.append(matchTotal)


    # compare the sequences of strs and the dna from the people in database
    for i in range(1, numberRows):
        check = ''
        for j in range(1, len(data[i])):
            if(strsCount[j-1] == int(data[i][j])):
                check += '0'
            else:
                check += '1'
        if(check.find('1') == -1):
            result = data[i][0]
            break

    #result of the search
    print(result)
    exit(0)


main()