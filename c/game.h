static void error(int error, const char *desc)
{
        fputs(desc, stderr);
}

typedef struct circle 
{
    float sx1, sy1, sx2, sy2, tx1, ty1, tx2, ty2, x1, y1, x2, y2, radius1, radius2, radius3, radius4;
}CIRCLE;

CIRCLE circ;
CIRCLE *cp;

typedef struct test
{
    float distX1, distY1, distX2, distY2, dist1, dist2;
}TEST;

TEST tes;
TEST *tp;

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

void testCollisionLeft()
{
    //float distX1 = cp->sx1 – cp->tx1;
    //float distY1 = cp->sy1 – cp->ty1;
    //float dist1 = sqrt((distX1 * distX1) + (distY1 * distY1));
    //return dist1 <= (cp->radius1 + cp->radius2)
}

void testCollisionRight()
{
    //float distX2 = cp->sx2 – cp->tx2;
    //float distY2 = cp->sy2 – cp->ty2;
    //float dist2 = sqrt((distX2 * distX2) + (distY2 * distY2));
    //return dist2 <= (cp->radius3 + cp->radius4)
}



static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods)
{	    
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
                glfwSetWindowShouldClose(w, GL_TRUE);
                
        switch (key)
        {
            case GLFW_KEY_A:
                if (action == GLFW_PRESS)
                    printf("A_Key Response.\n");
                    break;
            case GLFW_KEY_L:
                if (action == GLFW_PRESS)
                    printf("L_Key Response.\n");
                    break;
        }
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


