#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define M_PI        3.141592653
#define SIDE_NUM    36          // numnber of triangles we use to draw a circle  
#define RADIUS      50          // radius of a circle 
#define TOTAL_TIME  5.0         // 5 minutes 
#define NOTES       2           // number of node in the application
#define COLOR_NUM   3           // number of color RGB

#define Target_Y    RADIUS
#define Stimulus_Y  (SCREEN_HEIGHT - RADIUS)
#define SPEED       100         // moving speed

#define SLEEP_TIME  1           // 1 seconds
#define ALPHA_TIME  0.25    // 0.25 seconds
#define BLACK       255
#define WHITE       0
#define TRUE        1
#define FALSE       0

struct Circle {
    GLfloat x; // center points
    GLfloat y;
    GLfloat z;
    GLfloat radius; // circle radius
    GLint numberOfSides; // number of side
    GLfloat xVector; // horizontal vecotr
    GLfloat yVector; // vertical vector
    GLfloat colour[3]; // color of the circle
    int movable;
    GLfloat alphaStartTime;
};

typedef struct Circle Circle;

int keys[NOTES] = {GLFW_KEY_A, GLFW_KEY_L};
FILE * out;

Circle stimuliCircles[NOTES];
Circle targetCircles[NOTES];

void drawStimulus(Circle circle);
/**
 * draw a filled circle
 * @param           circle
 */

void drawTarget(Circle circle);
/**
 * draw a hollow circle
 * @param           circle
 */

void writeData(int state, int key);
/**
 * write to output.txt
 * @param           state correct or incorrect
 * @param key       the pressed key
 */

void keyPress(int index, int key);
/**
 * activate when a A or L is clicked
 * @param index     A is 0, L is 1
 * @param key       the pressed key
 */

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods);
/**
 * @param w
 * @param key
 * @param scancode
 * @param action
 * @param mods
 */

Circle initCircle(GLfloat xVal, GLfloat yVal);
/**
 * initialize a circle 
 * @param circle
 * @return an initialized circle 
 */

void initCircleArray(Circle circle[], GLfloat yVal);
/**
 * initialize array of circles 
 * @param circle array of circle
 * @param yVal preset y-value
 */

void moveDownward(Circle * circle);
/**
 * update y value of the circle
 * @param circle
 */

static void error(int error, const char *desc);
/**
 * @param error
 * @param desc
 */

int main(void) {
 
    GLFWwindow *window;

    // application time 
    static GLfloat last = 0;
    // color changing time 
    GLfloat timer = glfwGetTime();
    GLfloat start;
    start = timer;
    last = timer;

    int i;
    int movedCircle;

    // total running time
    GLfloat total = TOTAL_TIME * 60; // 1 minutes has 60 seconds

    // output file name
    char text[] = "output.txt";
    // output stream
    out = fopen(text, "w");

    srand(time(NULL));

    initCircleArray(stimuliCircles, Stimulus_Y);
    initCircleArray(targetCircles, Target_Y);

    glfwSetErrorCallback(error);

    if (!glfwInit() || out == NULL) {
        return -1;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ax", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glfwSetKeyCallback(window, key_callback);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    movedCircle = 0;

    while (timer - start < total
            && !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        timer = glfwGetTime();

        for (i = 0; i < NOTES; i++) {
            if (stimuliCircles[i].movable == TRUE) {
                drawTarget(targetCircles[i]);
                drawStimulus(stimuliCircles[i]);
            } else {
                drawStimulus(stimuliCircles[i]);
                drawTarget(targetCircles[i]);
            }

        }
        // whether we should move the circle 
        if (timer - last > SLEEP_TIME) {
            last = timer;
            movedCircle = movedCircle % NOTES;
            moveDownward(&stimuliCircles[movedCircle]);
            movedCircle++;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    fclose(out);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void moveDownward(Circle * circle) {
    // printf("%f ", circle->y);
    int i;
    if (circle->movable == TRUE) {
        circle->y += circle->yVector * SPEED;
    } else if (glfwGetTime() - circle->alphaStartTime
            >= ALPHA_TIME) {
        circle->movable = TRUE;
        for (i = 0; i < COLOR_NUM; i++) {
            circle->colour[i] = WHITE;
        }
    }


    if (circle->y < 0 - RADIUS) {
        circle->y = Stimulus_Y;
    }
}

void initCircleArray(Circle circle[], GLfloat yVal) {
    int i;
    GLfloat step = (GLfloat) SCREEN_WIDTH / (GLfloat) NOTES;
    GLfloat xVal = step / 2.0;
    for (i = 0; i < NOTES; i++) {
        circle[i] = initCircle(xVal + step * i, yVal);
    }
}

Circle initCircle(GLfloat xVal, GLfloat yVal) {
    int i;
    Circle c;

    // set color to white
    for (i = 0; i < COLOR_NUM; i++) {
        c.colour[i] = WHITE;
    }

    c.xVector = 0;
    if (yVal != RADIUS) {
        c.yVector = -1;
    }

    c.movable = TRUE;
    c.numberOfSides = SIDE_NUM;
    c.x = xVal;
    c.y = yVal;
    c.z = 0;
    c.radius = RADIUS;
    return c;
}

static void error(int error, const char *desc) {
    fputs(desc, stderr);
}

static void key_callback(GLFWwindow *w, int key, int scancode,
        int action, int mods) {
    int i;
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, GL_TRUE);
    }
    for (i = 0; i < NOTES; i++) { // look for the right key
        if (key == keys[i]) {
            keyPress(i, key);
            break;
        }
    }
}

void keyPress(int index, int key) {
    Circle * c1 = &stimuliCircles[index];
    Circle * c2 = &targetCircles[index];
    int i;
    if (c1->movable == TRUE) { // the circle is moving 
        if (abs(c1->y - c2->y) < RADIUS * 2.0) { // the stimulus and target touch 
            // start to invisible 
            c1->alphaStartTime = glfwGetTime();
            // fade away 
            for (i = 0; i < COLOR_NUM; i++) {
                c1->colour[i] = BLACK;
            }
            // stopping 
            c1->movable = FALSE;
            // write to file 
            writeData(TRUE, key);
        } else { // the stimulus circle doesn't touch the target circle
            writeData(FALSE, key);
        }
    }
}

void writeData(int state, int key) {
    if (out != NULL) {
        if (state == TRUE) {
            fprintf(out, "%c_key correct\n", key);
        } else {
            fprintf(out, "%c_key incorrect\n", key);
        }
    }
}

void drawStimulus(Circle circle) {
    GLint numberOfVertices = 1 + circle.numberOfSides;

    GLfloat doublePi = 2.0f * M_PI;

    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    GLfloat colour[circle.numberOfSides * 3];

    int i;
    int j;
    for (i = 0; i < numberOfVertices; i++) {

        j = i * 3;
        colour[j] = circle.colour[0];
        colour[j + 1] = circle.colour[1];
        colour[j + 2] = circle.colour[2];

        circleVerticesX[i] = circle.x + (circle.radius * cos(i * doublePi / circle.numberOfSides));
        circleVerticesY[i] = circle.y + (circle.radius * sin(i * doublePi / circle.numberOfSides));
        circleVerticesZ[i] = circle.z;
    }

    GLfloat allCircleVertices[numberOfVertices * 3];

    for (i = 0; i < numberOfVertices; i++) {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
        allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
    glColorPointer(3, GL_FLOAT, 0, colour);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawTarget(Circle circle) {
    GLint numberOfVertices = circle.numberOfSides + 1;
    int i;
    int j;
    GLfloat doublePi = 2.0f * M_PI;

    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    GLfloat colour[circle.numberOfSides * 3];

    for (i = 0; i < numberOfVertices; i++) {
        j = i * 3;
        colour[j] = circle.colour[0];
        colour[j + 1] = circle.colour[1];
        colour[j + 2] = circle.colour[2];
        circleVerticesX[i] = circle.x +
                (circle.radius * cos(i * doublePi / circle.numberOfSides));
        circleVerticesY[i] = circle.y +
                (circle.radius * sin(i * doublePi / circle.numberOfSides));
        circleVerticesZ[i] = circle.z;
    }

    GLfloat allCircleVertices[numberOfVertices * 3];

    for (i = 0; i < numberOfVertices; i++) {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
        allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
    glColorPointer(3, GL_FLOAT, 0, colour);
    glDrawArrays(GL_LINE_STRIP, 0, numberOfVertices);
    glDisableClientState(GL_VERTEX_ARRAY);
}
