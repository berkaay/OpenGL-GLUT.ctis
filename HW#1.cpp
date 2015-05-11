    //#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
#include <time.h>
#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define  TIMER_PERIOD  20 // Period for the timer.
#define  TIMER_ON      1   // 0:disable timer, 1:enable timer

int corx,cory,speed,numpac=0;
typedef struct{
    int x,y;
}point_t;

int height= WINDOW_HEIGHT;
int width = WINDOW_WIDTH;

typedef struct{
    point_t center;
    float speed;
}pacman_t;

pacman_t all[100];


bool animation = false ;

    //
    // to draw circle, center at (x,y)
    //  radius r
    //
void circle( int x, int y, int r )
{
#define PI 3.1415
    float angle ;
    
    glBegin( GL_POLYGON ) ;
    for ( int i = 0 ; i < 100 ; i++ )
    {
        angle = 2*PI*i/100;
        glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
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
    //void pacman(int x,int y){
    //glColor3f( 1, 1, 0 ) ;
    //circle(250- corx , cory -250, 40);


    //}

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

    //
    // To display onto window using OpenGL commands
    //
void drawPacs(pacman_t arr[]){
    
    for (int i = 0 ; i < numpac ;i++){
        
        glColor3f(0,0,0);
        circle(arr[i].center.x, arr[i].center.y, 43);
        glColor3f(1, 1, 0);
        circle(arr[i].center.x,arr[i].center.y,40);
        
        glBegin(GL_TRIANGLES);
        glColor3f (0.9,0.9,0.9);
        glVertex2f(arr[i].center.x ,arr[i].center.y);
        glVertex2f(arr[i].center.x + 50 , arr[i].center.y + 20);
        glVertex2f(arr[i].center.x + 50 , arr[i].center.y-20);
        
        glEnd();
        
        glColor3f(0,0,0);
        circle(arr[i].center.x, arr[i].center.y+20, 10);
        glColor3f(0, 0.8,1);
        circle(arr[i].center.x,arr[i].center.y+20,5);
        
        glBegin(GL_TRIANGLES);
        glColor3f(0, 0, 0);
        
        glVertex2f(arr[i].center.x ,arr[i].center.y-46);
        glVertex2f(arr[i].center.x+40,arr[i].center.y-(40*sqrtf(3))/2);
        glVertex2f(arr[i].center.x+40 ,arr[i].center.y-(40*sqrtf(3))/2-(40*sqrtf(3))/2);
        
        glVertex2f(arr[i].center.x ,arr[i].center.y-46);
        glVertex2f(arr[i].center.x-40,arr[i].center.y-(40*sqrtf(3))/2);
        glVertex2f(arr[i].center.x-40,arr[i].center.y- (40*sqrtf(3))/2-(40*sqrtf(3))/2);
        glEnd();
        
        glLineWidth( 3 ) ;
        glBegin(GL_LINES);
        glColor3f(0, 0, 0);
        glVertex2f(arr[i].center.x+50 ,arr[i].center.y+43);
        glVertex2f(arr[i].center.x-50 ,arr[i].center.y+43);
        glEnd();
        
        glRectf(arr[i].center.x+30, arr[i].center.y+43, arr[i].center.x-30, arr[i].center.y+100);
        
    }
    
}

void display()
{
    
        //
        // clear window to black
        //
    glClearColor( 0.9 ,0.9 , 0.9 , 0 );
    glClear( GL_COLOR_BUFFER_BIT );
    
    
    glColor3f(0, 0, 0);
    glRectf(-200,150,200,250);
    glColor3f(0, 0.5, 0);
    glRectf(-190,210,190,240);
    glColor3f(1,1,1);
    
    print(-50, 220, "BERKAY OZTURK",GLUT_BITMAP_9_BY_15);
    
    glColor3f(0, 0.5, 0);
    glRectf(-190,170,190,200);
    
    glColor3f(1, 1, 1);
    print(-40, 180, "21402305",GLUT_BITMAP_9_BY_15);
    
    glColor3f(1, 0, 0) ;
    vprint(90 , -230, GLUT_BITMAP_8_BY_13, "Mouse = (%d , %d)",corx,cory);
    drawPacs(all)  ;
    
    
    glutSwapBuffers();
    
    
}

    //
    // key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
    //
void onKeyDown(unsigned char key, int x, int y )
{
        // exit when ESC is pressed.
        //  if ( key == 27 )
        // exit(0);
    if( key == 27)
        exit(0);
        // to refresh the window it calls display() function
    if ( key== ' ' ){
        printf("gg\n\\");
        animation =!animation;
    }
    glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y )
{
        // exit when ESC is pressed.
        // if ( key == 27 )
        // exit(0);
    if( key == 27)
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
{ if ( button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN )
    if(numpac<100){
        {
            all[numpac].center.x =  x - 250;
            
            all[numpac].center.y = 250 - y ;
            
            all[numpac].speed=rand()%6-3;
            if(all[numpac].speed==0)
                all[numpac].speed+=1;
            
            numpac++;
            
        }
        
        
    }
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
    corx=x-250;
    cory=250-y;
    
        // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

#if TIMER_ON == 1
void onTimer( int v ) {
    
    glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
        // Write your codes here.
    if ( animation ) {
        
        for ( int i=0; i<numpac ; i++ ) {
            all[i].center.y += all[i].speed ;
            
            if ( all[i].center.y < -height /2 + 10 ||
                all[i].center.y > height / 2 - 10) {
                all[i].speed = -all[i].speed ;
            }
        }
        
    }
    
        // to refresh the window it calls display() function
    glutPostRedisplay() ; // display()
    
}
#endif


int main( int argc, char *argv[] )
{
    
    glutInit(&argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow( "164 Homework" ) ;
    
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
