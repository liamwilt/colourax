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

#include "game.h"

struct circle 
{
    float cy;
};

int main(void)
{

        CIRCLE position[10];                
        position[0].cy = -100;
        position[1].cy = -200;
        position[2].cy = -300;
        position[3].cy = -400;
        position[4].cy = -500;
        position[5].cy = -600;
        position[6].cy = -700;
        position[7].cy = -800;
        position[8].cy = -900;
        position[9].cy = -1000;
        
        int velocity_t = 75;
        int velocity_e = 100;
        int velocity_m = 200;
        int velocity_h = 300;

        int i, fb_width, fb_height;
        
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
            
            //Tracks
            
            drawTrack01();
            drawTrack02();
            
            //Stimuli            
               
            glPushMatrix();
            glTranslatef(0,(float) glfwGetTime() * velocity_h,0);
            drawStimulus(1280 * 0.25, position[0].cy);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0,(float) glfwGetTime() * velocity_m,0);
            drawStimulus(1280 * 0.75, position[0].cy);
            glPopMatrix();
            
            //Targets
            
            glPushMatrix();
            glTranslatef(0,0,0);
            drawTarget(1280 * 0.25, 720 * 0.90);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0,0,0);
            drawTarget(1280 * 0.75, 720 * 0.90);
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
