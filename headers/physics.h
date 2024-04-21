#include <object_handler.h>
//                                GENERAL USE FUNCTIONS


void circle_intersection(float x1, float y1, float rad1, float x2, float y2, float rad2, float *offset_x, float *offset_y){
	// if they are the same point, give up
	if(x1 == x2 && y1 == y2)
		return;

	//calculate movement vector for obj 1
	float vect_x = (x1 - x2);
	float vect_y = (y1 - y2);
	float amplitude = distance(0, 0, vect_x, vect_y);
	vect_x /= amplitude;
	vect_y /= amplitude;

	//convert vector into coordinates to move by
	float dist = distance(x1, y1, x2, y2);
	if(dist < rad1 + rad2) { //too close
		float half_dist_to_move = ((rad1 + rad2) - dist) / 2;
		*offset_x = vect_x * half_dist_to_move;
		*offset_y = vect_y * half_dist_to_move;
	} else {
		*offset_x = 0;
		*offset_y = 0;
	}
	
	if(isinf(*offset_x)) *offset_x = 0;
	if(isinf(*offset_y)) *offset_y = 0;
}

void rotate_around_origin(float *px, float *py, float theta){
	float c = cos(theta);
	float s = sin(theta);
	float original_x = *px;

	*px = c * (*px) - s * (*py);
	*py = s * (original_x) + c * (*py);
}

void rotate_around_point(float *px, float *py, float theta, float x, float y){
	float shifted_x = *px - x;
	float shifted_y = *py - y;

	rotate_around_origin(&shifted_x, &shifted_y, theta);

	*px = shifted_x + x;
	*py = shifted_y + y;
}

bool point_above_line(float x1, float y1, float x2, float y2, float px, float py){
	if(x1 == x2)
		return px > x1;

	float slope = (y2 - y1) / (x2 - x1);
	float y_intercept = y1 - slope * x1;

	return py < (px * slope + y_intercept);
}

void obj_add_force(int tag, float force_x, float force_y){
	objects[tag].collider.acceleration_x += force_x;
	objects[tag].collider.acceleration_y += force_y;
}
//simple verlet engine
//                                MAIN PHYSICS LOOP
void update_physics(){
	for(int i = 0; i <= max_tag; i++){
		if(!objects[i].empty && objects[i].collider.do_physics){
			float velocity_x = objects[i].transform.x - objects[i].transform.old_x;
			float velocity_y = objects[i].transform.y - objects[i].transform.old_y;
		
			objects[i].transform.old_x = objects[i].transform.x;
			objects[i].transform.old_y = objects[i].transform.y;
			
			objects[i].transform.x += (velocity_x + objects[i].collider.acceleration_x) * (1 - objects[i].collider.drag_coeff) + objects[i].collider.gravity_force_x;
			objects[i].transform.y += (velocity_y + objects[i].collider.acceleration_y) * (1 - objects[i].collider.drag_coeff) + objects[i].collider.gravity_force_y;
		}
	}

	//-----OBJECT COLLISIONS-----//
	for(int i = 0; i <= max_tag; i++){
		for(int j = i; j <= max_tag; j++){
			if(!objects[i].empty && !objects[j].empty && objects[i].collider.enabled && objects[j].collider.enabled && j != i){
				for(int c = 0; c < collider_amount; c++){
					for(int c2 = 0; c2 < collider_amount; c2++){
						//calculate world circle positions
						float rot_circle_x1 = objects[i].collider.circlex[c] + objects[i].transform.x;
						float rot_circle_y1 = objects[i].collider.circley[c] + objects[i].transform.y;
						rotate_around_point(&rot_circle_x1, &rot_circle_y1, objects[i].transform.rotation, objects[i].transform.x, objects[i].transform.y);

						float rot_circle_x2 = objects[j].collider.circlex[c2] + objects[j].transform.x;
						float rot_circle_y2 = objects[j].collider.circley[c2] + objects[j].transform.y;
						rotate_around_point(&rot_circle_x2, &rot_circle_y2, objects[j].transform.rotation, objects[j].transform.x, objects[j].transform.y);

						//move said circles
						float x_off = 0; float y_off = 0;
						circle_intersection(
							rot_circle_x1, rot_circle_y1, objects[i].collider.radius[c],
							rot_circle_x2, rot_circle_y2, objects[j].collider.radius[c2],
							&x_off, &y_off);

						objects[i].transform.x += x_off;
						objects[i].transform.y += y_off;
						objects[j].transform.x -= x_off;
						objects[j].transform.y -= y_off;
					}
				}
			}
		}
	}
}
