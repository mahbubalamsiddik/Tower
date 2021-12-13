#include<windows.h>
#include <stdio.h>  // this library is for standard input and output
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Gl/glut.h>// this library is for glut the OpenGL Utility Toolkit
#include <conio.h>
float speed = 0.0f;
float carx = 0;
bool bridgeMoving = false;
bool bridgeOpening = false;
/////////////////////////////////////////////Rain Code Start/////////////////////////////////////////////////////
#define RAINSIZE 50
int winWidth = 1000, winHeight = 1000;
int counter = 0;
time_t t;
float rotationAngle = 0;
int skyR = 135, skyG = 206, skyB = 235;
bool isDay = false;
bool isRaining = false;

struct drop
{
    float x = 400;
    float y = 400;
    float inc = 0.01;
    float radius = 5;
    float scale = 1.0;
    float rotationAngle = 0;
    float rotationInc = 1;
};

drop rain[RAINSIZE];

void initRain()
{
    srand((unsigned)time(&t));
    for (int i = 0; i < RAINSIZE; i++)
    {
        rain[i].x = rand() % winWidth;
        rain[i].y = rand() % winHeight;
        rain[i].inc = 1.5 + (float)(rand() % 100) / 1000.0;
        rain[i].radius = (float)(rand() % 8);
        rain[i].scale = (float)(rand() % 20000) / 1000.0;
        rain[i].rotationAngle = (float)(rand() % 3000) / 1000.0;
        rain[i].rotationInc = (float)(rand() % 100) / 1000.0;
        if ((rand() % 100) > 50)
        {
            rain[i].rotationInc = -rain[i].rotationInc;
        }
    }
}

void drawParticleShape(int i)
{
    glBegin(GL_POINTS);
    glVertex2d(rain[i].x, rain[i].y);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(rain[i].x, rain[i].y);
    glVertex2d(rain[i].x, rain[i].y + rain[i].radius * 2);
    glEnd();
}

void drawDrop(int i)
{
    glColor3f(242.0, 242.0, 242.0);
    glLineWidth(2);
    drawParticleShape(i);
    rain[i].y -= rain[i].inc;
    if (rain[i].y < 0)
    {
        rain[i].y = winHeight;
    }
}

void drawRain()
{
    for (int i = 0; i < RAINSIZE; i++)
    {
        drawDrop(i);
    }
}
float elapsedTime = 0, base_time = 0, fps = 0, frames;

void calcFPS()
{
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    if ((elapsedTime - base_time) > 1000.0)
    {
        fps = frames * 1000.0 / (elapsedTime - base_time);
        printf("fps: %f", fps);
        base_time = elapsedTime;
        frames = 0;
    }
    frames++;
}
/////////////////////////////////////////////Rain Code End/////////////////////////////////////////////////////
/////////////////////////////////////////////Background Code Start/////////////////////////////////////////////////////
void background()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3ub(255, 255, 255);

    glBegin(GL_QUADS);//Sky
    glColor3ub(skyR, skyG, skyB);
    glVertex2f(0, 600);
    glVertex2f(0, 300);
    glVertex2f(1000, 300);
    glVertex2f(1000, 600);

    glBegin(GL_QUADS);//Side
    glColor3ub(183, 177, 174);
    glVertex2f(0, 300);
    glVertex2f(0, 0);
    glVertex2f(1000, 0);
    glVertex2f(1000, 300);

    glEnd();


    glBegin(GL_POLYGON);//river
    glColor3ub(0, 105, 148);
    glVertex2f(425, 300);
    glVertex2f(100, 0);
    glVertex2f(900, 0);
    glVertex2f(575, 300);

    glEnd();

}



/////////////////////////////////////////////Background Code End/////////////////////////////////////////////////////
/////////////////////////////////////////////Sun Code Start/////////////////////////////////////////////////////
void circle(GLfloat rx, GLfloat ry, GLfloat x, GLfloat y)
{
    int i;
    float rad = 0;
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    for (i = 0; i <= 360; i++)
    {
        rad = i * (3.1416 / 180);
        glVertex2f(x + rx * cos(rad), y + ry * sin(rad));
    }
    glEnd();
}
void sun()
{
    glColor3ub(249.0, 215.0, 28.0);//sun color
    circle(50, 50, 700, 500);//sun possition
}
void make_cloud(int x, int y)
{
    glColor3f(1.0, 1.0, 1.0);
    circle(10, 10, x, y);
    circle(10, 10, x + 10, y);
    circle(10, 10, x + 30, y);
    circle(10, 10, x + 5, y - 10);
    circle(10, 10, x + 20, y - 10);
    circle(10, 10, x + 5, y + 10);
    circle(10, 10, x + 20, y + 10);
}



float tp3 = 0;
static float pm = 600.0;
float moveShip = 50.0f;
bool canNotPass = true;

void cloud_move_right(GLfloat t)
{
    tp3 = tp3 + t;
    if (tp3 > 1000)
        tp3 = 0;
    glutPostRedisplay();
}
void cloud()
{
    glPushMatrix();
    glTranslatef(tp3, 0, 0);
    make_cloud(0, 520);
    make_cloud(300, 480);
    make_cloud(700, 500);
    make_cloud(500, 520);
    //make_cloud(200,250);
    //make_cloud(-100,250);
    //make_cloud(-350,230);



    glPopMatrix();
    cloud_move_right(.1);
    glEnd();
}
void megh()
{
    circle(13, 4, 78, 71);
    circle(13, 4, 75, 67);
    circle(13, 4, 78, 62);
    circle(13, 4, 80, 65);
}
/////////////////////////////////////////////Cloud Code End/////////////////////////////////////////////////////
/////////////////////////////////////////////Plane Code Start//////////////////////////////////////////
void rectangle1(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    glBegin(GL_QUADS);
    glVertex2d(x1, y1);
    glVertex2d(x3, y3);
    glVertex2d(x4, y4);
    glVertex2d(x2, y2);
    glEnd();
}
void rectangle(int x, int y, int x1, int y1)
{
    glBegin(GL_QUADS);
    glVertex2d(x, y);
    glVertex2d(x, y1);
    glVertex2d(x1, y1);
    glVertex2d(x1, y);
    glEnd();
}
void make_plane()
{

    glColor3f(0.282, 0.239, 0.545);
    circle(20, 15, 0, 0);
    rectangle(0, -15, 70, 15);
    glColor3f(1.000, 0.000, 0.000);
    rectangle1(70, -15, 90, 35, 70, 15, 90, 40);
    glColor3f(1.0, 0.0, 0.0);
    rectangle1(30, 15, 50, 15, 60, 35, 55, 30);



    glColor3f(1.0, 1.0, 1.0);
    circle(5, 5, 10, 0);   //Windows
    circle(5, 5, 30, 0);
    circle(5, 5, 50, 0);
    circle(5, 5, 65, 0);
    rectangle(-5, -5, 0, 10);




}
void planeMove()
{
    pm = pm - .10;
    if (pm > 600)
        pm = -300;
    glutPostRedisplay();
}



void plane()
{
    planeMove();
    for (int i = 0; i < 1; i++)
    {



        glPushMatrix();
        glTranslatef(pm, 500, 0);
        make_plane();
        glPopMatrix();



    }
}

void ship()
{
    glPushMatrix();
    glTranslatef(0.0f, moveShip, 0.0f);
    glBegin(GL_POLYGON); //Dec starts from here
    glColor3ub(231, 232, 234); //1st floor
    glVertex2f(470.0f, 315.0f);
    glVertex2f(530.0f, 315.0f);
    glVertex2f(530.0f, 288.0f);
    glVertex2f(470.0f, 288.0f);
    glEnd();





    glBegin(GL_POLYGON);
    glColor3ub(214, 215, 219); //1st floor (middle ware)
    glVertex2f(465.0f, 315.0f);
    glVertex2f(535.0f, 315.0f);
    glVertex2f(535.0f, 320.0f);
    glVertex2f(465.0f, 320.0f);
    glEnd();





    glBegin(GL_POLYGON);
    glColor3ub(231, 232, 234); //second floor
    glVertex2f(470.0f, 315.0f);
    glVertex2f(530.0f, 315.0f);
    glVertex2f(530.0f, 288.0f);
    glVertex2f(470.0f, 288.0f);
    glEnd();





    //boat front
    //left side





    glBegin(GL_POLYGON);
    glColor3ub(12, 87, 118);
    glVertex2f(500.0f, 308.0f);
    glVertex2f(465.0f, 285.0f);
    glVertex2f(473.0f, 232.0f);
    glVertex2f(500.0f, 223.0f);
    glEnd();
    //right side
    glBegin(GL_POLYGON);
    glColor3ub(5, 71, 95);
    glVertex2f(500.0f, 308.0f);
    glVertex2f(535.0f, 285.0f);
    glVertex2f(527.0f, 232.0f);
    glVertex2f(500.0f, 223.0f);
    glEnd();





    //Bottom red border
    glBegin(GL_POLYGON);
    glColor3ub(203, 33, 42);
    glVertex2f(473.0f, 232.0f);
    glVertex2f(500.0f, 223.0f);
    glVertex2f(500.0f, 227.0f);
    glVertex2f(473.0f, 236.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3ub(203, 33, 42);
    glVertex2f(527.0f, 232.0f);
    glVertex2f(500.0f, 223.0f);
    glVertex2f(500.0f, 227.0f);
    glVertex2f(527.0f, 236.0f);
    glEnd();





    glPopMatrix();
}









/////////////////////////////////////////////Moon Code Start/////////////////////////////////////////////////////
void drawMoonCircle(float cx, float cy, float r, int num_segments)
{
    float theta = 3.1415926 * 2 / float(num_segments);
    float tangetial_factor = tanf(theta);//calculate the tangential factor
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    float radial_factor = cosf(theta);//calculate the radial factor

    float x = r;//we start at angle = 0

    float y = 0;

    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy);//output vertex

        float tx = -y;
        float ty = x;

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}
void moon()
{
    glColor3f(1.0f, 1.0f, 1.0f);//white color
    drawMoonCircle(760, 500, 40, 360);

    glColor3ub(23, 29, 32); //black
    drawMoonCircle(785, 510, 40, 360);
}
/////////////////////////////////////////////Moon Code End/////////////////////////////////////////////////////
/////////////////////////////////////////////Star Code Start/////////////////////////////////////////////////////
void star()
{


    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);

    glVertex2i(320, 470);

    glVertex2i(200, 550);

    glVertex2i(285, 560);

    glVertex2i(680, 520);

    glVertex2i(630, 480);

    glVertex2i(920, 530);

    glVertex2i(850, 580);

    glVertex2i(480, 480);

    glVertex2i(100, 490);

    glEnd();
}

/////////////////////////////////////////////Star Code End/////////////////////////////////////////////////////


void myInit(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPointSize(6.0);
    gluOrtho2D(0.0, 1000.0, 0.0, 600.0);

}

/////////////////////////////////////////////Display Code Start/////////////////////////////////////////////////////



void display()
{
    background();
    if(!isDay)
    {
        sun();
    }


    if(isDay)
    {
        moon();
        star();
    }
    cloud();
    ship();
    plane();
    if(isRaining)
    {
        drawRain();
    }
    calcFPS();

    counter++;
    glutPostRedisplay();


    //Left tower moving part
    glPushMatrix();
    glTranslatef(330, 105, 0);
    glRotatef(speed, 0.0f, 0.0f, 1.0f);
    glTranslatef(-330, -105, 0);
    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 128);//rgb color picker

    glVertex2f(330, 150); // x, y
    glVertex2f(330, 140); // x, y
    glVertex2f(500, 140); // x, y
    glVertex2f(500, 150); // x, y

    glEnd();

    glBegin(GL_LINES);

    glColor3f(0, 0, 128); // Red


    glVertex2f(330, 140);// x, y
    glVertex2f(500, 140);// x, y

    glEnd();


    glBegin(GL_LINE_LOOP);

    glColor3f(0, 0, 128); // Red


    glVertex2f(300, 105);// x, y
    glVertex2f(330, 110);// x, y
    glVertex2f(330, 140);// x, y
    glVertex2f(355, 115);// x, y
    glVertex2f(355, 140);// x, y
    glVertex2f(380, 118);// x, y
    glVertex2f(380, 140);// x, y
    glVertex2f(405, 123);// x, y
    glVertex2f(405, 140);// x, y
    glVertex2f(430, 127.5);// x, y
    glVertex2f(430, 140);// x, y
    glVertex2f(455, 132.5);// x, y
    glVertex2f(455, 140);// x, y
    glVertex2f(500, 140);// x, y

    glEnd();

    glPopMatrix();





    //Left tower upper left miner
    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(300, 430); // x, y
    glVertex2f(300, 400); // x, y
    glVertex2f(310, 400); // x, y
    glVertex2f(310, 430); // x, y

    glEnd();

    //Left tower upper left miner head
    glBegin(GL_POLYGON);
    glColor3ub(159, 135, 91);//rgb color picker

    glVertex2f(305, 445); // x, y
    glVertex2f(300, 430); // x, y
    glVertex2f(310, 430); // x, y

    glEnd();



    //Left tower upper middle miner
    glBegin(GL_POLYGON);
    glColor3ub(159, 135, 91);//rgb color picker

    glVertex2f(310, 430); // x, y
    glVertex2f(310, 400); // x, y
    glVertex2f(340, 400); // x, y
    glVertex2f(340, 430); // x, y

    glEnd();


    //Left tower upper middle miner head
    glBegin(GL_POLYGON);
    glColor3ub(159, 135, 91);//rgb color picker

    glVertex2f(325, 465); // x, y
    glVertex2f(310, 430); // x, y
    glVertex2f(340, 430); // x, y

    glEnd();


    //Left tower upper right miner
    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(340, 430); // x, y
    glVertex2f(340, 400); // x, y
    glVertex2f(350, 400); // x, y
    glVertex2f(350, 430); // x, y

    glEnd();


    //Left tower upper right miner head
    glBegin(GL_POLYGON);
    glColor3ub(159, 135, 91);//rgb color picker

    glVertex2f(345, 445); // x, y
    glVertex2f(340, 430); // x, y
    glVertex2f(350, 430); // x, y

    glEnd();

    //Left tower middle miner
    glBegin(GL_POLYGON);
    glColor3ub(211, 211, 211);//rgb color picker

    glVertex2f(325, 440); // x, y
    glVertex2f(315, 430); // x, y
    glVertex2f(315, 400); // x, y
    glVertex2f(335, 400); // x, y
    glVertex2f(335, 430); // x, y

    glEnd();


    //Right tower moving part

    glPushMatrix();
    glTranslatef(670, 105, 0);
    glRotatef(-speed, 0.0f, 0.0f, 1.0f);
    glTranslatef(-670, -105, 0);
    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 128);//rgb color picker

    glVertex2f(670, 150); // x, y
    glVertex2f(670, 140); // x, y
    glVertex2f(500, 140); // x, y
    glVertex2f(500, 150); // x, y

    glEnd();




    glBegin(GL_LINES);

    glColor3f(0, 0, 128); // Red


    glVertex2f(670, 140);// x, y
    glVertex2f(500, 140);// x, y

    glEnd();


    glBegin(GL_LINE_LOOP);

    glColor3f(0, 0, 128); // Red


    glVertex2f(700, 105);// x, y
    glVertex2f(670, 110);// x, y
    glVertex2f(670, 140);// x, y
    glVertex2f(645, 115);// x, y
    glVertex2f(645, 140);// x, y
    glVertex2f(620, 118);// x, y
    glVertex2f(620, 140);// x, y
    glVertex2f(595, 123);// x, y
    glVertex2f(595, 140);// x, y
    glVertex2f(570, 127.5);// x, y
    glVertex2f(570, 140);// x, y
    glVertex2f(545, 132.5);// x, y
    glVertex2f(545, 140);// x, y
    glVertex2f(500, 140);// x, y

    glEnd();

    glPopMatrix();





    //Right tower upper left miner
    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(650, 430); // x, y
    glVertex2f(650, 400); // x, y
    glVertex2f(660, 400); // x, y
    glVertex2f(660, 430); // x, y

    glEnd();

    //Right tower upper left miner head
    glBegin(GL_POLYGON);
    glColor3ub(159, 135, 91);//rgb color picker

    glVertex2f(655, 445); // x, y
    glVertex2f(650, 430); // x, y
    glVertex2f(660, 430); // x, y

    glEnd();



    //Right tower upper middle miner
    glBegin(GL_POLYGON);
    glColor3ub(159, 135, 91);//rgb color picker

    glVertex2f(660, 430); // x, y
    glVertex2f(660, 400); // x, y
    glVertex2f(690, 400); // x, y
    glVertex2f(690, 430); // x, y

    glEnd();


    //Right tower upper middle miner head
    glBegin(GL_POLYGON);
    glColor3ub(159, 135, 91);//rgb color picker

    glVertex2f(675, 465); // x, y
    glVertex2f(660, 430); // x, y
    glVertex2f(690, 430); // x, y

    glEnd();


    //Right tower upper right miner
    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(690, 430); // x, y
    glVertex2f(690, 400); // x, y
    glVertex2f(700, 400); // x, y
    glVertex2f(700, 430); // x, y

    glEnd();


    //Right tower upper right miner head
    glBegin(GL_POLYGON);
    glColor3ub(159, 135, 91);//rgb color picker

    glVertex2f(695, 445); // x, y
    glVertex2f(690, 430); // x, y
    glVertex2f(700, 430); // x, y

    glEnd();


    //Right tower middle miner
    glBegin(GL_POLYGON);
    glColor3ub(211, 211, 211);//rgb color picker

    glVertex2f(675, 440); // x, y
    glVertex2f(665, 430); // x, y
    glVertex2f(665, 400); // x, y
    glVertex2f(685, 400); // x, y
    glVertex2f(685, 430); // x, y

    glEnd();





 //Left side Car
    glPushMatrix();
    glTranslatef(carx, 0, 0);

    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);//rgb color picker

    glVertex2f(20, 170); // x, y
    glVertex2f(10, 160); // x, y
    glVertex2f(60, 160); // x, y
    glVertex2f(50, 170); // x, y

    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);//rgb color picker

    glVertex2f(0, 160); // x, y
    glVertex2f(0, 150); // x, y
    glVertex2f(75, 150); // x, y
    glVertex2f(75, 160); // x, y

    glEnd();

    // Left side car window

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(20, 167); // x, y
    glVertex2f(15, 160); // x, y
    glVertex2f(55, 160); // x, y
    glVertex2f(50, 167); // x, y

    glEnd();


    glBegin(GL_LINES);
    glColor3f(0, 0, 0); // Black

    glVertex2f(35, 170);// x, y
    glVertex2f(35, 160);// x, y

    glEnd();
    glPopMatrix();


    //Design part
    glBegin(GL_LINES);

    glColor3f(0, 0, 128); // Red


    glVertex2f(350, 400);// x, y
    glVertex2f(650, 400);// x, y

    glVertex2f(350, 380);// x, y
    glVertex2f(650, 380);// x, y
//Cross design part
    glVertex2f(350, 400);// x, y
    glVertex2f(370, 380);// x, y

    glVertex2f(370, 380);// x, y
    glVertex2f(390, 400);// x, y

    glVertex2f(390, 400);// x, y
    glVertex2f(410, 380);// x, y

    glVertex2f(410, 380);// x, y
    glVertex2f(430, 400);// x, y

    glVertex2f(430, 400);// x, y
    glVertex2f(450, 380);// x, y

    glVertex2f(450, 380);// x, y
    glVertex2f(470, 400);// x, y

    glVertex2f(470, 400);// x, y
    glVertex2f(490, 380);// x, y

    glVertex2f(490, 380);// x, y
    glVertex2f(510, 400);// x, y

    glVertex2f(510, 400);// x, y
    glVertex2f(530, 380);// x, y

    glVertex2f(530, 380);// x, y
    glVertex2f(550, 400);// x, y

    glVertex2f(550, 400);// x, y
    glVertex2f(570, 380);// x, y

    glVertex2f(570, 380);// x, y
    glVertex2f(590, 400);// x, y

    glVertex2f(590, 400);// x, y
    glVertex2f(610, 380);// x, y

    glVertex2f(610, 380);// x, y
    glVertex2f(630, 400);// x, y

    glVertex2f(630, 400);// x, y
    glVertex2f(650, 380);// x, y

    glEnd();


    //Left tower side road

    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 128);//rgb color picker

    glVertex2f(0, 150); // x, y
    glVertex2f(0, 135); // x, y
    glVertex2f(300, 135); // x, y
    glVertex2f(300, 150); // x, y

    glEnd();


    //Right tower side road

    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 128);//rgb color picker

    glVertex2f(700, 150); // x, y
    glVertex2f(700, 135); // x, y
    glVertex2f(1000, 135); // x, y
    glVertex2f(1000, 150); // x, y

    glEnd();



    //Right side Car

    glPushMatrix();
    glTranslatef(-carx, 0, 0);
    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 0);//rgb color picker

    glVertex2f(950, 170); // x, y
    glVertex2f(940, 160); // x, y
    glVertex2f(990, 160); // x, y
    glVertex2f(980, 170); // x, y

    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 0);//rgb color picker

    glVertex2f(925, 160); // x, y
    glVertex2f(925, 150); // x, y
    glVertex2f(1000, 150); // x, y
    glVertex2f(1000, 160); // x, y

    glEnd();

    // Right side car window

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(950, 167); // x, y
    glVertex2f(945, 160); // x, y
    glVertex2f(985, 160); // x, y
    glVertex2f(980, 167); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 0, 0); // Black

    glVertex2f(965, 170);// x, y
    glVertex2f(965, 160);// x, y

    glEnd();
    glPopMatrix();


    //Left tower
    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(300, 400); // x, y
    glVertex2f(300, 70); // x, y
    glVertex2f(350, 70); // x, y
    glVertex2f(350, 400); // x, y

    glEnd();

     //Left tower base
    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(290, 150); // x, y
    glVertex2f(290, 70); // x, y
    glVertex2f(360, 70); // x, y
    glVertex2f(360, 150); // x, y

    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(290, 70); // x, y
    glVertex2f(325, 65); // x, y
    glVertex2f(360, 70); // x, y

    glEnd();


    // Right Tower
    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(650, 400); // x, y
    glVertex2f(650, 70); // x, y
    glVertex2f(700, 70); // x, y
    glVertex2f(700, 400); // x, y

    glEnd();


    //Right Tower base
    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(640, 150); // x, y
    glVertex2f(640, 70); // x, y
    glVertex2f(710, 70); // x, y
    glVertex2f(710, 150); // x, y

    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(179, 151, 111);//rgb color picker

    glVertex2f(640, 70); // x, y
    glVertex2f(675, 65); // x, y
    glVertex2f(710, 70); // x, y

    glEnd();



    // Left side building 1

    glBegin(GL_QUADS);
    glColor3ub(237, 214, 108);//rgb color picker

    glVertex2f(0, 400); // x, y
    glVertex2f(0, 220); // x, y
    glVertex2f(30, 220); // x, y
    glVertex2f(30, 400); // x, y

    glEnd();


    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(10, 390); // x, y
    glVertex2f(10, 380); // x, y
    glVertex2f(20, 380); // x, y
    glVertex2f(20, 390); // x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(10, 360); // x, y
    glVertex2f(10, 350); // x, y
    glVertex2f(20, 350); // x, y
    glVertex2f(20, 360); // x, y

    glEnd();



    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(10, 330); // x, y
    glVertex2f(10, 320); // x, y
    glVertex2f(20, 320); // x, y
    glVertex2f(20, 330); // x, y

    glEnd();



    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(10, 300); // x, y
    glVertex2f(10, 290); // x, y
    glVertex2f(20, 290); // x, y
    glVertex2f(20, 300); // x, y

    glEnd();


    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(10, 270); // x, y
    glVertex2f(10, 260); // x, y
    glVertex2f(20, 260); // x, y
    glVertex2f(20, 270); // x, y

    glEnd();


    //window 6
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(10, 240); // x, y
    glVertex2f(10, 230); // x, y
    glVertex2f(20, 230); // x, y
    glVertex2f(20, 240); // x, y

    glEnd();




    // Left side building 2

    glBegin(GL_QUADS);
    glColor3ub(144, 143, 175);//rgb color picker

    glVertex2f(40, 430); // x, y
    glVertex2f(40, 240); // x, y
    glVertex2f(80, 240); // x, y
    glVertex2f(80, 430); // x, y

    glEnd();


    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(50, 420); // x, y
    glVertex2f(50, 410); // x, y
    glVertex2f(70, 410); // x, y
    glVertex2f(70, 420); // x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(50, 390); // x, y
    glVertex2f(50, 380); // x, y
    glVertex2f(70, 380); // x, y
    glVertex2f(70, 390); // x, y

    glEnd();


    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(50, 360); // x, y
    glVertex2f(50, 350); // x, y
    glVertex2f(70, 350); // x, y
    glVertex2f(70, 360); // x, y

    glEnd();



    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(50, 330); // x, y
    glVertex2f(50, 320); // x, y
    glVertex2f(70, 320); // x, y
    glVertex2f(70, 330); // x, y

    glEnd();



    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(50, 300); // x, y
    glVertex2f(50, 290); // x, y
    glVertex2f(70, 290); // x, y
    glVertex2f(70, 300); // x, y

    glEnd();


    //window 6
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(50, 270); // x, y
    glVertex2f(50, 260); // x, y
    glVertex2f(70, 260); // x, y
    glVertex2f(70, 270); // x, y

    glEnd();





    // Left side building 3

    glBegin(GL_QUADS);
    glColor3ub(246, 137, 132);//rgb color picker

    glVertex2f(100, 420); // x, y
    glVertex2f(100, 240); // x, y
    glVertex2f(150, 240); // x, y
    glVertex2f(150, 420); // x, y

    glEnd();

    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(110, 400); // x, y
    glVertex2f(110, 390); // x, y
    glVertex2f(140, 390); // x, y
    glVertex2f(140, 400); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(125, 400);// x, y
    glVertex2f(125, 390);// x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(110, 370); // x, y
    glVertex2f(110, 360); // x, y
    glVertex2f(140, 360); // x, y
    glVertex2f(140, 370); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(125, 370);// x, y
    glVertex2f(125, 360);// x, y

    glEnd();

    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(110, 340); // x, y
    glVertex2f(110, 330); // x, y
    glVertex2f(140, 330); // x, y
    glVertex2f(140, 340); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(125, 340);// x, y
    glVertex2f(125, 330);// x, y

    glEnd();

    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(110, 310); // x, y
    glVertex2f(110, 300); // x, y
    glVertex2f(140, 300); // x, y
    glVertex2f(140, 310); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(125, 310);// x, y
    glVertex2f(125, 300);// x, y

    glEnd();

    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(110, 280); // x, y
    glVertex2f(110, 270); // x, y
    glVertex2f(140, 270); // x, y
    glVertex2f(140, 280); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(125, 280);// x, y
    glVertex2f(125, 270);// x, y

    glEnd();


    // Left side building 4

    glBegin(GL_QUADS);
    glColor3ub(0, 255, 0);//rgb color picker

    glVertex2f(160, 450); // x, y
    glVertex2f(160, 270); // x, y
    glVertex2f(210, 270); // x, y
    glVertex2f(210, 450); // x, y

    glEnd();

    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(170, 440); // x, y
    glVertex2f(170, 430); // x, y
    glVertex2f(200, 430); // x, y
    glVertex2f(200, 440); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(185, 440);// x, y
    glVertex2f(185, 430);// x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(170, 410); // x, y
    glVertex2f(170, 400); // x, y
    glVertex2f(200, 400); // x, y
    glVertex2f(200, 410); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(185, 410);// x, y
    glVertex2f(185, 400);// x, y

    glEnd();

    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(170, 380); // x, y
    glVertex2f(170, 370); // x, y
    glVertex2f(200, 370); // x, y
    glVertex2f(200, 380); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(185, 380);// x, y
    glVertex2f(185, 370);// x, y

    glEnd();

    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(170, 350); // x, y
    glVertex2f(170, 340); // x, y
    glVertex2f(200, 340); // x, y
    glVertex2f(200, 350); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(185, 350);// x, y
    glVertex2f(185, 340);// x, y

    glEnd();

    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(170, 320); // x, y
    glVertex2f(170, 310); // x, y
    glVertex2f(200, 310); // x, y
    glVertex2f(200, 320); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(185, 320);// x, y
    glVertex2f(185, 310);// x, y

    glEnd();


    //window 6
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(170, 290); // x, y
    glVertex2f(170, 280); // x, y
    glVertex2f(200, 280); // x, y
    glVertex2f(200, 290); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(185, 290);// x, y
    glVertex2f(185, 280);// x, y

    glEnd();



    // Left side building 5

    glBegin(GL_QUADS);
    glColor3ub(157, 93, 179);//rgb color picker

    glVertex2f(220, 430); // x, y
    glVertex2f(220, 240); // x, y
    glVertex2f(260, 240); // x, y
    glVertex2f(260, 430); // x, y

    glEnd();

    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(230, 420); // x, y
    glVertex2f(230, 410); // x, y
    glVertex2f(250, 410); // x, y
    glVertex2f(250, 420); // x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(230, 390); // x, y
    glVertex2f(230, 380); // x, y
    glVertex2f(250, 380); // x, y
    glVertex2f(250, 390); // x, y

    glEnd();


    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(230, 360); // x, y
    glVertex2f(230, 350); // x, y
    glVertex2f(250, 350); // x, y
    glVertex2f(250, 360); // x, y

    glEnd();



    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(230, 330); // x, y
    glVertex2f(230, 320); // x, y
    glVertex2f(250, 320); // x, y
    glVertex2f(250, 330); // x, y

    glEnd();



    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(230, 300); // x, y
    glVertex2f(230, 290); // x, y
    glVertex2f(250, 290); // x, y
    glVertex2f(250, 300); // x, y

    glEnd();


    //window 6
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(230, 270); // x, y
    glVertex2f(230, 260); // x, y
    glVertex2f(250, 260); // x, y
    glVertex2f(250, 270); // x, y

    glEnd();





    //Background Buildings

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(30, 350); // x, y
    glVertex2f(30, 290); // x, y
    glVertex2f(40, 290); // x, y
    glVertex2f(40, 350); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(80, 370); // x, y
    glVertex2f(80, 300); // x, y
    glVertex2f(100, 300); // x, y
    glVertex2f(100, 370); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(150, 360); // x, y
    glVertex2f(150, 300); // x, y
    glVertex2f(160, 300); // x, y
    glVertex2f(160, 360); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(210, 350); // x, y
    glVertex2f(210, 290); // x, y
    glVertex2f(220, 290); // x, y
    glVertex2f(220, 350); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(260, 370); // x, y
    glVertex2f(260, 295); // x, y
    glVertex2f(280, 295); // x, y
    glVertex2f(280, 370); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(280, 360); // x, y
    glVertex2f(280, 300); // x, y
    glVertex2f(290, 300); // x, y
    glVertex2f(290, 360); // x, y

    glEnd();


    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(360, 340); // x, y
    glVertex2f(360, 300); // x, y
    glVertex2f(380, 300); // x, y
    glVertex2f(380, 340); // x, y

    glEnd();


    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(382.5, 320); // x, y
    glVertex2f(382.5, 300); // x, y
    glVertex2f(400, 300); // x, y
    glVertex2f(400, 320); // x, y

    glEnd();

    //School building Left side

    glBegin(GL_POLYGON);
    glColor3ub(209, 157, 107);//rgb color picker

    glVertex2f(50, 250); // x, y
    glVertex2f(50, 200); // x, y
    glVertex2f(250, 200); // x, y
    glVertex2f(250, 250); // x, y

    glEnd();

    //School roof Left side

    glBegin(GL_POLYGON);
    glColor3ub(133, 90, 55);//rgb color picker

    glVertex2f(60, 260); // x, y
    glVertex2f(50, 250); // x, y
    glVertex2f(250, 250); // x, y
    glVertex2f(245, 260); // x, y

    glEnd();

    //School window 1

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(70, 240); // x, y
    glVertex2f(70, 230); // x, y
    glVertex2f(80, 230); // x, y
    glVertex2f(80, 240); // x, y

    glEnd();


    //School window 2

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(120, 240); // x, y
    glVertex2f(120, 230); // x, y
    glVertex2f(130, 230); // x, y
    glVertex2f(130, 240); // x, y

    glEnd();

    //School window 3

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(170, 240); // x, y
    glVertex2f(170, 230); // x, y
    glVertex2f(180, 230); // x, y
    glVertex2f(180, 240); // x, y

    glEnd();

    //School window 4

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(220, 240); // x, y
    glVertex2f(220, 230); // x, y
    glVertex2f(230, 230); // x, y
    glVertex2f(230, 240); // x, y

    glEnd();


    //School window 5

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(70, 220); // x, y
    glVertex2f(70, 210); // x, y
    glVertex2f(80, 210); // x, y
    glVertex2f(80, 220); // x, y

    glEnd();


    //School window 6

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(120, 220); // x, y
    glVertex2f(120, 210); // x, y
    glVertex2f(130, 210); // x, y
    glVertex2f(130, 220); // x, y

    glEnd();

    //School window 7

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(170, 220); // x, y
    glVertex2f(170, 210); // x, y
    glVertex2f(180, 210); // x, y
    glVertex2f(180, 220); // x, y

    glEnd();

    //School window 8

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(220, 220); // x, y
    glVertex2f(220, 210); // x, y
    glVertex2f(230, 210); // x, y
    glVertex2f(230, 220); // x, y

    glEnd();


    //School door

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(140, 220); // x, y
    glVertex2f(140, 200); // x, y
    glVertex2f(160, 200); // x, y
    glVertex2f(160, 220); // x, y

    glEnd();




    // Right side building 1

    glBegin(GL_QUADS);
    glColor3ub(237, 214, 108);//rgb color picker

    glVertex2f(1000, 400); // x, y
    glVertex2f(1000, 220); // x, y
    glVertex2f(970, 220); // x, y
    glVertex2f(970, 400); // x, y

    glEnd();

    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(990, 390); // x, y
    glVertex2f(990, 380); // x, y
    glVertex2f(980, 380); // x, y
    glVertex2f(980, 390); // x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(990, 360); // x, y
    glVertex2f(990, 350); // x, y
    glVertex2f(980, 350); // x, y
    glVertex2f(980, 360); // x, y

    glEnd();



    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(990, 330); // x, y
    glVertex2f(990, 320); // x, y
    glVertex2f(980, 320); // x, y
    glVertex2f(980, 330); // x, y

    glEnd();



    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(990, 300); // x, y
    glVertex2f(990, 290); // x, y
    glVertex2f(980, 290); // x, y
    glVertex2f(980, 300); // x, y

    glEnd();


    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(990, 270); // x, y
    glVertex2f(990, 260); // x, y
    glVertex2f(980, 260); // x, y
    glVertex2f(980, 270); // x, y

    glEnd();


    //window 6
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(990, 240); // x, y
    glVertex2f(990, 230); // x, y
    glVertex2f(980, 230); // x, y
    glVertex2f(980, 240); // x, y

    glEnd();


    // Right side building 2

    glBegin(GL_QUADS);
    glColor3ub(0, 255, 0);//rgb color picker

    glVertex2f(960, 430); // x, y
    glVertex2f(960, 240); // x, y
    glVertex2f(920, 240); // x, y
    glVertex2f(920, 430); // x, y

    glEnd();

    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(950, 420); // x, y
    glVertex2f(950, 410); // x, y
    glVertex2f(930, 410); // x, y
    glVertex2f(930, 420); // x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(950, 390); // x, y
    glVertex2f(950, 380); // x, y
    glVertex2f(930, 380); // x, y
    glVertex2f(930, 390); // x, y

    glEnd();


    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(950, 360); // x, y
    glVertex2f(950, 350); // x, y
    glVertex2f(930, 350); // x, y
    glVertex2f(930, 360); // x, y

    glEnd();



    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(950, 330); // x, y
    glVertex2f(950, 320); // x, y
    glVertex2f(930, 320); // x, y
    glVertex2f(930, 330); // x, y

    glEnd();



    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(950, 300); // x, y
    glVertex2f(950, 290); // x, y
    glVertex2f(930, 290); // x, y
    glVertex2f(930, 300); // x, y

    glEnd();


    //window 6
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(950, 270); // x, y
    glVertex2f(950, 260); // x, y
    glVertex2f(930, 260); // x, y
    glVertex2f(930, 270); // x, y

    glEnd();





    // Right side building 3

    glBegin(GL_QUADS);
    glColor3ub(157, 93, 179);//rgb color picker

    glVertex2f(900, 420); // x, y
    glVertex2f(900, 240); // x, y
    glVertex2f(850, 240); // x, y
    glVertex2f(850, 420); // x, y

    glEnd();

    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(890, 400); // x, y
    glVertex2f(890, 390); // x, y
    glVertex2f(860, 390); // x, y
    glVertex2f(860, 400); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(875, 400);// x, y
    glVertex2f(875, 390);// x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(890, 370); // x, y
    glVertex2f(890, 360); // x, y
    glVertex2f(860, 360); // x, y
    glVertex2f(860, 370); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(875, 370);// x, y
    glVertex2f(875, 360);// x, y

    glEnd();

    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(890, 340); // x, y
    glVertex2f(890, 330); // x, y
    glVertex2f(860, 330); // x, y
    glVertex2f(860, 340); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(875, 340);// x, y
    glVertex2f(875, 330);// x, y

    glEnd();

    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(890, 310); // x, y
    glVertex2f(890, 300); // x, y
    glVertex2f(860, 300); // x, y
    glVertex2f(860, 310); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(875, 310);// x, y
    glVertex2f(875, 300);// x, y

    glEnd();

    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(890, 280); // x, y
    glVertex2f(890, 270); // x, y
    glVertex2f(860, 270); // x, y
    glVertex2f(860, 280); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(875, 280);// x, y
    glVertex2f(875, 270);// x, y

    glEnd();


    // Right side building 4

    glBegin(GL_QUADS);
    glColor3ub(246, 137, 132);//rgb color picker

    glVertex2f(840, 450); // x, y
    glVertex2f(840, 270); // x, y
    glVertex2f(790, 270); // x, y
    glVertex2f(790, 450); // x, y

    glEnd();

    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(830, 440); // x, y
    glVertex2f(830, 430); // x, y
    glVertex2f(800, 430); // x, y
    glVertex2f(800, 440); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(815, 440);// x, y
    glVertex2f(815, 430);// x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(830, 410); // x, y
    glVertex2f(830, 400); // x, y
    glVertex2f(800, 400); // x, y
    glVertex2f(800, 410); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(815, 410);// x, y
    glVertex2f(815, 400);// x, y

    glEnd();

    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(830, 380); // x, y
    glVertex2f(830, 370); // x, y
    glVertex2f(800, 370); // x, y
    glVertex2f(800, 380); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(815, 380);// x, y
    glVertex2f(815, 370);// x, y

    glEnd();

    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(830, 350); // x, y
    glVertex2f(830, 340); // x, y
    glVertex2f(800, 340); // x, y
    glVertex2f(800, 350); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(815, 350);// x, y
    glVertex2f(815, 340);// x, y

    glEnd();

    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(830, 320); // x, y
    glVertex2f(830, 310); // x, y
    glVertex2f(800, 310); // x, y
    glVertex2f(800, 320); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(815, 320);// x, y
    glVertex2f(815, 310);// x, y

    glEnd();


    //window 6
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(830, 290); // x, y
    glVertex2f(830, 280); // x, y
    glVertex2f(800, 280); // x, y
    glVertex2f(800, 290); // x, y

    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 255, 0); // Black

    glVertex2f(815, 290);// x, y
    glVertex2f(815, 280);// x, y

    glEnd();




    // Right side building 5

    glBegin(GL_QUADS);
    glColor3ub(144, 143, 175);//rgb color picker

    glVertex2f(780, 430); // x, y
    glVertex2f(780, 240); // x, y
    glVertex2f(740, 240); // x, y
    glVertex2f(740, 430); // x, y

    glEnd();


    //window 1
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(770, 420); // x, y
    glVertex2f(770, 410); // x, y
    glVertex2f(750, 410); // x, y
    glVertex2f(750, 420); // x, y

    glEnd();


    //window 2
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(770, 390); // x, y
    glVertex2f(770, 380); // x, y
    glVertex2f(750, 380); // x, y
    glVertex2f(750, 390); // x, y

    glEnd();


    //window 3
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(770, 360); // x, y
    glVertex2f(770, 350); // x, y
    glVertex2f(750, 350); // x, y
    glVertex2f(750, 360); // x, y

    glEnd();



    //window 4
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(770, 330); // x, y
    glVertex2f(770, 320); // x, y
    glVertex2f(750, 320); // x, y
    glVertex2f(750, 330); // x, y

    glEnd();



    //window 5
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(770, 300); // x, y
    glVertex2f(770, 290); // x, y
    glVertex2f(750, 290); // x, y
    glVertex2f(750, 300); // x, y

    glEnd();


    //window 6
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(770, 270); // x, y
    glVertex2f(770, 260); // x, y
    glVertex2f(750, 260); // x, y
    glVertex2f(750, 270); // x, y

    glEnd();



    //Background Buildings

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(970, 350); // x, y
    glVertex2f(970, 290); // x, y
    glVertex2f(960, 290); // x, y
    glVertex2f(960, 350); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(920, 370); // x, y
    glVertex2f(920, 300); // x, y
    glVertex2f(900, 300); // x, y
    glVertex2f(900, 370); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(850, 360); // x, y
    glVertex2f(850, 300); // x, y
    glVertex2f(840, 300); // x, y
    glVertex2f(840, 360); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(790, 350); // x, y
    glVertex2f(790, 290); // x, y
    glVertex2f(780, 290); // x, y
    glVertex2f(780, 350); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(740, 370); // x, y
    glVertex2f(740, 295); // x, y
    glVertex2f(720, 295); // x, y
    glVertex2f(720, 370); // x, y

    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(720, 360); // x, y
    glVertex2f(720, 300); // x, y
    glVertex2f(710, 300); // x, y
    glVertex2f(710, 360); // x, y

    glEnd();


    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(620, 340); // x, y
    glVertex2f(620, 300); // x, y
    glVertex2f(640, 300); // x, y
    glVertex2f(640, 340); // x, y

    glEnd();


    glBegin(GL_QUADS);
    glColor3ub(116, 116, 116);//rgb color picker

    glVertex2f(600, 320); // x, y
    glVertex2f(600, 300); // x, y
    glVertex2f(618.5, 300); // x, y
    glVertex2f(618.5, 320); // x, y

    glEnd();

    //Hospital building Right side

    glBegin(GL_POLYGON);
    glColor3ub(113, 243, 169);//rgb color picker

    glVertex2f(750, 250); // x, y
    glVertex2f(750, 200); // x, y
    glVertex2f(950, 200); // x, y
    glVertex2f(950, 250); // x, y

    glEnd();

    //Hospital roof Right side

    glBegin(GL_POLYGON);
    glColor3ub(40, 35, 93);//rgb color picker

    glVertex2f(755, 260); // x, y
    glVertex2f(750, 250); // x, y
    glVertex2f(950, 250); // x, y
    glVertex2f(940, 260); // x, y

    glEnd();


    //Hospital window 1

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(770, 240); // x, y
    glVertex2f(770, 230); // x, y
    glVertex2f(780, 230); // x, y
    glVertex2f(780, 240); // x, y

    glEnd();


    //Hospital window 2

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(820, 240); // x, y
    glVertex2f(820, 230); // x, y
    glVertex2f(830, 230); // x, y
    glVertex2f(830, 240); // x, y

    glEnd();

    //Hospital window 3

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(870, 240); // x, y
    glVertex2f(870, 230); // x, y
    glVertex2f(880, 230); // x, y
    glVertex2f(880, 240); // x, y

    glEnd();

    //Hospital window 4

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(920, 240); // x, y
    glVertex2f(920, 230); // x, y
    glVertex2f(930, 230); // x, y
    glVertex2f(930, 240); // x, y

    glEnd();


    //Hospital window 5

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(770, 220); // x, y
    glVertex2f(770, 210); // x, y
    glVertex2f(780, 210); // x, y
    glVertex2f(780, 220); // x, y

    glEnd();


    //Hospital window 6

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(820, 220); // x, y
    glVertex2f(820, 210); // x, y
    glVertex2f(830, 210); // x, y
    glVertex2f(830, 220); // x, y

    glEnd();

    //Hospital window 7

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(870, 220); // x, y
    glVertex2f(870, 210); // x, y
    glVertex2f(880, 210); // x, y
    glVertex2f(880, 220); // x, y

    glEnd();

    //Hospital window 8

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(920, 220); // x, y
    glVertex2f(920, 210); // x, y
    glVertex2f(930, 210); // x, y
    glVertex2f(930, 220); // x, y

    glEnd();


    //Hospital door

    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);//rgb color picker

    glVertex2f(840, 220); // x, y
    glVertex2f(840, 200); // x, y
    glVertex2f(860, 200); // x, y
    glVertex2f(860, 220); // x, y

    glEnd();





    //Right tower cable
    glBegin(GL_LINES);

    glColor3f(0, 0, 128); // Red


    glVertex2f(700, 350);// x, y
    glVertex2f(900, 150);// x, y


    glVertex2f(700, 350);// x, y
    glVertex2f(710, 330);// x, y
    glVertex2f(710, 330);// x, y
    glVertex2f(720, 310);// x, y
    glVertex2f(720, 310);// x, y
    glVertex2f(730, 290);// x, y
    glVertex2f(730, 290);// x, y
    glVertex2f(740, 270);// x, y
    glVertex2f(740, 270);// x, y
    glVertex2f(752.5, 250);// x, y
    glVertex2f(752.5, 250);// x, y
    glVertex2f(765, 235);// x, y
    glVertex2f(765, 235);// x, y

    glVertex2f(775, 225);// x, y  center point

    glVertex2f(775, 225);// x, y
    glVertex2f(785, 215);// x, y
    glVertex2f(785, 215);// x, y
    glVertex2f(800, 202.5);// x, y
    glVertex2f(800, 202.5);// x, y
    glVertex2f(820, 190);// x, y
    glVertex2f(820, 190);// x, y
    glVertex2f(840, 180);// x, y
    glVertex2f(840, 180);// x, y
    glVertex2f(860, 170);// x, y
    glVertex2f(860, 170);// x, y
    glVertex2f(880, 160);// x, y
    glVertex2f(880, 160);// x, y
    glVertex2f(900, 150);// x, y

//Right tower vertical cable
    glVertex2f(725, 325);// x, y
    glVertex2f(725, 135);// x, y

    glVertex2f(750, 300);// x, y
    glVertex2f(750, 135);// x, y

    glVertex2f(750, 300);// x, y connection cable
    glVertex2f(735, 280);// x, y connection cable

    glVertex2f(775, 275);// x, y
    glVertex2f(775, 135);// x, y

    glVertex2f(775, 275);// x, y connection cable
    glVertex2f(752.5, 250);// x, y connection cable

    glVertex2f(800, 250);// x, y
    glVertex2f(800, 135);// x, y

    glVertex2f(800, 250);// x, y connection cable
    glVertex2f(775, 225);// x, y connection cable

    glVertex2f(825, 225);// x, y
    glVertex2f(825, 135);// x, y

    glVertex2f(825, 225);// x, y connection cable
    glVertex2f(800, 202.5);// x, y connection cable

    glVertex2f(850, 200);// x, y
    glVertex2f(850, 135);// x, y

    glVertex2f(850, 200);// x, y connection cable
    glVertex2f(825, 190);// x, y connection cable

    glVertex2f(875, 175);// x, y
    glVertex2f(875, 135);// x, y

    glEnd();




    //Left tower cable
    glBegin(GL_LINES);

    glColor3f(0, 0, 128); // Red


    glVertex2f(300, 350);// x, y
    glVertex2f(100, 150);// x, y


    glVertex2f(300, 350);// x, y
    glVertex2f(290, 330);// x, y

    glVertex2f(290, 330);// x, y
    glVertex2f(280, 310);// x, y

    glVertex2f(280, 310);// x, y
    glVertex2f(270, 290);// x, y

    glVertex2f(270, 290);// x, y
    glVertex2f(260, 270);// x, y

    glVertex2f(260, 270);// x, y
    glVertex2f(247.5, 250);// x, y

    glVertex2f(247.5, 250);// x, y
    glVertex2f(235, 235);// x, y

    glVertex2f(235, 235);// x, y

    glVertex2f(225, 225);// x, y  center point

    glVertex2f(225, 225);// x, y
    glVertex2f(215, 215);// x, y
    glVertex2f(215, 215);// x, y
    glVertex2f(200, 202.5);// x, y
    glVertex2f(200, 202.5);// x, y
    glVertex2f(180, 190);// x, y
    glVertex2f(180, 190);// x, y
    glVertex2f(160, 180);// x, y
    glVertex2f(160, 180);// x, y
    glVertex2f(140, 170);// x, y
    glVertex2f(140, 170);// x, y
    glVertex2f(120, 160);// x, y
    glVertex2f(120, 160);// x, y
    glVertex2f(100, 150);// x, y


//Left tower vertical cable
    glVertex2f(275, 325);// x, y
    glVertex2f(275, 135);// x, y

    glVertex2f(250, 300);// x, y
    glVertex2f(250, 135);// x, y

    glVertex2f(250, 300);// x, y connection cable
    glVertex2f(265, 280);// x, y connection cable

    glVertex2f(225, 275);// x, y
    glVertex2f(225, 135);// x, y

    glVertex2f(225, 275);// x, y connection cable
    glVertex2f(247.5, 250);// x, y connection cable

    glVertex2f(200, 250);// x, ymiddleFan1();
    glVertex2f(200, 135);// x, y

    glVertex2f(200, 250);// x, y connection cable
    glVertex2f(225, 225);// x, y connection cable

    glVertex2f(175, 225);// x, y
    glVertex2f(175, 135);// x, y

    glVertex2f(175, 225);// x, y connection cable
    glVertex2f(200, 202.5);// x, y connection cable

    glVertex2f(150, 200);// x, y
    glVertex2f(150, 135);// x, y

    glVertex2f(150, 200);// x, y connection cable
    glVertex2f(180, 190);// x, y connection cable

    glVertex2f(125, 175);// x, y
    glVertex2f(125, 135);// x, y

    glEnd();





}


void update1(int value)
{

    if (bridgeMoving)
    {
        if (bridgeOpening && speed < 45.0)
            speed += 0.5f;
        if (!bridgeOpening && speed > 0)
            speed -= 0.5f;
    }

    if (carx > 1100)
        carx = -100;
    if (!bridgeMoving)
        carx += 5;

    if (canNotPass)
    {
        moveShip -= 0;
    }
    else
    {
        moveShip -= 1;
    }
    if(moveShip == -60){
        canNotPass=true;
    }
    if (moveShip < -300)
    {
        moveShip = 50;
        bridgeOpening = false;
    }
    glutPostRedisplay();
    glutTimerFunc(40, update1, 0);
}

void DisplayStart()
{
    glClearColor(0.00, 0.00, 0.00, 1.00); // Set background color to black and opaque

    glClear(GL_COLOR_BUFFER_BIT);
    display();
    glFlush();
}

void handleKeypress(unsigned char key, int x, int y)
{

    switch (key)
    {
    case 's':
        if (carx < 160 || carx > 740)
        {
            bridgeMoving = true;
            bridgeOpening = true;
        }
        break;
    case 'w':
        canNotPass=false; //press w ship moves
        break;
    case 'e':
        bridgeOpening = false;
        bridgeMoving = true;
        break;
    case 'd':
        if(speed < 1)
            bridgeMoving = false;
        break;
    case 'n':
        if (!isDay)
        {
            skyR = 23;
            skyG = 29;
            skyB = 32;
        }
        else
        {
            skyR = 135;
            skyG = 206;
            skyB = 235;
        }
        isDay = !isDay;
        break;
    case 'r':
        if (!isRaining)
            isRaining = true;
        else
            isRaining=false;
        break;
    }

}



int main(int argc, char** argv)
{

    srand(1);
    initRain();
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 600);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("The Historical Tower Bridge Of London");
    gluOrtho2D(0.0, 1000.0, 0.0, 600.0);
    glutDisplayFunc(DisplayStart);
    glutTimerFunc(20, update1, 0);
    glutKeyboardFunc(handleKeypress);
    glutMainLoop();
    return 0;

}


