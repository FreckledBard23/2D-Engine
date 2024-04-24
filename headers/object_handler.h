//structs


struct Wall {
	float x1;
	float y1;
	float x2;
	float y2;

	bool exists;
	bool display;
};

#define max_walls 512
struct Wall walls[512];

struct Transform {
	float x;
	float y;
	
	//to calculate velocity for verlet
	float old_x;
	float old_y;
	
	//only able to be rotated seperately, not by collisions
	float rotation;
};

#define collider_amount 16
struct Collider {
	bool enabled;
	
	//hitboxes
	float circlex[collider_amount];
	float circley[collider_amount];
	float radius[collider_amount];
	int colliders_used;
	
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
	int collider_index = 0;
	
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
			if(str_in[0] == 'p')
				sscanf(str_in, "physics_enabled %d collider_enabled %d drag_force %f gravity_x %f gravity_y %f", &load_obj.collider.do_physics, &load_obj.collider.enabled, &load_obj.collider.drag_coeff, &load_obj.collider.gravity_force_x, &load_obj.collider.gravity_force_y);
			if(str_in[0] == 'c'){
				sscanf(str_in, "collider x %f y %f radius %f", &load_obj.collider.circlex[collider_index], &load_obj.collider.circley[collider_index], &load_obj.collider.radius[collider_index]);
				collider_index++;
			}
		}
	}
	load_obj.collider.colliders_used = collider_index;
	//prevent random accelerations
	load_obj.collider.acceleration_x = 0;
	load_obj.collider.acceleration_y = 0;
	// Close the file
	fclose(fptr);
	return load_obj;
}
