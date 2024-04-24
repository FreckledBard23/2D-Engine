//      Player Definitions
float camera_x = 0;
float camera_y = 0;
float zoom = 10;

//example button recognition function
int player_tag = 0;
void keyboard(unsigned char key, int x, int y){
	if(key == 'w') { objects[player_tag].transform.y += 1; }
	if(key == 'a') { objects[player_tag].transform.x -= 1; }
	if(key == 's') { objects[player_tag].transform.y -= 1; }
	if(key == 'd') { objects[player_tag].transform.x += 1; }
	if(key == 'q') { zoom *= 1.1; }
	if(key == 'e') { zoom /= 1.1; }
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
	instantiate(&ob, &ob.transform, &temp_tag);

	walls[0].x1 = -10;
	walls[0].y1 = -10;
	walls[0].x2 = 10;
	walls[0].y2 = 10;
	walls[1].x1 = -9;
	walls[1].y1 = -9;
	walls[1].x2 = 9;
	walls[1].y2 = 9;
	walls[0].exists = true;
	walls[1].exists = true;
}

//MAIN CODE LOOP
void code_loop(){
}
