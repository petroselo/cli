#a apt dowload style loading bar.

#loading from 0 to 100%

from time import sleep
import random
import os


os.system('clear')
print("\n\n")
for i in range(20):
    print("\tDownloading VIRUS module {}".format(i+1))
    sleep(0.1)
 
print("\n\n\tInstalling: VIRUS:\n\n")
#Cool ass loading bar
for i in range(101):
    print('\t[' + '#' * i + '.' * (100-i) + '] {}%\r '.format(i) , end='',flush=True)
    sleep(random.randint(0,10)/50)
print('\nVirus successfully installed')

while(True):
    sleep(0.5)
    print("\n\n\n\n\n\n\t\tYou have been HACKED")
    sleep(0.5)
    os.system("clear")
