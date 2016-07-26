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

void drawStimulus(Circle * circle);
/**
 * draw a filled circle
 * @param circle
 */

void drawTarget(Circle circle);
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

void keyPress(int index, int key);
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
