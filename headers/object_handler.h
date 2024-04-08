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
