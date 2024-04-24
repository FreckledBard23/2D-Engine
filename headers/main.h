//      Player Definitions
float camera_x = 0;
float camera_y = 0;
float zoom = 10;

//example button recognition function
int player_tag = 0;
void keyboard(unsigned char key, int x, int y){
	//if(key == 'w') { objects[player_tag].transform.y += 1; }
	//if(key == 'a') { objects[player_tag].transform.x -= 1; }
	//if(key == 's') { objects[player_tag].transform.y -= 1; }
	//if(key == 'd') { objects[player_tag].transform.x += 1; }
	//if(key == 'q') { zoom *= 1.1; }
	//if(key == 'e') { zoom /= 1.1; }
}

//init function
int temp_tag = 0;
void main_start(){
	//init test_obj.txt
	//sets player_tag to be the index in obj array 
	struct gameobject ob;
	ob = obj_from_txt("player_obj.txt");
	instantiate(&ob, &ob.transform, &player_tag);
	ob = obj_from_txt("test_obj.txt");
	struct Transform t;
	for(int i = 0; i < 15; i++){
		t.x = 2 * i - 7.5;
		instantiate(&ob, &t, &temp_tag);
	}

	walls[0].x1 = -35;	walls[0].y1 = -20;
	walls[0].x2 = 35;	walls[0].y2 = -20;
	walls[1].x1 = 35;	walls[1].y1 = -20;
	walls[1].x2 = 35;	walls[1].y2 = 20;
	walls[2].x1 = 35;	walls[2].y1 = 20;
	walls[2].x2 = -35;	walls[2].y2 = 20;
	walls[3].x1 = -35;	walls[3].y1 = 20;
	walls[3].x2 = -35;	walls[3].y2 = -20;

	walls[0].exists = true;
	walls[1].exists = true;
	walls[0].display = true;
	walls[1].display = true;
	walls[2].exists = true;
	walls[3].exists = true;
	walls[2].display = true;
	walls[3].display = true;
}

//MAIN CODE LOOP
void code_loop(){
	objects[player_tag].transform.x = mouse_world_x;
	objects[player_tag].transform.y = mouse_world_y;
}
