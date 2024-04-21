//structs


struct Transform {
	float x;
	float y;
	
	//to calculate velocity for verlet
	float old_x;
	float old_y;
	
	//only able to be rotated seperately, not by collisions
	float rotation;
};

//TODO: FIX THIS MESS
struct Collider {
	bool enabled;
	
	//hitboxes
	float circlex;
	float circley;
	float radius;
	
	//verlet stuff
	bool do_physics;
	float acceleration_x;
	float acceleration_y;
	float drag_coeff;

	float gravity_force_x;
	float gravity_force_y;
};

#define sprite_rects 16
struct Sprite {
	//used to draw rectangles
	//can have up to 16 (value of sprite_rects)
	//rotated by theta radians
	//
	//              xside
	//         #---------------#
	//         |               |
	//   yside |(x, y) #       |
	//         |               |
	//         #---------------#

	float x[sprite_rects];
	float y[sprite_rects];
	float xside[sprite_rects];
	float yside[sprite_rects];
	uint32_t col[sprite_rects];
	float theta[sprite_rects];
};

struct gameobject {
	struct Transform transform;
	struct Collider collider;
	bool empty;

	struct Sprite sprite;
};



#define max_objects 65536
struct gameobject objects[max_objects];
int max_tag = 1;

void instantiate(struct gameobject *prefab, struct Transform *spawn_pos, int* tag){
	bool fit = false;
	int index = 0;
	while(!fit && index < max_objects){
		if(objects[index].empty == true){
			objects[index] = *prefab;
			objects[index].transform = *spawn_pos;
			objects[index].transform.old_x = objects[index].transform.x;
			objects[index].transform.old_y = objects[index].transform.y;
			objects[index].empty = false;			
			*tag = index;
			
			if(index > max_tag){
				max_tag = index;
			}

			fit = true;

			return;
		}
		index++;
	}

	*tag = -1;
}

void replace_obj(struct gameobject *prefab, struct Transform *spawn_pos, int tag_in){
	objects[tag_in] = *prefab;
	objects[tag_in].transform = *spawn_pos;
	objects[tag_in].empty = false;
}

//loads gameobject from text file
struct gameobject obj_from_txt(const char* filename){
	FILE *fptr;

	// Open a file in read mode
	fptr = fopen(filename, "r");

	// Line buffer
	char str_in[256];
	struct gameobject load_obj;
	int sprite_rect_index = 0;
	
	while(fgets(str_in, 256, fptr) != NULL){
		// Remove comments
		if(str_in[0] != '#'){
			//                  TRANSFORM VALUES
			if(str_in[0] == 'x')
				sscanf(str_in, "x %f y %f r %f", &load_obj.transform.x, &load_obj.transform.y, &load_obj.transform.rotation);
			if(str_in[0] == 'r'){
				sscanf(str_in, "rect x %f y %f xside %f yside %f theta %f color %x", &load_obj.sprite.x[sprite_rect_index], &load_obj.sprite.y[sprite_rect_index], &load_obj.sprite.xside[sprite_rect_index], &load_obj.sprite.yside[sprite_rect_index], &load_obj.sprite.theta[sprite_rect_index], &load_obj.sprite.col[sprite_rect_index]);
				sprite_rect_index++;
			}
		}
	}
	// Close the file
	fclose(fptr);
//	printf("color %x", load_obj.sprite.col[sprite_rect_index]);	
	return load_obj;
}
