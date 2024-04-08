//      Object Struct Defines
struct Transform {
	float x;
	float y;
	
	//only able to be rotated seperately, not by collisions
	float rotation;
	
	bool draw_on_top;
};

//TODO: FIX THIS MESS
struct Collider {
	float pointsx[32];
	float pointsy[32];
};

#define sprite_rects 16
struct Sprite {
	//used to draw rectangles
	//can have up to 16
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


//                                GENERAL USE FUNCTIONS


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
