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
	objects[player_tag].collider.radius = 1.414;
	objects[player_tag].collider.drag_coeff = 0.1;
	ob = obj_from_txt("test_obj.txt");
	instantiate(&ob, &ob.transform, &temp_tag);
	objects[temp_tag].collider.radius = 0.707;
	objects[temp_tag].collider.drag_coeff = 0.01;
}

//MAIN CODE LOOP
void code_loop(){
}
