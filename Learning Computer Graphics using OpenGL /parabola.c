#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<math.h>

#include<stdio.h>

#define PI 3.141589
#define toRadians(x) PI/180*x

int A,B,alpha;
void myinit()
{
    glClearColor(0,1,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    //glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,400,0,400);
}

void draw()

{
    glBegin(GL_LINE_STRIP);
    glColor3f(1,0,0);

    double X=0,Y;
    double xcor,ycor;

    for(X=-200;X<=200;X+=0.001)
    {
      Y=X*X/100;
      glVertex2f(X+200,Y);
    }

    glEnd();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);

    glutCreateWindow("Parabola");
    myinit();

    glutDisplayFunc(draw);
    glutMainLoop();
}
