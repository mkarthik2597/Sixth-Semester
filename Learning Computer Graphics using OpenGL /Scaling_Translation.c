#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<stdio.h>

void drawSquare()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,1,0,0);

    glPointSize(10);
    glBegin(GL_POLYGON);

    glColor3f(1,0,0);
    glVertex2f(5,5);
    glVertex2f(15,5);
    glVertex2f(15,15);
    glVertex2f(5,15);

    glEnd();
    glFlush();
}

void Translate (int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_UP: glTranslatef(0,1,0);glutPostRedisplay();break;
    case GLUT_KEY_RIGHT: glTranslatef(1,0,0);glutPostRedisplay();break;
    case GLUT_KEY_DOWN: glTranslatef(0,-1,0);glutPostRedisplay();break;
    case GLUT_KEY_LEFT: glTranslatef(-1,0,0);glutPostRedisplay();break;
  }
}

void Scaling (unsigned char key, int x, int y)
{
  switch(key)
  {
    case '+':glScaled(2,2,2);glutPostRedisplay();break;
    case '-':glScaled(0.5,0.5,0.5);glutPostRedisplay();break;
  }
}


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
  glutInitWindowSize(800,800);
  glutInitWindowPosition(0,0);

  glutCreateWindow("Square");

  glClearColor(0,1,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  gluOrtho2D(0,300,0,300);

  glutDisplayFunc(drawSquare);
  glutKeyboardFunc(Scaling);
  glutSpecialFunc(Translate);

  glutMainLoop();
}
