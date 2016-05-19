#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *w;
GLFWcursor *cursor;

#ifndef Pi
# define Pi 3.141592653
#endif

#define GLFW_HAND_CURSOR   0x00036004

double cursor_x, cursor_y;
double pos_x, pos_y;
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

void writeData(GLFWwindow* w, double pos_x, double pos_y)
{
    FILE *f = fopen("data.txt", "a");
                            
    fprintf(f,"+++%0.3f: Cursor position: %f %f (%+f %+f)\n",
           glfwGetTime(),
           pos_x, pos_y, pos_x - cursor_x, pos_y - cursor_y);
    
    cursor_x = pos_x;
    cursor_y = pos_y;
    fclose(f);
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
        int i;
        float e[3] = {140,120,100};
        float m[3] = {90,80,70};
        float h[3] = {60,50,40};
        
        glfwSetErrorCallback(error);
        
        if (!glfwInit())
            exit(EXIT_FAILURE);

        w = glfwCreateWindow(1024, 768, "AxTest", NULL, NULL);
        if (!w)
        {
            glfwTerminate();
            return 1;
        }
        
        glfwMakeContextCurrent(w);
        cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);        
        glfwSetCursor(w, cursor);
        glfwSetCursorPosCallback(w, writeData);       
        glfwSetKeyCallback(w, key_callback);
        glfwGetFramebufferSize(w, &fb_width, &fb_height);
           
        do{
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, fb_width, fb_height, 0, 0, 1);
            glMatrixMode(GL_MODELVIEW);
                           
            drawCircle(1024 * 0.33,768 * 0.25,e[2]);
            drawCircle(1024 * 0.66,768 * 0.25,m[1]);
            
            drawTarget(1024 * 0.33, 768 * 0.75,50);
            drawTarget(1024 * 0.66, 768 * 0.75,50);

            glfwSwapBuffers(w);
            glfwPollEvents();
          }
        while (!glfwWindowShouldClose(w));
        
        glfwDestroyWindow(w);
        glfwDestroyCursor(cursor);
            
        glfwTerminate();
        exit(EXIT_SUCCESS);

        return 0;
}
