void hex_color_to_glut(uint32_t color){
	glColor3f((float)(0xFF0000 & color) / 16777216, (float)(0xFF00 & color) / 65536, (float)(0xFF & color) / 256);
}

void draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
	glBegin(GL_LINES);
	hex_color_to_glut(color);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

void setPixel(uint32_t color, int x, int y){
	glBegin(GL_POINTS);
	hex_color_to_glut(color);
	glVertex2i(x, y);
	glEnd();
}

//takes in center x and y and x and y side lengths to draw a filled rectangle
void draw_box_filled(int x, int y, uint32_t color, int xside, int yside){
    	glBegin(GL_POLYGON);
	hex_color_to_glut(color);
   	glVertex2i(x - xside / 2, y - yside / 2); 
   	glVertex2i(x - xside / 2, y + yside / 2); 
   	glVertex2i(x + xside / 2, y + yside / 2);  
   	glVertex2i(x + xside / 2, y - yside / 2); 
	glEnd();
}

//takes in centerx and y with side lengths and theta to rotate a rectangle
void draw_box_rotated(int x, int y, uint32_t color, int xside, int yside, float theta){
	float halfx = xside / 2;
	float halfy = yside / 2;
	float px1 = x - halfx;
	float py1 = y - halfy;
	float px2 = x - halfx;
	float py2 = y + halfy;
	float px3 = x + halfx;
	float py3 = y + halfy;
	float px4 = x + halfx;
	float py4 = y - halfy;

	float s = sin(theta);
	float c = cos(theta);
	float rot_x1 = c * (px1 - x) - s * (py1 - y) + x;
	float rot_y1 = s * (px1 - x) + c * (py1 - y) + y;
	float rot_x2 = c * (px2 - x) - s * (py2 - y) + x;
	float rot_y2 = s * (px2 - x) + c * (py2 - y) + y;
	float rot_x3 = c * (px3 - x) - s * (py3 - y) + x;
	float rot_y3 = s * (px3 - x) + c * (py3 - y) + y;
	float rot_x4 = c * (px4 - x) - s * (py4 - y) + x;
	float rot_y4 = s * (px4 - x) + c * (py4 - y) + y;

	glBegin(GL_POLYGON);
	hex_color_to_glut(color);
   	glVertex2i(rot_x1, rot_y1); 
   	glVertex2i(rot_x2, rot_y2); 
   	glVertex2i(rot_x3, rot_y3);  
   	glVertex2i(rot_x4, rot_y4); 
	glEnd();
}
