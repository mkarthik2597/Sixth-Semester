#include<GL/glut.h>
#include<math.h>
#include<stdio.h>

void myinit()
{
    glClearColor(0,0,1,0);
    glClear(GL_COLOR_BUFFER_BIT);

    //glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,400,0,400);
}

void draw()
{
  /* Front Wall */
  // glPointSize(10);
  glBegin(GL_POLYGON);
  glColor3f(0.678,1.000,0.184);
  glVertex2f(100,250);
  glColor3f(0.498, 1.000, 0.831);
  glVertex2f(300,250);
  glColor3f(1.000, 1.000, 0.000);
  glVertex2f(100,0);
  glColor3f(1.000, 1.000, 0.000);
  glVertex2f(300,0);
  glEnd();

  /* Front Door */
  glFlush();



}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);

    glutCreateWindow("House");
    myinit();

    glutDisplayFunc(draw);
    glutMainLoop();
}
