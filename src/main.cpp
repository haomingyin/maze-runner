//
//  A game that you can control the robot to seek a way out of the maze.
//
//  Created by Haoming Yin on 29/03/17.
//  Copyright © 1017 Haoming Yin. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "loadTGA.h"
using namespace std;


GLuint txId[4], skyBoxId[6];   //Texture ids
GLUquadricObj*	bodySphere;
float  eye_x = 0,  eye_y = 2.2,  eye_z = 17;    //Initial camera position
float look_x = 0, look_y = 2.2, look_z = 0;    //"Look-at" point along -z direction
//float  theta = 0;                              //Look angle
float step = 0;								   //camera motion
int rotateAngle = 90;
float directionAngle = 0;                   //Look angle && robot moving direction angle
int fAngle = 0;    //final mark rotate angle
float tAngle = 0;     // tree rotate angle
float wallX1[15] = {-20, 2, -14, -5, -14,-8, 1, 6, -8, -3, 6, -20, 6, 10, 13};
float wallX2[15] = {-2, 20, -9, 16, -13, 2, 2, 20, -7, 16, 7, 1, 20, 14, 15};
float wallZ1[15] = {11, 11, 5,5, -6, -1, 0, -1, -12, -7, -12, -13, -13, 17.5, 15};
float wallZ2[15] = {12, 12, 6, 6, 5, 0, 5, 0, -1, -6, -7, -12, -12, 18.5, 17};
int robotView = 1;
int isFinished = 0;
int sign = 1;

float green[4] = {0.094, 0.568, 0.215, 1};
float red[4] = {0.901, 0, 0.043, 1};
float black[4] = {0, 0, 0, 1};
float dark[4] = {0.4, 0.4, 0.4, 1.0};
float grey[4] = {0.7, 0.7, 0.7, 1.0};
float white[4]  = {1.0, 1.0, 1.0, 1.0};


float lpos[4] = {-20., 40., -15., 1.0};  //light's position

//--------------------------------------------------------------------------------
void loadTexture()
{
    glGenTextures(6, skyBoxId); 	// Create 6 skybox texture ids
    glGenTextures(4, txId); 	// Create 2 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture for robot body
    loadTGA("textures/metal.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture for robot head
    loadTGA("textures/metal1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture for wall
    loadTGA("textures/wall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture for floor
    loadTGA("textures/grass.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, skyBoxId[0]);  //Use this texture for top
    loadTGA("textures/tropday_up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, skyBoxId[1]);  //Use this texture fot buttom
    loadTGA("textures/tropday_down.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, skyBoxId[2]);  //Use this texture for left
    loadTGA("textures/tropday_left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, skyBoxId[3]);  //Use this texture for right
    loadTGA("textures/tropday_right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, skyBoxId[4]);  //Use this texture for front
    loadTGA("textures/tropday_front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, skyBoxId[5]);  //Use this texture for back
    loadTGA("textures/tropday_back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void drawSkyBox()
{
    //glColor3f(0.5, 0.5, 0.5);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glEnable(GL_TEXTURE_2D);
    
    if (robotView == 1)
    {
        ////////////////////// UP ///////////////////////
        glBindTexture(GL_TEXTURE_2D, skyBoxId[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0,  1);glVertex3f(-20.01, 10, 20.01);
        glTexCoord2f(0,  0);glVertex3f(-20.01, 10, -20.01);
        glTexCoord2f(1,  0);glVertex3f(20.01, 10, -20.01);
        glTexCoord2f(1,  1);glVertex3f(20.01, 10, 20.01);
        glEnd();
    }
    
//    ////////////////////// DOWN ///////////////////////
//    glBindTexture(GL_TEXTURE_2D, skyBoxId[1]);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0,  0.999);glVertex3f(-20.01, 0, -20.01);
//    glTexCoord2f(0,  0);glVertex3f(-20.01, 0, 20.01);
//    glTexCoord2f(0.999,  0);glVertex3f(20.01, 0, 20.01);
//    glTexCoord2f(0.999,  0.999);glVertex3f(20.01, 0, -20.01);
//    glEnd();
    
    ////////////////////// LEFT ///////////////////////
    glBindTexture(GL_TEXTURE_2D, skyBoxId[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0,  0.999);glVertex3f(-20.01, 10, 20.01);
    glTexCoord2f(0,  0);glVertex3f(-20.01, 0, 20.01);
    glTexCoord2f(0.999,  0);glVertex3f(-20.01, 0, -20.01);
    glTexCoord2f(0.999,  0.999);glVertex3f(-20.01, 10, -20.01);
    glEnd();
    
    ////////////////////// RIGHT ///////////////////////
    glBindTexture(GL_TEXTURE_2D, skyBoxId[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0,  0.999);glVertex3f(20.01, 10, -20.01);
    glTexCoord2f(0,  0);glVertex3f(20.01, 0, -20.01);
    glTexCoord2f(0.999,  0);glVertex3f(20.01, 0, 20.01);
    glTexCoord2f(0.999,  0.999);glVertex3f(20.01, 10, 20.01);
    glEnd();
    
    if (robotView == 1)
    {
        ////////////////////// FRONT ///////////////////////
        glBindTexture(GL_TEXTURE_2D, skyBoxId[4]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.999,  0.999);glVertex3f(20.01, 10, 20.01);
        glTexCoord2f(0,  0.999);glVertex3f(20.01, 0, 20.01);
        glTexCoord2f(0,  0);glVertex3f(-20.01, 0, 20.01);
        glTexCoord2f(0.999,  0);glVertex3f(-20.01, 10, 20.01);
        glEnd();
    }
    
    ////////////////////// BACK ///////////////////////
    glBindTexture(GL_TEXTURE_2D, skyBoxId[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0,  0.999);glVertex3f(-20.01, 10, -20.01);
    glTexCoord2f(0,  0);glVertex3f(-20.01, 0, -20.01);
    glTexCoord2f(0.999,  0);glVertex3f(20.01, 0, -20.01);
    glTexCoord2f(0.999,  1);glVertex3f(20.01, 10, -20.01);
    
    glEnd();
}

void drawFloor()
{
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);  //supperss specular light

    glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);
    
    
    //The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);
    float s = 0;
    float t = 1;
    float unit = 0.02;
    for(float i = -22.5; i < 22.5; i+=0.1)
    {
        s = 0;
        for(float j = -22.5;  j < 22.5; j+=0.1)
        {
            glTexCoord2f(s,t); glVertex3f(i, -0.01, j);
            glTexCoord2f(s,t - unit); glVertex3f(i, -0.01, j+0.1);
            glTexCoord2f(s + unit,t - unit);glVertex3f(i+0.1, -0.01, j+0.1);
            glTexCoord2f(s + unit,t); glVertex3f(i+0.1, -0.01, j);
            s += unit;
        }
        t = t - unit;
    }
    glEnd();
    // add lab3 code to reset specular reflections
}

void drawWall(float x1, float x2, float z1, float z2, float height)
{
    // length, width, height for texture mapping usage. Height is also map to t = 1.
    float length = abs((x1 - x2) / 4);
    float width = abs((z1 - z2) / 4);
    float h = abs(height / 4);
    
    // front
    glTexCoord2f(0,  h);glVertex3f(x1, height, z2);
    glTexCoord2f(0,  0);glVertex3f(x1, 0, z2);
    glTexCoord2f(length,  0);glVertex3f(x2, 0, z2);
    glTexCoord2f(length,  h);glVertex3f(x2, height, z2);
    
    // back
    glTexCoord2f(0,  h);glVertex3f(x1, height, z1);
    glTexCoord2f(0,  0);glVertex3f(x1, 0, z1);
    glTexCoord2f(length,  0);glVertex3f(x2, 0, z1);
    glTexCoord2f(length,  h);glVertex3f(x2, height, z1);
    
    // left
    glTexCoord2f(0,  h);glVertex3f(x1, height, z1);
    glTexCoord2f(0,  0);glVertex3f(x1, 0, z1);
    glTexCoord2f(width,  0);glVertex3f(x1, 0, z2);
    glTexCoord2f(width,  h);glVertex3f(x1, height, z2);
    
    // right
    glTexCoord2f(0,  h);glVertex3f(x2, height, z2);
    glTexCoord2f(0,  0);glVertex3f(x2, 0, z2);
    glTexCoord2f(width,  0);glVertex3f(x2, 0, z1);
    glTexCoord2f(width,  h);glVertex3f(x2, height, z1);
    
    // top
    glTexCoord2f(0,  abs((z1 - z2) / 4));glVertex3f(x1, height, z1);
    glTexCoord2f(0,  0);glVertex3f(x1, height, z2);
    glTexCoord2f(length,  0);glVertex3f(x2, height, z2);
    glTexCoord2f(length,  abs((z1 - z2) / 4));glVertex3f(x2, height, z1);
}

void drawWalls()
{
    
    float height = 1;
    if (robotView == 1) height = 4;
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    
    glBegin(GL_QUADS);

    // left edge wall
    glTexCoord2f(0,  1);glVertex3f(-20, height, 11);
    glTexCoord2f(0,  0);glVertex3f(-20, 0, 11);
    glTexCoord2f(5.5,  0);glVertex3f(-20, 0, -12);
    glTexCoord2f(5.5,  1);glVertex3f(-20, height, -12);
    
    // right edge wall
    glTexCoord2f(0,  1);glVertex3f(20, height, 11);
    glTexCoord2f(0,  0);glVertex3f(20, 0, 11);
    glTexCoord2f(5.5,  0);glVertex3f(20, 0, -12);
    glTexCoord2f(5.5,  1);glVertex3f(20, height, -12);
    
    
    // walls in stage
    for (int i = 0; i < 13; i++){
        drawWall(wallX1[i], wallX2[i], wallZ1[i], wallZ2[i], height);
    }
    glEnd();
    
}

void drawTree()
{
    float shadowMat[16] = { lpos[1],0,0,0, -lpos[0],0,-lpos[2],-1,  0,0,lpos[1],0,  0,0,0,lpos[1]};
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    
    //glColorMaterial(GL_FRONT, GL_SPECULAR);
    glMaterialfv(GL_FRONT, GL_SPECULAR, green);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);
    
    glPushMatrix();
    glColor3f(0.094, 0.568, 0.215);
    glTranslatef(14, 3.2, 16);
    glRotatef(tAngle, 1, 0, 0);
    glRotatef(-90, 1, 0 , 0);
    glutSolidCone(1.15, 2, 50, 50);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.094, 0.568, 0.215);
    glTranslatef(14, 1.5, 16);
    glRotatef(tAngle, 1, 0, 0);
    glRotatef(-90, 1, 0 , 0);
    glutSolidCone(1.5, 3, 50, 50);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    
    glPushMatrix();
    glColor3f(0.454, 0.341, 0.043);
    glTranslatef(14, 0, 16);
    glScalef(1, 10, 1);
    glutSolidCube(0.4);
    glPopMatrix();
    
}

void drawBench()
{
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    
    glPushMatrix();
    glColor3f(0.454, 0.341, 0.043);
    glTranslatef(12, 1, 18);
    glScalef(10, 0.5, 2.5);
    glutSolidCube(0.4);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.454, 0.341, 0.043);
    glTranslatef(10.5, 0, 18);
    glScalef(1.2, 5, 1);
    glutSolidCube(0.4);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.454, 0.341, 0.043);
    glTranslatef(13.5, 0, 18);
    glScalef(1.2, 5, 1);
    glutSolidCube(0.4);
    glPopMatrix();
}

void drawFinial()
{
    if (isFinished != 0)
    {
        glColor3f(0.094, 0.568, 0.215);
    } else {
        glColor3f(0.901, 0, 0.043);
    }
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 80);
    
    glPushMatrix();
    glTranslatef(3.5, 3.5, -12.5);
    glRotatef(0, 1, 0, 1);
    glScalef(0.7, 0.7, 0.7);
    glRotatef(fAngle, 0,1,0);
    glutSolidOctahedron();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3.5, 3.5, -12.5);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.1, 1, 40, 50);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
}

// when theta and step changes, we need to check if the eye is out of sky box.
// then we can move the camera.
void moveCamera()
{
    float dir_x = -sin(directionAngle), dir_y = 0,  dir_z = -cos(directionAngle);
    float d = 0.8;
    float r = d + 0.205;  // ridius for wall/skybox
    
    int collisionDetected = 0;
    float temp_look_x = eye_x + dir_x * step;
    float temp_look_z = eye_z + dir_z * step;
    
    // check if robot is hit the wall
    for (int i = 0; i < 15; i++)
    {
        if (temp_look_x >= wallX1[i] - r  && temp_look_x <= wallX2[i] + r &&
            temp_look_z >= wallZ1[i] - r && temp_look_z <= wallZ2[i] + r)
        {
            collisionDetected = 1;
            break;
        }
    }
    
    // check if robot is out of skybox
    if (temp_look_x > 20 - r || temp_look_x < -20 + r ||
        temp_look_z > 20 - r || temp_look_z < -20 + r)
    {
        collisionDetected = 1;
    }
    
    if (collisionDetected == 0)
    {
        eye_x += dir_x * step;
        eye_z += dir_z * step;
    }
    
    // check if reaches final point
    if (temp_look_z < -13.2) isFinished = 1;

    look_x = eye_x + dir_x * d;
    look_z = eye_z + dir_z * d;
    
    step = 0;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (robotView == 1) {
        gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);
    } else if (robotView == 0) {
        gluLookAt(eye_x, 6.2, eye_z + 7,  look_x, 2, look_z,   0, 1, 0);
    } else if (robotView == 2) {
        gluLookAt(0, 45, 33,  0, 0, 0,   0, 1, 0);
    }
    
    // set head lighting
    float eye_pos[] = {eye_x, 3, eye_z, 1.0};
    float eye_dir[] = {float(dir_x * 2), -1.5, float(dir_z * 2), 1.0};
    
    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, eye_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, eye_dir);
    glPopMatrix();
}

void drawRobot() {

    
    glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, grey);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);

    
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1, 1, 1);
    
    glEnable(GL_TEXTURE_2D);
    
    //robot body
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glPushMatrix();
    glTranslatef(eye_x, 0, eye_z);
    glTranslatef(0, 1, 0);
    glRotatef(-rotateAngle, cos(directionAngle), 0, -sin(directionAngle));   //make the sphere axis vertical
    gluSphere(bodySphere, 1.0, 100, 100);
    glPopMatrix();
    
    //robot head
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glPushMatrix();
    glTranslatef(eye_x, 0, eye_z);
    glTranslatef(0, 2.15, 0);
    glRotatef(directionAngle * 180 / 3.141592654, 0, 1, 0);
    glRotatef(180, 0, 1, 0);   //make the sphere axis vertical
    gluSphere(bodySphere, 0.5, 100, 100);
    glPopMatrix();

    //robot leye
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glTranslatef(eye_x, 0, eye_z);
    glRotatef(directionAngle * 180 / 3.141592654, 0, 1, 0);
    glTranslatef(0.2, 2.2, -0.4);
    glutSolidSphere(0.1, 35, 35);
    glPopMatrix();
    
    //robot reye
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glTranslatef(eye_x, 0, eye_z);
    glRotatef(directionAngle * 180 / 3.141592654, 0, 1, 0);
    glTranslatef(-0.2, 2.2, -0.4);
    glutSolidSphere(0.1, 35, 35);
    glPopMatrix();
    
    //flash light
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.85, 0.85, 0.85);
    glPushMatrix();
    glTranslatef(eye_x, 0, eye_z);
    glRotatef(directionAngle * 180 / 3.141592654, 0, 1, 0);
    glTranslatef(0, 2.5, -0.35);
    glutSolidSphere(0.08, 35, 35);
    glPopMatrix();
    
    //robot body shadow
    float shadowMat[16] = { lpos[1],0,0,0, -lpos[0],0,-lpos[2],-1,  0,0,lpos[1],0,  0,0,0,lpos[1]};
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf(shadowMat);
    glTranslatef(eye_x, 0, eye_z);
    glTranslatef(0, 1, 0);
    glRotatef(-rotateAngle, cos(directionAngle), 0, -sin(directionAngle));   //make the sphere axis vertical
    glColor3f(0.25, 0.25, 0.25);
    gluSphere(bodySphere, 1.0, 100, 100);
    glPopMatrix();
    
    //robot head shadow
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glPushMatrix();
    glMultMatrixf(shadowMat);
    glTranslatef(eye_x, 0, eye_z);
    glTranslatef(0, 2.15, 0);
    glRotatef(directionAngle * 180 / 3.141592654, 0, 1, 0);
    glRotatef(180, 0, 1, 0);   //make the sphere axis vertical
    gluSphere(bodySphere, 0.5, 100, 100);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glFlush();
}

void display()
{
    moveCamera();
    drawSkyBox();
    drawRobot();
    drawWalls();
    drawTree();
    drawBench();
    drawFinial();
    drawFloor();
    glutSwapBuffers();
}

void rotateBody(int value)
{
    fAngle += 1;
    fAngle %= 360;
    
    if (tAngle >= 5)  sign = -1;
    if (tAngle <= -5) sign = 1;
    
    tAngle = sign * 0.1 + tAngle;
    
    glutPostRedisplay();
    glutTimerFunc(20, rotateBody, 0);
}

void special(int key, int x, int y)
{
    step = 0;
    if(key == GLUT_KEY_LEFT) {directionAngle += 0.05;}  //in radians
    else if(key == GLUT_KEY_RIGHT) {directionAngle -= 0.05;}
    else if(key == GLUT_KEY_DOWN) {step = -0.2; rotateAngle -= 5;}
    else if(key == GLUT_KEY_UP) {step = 0.2; rotateAngle += 5;}

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c' || key =='C') {
        robotView = (robotView + 1) % 3;
    }
    glutPostRedisplay();
}


void initialise()
{
//////////////  initialize for robot body (sphere) ///////
    bodySphere = gluNewQuadric();
    gluQuadricDrawStyle(bodySphere, GLU_FILL);
    gluQuadricTexture(bodySphere, GL_TRUE);
    gluQuadricNormals(bodySphere, GLU_SMOOTH);
//////////////////////////////////////////////////////////
    
    glClearColor(0., 1., 1., 1.);  //Background colour  = cyan
    loadTexture();
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_LIGHTING);		//Enable OpenGL states
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, dark);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, grey);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
   
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, dark);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, grey);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,70);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45., 1., 1., 200.);
    
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize (800, 800);
    glutInitWindowPosition (20, 20);
    glutCreateWindow ("The maze runner");
    initialise();
    
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    
    rotateBody(0);
    
    glutMainLoop();
    return 0;
}
