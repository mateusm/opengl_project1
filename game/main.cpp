#include <iostream>
using namespace std;
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>

#define window_width 1480
#define window_height 900


unsigned char header[54];
unsigned int dataPos;
unsigned int width, height;
unsigned int imageSize;
int wall_height = 2;

GLfloat speed = .02;

GLfloat player_x = 0;
GLfloat player_y = 0;
GLfloat player_z = 0;
GLfloat player_y_angle = 0;
GLfloat player_x_angle = 0;
GLfloat player_step_front = 0;
GLfloat player_step_side = 0;
GLfloat map_player_x = 0;
GLfloat map_player_y = 0;

//TRACTOR
GLfloat tractor_x = 0;
GLfloat tractor_y = 0;
GLfloat tractor_z = 0;
GLfloat tractor_turn = 0;
GLfloat tractor_step = 0;
GLfloat tractor_1_arm = -80;
GLfloat tractor_2_arm = 110;
GLfloat tractor_3_arm = 70;
GLfloat tractor_4_arm = 0;
GLfloat tractor_garra = 0;

GLfloat tractor_arm_turn = 0;

GLuint stone_floor, stone_wall, color_map;




GLfloat mat_ambient[]={.8,.8,.8, 1.0};
GLfloat mat_diffuse_green[]={.0,.1,.0,1};//(R,G,B, transparency)
GLfloat mat_diffuse_yellow[]={.1,.1,.0,1};//(R,G,B, transparency)
GLfloat mat_diffuse_blue[]={.0,.0,.1,1};//(R,G,B, transparency)
GLfloat mat_diffuse_black[]={.0,.0,.0,1};//(R,G,B, transparency)
GLfloat mat_diffuse_white[]={.1,.1,.1,1};//(R,G,B, transparency)


GLfloat mat_specular[]={1.,1.,.5, 1.0};
GLfloat mat_shininess[]={128.};	//this needs specular (work together)
GLfloat light0_position[]={1., 1., -5., .0};


////////FILE TO TEXTURE/////////////////////
int fileToTexture(const char * imagepath, GLuint &textureID)
{
    unsigned char * data;
    int i;
    char temp;
    
    FILE * file = fopen(imagepath, "rb");
    if (!file) { printf("Image could not be opened\n"); return 0; }
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file\n");
        return false;
    }
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file\n");
        return 0;
    }
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    if (imageSize == 0) { imageSize = width*height * 3; }
    if (dataPos == 0) { dataPos = 54; }
    
    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    
    for(i=2; i<imageSize; i+=3){
        temp=data[i-2];
        data[i-2]=data[i];
        data[i] = temp;
    }
    
    fclose(file);
    
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    return 1;
}

/////////////////////////////////////////////

///DRAW TEXTURE.///////////////////////////

void drawTexture_ground(GLuint textureID){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-0.5, -.5, 0.5); // inferior esquerdo
    glTexCoord2f(0, 1); glVertex3f(-0.5, -.5, -0.5); // superior esquerdo
    glTexCoord2f(1, 1); glVertex3f( 0.5, -.5, -0.5); // superior direito
    glTexCoord2f(1, 0); glVertex3f( 0.5, -.5, 0.5); // inferior direito
    glEnd() ;
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
}

void drawTexture_wall(GLuint textureID){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-0.5, -.5, 0.5); // inferior esquerdo
    glTexCoord2f(0, 1); glVertex3f(-0.5, .5, 0.5); // superior esquerdo
    glTexCoord2f(1, 1); glVertex3f( 0.5, .5, 0.5); // superior direito
    glTexCoord2f(1, 0); glVertex3f( 0.5, -.5, 0.5); // inferior direito
    glEnd() ;
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
}

void drawTexture_wall2(GLuint textureID){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-0.5, -.5, 0.5); // inferior esquerdo
    glTexCoord2f(0, 1); glVertex3f(-0.5, .5, 0.5); // superior esquerdo
    glTexCoord2f(1, 1); glVertex3f(-0.5, .5, -0.5); // superior direito
    glTexCoord2f(1, 0); glVertex3f(-0.5, -.5,-0.5); // inferior direito
    glEnd() ;
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
}

void drawTexture_wall_color_map(GLuint textureID){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-.5, -.5, 0.5); // inferior esquerdo
    glTexCoord2f(0, 1); glVertex3f(-.5, .5, 0.5); // superior esquerdo
    glTexCoord2f(1, 1); glVertex3f( .5, .5, 0.5); // superior direito
    glTexCoord2f(1, 0); glVertex3f( .5, -.5, 0.5); // inferior direito
    glEnd() ;
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
}
////////////////////////////////////////////

void draw_tractor(){
    
    glPushMatrix(); //move around
    
    glTranslatef(tractor_x, tractor_y, tractor_z);
    glRotatef(tractor_turn, 0, 1, 0);
    
    glPushMatrix(); ////body
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glScalef(2, .7, 1);
    glutSolidCube(.7);
    glPopMatrix();
    
    glPushMatrix(); ///top
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glTranslatef(.25, .4, 0);
    glScalef(1.2, .7, 1);
    glutSolidCube(.7);
    glPopMatrix();
    
    glPushMatrix(); //right wheel
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_black);
    glTranslatef(0, -.3, 0.35);
    glScalef(2.1, .5, .6);
    glutSolidCube(.7);
    glPopMatrix();
    
    glPushMatrix(); //left wheel
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_black);
    glTranslatef(0, -.3, -0.35);
    glScalef(2.1, .5, .6);
    glutSolidCube(.7);
    glPopMatrix();
    
    
    
    glPushMatrix(); //ARM TURN
    
    glTranslatef(-.7, 0, 0);
    glRotatef(tractor_arm_turn, 0, 1, 0);
    glTranslatef(.7, 0, 0);
    
    glPushMatrix(); //#1 arm TURN
    glTranslatef(-.7, 0, 0);
    glRotatef(tractor_1_arm, 0, 0, 1);
    glTranslatef(.7, 0, 0);
    
    glPushMatrix(); //#1 arm
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glTranslatef(-1.3, 0, 0);
    glScalef(2, .3, .3);
    glutSolidCube(.7);
    glPopMatrix();
    
    glPushMatrix(); //#2 arm TURN
    glTranslatef(-2, 0, 0);
    glRotatef(tractor_2_arm, 0, 0, 1);
    glTranslatef(2, 0, 0);
    
    glPushMatrix(); //#2 arm
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glTranslatef(-2.4, 0, 0);
    glScalef(1.2, .3, .3);
    glutSolidCube(.7);
    glPopMatrix();
    
    glPushMatrix(); //#3 arm TURN
    glTranslatef(-2.8, 0, 0);
    glRotatef(tractor_3_arm, 0, 0, 1);
    glTranslatef(2.8, 0, 0);
    
    glPushMatrix(); //#3 arm
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glTranslatef(-3.2, 0, 0);
    glScalef(1.2, .3, .3);
    glutSolidCube(.7);
    glPopMatrix();
    ////////////////////////////////////////////////////////////////
    
    glPushMatrix(); //garra 1 turn
    glTranslatef(-3.5, 0, 0);
    glRotatef(120, 1, 0, 0);
    glRotatef(tractor_garra, 0, 0, 1);
    
    glTranslatef(3.5, 0, 0);
    
    glPushMatrix(); //#1 garra
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glTranslatef(-3.8, 0, 0);
    glScalef(.6, .15, .15);
    glutSolidCube(.7);
    glPopMatrix();
    glPopMatrix(); //garra
    
    
    
    
    glPushMatrix(); //garra 2 turn
    glTranslatef(-3.5, 0, 0);
    glRotatef(240, 1, 0, 0);
    glRotatef(tractor_garra, 0, 0, 1);
    glTranslatef(3.5, 0, 0);
    
    glPushMatrix(); //#2 garra
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glTranslatef(-3.8, 0, 0);
    glScalef(.6, .15, .15);
    glutSolidCube(.7);
    glPopMatrix();
    glPopMatrix(); //garra
    
    
    
    glPushMatrix(); //garra 3turn
    glTranslatef(-3.5, 0, 0);
    glRotatef(tractor_garra, 0, 0, 1);
    glTranslatef(3.5, 0, 0);
    
    glPushMatrix(); //#3 garra
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glTranslatef(-3.8, 0, 0);
    glScalef(.6, .15, .15);
    glutSolidCube(.7);
    glPopMatrix();
    glPopMatrix(); //garra
    
    
    
    ////////////////////////////////////////////////////////////////
    
    glPopMatrix(); // arm turn
    glPopMatrix(); //1 arm turn
    glPopMatrix(); //2 arm turn
    glPopMatrix(); //3 arm turn
    glPopMatrix(); //move around
    
}




///////////////////////////////////////////////

void init(void)
{	glClearColor(1,1,1,1);	//background
    glShadeModel(GL_FLAT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,mat_diffuse_yellow);
    glLightfv(GL_LIGHT0,GL_SPECULAR,mat_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    
    
    fileToTexture("/Users/mateusmesturini/Dropbox/Morgan Classes/Gaming Programming/game/textures/stone_floor.bmp", stone_floor);
    fileToTexture("/Users/mateusmesturini/Dropbox/Morgan Classes/Gaming Programming/game/textures/stone_wall.bmp", stone_wall);
    fileToTexture("/Users/mateusmesturini/Dropbox/Morgan Classes/Gaming Programming/game/textures/color_map.bmp", color_map);
    
    
}

///////////////////////////////////////////////



void display(void)
{ glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GLfloat i, j;
    
    glPushMatrix();
    
    glRotatef(player_x_angle, 1, 0, 0);
    glRotatef(player_y_angle, 0, 1, 0);
    glTranslatef(player_x, player_y, player_z);
    
    glPushMatrix();
    
    for(i=-10; i<10; i++){
        for(j=-10; j<10; j++){
            glPushMatrix();
            glTranslatef(i, 0, j);
            drawTexture_ground(stone_floor);
            glPopMatrix();
        }
    }
    
    for(i=-10; i<10; i++){ //move on x
        for(j=0; j<wall_height; j++){ //move on y
            
            glPushMatrix();
            glTranslatef(i, j, -11);
            drawTexture_wall(stone_wall);
            glPopMatrix();
        }
    }
    
    for(i=-10; i<10; i++){ //move on x
        for(j=0; j<wall_height; j++){ //move on y
            
            glPushMatrix();
            glTranslatef(-10, j, i);
            drawTexture_wall2(stone_wall);
            glPopMatrix();
        }
    }
    
    for(i=0; i<1; i++){ //move on x
        for(j=0; j<1; j++){ //move on y
            
            glPushMatrix();
            glTranslatef(i, j, -7);
            drawTexture_wall_color_map(color_map); ////// ,,,,,,,MAPA
            glPopMatrix();
        }
    }
    
    glPopMatrix();
    
    //MAP_player
    glPushMatrix();
    glTranslatef(map_player_x, map_player_y, -6.48);
    glutSolidSphere(.04, 10, 10); //----------bolinha green
    glPopMatrix();
    
    /////////// TRACTOR
    
    glPushMatrix();
    glTranslatef(0, 0, -3);
    draw_tractor();
    glPopMatrix();
    
    //////////////
    ///MIRA E MAPA
    glPushMatrix();
    glTranslatef(-player_x, -player_y, -player_z);
    glRotatef(-player_y_angle, 0, 1, 0);
    glRotatef(-player_x_angle, 1, 0, 0);
    glTranslatef(0, 0, -.5);
    
    glutWireSphere(.02, 4, 2);
    glPopMatrix();
    
    glPushMatrix();
    
    /////////////////////////
    glTranslatef(0, 0, -3);
    
    //glutSolidCube(1);
    glPopMatrix();

    
    glPopMatrix();

    glutSwapBuffers();
}

void spinDisplay()
{

    player_x -= player_step_front*sin(player_y_angle* M_PI / 180.0);
    player_z += player_step_front*cos(player_y_angle* M_PI / 180.0);
    player_y += player_step_front*tan(player_x_angle* M_PI / 180.0);
    
    player_x -= player_step_side*sin((player_y_angle+90)* M_PI / 180.0);
    player_z += player_step_side*cos((player_y_angle+90)* M_PI / 180.0);
    
    
    map_player_x -= (player_step_front*sin(player_y_angle* M_PI / 180.0))/20;
    map_player_y += (player_step_front*cos(player_y_angle* M_PI / 180.0))/20;
    map_player_x += (player_step_side*sin((player_y_angle+90)* M_PI / 180.0))/20;
    map_player_y -= (player_step_side*cos((player_y_angle+90)* M_PI / 180.0))/20;
    
    tractor_x -= tractor_step*cos((tractor_turn)* M_PI / 180.0);
    tractor_z += tractor_step*sin((tractor_turn)* M_PI / 180.0);
    
    

    glutPostRedisplay();
}



void keyboard(unsigned char key, int xx, int yy)
{glutIdleFunc(spinDisplay);
    switch(key)
    {

        case 'w':   player_step_front = +speed; break;
        case 's':   player_step_front = -speed; break;
        case 'W':   player_step_front = +speed*2; break;
        case 'S':   player_step_front = -speed*2; break;
        case 'd':   player_step_side = +speed; break;
        case 'a':   player_step_side = -speed; break;
        case 'D':   player_step_side = +speed*2; break;
        case 'A':   player_step_side = -speed*2; break;
            
        case '1':   if(tractor_1_arm<50)tractor_1_arm += 1; break;
        case '!':   if(tractor_1_arm>-110)tractor_1_arm -= 1; break;
        case '2':   tractor_2_arm += 1; break;
        case '@':   tractor_2_arm -= 1; break;
        case '3':   tractor_3_arm += 1; break;
        case '#':   tractor_3_arm -= 1; break;
        case '[':   tractor_arm_turn += 1; break;
        case ']':   tractor_arm_turn -= 1; break;
        case 'i':   tractor_step = speed; break;
        case 'k':   tractor_step = -speed; break;
        case 'j':   tractor_turn += 1; break;
        case 'l':   tractor_turn -= 1; break;
        case 'e':   tractor_garra -= 1; break;
        case 'q':   tractor_garra += 1; break;
        
            
    }
    glutPostRedisplay();
}


void releasekey(unsigned char key,int x,int y){

    glutIdleFunc(spinDisplay);
    switch(key)
    {
            
        case 'w':   player_step_front = +0; break;
        case 's':   player_step_front = -0; break;
        case 'W':   player_step_front = +0; break;
        case 'S':   player_step_front = -0; break;
        case 'd':   player_step_side = +0; break;
        case 'a':   player_step_side = -0; break;
        case 'D':   player_step_side = +0; break;
        case 'A':   player_step_side = -0; break;
            
        case 'i':   tractor_step = 0; break;
        case 'k':   tractor_step = 0; break;
            
            
    }
    glutPostRedisplay();

}



void SpecialKeys(int key, int x, int y) //SPECIAL KEYS FOR ARROWS
{glutIdleFunc(spinDisplay);
    switch (key)
    {
        case GLUT_KEY_RIGHT:    player_y_angle += 3; break;
        case GLUT_KEY_LEFT:     player_y_angle -= 3; break;
        case GLUT_KEY_UP:       player_x_angle -= 3; break;
        case GLUT_KEY_DOWN:     player_x_angle += 3; break;
    }
    glutPostRedisplay();
}


///////////MOUSE/////

void mouse( int x, int y )
{
    // 4 bytes per pixel (RGBA), 1x1 bitmap
    vector< unsigned char > pixels( 1 * 1 * 4 );
    glReadPixels ( x, glutGet( GLUT_WINDOW_HEIGHT ) - y,1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
    
    cout << "r: " << (int)pixels[0] << endl;
    cout << "g: " << (int)pixels[1] << endl;
    cout << "b: " << (int)pixels[2] << endl;
    cout << "a: " << (int)pixels[3] << endl;
    cout << endl;
}

////////////////////////////////////////












void reshape(int w, int h)
{	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    glLoadIdentity();
    gluPerspective(80., (GLfloat) w/(GLfloat) h, .1, 190.);
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();	//important: last line must be glLoadIdentity() for coloring with openGL
    glTranslatef (.0, .0, 0.); //do not translate at all if glLoadIdentity() is used
}

int main(int argc, char **argv)
{ glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (window_width, window_height);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Counter Strike");
    init();
    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(releasekey);
    glutSpecialFunc(SpecialKeys);
    glutReshapeFunc(reshape);
    glutMainLoop();
}