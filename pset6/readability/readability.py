from cs50 import get_string

def main():
    text = get_string('Text: ')

    l = countLetters(text)
    w = countWords(text)
    s = countSentences(text)

    grade = colemanliau(l, w, s)
    if(grade < 1):
        print('Before Grade 1')
    elif(grade >= 16):
        print('Grade 16+')
    else:
        print(f'Grade {grade}')

#functions to count words/letters/sentences
def countLetters(text):
    letters = 0
    for i in range(len(text)):
        if(text[i].isalpha()):
            letters += 1
        else:
            continue
    return letters

def countWords(text):
    words = 1
    for i in range(len(text)):
        if(text[i].isspace()):
            words += 1
        else:
            continue
    return words

def countSentences(text):
    sentences = 0
    for i in range(len(text)):
        if(('.' in text[i]) or ('!' in text[i]) or ('?' in text[i])):
            sentences += 1
        else:
            continue
    return sentences

#average function
def average(value, words):
   return (value * 100) / words


#Coleman Liau formula
def colemanliau(l, w, s):
    L = average(l, w)
    S = average(s, w)

    return round(0.0588 * L - 0.296 * S - 15.8)


main()

