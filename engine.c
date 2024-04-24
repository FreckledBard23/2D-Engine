#include <defines.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#define PI 3.1415926535

// To use time library of C
#include <time.h>

int mouse_screen_x, mouse_screen_y;
float mouse_world_x, mouse_world_y;
//mouse world only works correctly if
//              - not fullscreen
//              - fullscreen with screenx and screeny correctly mapped to monitor resolution 

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

float distance(float x1, float y1, float x2, float y2){
    float a = x1 - x2;
    float b = y1 - y2;
    return sqrt(a * a + b * b);
}

float deltatime = 0;
#define frame_time_length (1000 / frame_limit) //length of second in millis / frames in second

//      Object Struct Defines
#include <physics.h>

//       ----------------------SCREEN----------------------
#include <glut.h>
#include <draw_tools.h>
//       ----------------------SCREEN----------------------


//game code location (./headers/main.h)
#include <main.h>

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i = 0; i < max_walls; i++){
		if(walls[i].exists && walls[i].display){
			draw_line(
					(walls[i].x1 - camera_x) * zoom + screenx / 2,				    
					(walls[i].y1 - camera_y) * zoom + screeny / 2,
					(walls[i].x2 - camera_x) * zoom + screenx / 2,
					(walls[i].y2 - camera_y) * zoom + screeny / 2, 
					0xFFFFFFFF);
		}
	}


	for(int i = 0; i <= max_tag; i++){
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
			
			if(sidex != 0 && sidey != 0)
				draw_box_rotated((centerx + obj_posx - camera_x) * zoom + screenx / 2, (centery + obj_posy - camera_y) * zoom + screeny / 2, objects[i].sprite.col[s], sidex * zoom, sidey * zoom, objects[i].sprite.theta[s] + obj_rot); 
		}
	}

	glFlush();
	glutSwapBuffers();
}

void mouseMotion(int x, int y){
	mouse_screen_x = x;
	mouse_screen_y = -y;
	mouse_world_x = (float)(x - screenx / 2 + camera_x) / zoom;
	mouse_world_y = (float)(-y + screeny / 2 + camera_y) / zoom;
}

void update(int a){
	update_physics();
	code_loop(); //calls main code loop from headers/main.h

	//tells opengl to update screen
	glutPostRedisplay();
	//calls update after frame_time_length
	glutTimerFunc(frame_time_length, update, 0);
}

int main(int argc, char** argv) {
    	//init object arr
    	for(int i = 0; i < max_objects; i++){
		objects[i].empty = true;
		objects[i].collider.enabled = false;
	}

	//init walls
	for(int i = 0; i < max_walls; i++)
		walls[i].exists = false;

	//in headers/main.h
	main_start();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenx, screeny);
	glutCreateWindow("2D-Engine");
	glutDisplayFunc(display);
	gluOrtho2D(0,screenx,0,screeny);
	glClearColor(0,0,0,0);
	if(auto_fullscreen)
		glutFullScreen();
	glutTimerFunc(frame_time_length, update, 0);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMotion);
	glutMainLoop();
    	return 0;
}
