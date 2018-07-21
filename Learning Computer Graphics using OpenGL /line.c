#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>

void myinit()
{
    glClearColor(0,1,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    //glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,20,0,20);
}

void draw()
{
    glLineWidth(3);
    glColor3f(1,0,0);
    glBegin(GL_LINES);

    int i;
    for(i=0;i<9;i++)
    glVertex2f(2*i+1,2*i+1);

    glEnd();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize(250,250);
    glutInitWindowPosition(0,0);

    glutCreateWindow("Lines");
    myinit();
    glutDisplayFunc(draw);
    glutMainLoop();
}
