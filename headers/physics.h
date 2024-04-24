#include <object_handler.h>
//                                GENERAL USE FUNCTIONS


//                                          GENERATED WITH CHATGPT


// Function to check if a line segment intersects a circle
bool doesIntersect(float x1, float y1, float x2, float y2, float cx, float cy, float radius, float *closestX, float *closestY, float *distToCenter) {
    // Calculate direction vector of the line segment
    float dx = x2 - x1;
    float dy = y2 - y1;

    // Calculate vector from one endpoint of the line segment to the center of the circle
    float ex = cx - x1;
    float ey = cy - y1;

    // Calculate dot product of direction vector and vector to circle center
    float dotProduct = dx * ex + dy * ey;

    // Calculate squared length of the direction vector
    float lengthSquared = dx * dx + dy * dy;

    // Parameter t for the closest point on the line segment to the circle center
    float t;

    // Check if the closest point is beyond the endpoints of the line segment
    if (dotProduct <= 0) {
        t = 0;
    } else if (dotProduct >= lengthSquared) {
        t = 1;
    } else {
        t = dotProduct / lengthSquared;
    }

    // Closest point on the line segment to the circle center
    *closestX = x1 + t * dx;
    *closestY = y1 + t * dy;

    // Calculate distance between the closest point and the circle center
    *distToCenter = distance(*closestX, *closestY, cx, cy);
    // Check if the distance is less than or equal to the radius
    return *distToCenter <= radius;
}

// Function to calculate the dot product of two vectors
double dotProduct(double x1, double y1, double x2, double y2) {
    return x1 * x2 + y1 * y2;
}

// Function to calculate the reflection vector
void reflect(float Mx, float My, float x1, float y1, float x2, float y2, float *Rx, float *Ry) {
    // Calculate the direction vector of the line
    double lineVectorX = x2 - x1;
    double lineVectorY = y2 - y1;

    // Calculate the normal vector of the line
    double normalX = -lineVectorY; // Perpendicular vector to the line
    double normalY = lineVectorX;

    // Normalize the normal vector
    double magnitude = sqrt(normalX * normalX + normalY * normalY);
    normalX /= magnitude;
    normalY /= magnitude;

    // Calculate the dot product of M and N
    double dot = dotProduct(Mx, My, normalX, normalY);

    // Calculate the reflection vector
    *Rx = (float)(Mx - 2 * dot * normalX);
    *Ry = (float)(My - 2 * dot * normalY);
}

float calculateSlope(float x1, float y1, float x2, float y2) {
    // Calculate the slope of the line passing through points (x1, y1) and (x2, y2)
    return (y2 - y1) / (x2 - x1);
}

float calculateYIntercept(float x1, float y1, float x2, float y2) {
    // Calculate the y-intercept of the line passing through points (x1, y1) and (x2, y2)
    return y1 - calculateSlope(x1, y1, x2, y2) * x1;
}

void reflectPointOverLine(float *x, float *y, float x1, float y1, float x2, float y2) {
    // Calculate the slope and y-intercept of the line passing through points (x1, y1) and (x2, y2)
    float m = calculateSlope(x1, y1, x2, y2);
    float c = calculateYIntercept(x1, y1, x2, y2);

    // Calculate the perpendicular slope
    float perp_slope = -1 / m;

    // Calculate the equation of the perpendicular line passing through point (x, y)
    float perp_c = *y - perp_slope * *x;

    // Calculate the intersection point of the two lines
    float intersection_x = (c - perp_c) / (perp_slope - m);
    float intersection_y = m * intersection_x + c;

    // Calculate the reflection point
    *x = 2 * intersection_x - *x;
    *y = 2 * intersection_y - *y;
}


//                                          END OF CHATGPT CODE

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
		//MOVEMENT ACCORDING TO VERLET INTEGRATION
		if(!objects[i].empty && objects[i].collider.do_physics){
			float velocity_x = objects[i].transform.x - objects[i].transform.old_x;
			float velocity_y = objects[i].transform.y - objects[i].transform.old_y;
		
			objects[i].transform.old_x = objects[i].transform.x;
			objects[i].transform.old_y = objects[i].transform.y;
		
			objects[i].transform.x += (velocity_x + objects[i].collider.acceleration_x) * (1 - objects[i].collider.drag_coeff) + objects[i].collider.gravity_force_x;
			objects[i].transform.y += (velocity_y + objects[i].collider.acceleration_y) * (1 - objects[i].collider.drag_coeff) + objects[i].collider.gravity_force_y;
		}
	}
	//COLLISIONS WITH WALLS
	for(int i = 0; i <= max_tag; i++){
		if(!objects[i].empty){
			for(int w = 0; w < max_walls; w++){
				if(walls[w].exists){
					for(int c = 0; c < collider_amount; c++){
						float rot_circle_x = objects[i].collider.circlex[c] + objects[i].transform.x;
						float rot_circle_y = objects[i].collider.circley[c] + objects[i].transform.y;
						rotate_around_point(&rot_circle_x, &rot_circle_y, objects[i].transform.rotation, objects[i].transform.x, objects[i].transform.y);
						float closex, closey, d;
						bool hit = doesIntersect(walls[w].x1, walls[w].y1, walls[w].x2, walls[w].y2, rot_circle_x, rot_circle_y, objects[i].collider.radius[c], &closex, &closey, &d);
						if(hit){
							float reflect_vector_x; float reflect_vector_y;
							reflect(objects[i].transform.x - objects[i].transform.old_x,
								objects[i].transform.y - objects[i].transform.old_y,
								walls[w].x1, walls[w].y1, walls[w].x2, walls[w].y2,
								&reflect_vector_x, &reflect_vector_y);
	
							//copied code from circle intersection
							// if they are the same point, give up
							float dist = distance(rot_circle_x, rot_circle_y, closex, closey);
							if(dist != 0){
								float old_rot_circle_x = objects[i].collider.circlex[c] + objects[i].transform.old_x;
								float old_rot_circle_y = objects[i].collider.circley[c] + objects[i].transform.old_y;
								rotate_around_point(&old_rot_circle_x, &old_rot_circle_y, objects[i].transform.rotation, objects[i].transform.old_x, objects[i].transform.old_y);

								bool old_side = point_above_line(walls[w].x1, walls[w].y1, walls[w].x2, walls[w].y2, old_rot_circle_x, old_rot_circle_y);
								bool current_side = point_above_line(walls[w].x1, walls[w].y1, walls[w].x2, walls[w].y2, rot_circle_x, rot_circle_y);

								//calculate movement vector for obj 1
								float vect_x = (objects[i].transform.x - closex);
								float vect_y = (objects[i].transform.y - closey);
								float amplitude = distance(0, 0, vect_x, vect_y);
								vect_x /= amplitude;
								vect_y /= amplitude;
	
								vect_x *= objects[i].collider.radius[c] - dist;
								vect_y *= objects[i].collider.radius[c] - dist;
								
								objects[i].transform.x += vect_x;
								objects[i].transform.y += vect_y;
								
								//glitched through wall
								if(old_side != current_side){
									float new_x = rot_circle_x; float new_y = rot_circle_y;
									reflectPointOverLine(&new_x, &new_y, walls[w].x1, walls[w].y1, walls[w].x2, walls[w].y2);
									float return_vector_x = new_x - rot_circle_x;
									float return_vector_y = new_y - rot_circle_y;

									objects[i].transform.x += return_vector_x;
									objects[i].transform.y += return_vector_y;
								}

								objects[i].transform.old_x = objects[i].transform.x - reflect_vector_x;
								objects[i].transform.old_y = objects[i].transform.y - reflect_vector_y;
							}
						}
					}
				}
			}
		}
	}
	
	//OBJECT COLLISIONS
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
