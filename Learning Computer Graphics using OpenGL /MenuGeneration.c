#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<math.h>
#include<stdlib.h>

#define PI 3.141589
#define toRadians(x) PI/180*x

#define TRIANGLE 3
#define SQUARE 4
#define PENTAGON 5
#define EXIT 0

void DrawShape(int sides)
{
  glClearColor(0,1,0,0);
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_POLYGON);
  glColor3f(1,0,0);

  double theta;
  double xcor,ycor;

  /* Coordinates are Rcos(theta) and Rsin(theta) with the origin translated to the centre */
  for(theta=0;theta<360;theta+=360.0/sides)
  {
      xcor= 10+5*cos(toRadians(theta));
      ycor= 10+5*sin(toRadians(theta));
      glVertex2f(xcor,ycor);
  }

  glEnd();

  glFlush();
}

void SelectShapes(int shape)
{
  switch(shape)
  {
    case TRIANGLE: DrawShape(TRIANGLE);break;
    case SQUARE: DrawShape(SQUARE);break;
    case PENTAGON: DrawShape(PENTAGON);break;
  }
}

void MainMenuSelection(int value)
{
  if(value == EXIT)
  exit(0);
}

void GenerateMenu()
{
	int mainMenu, subMenu;

  subMenu = glutCreateMenu(SelectShapes);
  glutAddMenuEntry("Triangle",TRIANGLE);
  glutAddMenuEntry("Square",SQUARE);
  glutAddMenuEntry("Pentagon",PENTAGON);

  mainMenu = glutCreateMenu(MainMenuSelection);
  glutAddSubMenu("Draw Shapes",subMenu);
  glutAddMenuEntry("Exit",EXIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
  glutInitWindowSize(250,250);
  glutInitWindowPosition(0,0);

  glutCreateWindow("Square");

  glClearColor(0,1,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  gluOrtho2D(0,20,0,20);
  GenerateMenu();

  glutMainLoop();
}
