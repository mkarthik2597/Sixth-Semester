#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>

/* myinit is a user defined function. If the prototype is not declared,
   define the function here */
void myinit()
{
    glClearColor(0,1,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    //glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,20,0,20);
}

void draw()
{
    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3f(1,0,0);
    glVertex2f(10,10);
    glEnd();

    glFlush();
}

int main(int argc, char **argv)
{
    /* Note: The order of function calls must be preserved */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize(250,250);
    glutInitWindowPosition(0,0);

    glutCreateWindow("Lines");
    myinit();
    glutDisplayFunc(draw);
    glutMainLoop();
}
