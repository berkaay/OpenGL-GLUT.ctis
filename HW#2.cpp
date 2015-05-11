



    //#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <gl/GLUT.h>

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define  TIMER_PERIOD  70 // Period for the timer.
#define  TIMER_ON      1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

bool up = false, down = false, right = false, left = false;

typedef struct {
    float x,y;
}point_t;

typedef struct{
    point_t v1,v2;
    float A,B,C;
}line_t;

typedef struct {
    point_t v1, v2;// 2 vertices of the rect
    float g;//green
    
}rect_t;

rect_t rectangle[10]={0};

point_t mouse;
line_t mouseline={{-150,0}};
point_t clickmouse;

line_t wall= { {150,180} , {150,-120} };
point_t p ;
bool animation=false;

    //
    // to draw circle, center at (x,y)
    //  radius r
    //
void circle( int x, int y, int r )
{
    float angle ;
    glBegin( GL_POLYGON ) ;
    for ( int i = 0 ; i < 360 ; i+=5 )
    {
        angle = i * D2R ;
        glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
    float angle;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 5)
    {
        angle = i * D2R;
        glVertex2f(x + r*cos(angle), y + r*sin(angle));
    }
    glEnd();
}

    // display string message
void print(int x, int y, char *string, void *font )
{
    int len, i ;
    
    glRasterPos2f( x, y );
    len = (int) strlen( string );
    for ( i =0; i<len; i++ )
    {
        glutBitmapCharacter( font, string[i]);
    }
}

    // display text with variables.
void vprint(int x, int y, void *font, char *string , ...)
{
    va_list ap;
    va_start ( ap, string );
    char str[1024] ;
    vsprintf( str, string, ap ) ;
    va_end(ap) ;
    
    int len, i ;
    glRasterPos2f( x, y );
    len = (int) strlen( str );
    for ( i =0; i<len; i++ )
    {
        glutBitmapCharacter( font, str[i]);
    }
}

bool testPoint( point_t p, line_t line ) {
    return (p.y<=wall.v1.y && p.y>wall.v2.y);
}

bool intersectLines( line_t l1, line_t l2 ) {
    float denom = l1.A * l2.B - l2.A * l1.B ;
    if ( denom == 0 ) return false ;
    p.x = (l1.C * l2.B - l2.C * l1.B ) / denom ;
    p.y = (l1.A * l2.C - l2.A * l1.C ) / denom ;
    return testPoint(p, l1);
}

line_t findEq( line_t line ) {
    line.A = line.v1.y - line.v2.y ;
    line.B = line.v2.x - line.v1.x ;
    line.C = line.A * line.v1.x + line.B * line.v1.y ;
    return line ;
}

float fx(float x)
{
    return -1*mouseline.A/mouseline.B * x + mouseline.C/mouseline.B;
}

void drawlabel(){
    glColor3f(1,1,1);
    print(-50,230,"BERKAY OZTURK",GLUT_BITMAP_8_BY_13);
    print(-50,220,"HW#2",GLUT_BITMAP_8_BY_13);
}

void drawRect(){
        // DRAW A RECTANGLE
    wall = findEq(wall);
    glLineWidth(1);
    for (int i=0; i<10; i++)
    {
        (rectangle+i)->v1.x = 150;
        (rectangle+i)->v1.y = 150-(30*i);
        (rectangle+i)->v2.x = 170;
        (rectangle+i)->v2.y = 180-(30*i);
        
        glColor3f(0, ((rectangle+i)->g),0);
        glRectf((rectangle+i)->v1.x, (rectangle+i)->v1.y, (rectangle+i)->v2.x, (rectangle+i)->v2.y);
        
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(150, 150-i*30);
        glVertex2f(170, 150-i*30);
        glVertex2f(170, 180-i*30);
        glVertex2f(150, 180-i*30);
        glEnd();
        
        if (animation)
        {
            if(p.y<180 && p.y>150)
                rectangle[0].g +=0.005;
            else if(p.y<150 && p.y>120)
                rectangle[1].g +=0.005;
            else if(p.y<120 && p.y>90)
                rectangle[2].g +=0.005;
            else if(p.y<90 && p.y>60)
                rectangle[3].g +=0.005;
            else if(p.y<60 && p.y>30)
                rectangle[4].g +=0.005;
            else if(p.y<30 && p.y>0)
                rectangle[5].g +=0.005;
            else if(p.y<0 && p.y>-30)
                rectangle[6].g +=0.005;
            else if(p.y<-30 && p.y>-60)
                rectangle[7].g +=0.005;
            else if(p.y<-60 && p.y>-90)
                rectangle[8].g +=0.005;
            else if(p.y<-90 && p.y>-120)
                rectangle[9].g +=0.005;
        }
        
    }
}

void drawCircular(void){
    
    glColor3f(0,0,.9);
    circle(-150,0,15);
    glColor3f(1,0,1);
    circle(-150,0,8);
    
}

void drawLine(){
    mouseline.v2 = mouse;
    mouseline = findEq(mouseline);
    
    if (mouse.x>=150)
    {
        if (!animation)
        {
            if (intersectLines(wall, mouseline)){
                glColor3f(1,1,0);
                circle(150,p.y,5);
                glColor3f(0,0,0);
                circle(150,p.y,3);
                
                glColor3f(0,1,0);
            }
            else
                glColor3f(1,0,0);
            
            glBegin(GL_LINES);
            glVertex2d(-150,0);
            glVertex2d(mouse.x,mouse.y);
            glEnd();
        }
        
        if (intersectLines(wall, mouseline))
        {
            glColor3f(1,1,0);
            vprint(-200, -225, GLUT_BITMAP_8_BY_13, "y = %.1f x + %.1f", -1*mouseline.A/mouseline.B, mouseline.C/mouseline.B);
            vprint(-200,-215,GLUT_BITMAP_8_BY_13,"intersection = (%.1f,%.1f",p.x,p.y);
        }
    }
}

    // To display onto window using OpenGL commands
    //
void lightning(){
    point_t lightline[10];
    int i;
    
    lightline[0].x = -150;
    lightline[0].y = 0;
    
    lightline[9].x = p.x;
    lightline[9].y = p.y;
    
    for (i=1; i<9; i++)
    {
        lightline[i].x = lightline[0].x + (30*i);
        lightline[i].y = fx(lightline[i].x) + rand()%25;
        lightline[i].y = fx(lightline[i].x) - rand()%25;
    }
    
    if (mouse.x > 150 && intersectLines(wall, mouseline) && animation)
    {
        glLineWidth(8);
        glColor3f(1,0.5,0.5);
        glBegin(GL_LINE_STRIP);
        for (i=0; i<10; i++)
            glVertex2f(lightline[i].x, lightline[i].y);
        glEnd();
        
        glLineWidth(2);
        glColor3f(1,1,1);
        glBegin(GL_LINE_STRIP);
        for (i=0; i<10; i++)
            glVertex2f(lightline[i].x, lightline[i].y);
        glEnd();
    }
}

void display()
{
    
        //
        // clear window to black
        //
    glClearColor( 0, 0 , 0 , 0 );
    glClear( GL_COLOR_BUFFER_BIT );
    
    drawCircular();
    drawlabel();
    drawRect();
    drawLine();
    lightning();
        //drawWall();
        //changeColor();
    
    glutSwapBuffers();
    
}

    //
    // key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
    //
void onKeyDown(unsigned char key, int x, int y )
{
        // exit when ESC is pressed.
    if ( key == 27 )
        exit(0);
    
        // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y )
{
        // exit when ESC is pressed.
    if ( key == 27 )
        exit(0);
    
        // to refresh the window it calls display() function
    glutPostRedisplay() ;
}
    //
    // Special Key like GLUT_KEY_F1, F2, F3,...
    // Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
    //
void onSpecialKeyDown( int key, int x, int y )
{
        // Write your codes here.
    switch (key) {
        case GLUT_KEY_UP: up = true; break;
        case GLUT_KEY_DOWN: down = true; break;
        case GLUT_KEY_LEFT: left = true; break;
        case GLUT_KEY_RIGHT: right = true; break;
            
    }
        // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

    //
    // Special Key like GLUT_KEY_F1, F2, F3,...
    // Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
    //
void onSpecialKeyUp( int key, int x, int y )
{
        // Write your codes here.
    switch (key) {
        case GLUT_KEY_UP: up = false; break;
        case GLUT_KEY_DOWN: down = false; break;
        case GLUT_KEY_LEFT: left = false; break;
        case GLUT_KEY_RIGHT: right = false; break;
            
    }
        // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

    //
    // When a click occurs in the window,
    // It provides which button
    // buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
    // states  : GLUT_UP , GLUT_DOWN
    // x, y is the coordinate of the point that mouse clicked.
    //
void onClick( int button, int stat, int x, int y )
{
        // Write your codes here.
    if(button==GLUT_LEFT_BUTTON && stat== GLUT_DOWN)
        animation=true;
    else if (button==GLUT_LEFT_BUTTON && stat== GLUT_UP)
        animation=false;
    
        // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

    //
    // This function is called when the window size changes.
    // w : is the new width of the window in pixels.
    // h : is the new height of the window in pixels.
    //
void onResize( int w, int h )
{
        // To setup coordinate system
    glViewport( 0,0,w,h) ;
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
    
}

void onMoveDown( int x, int y ) {
        // Write your codes here.
    
    
        // to refresh the window it calls display() function	
        //glutPostRedisplay() ;
}

void onMove( int x, int y ) {
        // Write your codes here.
    mouse.x=x-250;
    mouse.y=250-y;
    
    
        // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

#if TIMER_ON == 1
void onTimer( int v ) {
    
    glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
        // Write your codes here.
    
        // to refresh the window it calls display() function
    glutPostRedisplay() ; // display()
    
}
#endif

int main( int argc, char *argv[] )
{
    
    glutInit(&argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow( "Template File" ) ;
    
    glutDisplayFunc( display ) ;
    glutReshapeFunc( onResize );
        //
        // keyboard registration
        //
    glutKeyboardFunc( onKeyDown ) ;
    glutSpecialFunc( onSpecialKeyDown ) ;
    
    glutKeyboardUpFunc( onKeyUp ) ;
    glutSpecialUpFunc( onSpecialKeyUp ) ;
    
        //
        // mouse registration
        //
    glutMouseFunc( onClick) ;
    glutMotionFunc( onMoveDown ) ;
    glutPassiveMotionFunc( onMove ) ; 
    
#if  TIMER_ON == 1
        // timer event
    glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
#endif
    
    glutMainLoop();
    return 0;}

