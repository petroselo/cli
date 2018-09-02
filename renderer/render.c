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
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

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

//maybe have common variables for triangle here as global so we are not constantly redefining them

/* Functions */
static float dot(point_t p1, point_t p2) {
	return p1.x * p2.x + p1.y * p2.y;
}

static int point_in_triangle(point_t p, triangle_t tri) { //see http://blackpawn.com/texts/pointinpoly/
	//Vectors
	point_t v0 = {tri.p3.x - tri.p1.x, tri.p3.y - tri.p1.y};
	point_t v1 = {tri.p2.x - tri.p1.x, tri.p2.y - tri.p1.y};
	point_t v2 = {p.x - tri.p1.x, p.y - tri.p1.y};
	//Dot products
	float dot00 = dot(v0, v0);
  float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);
	//Barycentric coords
	float denominator = dot00 * dot11 - dot01 * dot01;
	float u = (dot11 * dot02 - dot01 * dot12) / denominator;
	float v = (dot00 * dot12 - dot01 * dot02) / denominator;

	return (u >= 0) && (v >= 0) && (u + v < 1);
}

//this function will define the mapping of charblocks to points
static point_t char_to_point(int col, int line) {
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

static void move_cursor(int x, int y){ //temp to be replaced with more effienct direct strings once I figure out how
	printf("%s",cursor_home);
	for(int i = 0;i<y;i++)
		printf("%s",cursor_down);
	for(int i = 0;i<x;i++)
		printf("%s",cursor_right);
}
//temp function until scene rendering function is in place. This will take a scne or a list of triangles in a certain order and figure out which one to render for each pixel. for now this is a good test.
static void render_triangle(triangle_t tri){
	printf("%s", clear_screen);
	for(int col = 0; col < columns; col++) {
		for (int lin = 0; lin < lines; lin++) {
			if(point_in_triangle(char_to_point(col, lin), tri)) {
				move_cursor(col, lin);
				printf("\033[32;40m█");
				//usleep(1000);
			}
		}
	}
}


/* Global Functions */
int main(int argc, char *argv[]) {
	// ask user for char ratio. else default to 1:2 or 1:1.5 for now
	//get the terminal attributes
	get_term_attr();

	//, disable buffering,make cursor invisible, open new screen, clear screen
	setbuf(stdout, NULL);
	printf("%s%s%s",cursor_invisible, enter_ca_mode, clear_screen);

	triangle_t tri1;
	tri1.p1.x = 10;
	tri1.p1.y = 10;
	tri1.p2.x = 80;
	tri1.p2.y = 10;
	tri1.p3.x = 10;
	tri1.p3.y = 80;

	render_triangle(tri1);
	usleep(5000000);
	printf("%s%s%s",exit_ca_mode, cursor_visible, clear_screen); //reset colour scheme to not green


	return 0;
}
