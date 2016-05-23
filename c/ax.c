#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *w;

int fb_width, fb_height;

static void error(int error, const char *desc)
{
        fputs(desc, stderr);
}

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods)
{
        if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
                glfwSetWindowShouldClose(w, GL_TRUE);
}

void drawCircle(float cx, float cy, float radius) 
{ 
    float num_segments = 360;
	float theta = 2 * 3.1415926 / num_segments; 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = radius;//we start at angle = 0 
	float y = 0; 
    
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 0, 1); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();
}

void drawTarget(float cx, float cy, float radius) 
{ 
    float num_segments = 360;
	float theta = 2 * 3.1415926 / num_segments; 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = radius;//we start at angle = 0 
	float y = 0; 
    
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd(); 
}


int main(void)
{
        srand(time(NULL));
        int trial_e = rand() % (150 + 1 - 110) + 110;
        int trial_m = rand() % (100 + 1 - 70) + 70;
        int trial_h = rand() % (60 + 1 - 30) + 30;
        
        int train;
        int velocity_e;
        int velocity_m;
        int velocity_h;

        int i;
        //float e[3] = {150,130,110};
        //float m[3] = {90,80,70};
        //float h[3] = {60,50,40};
        
        glfwSetErrorCallback(error);
        
        if (!glfwInit())
            exit(EXIT_FAILURE);

        w = glfwCreateWindow(1280, 720, "AxTest", NULL, NULL);
        if (!w)
        {
            glfwTerminate();
            return 1;
        }
        
        glfwMakeContextCurrent(w);
       
        glfwSetKeyCallback(w, key_callback);
        glfwGetFramebufferSize(w, &fb_width, &fb_height);
        glfwSetWindowPos(w, 300, 150);
           
        do{
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, fb_width, fb_height, 0, 0, 1);
            glMatrixMode(GL_MODELVIEW);
            
            //Stimuli            
               
            glPushMatrix();
            glTranslatef(0,(float) glfwGetTime() * 100,0);
            drawCircle(1280 * 0.25, -100, trial_m);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0,(float) glfwGetTime() * 200,0);
            drawCircle(1280 * 0.75, -200, trial_e);
            glPopMatrix();
            
            //Targets
            
            glPushMatrix();
            glTranslatef(0,0,0);
            drawTarget(1280 * 0.25, 720 * 0.75,50);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0,0,0);
            drawTarget(1280 * 0.75, 720 * 0.75,50);
            glPopMatrix();

            glfwSwapBuffers(w);
            glfwPollEvents();
          }
        while (!glfwWindowShouldClose(w));
        
        glfwDestroyWindow(w);
            
        glfwTerminate();
        exit(EXIT_SUCCESS);

        return 0;
}
