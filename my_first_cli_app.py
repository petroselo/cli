#Learning how to make a cli app

import os
from time import sleep

information = [1,2,3,4,5,6,7,8,9]

for i in information:
    os.system("clear")
    print("\t*********\n\tTitle Bar\n\t*********")
    
    
    print("\n\n\n\n\n")
    for x in range(6):
        print("\t",i + x)
    sleep(1)

