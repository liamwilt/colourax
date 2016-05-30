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

int main(void)
{
        srand(time(NULL));
        
        //int trial_e = rand() % (150 + 1 - 110) + 110; randomized radii for original concept
        //int trial_m = rand() % (100 + 1 - 70) + 70;
        //int trial_h = rand() % (60 + 1 - 30) + 30;
        
        int velocity_t = 75;
        int velocity_e = 100;
        int velocity_m = 200;
        int velocity_h = 300;
        
        int start_pos[10] = {-100, -200, -300, -400, -500, -600, -700, -800, -900, -1000};

        int i, fb_width, fb_height;
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
            
            //Tracks
            
            drawTrack01();
            drawTrack02();
            
            //Stimuli            
               
            glPushMatrix();
            glTranslatef(0,(float) glfwGetTime() * velocity_e,0);
            drawCircle(1280 * 0.25, start_pos[0]);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0,(float) glfwGetTime() * velocity_m,0);
            drawCircle(1280 * 0.75, start_pos[1]);
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
