typedef struct circle 
{
    float sx1, sy1, sx2, sy2, tx1, ty1, tx2, ty2, x1, y1, x2, y2, radius1, radius2;
}CIRCLE;

static void error(int error, const char *desc)
{
        fputs(desc, stderr);
}

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods)
{
        // Calculate difference between centres
	    // distX = Center1.X – Center2.X
	    // distY = Center1.Y – Center2.Y
	    // Get distance with Pythagoras
	    // dist = sqrt((distX * distX) + (distY * distY))
	    
	return dist <= (R1 + R2)

        if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
                glfwSetWindowShouldClose(w, GL_TRUE);
                
        switch (key)
        {
            case GLFW_KEY_A:
                if (action == GLFW_PRESS)
                    printf("A_Key Pressed.\n");
                    break;
            case GLFW_KEY_L:
                if (action == GLFW_PRESS)
                    printf("L_Key Pressed.\n");
                    break;
        }
}

void drawStimulus(float sx, float sy) 
{ 
	float theta = 2 * 3.1415926 / 360; 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = 50;
	float y = 0; 
    
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 0, 1); 
	for(int ii = 0; ii < 360; ii++) 
	{ 
		glVertex2f(x + sx, y + sy);//output vertex 
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();
}

void drawTarget(float tx, float ty) 
{ 
	float theta = 2 * 3.1415926 / 360; 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = 50; 
	float y = 0; 
    
	glBegin(GL_LINE_LOOP);
	glColor4f(1, 1, 1, 0.50); 
	for(int ii = 0; ii < 360; ii++) 
	{ 
		glVertex2f(x + tx, y + ty);//output vertex 
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd(); 
}

void drawTrack01()
{
    glLineWidth(1.0);
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(1280 * 0.25,0.0,0.0);
    glVertex3f(1280 * 0.25,720*0.90-50,0.0);
    glEnd();
}

void drawTrack02()
{
    glLineWidth(1.0);
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(1280 * 0.75,0.0,0.0);
    glVertex3f(1280 * 0.75,720*0.90-50,0.0);
    glEnd();
}


