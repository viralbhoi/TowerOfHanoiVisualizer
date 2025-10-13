/*

--------------------------------------
|  Subject: Programming Lab (2CP08)  |
|  Faculty: Dr. N. M. Patel          |
|                                    |
|  Project Title - Tower Of Hanoi    |
|                                    |
|  Members:-                         |
|  23CP027 - Akshay Doshi            |
|  23CP029 - Viral Bhoi              |
|  23CP038 - Jay Patel               |
--------------------------------------

*/

#include <windows.h>
#include "glut.h"
#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

int NUM_DISKS = 5;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float DISK_HEIGHT = 20;
const float ROD_WIDTH = 10;

vector<stack<int>> rods(3);
vector<vector<float>> diskColors;

struct Move {
    int from, to;
};

vector<Move> moves;
int moveIndex = 0;
string currentMoveText = "";

// Converts rod index to name A/B/C
char rodName(int index) {
    return 'A' + index;
}

// Utility: Render text on screen at position (x, y)
void renderBitmapText(float x, float y, void *font, string text) {
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(font, c);
}

// Generate random colors for each disk
void generateColors(int n) {
    diskColors.clear();
    for (int i = 0; i < n; ++i) {
        diskColors.push_back({
            (float)(rand() % 256) / 255.0f,
            (float)(rand() % 256) / 255.0f,
            (float)(rand() % 256) / 255.0f
        });
    }
}

// Draws a disk on rodIndex at vertical position "position"
void drawDisk(int rodIndex, int position, int diskSize) {
    float baseX = (rodIndex + 1) * WINDOW_WIDTH / 4;
    float centerY = 100 + position * DISK_HEIGHT;
    float diskWidth = 30 + diskSize * 20;

    float left = baseX - diskWidth / 2;
    float right = baseX + diskWidth / 2;
    float top = centerY + DISK_HEIGHT / 2;
    float bottom = centerY - DISK_HEIGHT / 2;

    glColor3f(diskColors[diskSize][0], diskColors[diskSize][1], diskColors[diskSize][2]);

    glBegin(GL_QUADS);
        glVertex2f(left, bottom);
        glVertex2f(right, bottom);
        glVertex2f(right, top);
        glVertex2f(left, top);
    glEnd();
}

// Draws a vertical rod
void drawRod(int index) {
    float x = (index + 1) * WINDOW_WIDTH / 4;

    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(x - ROD_WIDTH / 2, 100);
        glVertex2f(x + ROD_WIDTH / 2, 100);
        glVertex2f(x + ROD_WIDTH / 2, 400);
        glVertex2f(x - ROD_WIDTH / 2, 400);
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw rods
    for (int i = 0; i < 3; ++i)
        drawRod(i);

    // Draw disks on each rod from bottom to top
    for (int i = 0; i < 3; ++i) {
        vector<int> disks;
        stack<int> temp = rods[i];
        while (!temp.empty()) {
            disks.push_back(temp.top());
            temp.pop();
        }
        for (int j = disks.size() - 1, pos = 0; j >= 0; --j, ++pos) {
            drawDisk(i, pos, disks[j]);
        }
    }

    // Display move text
    glColor3f(0, 0, 0);
    renderBitmapText(250, 500, GLUT_BITMAP_HELVETICA_18, currentMoveText);

    glutSwapBuffers();
}

// Timer callback to perform the next move
void timer(int) {
    if (moveIndex < moves.size()) {
        Move m = moves[moveIndex];
        if (!rods[m.from].empty()) {
            int disk = rods[m.from].top();
            rods[m.from].pop();
            rods[m.to].push(disk);

            // Display movement info
            currentMoveText = "Moving disk " + to_string(disk + 1) +
                              " from Rod " + rodName(m.from) +
                              " to Rod " + rodName(m.to);
        }

        moveIndex++;
        glutPostRedisplay();
        glutTimerFunc(2000, timer, 0); // delay of 1.5 seconds
    }
}

// Recursive Tower of Hanoi move generation
void hanoi(int n, int from, int to, int aux) {
    if (n == 0) return;
    hanoi(n - 1, from, aux, to);
    moves.push_back({from, to});
    hanoi(n - 1, aux, to, from);
}

// Start/reset the animation
void startHanoiAnimation() {
    rods = vector<stack<int>>(3);
    moves.clear();
    moveIndex = 0;

    for (int i = NUM_DISKS - 1; i >= 0; --i)
        rods[0].push(i);

    generateColors(NUM_DISKS);
    hanoi(NUM_DISKS, 0, 2, 1);
    glutTimerFunc(2000, timer, 0); // initial 1s delay
    currentMoveText = "Starting Tower of Hanoi with " + to_string(NUM_DISKS) + " disks";
    glutPostRedisplay();
}

// Menu callback
void menu(int option) {
    if (option == 5 || option == 7 || option == 10) {
        NUM_DISKS = option;
        startHanoiAnimation();
    } else if (option == 0) {
        exit(0);
    }
}

// Create right-click menu
void createMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("5 Disks", 5);
    glutAddMenuEntry("7 Disks", 7);
    glutAddMenuEntry("10 Disks", 10);
    glutAddMenuEntry("Exit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initial OpenGL setup
void init() {
    srand(time(0));
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    startHanoiAnimation();
    createMenu();
}

// Entry point
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Tower of Hanoi - Animated with GLUT");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
