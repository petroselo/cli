/*
	This is to work on a rendering engine in the terminal.
	I will be using term.h and maybe curses if I need its functionality.

	Rough game plan. Render basic triangles of a certain color.
	move to 3d rendering once this is working.
	Trying to keep this a bit neater than the hangman game.

	Author: Petroselo

*/

/* Includes */
#include <stdio.h>
#include <term.h>
#include <termios.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include "colors.h"

/* Macros */

/* Typedefs, Enums and Structs */

typedef struct {
	float x;
	float y;
	} point_t; //should this have color? maybe for more general yes. also a vector

typedef struct {
	point_t p1;
	point_t p2;
	int color;
} line_t; //maybe make this a segment and a line differently

typedef struct {
	point_t p1;
	point_t p2;
	point_t p3;
	int color;
	} triangle_t;

/* Compilation Unit Variables */

//termios structures
struct termios current_term, use_term;

//point_in_triangle variables
static point_t v0;
static point_t v1;
static point_t v2;
static float dot00;
static float dot01;
static float dot02;
static float dot11;
static float dot12;
static float denominator;
static float u;
static float v;

//main loop char getter
char inp;
//support half chars
int support_half_char;

int top_in;
int bot_in;

/* Functions */
static float dot(point_t p1, point_t p2) {
	return p1.x * p2.x + p1.y * p2.y;
}

static int point_in_triangle(point_t p, triangle_t tri) { //see http://blackpawn.com/texts/pointinpoly/
	//Vectors
	v0.x = tri.p3.x - tri.p1.x;
	v0.y = tri.p3.y - tri.p1.y;
	v1.x = tri.p2.x - tri.p1.x;
	v1.y = tri.p2.y - tri.p1.y;
	v2.x = p.x - tri.p1.x;
	v2.y = p.y - tri.p1.y;
	//Dot products
	dot00 = dot(v0, v0);
  dot01 = dot(v0, v1);
	dot02 = dot(v0, v2);
	dot11 = dot(v1, v1);
	dot12 = dot(v1, v2);
	//Barycentric coords
	denominator = dot00 * dot11 - dot01 * dot01;
	u = (dot11 * dot02 - dot01 * dot12) / denominator;
	v = (dot00 * dot12 - dot01 * dot02) / denominator;

	return (u >= 0) && (v >= 0) && (u + v < 1);
}

//this function will define the mapping of charblocks to points
static point_t char_to_point(float col, float line) {
	//for not make default ratio for mapping as this will work for testing.
	return (point_t) {col, line*2};
}

static void get_term_attr() {
	int errget;
	setupterm((char *)0, fileno(stdout), &errget);
	if (errget < 1) {
		puts("Failed to get terminal information");
		exit(1);
	}
}
//temp function until scene rendering function is in place. This will take a scne or a list of triangles in a certain order and figure out which one to render for each pixel. for now this is a good test.
static void render_triangle(triangle_t tri){
	printf( "\033[40m%s", clear_screen);
	if (support_half_char) {
		for(int col = 0; col < columns; col++) {
			for (int lin = 0; lin < lines; lin++) {
				top_in = point_in_triangle(char_to_point(col, lin), tri);
				bot_in = point_in_triangle(char_to_point(col, lin+0.5), tri);
				if (bot_in || top_in){
					printf("\033[%d;%dH\033[%d;%dm\u2580\033[39;49m",lin, col, top_in?32:30, bot_in?42:40);
				}
			}
		}
	}
	else {
		for(int col = 0; col < columns; col++) {
			for (int lin = 0; lin < lines; lin++) {
				if(point_in_triangle(char_to_point(col, lin), tri)) {
					printf("\033[%d;%dH\033[32;40m\u2588\033[39;49m",lin, col);
					///usleep(1000);
				}
			}
		}
	}
}


/* Global Functions */
int main(int argc, char *argv[]) {
	// ask user for char ratio. else default to 1:2 or 1:1.5 for now
	//get the terminal attributes
	get_term_attr();

	support_half_char = strcmp(getenv("TERM"), "xterm-256color") ? 0 : 1;

	//make terminal raw
	tcgetattr(STDIN_FILENO, &current_term);
	use_term=current_term;
	use_term.c_lflag &=(~ICANON & ~ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &use_term);

	//, disable buffering,make cursor invisible, open new screen, clear screen
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	printf("%s%s%s",cursor_invisible, enter_ca_mode, clear_screen);

	triangle_t tri1;
	tri1.p1.x = 50;
	tri1.p1.y = 5;
	tri1.p2.x = 5;
	tri1.p2.y = 80;
	tri1.p3.x = 100;
	tri1.p3.y = 50;

	render_triangle(tri1);
	while (1) {
		inp = getchar();
		if (inp == ' ')
			break;
		if (inp == 'w')
			tri1.p1.y--;
		if (inp == 'a')
			tri1.p1.x--;
		if (inp == 's')
			tri1.p1.y++;
		if (inp == 'd')
			tri1.p1.x++;
		if (inp == 'i')
			tri1.p2.y--;
		if (inp == 'j')
			tri1.p2.x--;
		if (inp == 'k')
			tri1.p2.y++;
		if (inp == 'l')
			tri1.p2.x++;
		if (inp == 'g')
			tri1.p3.y--;
		if (inp == 'v')
			tri1.p3.x--;
		if (inp == 'b')
			tri1.p3.y++;
		if (inp == 'n')
			tri1.p3.x++;



		render_triangle(tri1);
	}

	printf("%s%s%s",exit_ca_mode, cursor_visible, clear_screen); //reset colour scheme to not green
	setvbuf(stdout, NULL, _IOLBF, 0); //0 maybe should be BUFSIZE
	setvbuf(stdin, NULL, _IOLBF, 0);

	//reset terminal mode
	tcsetattr(STDIN_FILENO, TCSANOW, &current_term);

	return 0;
}
