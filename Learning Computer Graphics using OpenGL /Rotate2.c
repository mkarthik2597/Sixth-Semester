#include <GL/glut.h>
#include <stdlib.h>

void initgl();
void display(void);
void mouse(int button, int state, int x, int y);
void spinDisplay(void);


static GLfloat spin =0.0;

/* main program - setup window using GLUT */
int
main(int argc, char **argv)
{
    /* Initialise GLUT window manger */
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowSize(500, 500);
    glutCreateWindow("Square");

    glutDisplayFunc(display);   /* Register a drawing function with GLUT */
    glutMouseFunc(mouse);

    initgl();                   /* initialise the opengl */

    glutMainLoop();             /* loop forever & start displaying */
    return  0;
}

/* Initialisation of OpenGL */
void initgl()
{

	glMatrixMode(GL_PROJECTION); //hello

    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
	gluOrtho2D(0.0, 1.0, 0.0,1.0);
    glClearColor (1.0, 1.0, 1.0,1.0); /* background colour */
    glColor3f(1.0,0.0,0.0);       /* foreground colour */
}

/* draw a square */
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);


   glPushMatrix(); /* save current transformation state on stack */

	glRotatef(spin,0,0,1);/* rotate about z-axis */


   glBegin(GL_POLYGON);             /* POLYGON with 4 Verticies */
      glVertex3f (0.25, 0.25, 0.0);
      glVertex3f (0.75, 0.25, 0.0);
      glVertex3f (0.75, 0.75, 0.0);
      glVertex3f (0.25, 0.75, 0.0);
   glEnd();

   glPopMatrix(); /* reset to previous transformation state */

   //glFlush();
   glutSwapBuffers();
}


void mouse(int button, int state, int x, int y)
{
       switch (button) {
          case GLUT_LEFT_BUTTON:
             if (state == GLUT_DOWN)
				glutIdleFunc(spinDisplay); /* start spin */

			   break;
         case GLUT_RIGHT_BUTTON:
               if (state == GLUT_DOWN)
		 glutIdleFunc(NULL);    /* stop spin */
         break;
             default:
         break;
       }
}

/* update angle of display */
void spinDisplay(void)
{
  spin = spin + 2.0;
	if (spin > 360.0)
		spin = spin - 360.0;

  glutPostRedisplay();

}
