typedef struct circle CIRCLE;

static void error(int error, const char *desc)
{
        fputs(desc, stderr);
}

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods)
{
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

void drawStimulus(float cx, float cy) 
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
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();
}

void drawTarget(float cx, float cy) 
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
		glVertex2f(x + cx, y + cy);//output vertex 
        
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


