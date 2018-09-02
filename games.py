#An application with the option to play games.
# Author: Paul Wernicke
#Developed in python for unix. (may add other platforms later)

import os
from time import sleep
import random
import math


####################
# Global functions #
####################
def clear():
    os.system('clear')

#########
# Games #
#########

#Hangman
def game_hangman():
    clear()
    print("coming soon")
    sleep(2)
    return ''
    
#################
# The main menu #
#################
def display_menu():
    clear()
    print("\nMAIN MENU\n")
    print("AVAILABLE GAMES:")
    print("  <1> Hangman")
    print("  <> tbd")
    print("  <> tbd>")
    print("\n  <q> Quit")
    

menu_input = ''
while menu_input not in ['q', 'quit', 'exit']:
    display_menu()
    menu_input = input("Select your game: ")
    
    if menu_input == '1':
        menu_input == game_hangman() #games will return blank or q
    
###########
# Hangman #
###########



