#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define M_PI        3.141592653
#define SIDE_NUM    36              // numnber of triangles we use to draw a circle  
#define RADIUS      50              // radius of a circle 
#define TOTAL_TIME  5.0             // 5 minute run-time 
#define BARS        2               // number of "bars" in the application
#define COLOR_NUM   3               // number of total colors used in RGB

#define TARGET_Y    RADIUS
#define STIMULUS_Y  (SCREEN_HEIGHT + (2*RADIUS))
#define SPEED       5               // movement speed

#define SLEEP_TIME  0.01         
#define DELAY_TIME  1           
#define ALPHA_TIME  0.33
#define ALPHA_STEP  1.0 / 250.0
#define BLACK       255
#define BLACK_F     0.0
#define WHITE       0
#define TRUE        1
#define FALSE       0


struct Circle {
    GLfloat x;                      // center points
    GLfloat y;
    GLfloat z;
    GLfloat radius;                 // circle radius
    GLint numberOfSides;            
    GLfloat xVector;                // horizontal vecotr
    GLfloat yVector;                // vertical vector
    GLfloat colour[3];              // color of the circle
    int movable;
    GLfloat invisibleStartTime;
    int fadeAway;
    GLfloat alpha;
};

typedef struct Circle Circle;

struct Node {
    Circle circle;
    struct Node * next;
};

typedef struct Node Node;

struct Queue {
    Node * head;
    Node * tail;
};

typedef struct Queue Queue;


Node * newNode(GLfloat xVal);
/**
 * create a new node
 * @return 
 */

Queue * newQueue();
/**
 * create a new queue
 * @return 
 */

void push_back(Queue * queue, int index);
/**
 * push new node to queue
 * @param queue
 */

void pop_front(Queue * queue);
/**
 * pop old node off queue
 * @param queue
 */

Node * get_front(Queue * queue);
/**
 * get head of the queue
 * @param queue
 * @return 
 */

void initArrayOfQueue(Queue * stimuliQ[BARS]);
/**
 * initialize the array,  
 * @param stimuliQ
 */

void drawQueue(Queue * q);
/**
 * draw all circles in a queue
 * @param q
 */

void drawAll(Queue * stimuliQ[BARS]);
/**
 * draw all circles
 * @param stimuliQ
 */

int keys[BARS] = {GLFW_KEY_A, GLFW_KEY_L};
FILE * out;

// array of queue of circles 
Queue * stimuli[BARS];
// array of target circles
Circle targetCircles[BARS];

void drawCircle(Circle * circle);
/**
 * draw a filled circle
 * @param circle
 */

void drawHollowCircle(Circle circle);
/**
 * draw a hollow circle
 * @param circle
 */

void writeData(int state, int key);
/**
 * write to output.txt
 * @param state correct or incorrect
 * @param key the pressed key
 */

void keyClick(int index, int key);
/**
 * fire when a A or L is clicked
 * @param index A is 0, L is 1
 * @param key the pressed key
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

void moveDownward(Queue * q);
/**
 * update y value of the circle
 * @param q queue of nodes of circle 
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
    static GLfloat lastDelay = 0;
    // color changing time 
    static GLfloat lastColorChange = 0;
    GLfloat timer = glfwGetTime();
    GLfloat start;
    start = timer;
    last = timer;
    lastDelay = timer;


    int i;
    int movedCircle;

    GLfloat total = TOTAL_TIME * 60;

    char text[] = "output.txt";
    out = fopen(text, "w");

    Node * currentNode;

    // initialize all circles in the array
    initArrayOfQueue(stimuli);
    initCircleArray(targetCircles, TARGET_Y);

    glfwSetErrorCallback(error);

    if (!glfwInit() || out == NULL) {
        return -1;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dice", NULL, NULL);

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

    // set background color to white
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // enable blend mode to have transparent effects  
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    movedCircle = 0;
    // Loop until the user closes the window or program terminates
    while (timer - start < total
            && !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        timer = glfwGetTime();

        // set the correct order of appearance 
        for (i = 0; i < BARS; i++) {
            drawHollowCircle(targetCircles[i]);
            drawQueue(stimuli[i]);
        }
        // whether we should move the circle         
        if (timer - lastDelay > DELAY_TIME) {
            lastDelay = timer;
            movedCircle = movedCircle % BARS;
            push_back(stimuli[movedCircle], movedCircle);
            movedCircle++;
        }


        if (timer - last > SLEEP_TIME) {
            last = timer;
            for (i = 0; i < BARS; i++) {
                moveDownward(stimuli[i]);
            }
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    fclose(out);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void moveDownward(Queue * q) {

    Node * head;
    GLfloat timer;
    if (q != NULL) {
        head = q->head;
        while (head != NULL) {
            if (head->circle.movable == TRUE) {
                head->circle.y += head->circle.yVector * SPEED;
            }
            head = head->next;
        }

        head = q->head;
        timer = glfwGetTime();
        if (head != NULL) {
            if (head->circle.y < 0 - RADIUS) {
                pop_front(q);
            } else if (head->circle.fadeAway == TRUE
                    && head->circle.invisibleStartTime > 0
                    && timer - head->circle.invisibleStartTime > ALPHA_TIME) {
                pop_front(q);
            }
        }
    }
}

void initCircleArray(Circle circle[], GLfloat yVal) {
    int i;
    GLfloat step = (GLfloat) SCREEN_WIDTH / (GLfloat) BARS;
    GLfloat xVal = step / 2.0;
    for (i = 0; i < BARS; i++) {
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
    c.invisibleStartTime = -1;
    c.numberOfSides = SIDE_NUM;
    c.x = xVal;
    c.y = yVal;
    c.z = 0;
    c.radius = RADIUS;
    c.fadeAway = FALSE;
    c.alpha = 1.0;
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
    for (i = 0; i < BARS; i++) { // look for the right key
        if (key == keys[i]) {
            keyClick(i, key);
            break;
        }
    }
}

void keyClick(int index, int key) {
    Queue * q = stimuli[index];
    Circle * c2 = &targetCircles[index];
    Node * head = get_front(q);
    Circle * c1;
    int i;

    if (head != NULL) {
        c1 = &(head->circle);
        if (c1->movable == TRUE) { // the circle is moving 
            if (abs(c1->y - c2->y) < RADIUS * 2.0) { // the stimuli and target are touching 
                // begin to fade or modify alpha 
                c1->invisibleStartTime = glfwGetTime();
                // fade away 

                c1->fadeAway = TRUE;
                c1->alpha = 1.0;
                for (i = 0; i < COLOR_NUM; i++) {
                    c1->colour[i] = BLACK_F;
                }

                // stopping 
                c1->movable = FALSE;
                // write to file 
                writeData(TRUE, key);
            } else { // the stimuli is not touching the target
                writeData(FALSE, key);
            }
        }
    } else { 
        writeData(FALSE, key);
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

void drawCircle(Circle * circle) {
    GLint numberOfVertices = 1 + circle->numberOfSides;

    GLfloat doublePi = 2.0f * M_PI;

    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];

    int i;
    int j;
    for (i = 0; i < numberOfVertices; i++) {

        circleVerticesX[i] = circle->x + (circle->radius * cos(i * doublePi / circle->numberOfSides));
        circleVerticesY[i] = circle->y + (circle->radius * sin(i * doublePi / circle->numberOfSides));
        circleVerticesZ[i] = circle->z;
    }

    for (i = 0; i < numberOfVertices; i++) {
        glBegin(GL_TRIANGLES);
        glColor4f(circle->colour[0], circle->colour[1], circle->colour[2], circle->alpha);
        glVertex3f(circle->x, circle->y, circle->z);
        glVertex3f(circleVerticesX[i], circleVerticesY[i], circle->z);
        glVertex3f(circleVerticesX[(i + 1) % numberOfVertices], circleVerticesY[(i + 1) % numberOfVertices], circle->z);
        glEnd();
    }

    if (circle->fadeAway == TRUE &&
            circle->alpha > 0.000) {
        circle->alpha -= ALPHA_STEP;
    }
}

void drawHollowCircle(Circle circle) {
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

void drawAll(Queue * stimuliQ[BARS]) {
    int i;
    for (i = 0; i < BARS; i++) {
        drawQueue(stimuliQ[i]);
    }
}

void drawQueue(Queue * q) {
    Node * head;
    if (q != NULL) {
        head = q->head;
        while (head != NULL) {
            drawCircle(&(head->circle));
            head = head->next;
        }
    }
}

void initArrayOfQueue(Queue * stimuliQ[BARS]) {
    int i;
    for (i = 0; i < BARS; i++) {
        stimuliQ[i] = newQueue();
    }
}

Node * get_front(Queue * queue) {
    if (queue != NULL &&
            queue->head != NULL) {
        return queue->head;
    }
    return NULL;
}

void pop_front(Queue * queue) {
    Node * tmp;
    if (queue != NULL &&
            queue->head != NULL) {
        tmp = queue->head;
        queue->head = tmp->next;
        if (queue->head == NULL) {
            queue->tail = NULL;
        }
        tmp->next = NULL;
        free(tmp);
    }
}

void push_back(Queue * queue, int index) {
    Node * n;
    GLfloat step = (GLfloat) SCREEN_WIDTH / (GLfloat) BARS;
    GLfloat xVal = step / 2.0;
    xVal += step * index;
    n = newNode(xVal);
    if (queue->head == NULL) {
        queue->head = n;
        queue->tail = n;
    } else {
        queue->tail->next = n;
        queue->tail = n;
    }
}

Queue * newQueue() {
    Queue * q = (Queue *) malloc(sizeof (Queue));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

Node * newNode(GLfloat xVal) {
    Node * n = (Node *) malloc(sizeof (Node));
    n->circle = initCircle(xVal, STIMULUS_Y);
    n->next = NULL;
    return n;
}
