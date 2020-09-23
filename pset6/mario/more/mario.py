from cs50 import get_int

h = 0;
while(True):
    h = get_int('Height: \n')
    if(h > 0 and h <= 8):
        break

for i in range(h):
    spaces = ' ' * (h-i-1)
    blocks = '#' * (i+1)
    print(f'{spaces}{blocks}  {blocks}')


