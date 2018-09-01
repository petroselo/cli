/*
    A hangman game to practice using the <term.h> library which I hope is not
    cureses but a lowerlevel implementation to access terminal attributes.

    Author: Petroselo

		Notes: Game not resizable. Possible that several glitches may happen. Not fully tested.

    term.h: http://pubs.opengroup.org/onlinepubs/7908799/xcurses/terminfo.html#tag_002_001_003

*/

/*Includes*/
#include <stdio.h>
#include <term.h> // link -ltinfo during compilation to get terminfo file
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

/*Global definitions*/

typedef enum{ PLAYING, WON, LOST} state_t;
state_t game_state = 0;
char word[128];
char guess_word[128];
char used_letters[16] = "               ";

static void open_term(void) {
	int errget;
	setupterm((char *)0, fileno(stdout), &errget);
	if (errget < 1) {
		puts("Failed to get terminal information");
			exit(1);
	}
}

static uint32_t count_lines(char *words) {
	uint32_t count = 0;
	char ch;
	FILE *fp;
	fp = fopen(words, "r");
	do {
		ch = fgetc(fp);
		if(ch == '\n')
			count++;
	} while(ch != EOF);
	fclose(fp);
	return count;
}

static void move_cursor(int x, int y){
	printf("%s",cursor_home);
	for(int i = 0;i<y;i++)
		printf("%s",cursor_down);
	for(int i = 0;i<x;i++)
		printf("%s",cursor_right);
}

static void print_game(int m) {
	printf("%s\n\n\n\n\n", clear_screen);
	printf("        %s      %s\n",m>1?"█▀▀▀▀▀▀█":"        ", used_letters);
	printf("        %s      %s\n", m>0?"█":" ", m>2?"│":" ");
	printf("        %s      %s\n", m>0?"█":" ", m>3?"Ô":" ");
	printf("        %s     %s%s%s\n", m>0?"█":" ", m>5?"\\":" ", m>4?"┼":" ", m>6?"/":" ");
	printf("        %s      %s\n", m>0?"█":" ", m>4?"│":" ");
	printf("        %s     %s %s\n", m>0?"█":" ", m>8?"/":" ", m>7?"\\":" ");
	printf("     ███████          %s\n", guess_word);
	move_cursor(5, 15);
	printf("(Space to quit)");
	move_cursor(5, 13);
	printf("Guess: ");
}

static void print_lost() {
	move_cursor(5, 15);
	printf("YOU LOST!          ");
}

static void print_won() {
	move_cursor(5, 15);
	printf("YOU WON!           ");
}

int main(int argc, char *argv[]) {
	//get terminal information
	open_term();

	//get word list name
	if(argc != 2) {
		printf("Invalid options. Usage ./hangman <wordlist>\n");
		exit(1);
	}
	char *words = argv[1];



	//initialise random
	time_t t;
	srand( (unsigned) time(&t));

	//read a random line from file
	int r = rand() % count_lines(words);
	FILE *fp;
	fp = fopen(words, "r");
	for(int i = 0; i<r; i++) {
		fgets(word, 127, fp);
	}
	fgets(word, 127, fp);

	//remove trailing \n and put \0 in also in used letters
	for(int i = 0; i < 128; i++) {
		if (word[i] == '\n') {
			word[i] = '\0';
		}
	}

	//set up guess word and used letters
	used_letters[31] = '\0';
	int len = strlen(word);
	for(int i = 0; i < len; i++){
		guess_word[2*i] = '_';
		guess_word[2*i+1] = ' ';
	}
	guess_word[2*len] = '\0';

	//turn off stdout buffering for immediate printing
	setbuf(stdout, NULL);

	//enter new terminal
 	printf("%s", enter_ca_mode);

 	char input_ch;
	int incorrect = 0;
	int correct = 0;
	int correct_b;

	while(game_state == PLAYING) {
		print_game(incorrect);
		input_ch = getchar();
		if(input_ch == ' ')
			break;
		if (input_ch == '\n')
			continue;

		for(int i = 0; i < 2*len; i++){
			if (toupper(input_ch) == toupper(guess_word[i])) {
				continue;
			}
		}

		correct_b = 0;
		for(int i = 0; i < len; i++){
			if (toupper(input_ch) == toupper(word[i])) {
				guess_word[i*2] = word[i];
				correct_b = 1;
				correct++;
			}
		}
		if (!correct_b) {
			int already_used = 0;
			for(int i = 0; i < strlen(used_letters); i++){
				if (toupper(input_ch) == toupper(used_letters[i])) {
				already_used = 1;
				}
			}
			if(!already_used) {
				used_letters[2*incorrect] = toupper(input_ch);
				incorrect++;
			}
		}
		if (incorrect >= 9) {
			print_game(incorrect);
			print_lost();
			game_state = LOST;
			usleep(4000000);
		}
		if (correct == len) {
			print_game(incorrect);
			print_won();
			game_state = WON;
			usleep(4000000);
		}
	}//while end

	//return from game terminal
	printf("%s\n", exit_ca_mode);
	printf("The word was: %s\n", word);

	return 0;
}
