//#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define PI 3.1415926535

// To use time library of C
#include <time.h>
 
void delay(float number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}


//       ----------------------SCREEN----------------------
#define screenx 1920 
#define screeny 1080
#include <glut.h>
#include <draw_tools.h>
//       ----------------------SCREEN----------------------


float distance(int x1, int y1, int x2, int y2){
    float a = fabs(x1 - x2);
    float b = fabs(y1 - y2);
    return sqrt(a * a + b * b);
}


//      Object Struct Defines
#include <physics.h>
#include <object_handler.h>

//      Player Definitions
float camera_x = 0;
float camera_y = 0;
float zoom = 10;
float theta = 0;

float deltatime = 0;
#define frame_time_length (1000 / 60) //length of second in millis / frames in second

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	
	for(int i = 0; i < max_tag; i++){
		float obj_rot = objects[i].transform.rotation;
		float obj_posx = objects[i].transform.x;
		float obj_posy = objects[i].transform.y;
		
		for(int s = 0; s < sprite_rects; s++){
			float centerx = objects[i].sprite.x[s];
			float centery = objects[i].sprite.y[s];
			float sidex = objects[i].sprite.xside[s];
			float sidey = objects[i].sprite.yside[s];
			
			//rotate all centers if the object is rotated
			rotate_around_origin(&centerx, &centery, obj_rot);
			
			draw_box_rotated((centerx - camera_x) * zoom + screenx / 2, (centery - camera_y) * zoom + screeny / 2, objects[i].sprite.col[s], sidex * zoom, sidey * zoom, objects[i].sprite.theta[s] + obj_rot); 
		}
	}

	glFlush();
	glutSwapBuffers();
}

void update(int a){
	objects[0].transform.rotation += 0.016 * 2 * PI;

	glutPostRedisplay();
	glutTimerFunc(frame_time_length, update, 0);
}

int main(int argc, char** argv) {
    	//init object arr
    	for(int i = 0; i < max_objects; i++){
		objects[i].empty = true;
	}

	int tag = 0;
	struct gameobject ob = obj_from_txt("test_obj.txt");
	instantiate(&ob, &ob.transform, &tag);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenx, screeny);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	gluOrtho2D(0,screenx,screeny,0);
	glClearColor(0,0,0,0);
	glutFullScreen();
	glutTimerFunc(frame_time_length, update, 0);
	glutMainLoop();
    	return 0;
}
