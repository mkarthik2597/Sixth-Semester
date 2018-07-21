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
    gluOrtho2D(0,20,0,20);
}

void draw()

{
    glBegin(GL_POLYGON);
    glColor3f(1,0,0);

    double theta;
    double xcor,ycor,Xcor,Ycor;
    for(theta=0;theta<360;theta+=10)
    {
        xcor=A*cos(toRadians(theta));
        ycor=B*sin(toRadians(theta));

        Xcor=10+xcor*cos(toRadians(alpha))-ycor*sin(toRadians(alpha));
        Ycor=10+ycor*cos(toRadians(alpha))+xcor*sin(toRadians(alpha));
        glVertex2f(Xcor,Ycor);
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


    printf("Enter the value of semi-major axis:");
    scanf("%d",&A);
    printf("Enter the value of semi-minor axis:");
    scanf("%d",&B);
    printf("Enter the angle of axis rotation (in degrees):");
    scanf("%d",&alpha);

    glutCreateWindow("Ellipse");
    myinit();

    glutDisplayFunc(draw);
    glutMainLoop();
}
