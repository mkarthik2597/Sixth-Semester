#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<math.h>

#include<stdio.h>

#define PI 3.141589
#define toRadians(x) PI/180*x

/* glutDisplayFunc expects a function pointer as its parameter. The
only way to use variables in that callback, is to make them global */
int sides,R;

void myinit()
{
    glClearColor(0,1,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(0,20,0,20);
}

void draw()

{
    glBegin(GL_POLYGON);
    glColor3f(1,0,0);

    double theta;
    double xcor,ycor;

    /* Coordinates are Rcos(theta) and Rsin(theta) with the origin translated to the centre */
    for(theta=0;theta<360;theta+=360.0/sides)
    {
        xcor= 10+R*cos(toRadians(theta));
        ycor= 10+R*sin(toRadians(theta));
        glVertex2f(xcor,ycor);
    }

    glEnd();

    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize(250,250);
    glutInitWindowPosition(0,0);

    printf("Enter the number of sides in the polygon:");
    scanf("%d",&sides);
    printf("\nEnter the radius of the polygon:");
    scanf("%d",&R);

    /* Do not create a window before taking in the user input */
    glutCreateWindow("Regular Polygon");
    myinit();

    glutDisplayFunc(draw);
    glutMainLoop();
}
