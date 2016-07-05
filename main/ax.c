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

GLFWwindow *window;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define M_PI 3.141592653
#define RADIUS 50
#define CIRCLE_NUM <--- Undefined as the circles should be generated once every second
#define TARGET_NUM 2

struct Circle {
    GLfloat x;              // center points
    GLfloat y;
    GLfloat z;
    GLfloat radius;         // circle radius
    GLint numberOfSides;    // number of 'sides' (or triangles) used to draw the circles
    GLfloat xVector;        // horizontal vector
    GLfloat yVector;        // vertical vector
    GLfloat colour[3];      // color of the circle
};

typedef struct Circle Circle;

struct Target {
    GLfloat xT;              // center points
    GLfloat yT;
    GLfloat zT;
    GLfloat radiusT;         // circle radius
    GLint numberOfSides;    // number of 'sides' (or triangles) used to draw the circles
};

typedef struct Target Target;

int chosenCircleIndex = 0;
Circle * currentCircle = NULL;

void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides );

void drawTarget( GLfloat xT, GLfloat yT, GLfloat zT, GLfloat radiusT, GLint numberOfSides );

Circle initCircle(Circle circle);
/**
 * @param circle
 * @return an initialized circle 
 */

void initCircleArray(Circle circle[], int length);
/**
 * @param circle array
 * @param length    length of the array
 */
 
int testCollision(Circle c1, Circle c2);
/**
 * @param c1        circle 
 * @param c2        circle
 * @return 0        if they don't collide with each other 
 *         1        if they collide
 */
 
Target initTarget(Target target);
/**
 * @param circle
 * @return an initialized circle 
 */

void initTargetArray(Target target[], int length);
/**
 * @param circle array
 * @param length    length of the array
 */

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods);

static void error(int error, const char *desc);

int main( void )
{

    static GLfloat last = 0;
    GLfloat timer = glfwGetTime();
    GLfloat start;
    start = timer;
    last = timer;
    
    Circle circles[CIRCLE_NUM];
    int length = CIRCLE_NUM;
    int i;
    
    Target targets[TARGET_NUM];
    int length = TARGET_NUM;
    int j;
    
    // time to wait before  moving the cirle 
    GLfloat sleep = 0.03;
    // total running time
    GLfloat total = TOTAL_TIME * 60; // 1 minutes has 60 seconds
    
    initCircleArray(circles, length);
    initTargetArray(targets, length);
    
    glfwSetErrorCallback(error);
    
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "ax", NULL, NULL );
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    glfwMakeContextCurrent( window );
     
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT ); 
    glMatrixMode( GL_PROJECTION ); 
 
    glLoadIdentity( ); 
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1 ); 
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( ); 
    
    glfwSetKeyCallback(window, key_callback);
    
    while (timer - start < total
            && !glfwWindowShouldClose(window)) {
            
        glClear(GL_COLOR_BUFFER_BIT);
        timer = glfwGetTime();
        
        }


















void initCircleArray(Circle circle[], int length) {
    int i;
    
    // set position
    target[0].x = SCREEN_WIDTH * (1/4);
    target[1].x = SCREEN_WIDTH * (3/4);

    target[0].y = SCREEN_HEIGHT - RADIUS;
    target[1].y = SCREEN_HEIGHT - RADIUS;

    // init circles    
    for (i = 0; i < length; i++) {
        circle[i] = initCircle(circle[i]);
    }

}

Circle initCircle(Circle circle) {
    Circle c = circle;
    int direction[] = {1, -1};
    c.numberOfSides = SIDE_NUM;
    c.radius = RADIUS;
    c.z = 0;

    c.xVector = 0.0;
    c.yVector = 0.0;

    // set central position
    while (c.xVector == 0.0) {
        c.xVector = (rand() % DISTANCE) * direction[rand() % 2];
    }

    while (c.yVector == 0.0) {
        c.yVector = (rand() % DISTANCE) * direction[rand() % 2];
    }
    // set default color to blue
    c.colour[0] = 0;
    c.colour[1] = 0;
    c.colour[2] = 255;
    return c;
}

void initTargetArray(Target target[], int length) {
    int i;
    
    // set position
    target[0].x = SCREEN_WIDTH * (1/4);
    target[1].x = SCREEN_WIDTH * (3/4);

    target[0].y = SCREEN_HEIGHT * (5/6);
    target[1].y = SCREEN_HEIGHT * (5/6);

    // init targets    
    for (i = 0; i < length; i++) {
        target[i] = initTarget(target[i]);
    }
}

Target initTarget(Target target) {
    Target t = target;
    t.numberOfSides = SIDE_NUM;
    t.radiusT = RADIUS;
    t.z = 0;

    return t;
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

static void error(int error, const char *desc)
{
        fputs(desc, stderr);
}

void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides )
{
    GLint numberOfVertices = numberOfSides + 1;
    
    GLfloat doublePi = 2.0f * M_PI;
    
    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    
    int i;
    for ( i = 0; i < numberOfVertices; i++ )
    {
        circleVerticesX[i] = x + ( radius * cos( i * doublePi / numberOfSides ) );
        circleVerticesY[i] = y + ( radius * sin( i * doublePi / numberOfSides ) );
        circleVerticesZ[i] = z;
    }
    
    GLfloat allCircleVertices[numberOfVertices * 3];
    
    for (  i = 0; i < numberOfVertices; i++ )
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[( i * 3 ) + 1] = circleVerticesY[i];
        allCircleVertices[( i * 3 ) + 2] = circleVerticesZ[i];
    }
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glColor3f(1, 0, 1);
    glVertexPointer( 3, GL_FLOAT, 0, allCircleVertices );
    glDrawArrays( GL_TRIANGLE_FAN, 0, numberOfVertices );
    glDisableClientState( GL_VERTEX_ARRAY );
}

void drawTarget( GLfloat xT, GLfloat yT, GLfloat zT, GLfloat radiusT, GLint numberOfSides )
{
    GLint numberOfVertices = numberOfSides + 1;
    
    GLfloat doublePi = 2.0f * M_PI;
    
    GLfloat targetVerticesX[numberOfVertices];
    GLfloat targetVerticesY[numberOfVertices];
    GLfloat targetVerticesZ[numberOfVertices];
    
    int j;
    for ( j = 0; j < numberOfVertices; j++ )
    {
        targetVerticesX[j] = x + ( radius * cos( i * doublePi / numberOfSides ) );
        targetVerticesY[j] = y + ( radius * sin( i * doublePi / numberOfSides ) );
        targetVerticesZ[j] = z;
    }
    
    GLfloat allCircleVertices[numberOfVertices * 3];
    
    for (  j = 0; j < numberOfVertices; j++ )
    {
        allCircleVertices[j * 3] = targetVerticesX[j];
        allCircleVertices[( j * 3 ) + 1] = targetVerticesY[j];
        allCircleVertices[( j * 3 ) + 2] = targetVerticesZ[j];
    }
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, allCircleVertices );
    glDrawArrays( GL_LINE_LOOP, 0, numberOfVertices );
    glDisableClientState( GL_VERTEX_ARRAY );
}

