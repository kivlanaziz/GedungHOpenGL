#ifdef __APPLE__
#include <GLUT/glut.h>
#include <iostream>
#include <Math.h>
#else
#include <GL/glut.h>
#include <Math.h>
#include <stdio.h>
#include "imageloader.h"
//#include "161511016.h"
#endif

#define JML_ASSET 10

// angle of rotation for the camera direction
float angleH=0.0,angleV=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f,ly=-0.4f;
// XZ position of the camera
float x=-15.0f,z=25.0f,y=1.50f,yy=0;
//when no key is being presses
float deltaAngleH = 0.0f, deltaAngleV = 0.0f;
float deltaMove = 0;
float deltaMoveSide = 0;
float deltaMoveUp = 0;

double rotate_y=0;
double rotate_x=0;

char* assets[JML_ASSET] = {"pillar.bmp","dirt.bmp","sky.bmp","concrete.bmp","window.bmp","wall.bmp","door.bmp",NULL,NULL,NULL};
GLuint _textureId[JML_ASSET];

void changeSize(int width, int height) {

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width/(float)height, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void calculatePosition(float deltaMove) {
    x += deltaMove * lx * 0.05f;
    z += deltaMove * lz * 0.05f;
}

void calculatePositionSide(float deltaMoveSide) {
    x -= deltaMoveSide * lz * 0.05f;
    z += deltaMoveSide * lx * 0.05f;
}

void calculatePositionUp(float deltaMoveUp){
    y += deltaMoveUp * 0.05f;
}

void calculateAngleH(float deltaAngleH) {
    angleH += deltaAngleH;
    lx = sin(angleH);
    lz = -cos(angleH);
}

void calculateAngleV(float deltaAngleV) {
    angleV += deltaAngleV;
    ly = sin(angleV);
}

void drawCube(GLfloat width, GLfloat height, GLfloat depth){
    width *= 0.5;
    height *= 0.5;
    depth *= 0.5;

    GLfloat cube[6][4][3] = {
            {{-width,-height, -depth},{width, -height, -depth},{width, height, -depth},{-width, height, -depth}},	//front
            {{-width,-height, depth},{width, -height, depth},{width, height, depth},{-width, height, depth}},		//back
            {{width, -height, -depth},{width, -height, depth},{width, height, depth},{width, height, -depth}},		//right
            {{-width, -height, depth},{-width, -height, -depth},{-width, height, -depth},{-width, height, depth}},	//left
            {{width, height, depth},{-width, height, depth},{-width, height, -depth},{width, height, -depth}},		//top
            {{-width, -height, depth},{width, -height, depth},{width, -height, -depth},{-width, -height, -depth}} 	//bottom
    };

    for (int i = 0; i < 6; i++){
        glPushMatrix();
            glBegin(GL_POLYGON);
                //glColor3f(TexFront,TexBack,TexLeft);
                glTexCoord2f(0,0);
                glVertex3f(cube[i][0][0], cube[i][0][1], cube[i][0][2]);
                glTexCoord2f(1,0);
                glVertex3f(cube[i][1][0], cube[i][1][1], cube[i][1][2]);
                glTexCoord2f(1,1);
                glVertex3f(cube[i][2][0], cube[i][2][1], cube[i][2][2]);
                glTexCoord2f(0,1);
                glVertex3f(cube[i][3][0], cube[i][3][1], cube[i][3][2]);
            glEnd();
        glPopMatrix();
    }
}

void drawObjCube(GLfloat posx, GLfloat posy, GLfloat posz,GLfloat width, GLfloat height, GLfloat depth){
    glPushMatrix();
        glTranslatef(posx,posy,posz);
        drawCube(width,height,depth);
    glPopMatrix();
}

void baseBuilding(){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId[3]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        /***************** Lantai 1 *****************/
        drawObjCube(0,0.0,0,15,0.5,40);//Gedung kanan
        drawObjCube(-17.5,0.0,-12.5,20,0.5,15);//Gedung Kiri
        /***************** Lantai 2 *****************/
        drawObjCube(0,4.25,0,15,0.5,40);//Gedung kanan
        drawObjCube(-17.5,4.25,-12.5,20,0.5,15);//Gedung Kiri
        /***************** Front Building ***********/
        drawObjCube(-10.5f,0,-2.5,7,0.5,5);
        /***************** Roof Top *****************/
        drawObjCube(0,8.25,0.5,15,0.5,42);//Gedung kanan
        drawObjCube(-17.5,8.25,-12.5,20,0.5,15);//Gedung Kiri
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
}

void skyBox(){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId[2]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glColor3f(0,0,1);
        drawObjCube(0.0,0.0,0,100.0,100.0,100.0);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void ground(){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId[1]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        drawObjCube(0.0,-0.5,0,100.0,1.0,100.0);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void pillar(){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId[3]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        /************* Pillar Gedung Kiri Depan *****************/
        drawObjCube(-25.5f,0.5,-5,1.0,15.0,1.0);
        drawObjCube(-19.5f,0.5,-5,1.0,15.0,1.0);
        drawObjCube(-13.5f,0.5,-5,1.0,15.0,1.0);



        /************* Pillar Gedung Kiri Belakang **************/
        drawObjCube(-25.5f,0.5,-20,1.0,15.0,1.0);
        drawObjCube(-19.5f,0.5,-20,1.0,15.0,1.0);
        drawObjCube(-13.5f,0.5,-20,1.0,15.0,1.0);

        /************* Pillar Gedung Pojok Kanan **************/
        drawObjCube(7.5,0.5,-5,1.0,15.0,1.0);
        drawObjCube(7.5f,0.5,-20,1.0,15.0,1.0);
        drawObjCube(-7.5f,0.5,-20,1.0,15.0,1.0);
        drawObjCube(-7.5f,0.5,-5,1.0,15.0,1.0);

        /************* Pillar Gedung Kanan Samping Kiri**************/
        drawObjCube(-7.5f,0.5,0,1.0,15.0,1.0);
        drawObjCube(-7.5f,0.5,6,1.0,15.0,1.0);
        drawObjCube(-7.5f,0.5,12,1.0,15.0,1.0);
        drawObjCube(-7.5f,0.5,18,1.0,15.0,1.0);

        /************* Pillar Gedung Kanan Samping Kanan**************/
        drawObjCube(7.5f,0.5,0,1.0,15.0,1.0);
        drawObjCube(7.5f,0.5,6,1.0,15.0,1.0);
        drawObjCube(7.5f,0.5,12,1.0,15.0,1.0);
        drawObjCube(7.5f,0.5,18,1.0,15.0,1.0);

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void window(){
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, _textureId[4]);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /************* Lantai 1 *********************************/
    /************* Window Gedung Kiri Depan *****************/
    drawObjCube(-22.5f,0.5,-5,5.0,7.5,0.5);
    drawObjCube(-16.5f,0.5,-5,5.0,7.5,0.5);
    drawObjCube(-10.5f,0.5,-5,5.0,7.5,0.5);

    /************* Window Gedung Kiri Belakang **************/
    drawObjCube(-22.5f,0.5,-20,5.0,7.5,0.5);
    drawObjCube(-16.5f,0.5,-20,5.0,7.5,0.5);
    drawObjCube(-10.5f,0.5,-20,5.0,7.5,0.5);

    /************* Window Gedung Kanan Samping Kiri**************/
    drawObjCube(-7.5f,0.5,3,0.5,7.5,5.0);
    drawObjCube(-7.5f,0.5,9,0.5,7.5,5.0);
    drawObjCube(-7.5f,0.5,15,0.5,7.5,5.0);

    /************* Window Gedung Kanan Samping Kiri**************/
    drawObjCube(7.5f,0.5,3,0.5,7.5,5.0);
    drawObjCube(7.5f,0.5,9,0.5,7.5,5.0);
    drawObjCube(7.5f,0.5,15,0.5,7.5,5.0);

    /************* Window Gedung Kanan Depan ********************/
    drawObjCube(-4.0f,0.5,20.5,6,7.5,1);
    drawObjCube(4.0f,0.5,20.5,6,7.5,1);

    /************* Lantai 2 *********************************/
    /************* Window Gedung Kiri Depan *****************/
    drawObjCube(-22.5f,6.125,-5,5.0,3.75,0.5);
    drawObjCube(-16.5f,6.125,-5,5.0,3.75,0.5);
    drawObjCube(-10.5f,6.125,-5,5.0,3.75,0.5);

    /************* Window Gedung Kiri Belakang **************/
    drawObjCube(-22.5f,6.125,-20,5.0,3.75,0.5);
    drawObjCube(-16.5f,6.125,-20,5.0,3.75,0.5);
    drawObjCube(-10.5f,6.125,-20,5.0,3.75,0.5);

    /************* Window Gedung Kanan Samping Kiri**************/
    drawObjCube(-7.5f,6.125,3,0.5,3.75,5.0);
    drawObjCube(-7.5f,6.125,9,0.5,3.75,5.0);
    drawObjCube(-7.5f,6.125,15,0.5,3.75,5.0);

    /************* Window Gedung Kanan Samping Kiri**************/
    drawObjCube(7.5f,6.125,3,0.5,3.75,5.0);
    drawObjCube(7.5f,6.125,9,0.5,3.75,5.0);
    drawObjCube(7.5f,6.125,15,0.5,3.75,5.0);

    /************* Window Gedung Kanan Depan ********************/
    drawObjCube(-4.0f,6.125,20.5,6,3.75,1);
    drawObjCube(4.0f,6.125,20.5,6,3.75,1);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopMatrix();
}

void wall(){
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[5]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /************* Lantai 1 **************************/
    /************* Wall Gedung Kanan *****************/
    drawObjCube(7.5,0.5,-12.5,1.0,7.5,14);
    drawObjCube(0.0f,0.5,-20,14.0,7.5,1.0);
    drawObjCube(-26.75f,0.5,-12.5,1.5,7.5,15);
    drawObjCube(7.5f,0.5,-2.5,1.0,7.5,4);
    /************* Wall Ruang TA *********************/
    drawObjCube(-7.5,0.5,-8.5,1,7.5,6);
    drawObjCube(-4.0f,0.5,-5,6,7.5,1);
    drawObjCube(-0.5,0.5,-8,1,7.5,7);
    drawObjCube(-4.5f,0.5,-11,5,7.5,1);
    /************* Wall Ruang Sebelah Perpus *********/
    drawObjCube(-7.5,0.5,-16.5,1,7.5,6);
    drawObjCube(-0.5,0.5,-16.5,1,7.5,7);
    drawObjCube(-4.5f,0.5,-13.5,5,7.5,1);
    /************* Wall Gedung Kanan Depan ***********/
    drawObjCube(7.5f,0.5,20,1,15,3);
    drawObjCube(-7.5f,0.5,20,1,15,3);
    /************* Wall Ruang 1 **********************/
    drawObjCube(-4.0f,0.5,-0,6,7.5,1);
    drawObjCube(-1.5,0.5,11,1,7.5,19);
    /************* Wall Ruang 2 **********************/
    drawObjCube(4.5f,0.5,-0,5,7.5,1);
    drawObjCube(2.5,0.5,11,1,7.5,19);
    /************* Wall Depan Perpus *****************/
    drawObjCube(2.5,0.5,-13.5,1,7.5,14);
    drawObjCube(4.5f,0.5,-5.0,5,7.5,1);

    /************* Lantai 2 **************************/
    /************* Wall Gedung Kanan *****************/
    drawObjCube(7.5,6.125,-12.5,1.0,3.75,14);
    drawObjCube(0.0f,6.125,-20,14.0,3.75,1.0);
    drawObjCube(-26.75f,6.125,-12.5,1.5,3.75,15);
    drawObjCube(7.5f,6.125,-2.5,1.0,3.75,4);
    /************* Wall Ruang TA *********************/
    drawObjCube(-7.5,6.125,-8.5,1,3.75,6);
    drawObjCube(-4.0f,6.125,-5,6,3.75,1);
    drawObjCube(-0.5,6.125,-8,1,3.75,7);
    drawObjCube(-4.5f,6.125,-11,5,3.75,1);
    /************* Wall Ruang Sebelah Perpus *********/
    drawObjCube(-7.5,6.125,-16.5,1,3.75,6);
    drawObjCube(-0.5,6.125,-16.5,1,3.75,7);
    drawObjCube(-4.5f,6.125,-13.5,5,3.75,1);
    /************* Wall Gedung Kanan Depan **********/
    drawObjCube(0,6.125,20.5,4,3.75,0.5);
    drawObjCube(-7.5f,6.125,-3,0.5,3.75,5.0);
    /************* Kanopi ***************************/
    //drawObjCube(-22.5f,4.25,-4,5,0.5,1);
    //drawObjCube(-16.5f,4.25,-4,5,0.5,1);
    //drawObjCube(-22.5f,4.25,-21,5,0.5,1);
    //drawObjCube(-16.5f,4.25,-21,5,0.5,1);
    //drawObjCube(-10.5f,4.25,-21,5,0.5,1);

    drawObjCube(-8.5f,4.25,3,1,0.5,5.0);
    drawObjCube(-8.5f,4.25,9,1,0.5,5.0);
    drawObjCube(-8.5f,4.25,15,1,0.5,5.0);

    drawObjCube(8.5f,4.25,3,1,0.5,5.0);
    drawObjCube(8.5f,4.25,9,1,0.5,5.0);
    drawObjCube(8.5f,4.25,15,1,0.5,5.0);

    drawObjCube(-4.0f,4.25,21.5,6,0.5,1);
    drawObjCube(4.0f,4.25,21.5,6,0.5,1);

    /**************************************************/
    drawObjCube(-13.5f,0.5,0,1.0,15.0,1.0);
    drawObjCube(-10.5f,4.25,0,7,1,1.0);
    drawObjCube(-13.5f,4.25,-2.5,1.0,1.0,5.0);
    drawObjCube(-10.5f,6.5,-2.5,7,4,5);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void door(){
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[6]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /************* Wall Gedung Kanan *****************/
    drawObjCube(-7.5,0.5,-12.5,1,7.5,2);
    drawObjCube(-7.5f,0.5,-3,0.5,7.5,5.0);
    drawObjCube(0,0.5,20.5,4,7.5,0.5);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void frontBuilding(){

}

void renderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if (deltaMove) calculatePosition(deltaMove);
    if (deltaMoveSide) calculatePositionSide(deltaMoveSide);
    if (deltaAngleH) calculateAngleH(deltaAngleH);
    if (deltaAngleV) calculateAngleV(deltaAngleV);
    if (deltaMoveUp) calculatePositionUp(deltaMoveUp);
    //printf("x=%f, y=%f, z=%f, lx=%f, ly=%f, lz = %f\n",x,y,z,lx,ly,lz);
    //gluLookAt(0,0,0,rotate_x,rotate_y,0,0,0,0);
    gluLookAt(x, y, z,
              x + lx,y + ly,z + lz,
              0.0f,1.0f,0.0f);

    ground();

    skyBox();

    baseBuilding();

    window();

    pillar();

    wall();

    door();

    glutSwapBuffers();
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void keyboardPress(unsigned char key, int xx, int yy) {
    switch (key) {
        case 'q' : deltaAngleH = -0.005f; break;
        case 'e' : deltaAngleH = 0.005f; break;
        case 'r' : deltaAngleV = 0.005f; break;
        case 'f' : deltaAngleV = -0.005f; break;
        case 'w' : deltaMove = 0.5f; break;
        case 's' : deltaMove = -0.5f; break;
        case 'd' : deltaMoveSide = 0.5f; break;
        case 'a' : deltaMoveSide = -0.5f; break;
        case 'p' : deltaMoveUp = 0.5f; break;
        case 'l' : deltaMoveUp = -0.5f; break;
    }
}

void keyboardRelease(unsigned char key, int x, int y) {
    switch (key) {
        case 'q' :
        case 'e' : deltaAngleH = 0.0f; break;
        case 'r' :
        case 'f' : deltaAngleV = 0.0f; break;
        case 'w' :
        case 's' : deltaMove = 0; break;
        case 'd' :
        case 'a' : deltaMoveSide = 0; break;
        case 'p' :
        case 'l' : deltaMoveUp = 0; break;
    }
}

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
            //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

void loadAssets(){
    int i = 0;

    printf("Loading Assets!\n\n");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    while(assets[i] != NULL){
        printf("Loading Assets [%d/%d] : %s\n",i+1, JML_ASSET, assets[i]);
        Image* image = loadBMP(assets[i]);
        _textureId[i] = loadTexture(image);
        delete image;
        i++;
    }
}

int main(int argc, char **argv) {

    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(1024,768);
    glutCreateWindow("GLUT 3D Object - Kivlan Aziz 161511016");

    loadAssets();

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    glutKeyboardFunc(keyboardPress);
// here are the new entries
    glutIgnoreKeyRepeat(1);
//    glutSpecialUpFunc(keyboardRelease);
    glutKeyboardUpFunc(keyboardRelease);

    initGL();
    //glutTimerFunc(0, timer, 0)
    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}