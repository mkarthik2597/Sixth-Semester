#include <graphics.h>
#include <stdio.h>
#include <math.h>

int main( )
{
    int i,gd,gm;

    float x1;
    printf("Enter the value of x1 : ");
    scanf("%f",&x1);

    float y1;
    printf("Enter the value of y1 : ");
    scanf("%f",&y1);

    float x2;
    printf("Enter the value of x2 : ");
    scanf("%f",&x2);

    float y2;
    printf("Enter the value of y1 : ");
    scanf("%f",&y2);

    /* Detects system's graphics adapter and mode */
    detectgraph(&gd,&gm);

    // initgraph initializes the graphics system by loading a graphics driver from disk
    initgraph(&gd,&gm,"");

    float dx,dy;
    dx=abs(x2-x1);
    dy=abs(y2-y1);

    float pixel;
    if(dx>=dy)
    pixel=dx;
    else
    pixel=dy;

    dx=dx/pixel;
    dy=dy/pixel;

    float x,y;
    x=x1;
    y=y1;

    i=1;
    while(i<=pixel)
    {
          putpixel(x,y,RED);
          x=x+dx;
          y=y+dy;
          i=i+1;
          // The function pauses the computation for the the specified number of milliseconds.
          // delay(100);
    }
    getch();
    // closegraph deallocates all memory allocated by the graphics system, then restores the screen to the mode it was in before you called initgraph
    closegraph();
}
