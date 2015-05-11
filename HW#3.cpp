    //#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <gl/GLUT.h>

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500
#define  TIMER_PERIOD  20 // Period for the timer.
#define  TIMER_ON      1     // 0:disable timer, 1:enable timer
#define D2R 0.0174532
#define PI 3.1415
    //Vector Library

typedef struct {
    float x, y ;
} vec_t ;

float magV( vec_t v ) {
    return sqrt( v.x * v.x + v.y * v.y ) ;
}
vec_t addV( vec_t v1, vec_t v2 ) {
    vec_t tmp ;
    tmp.x = v1.x + v2.x ;
    tmp.y = v1.y + v2.y ;
    return tmp ;
}
vec_t subV( vec_t v1, vec_t v2 ) {
    vec_t tmp ;
    tmp.x = v1.x - v2.x ;
    tmp.y = v1.y - v2.y ;
    return tmp ;
}
vec_t mulV( float k, vec_t v ) {
    vec_t tmp ;
    tmp.x = k * v.x ;
    tmp.y = k * v.y ;
    return tmp ;
}
vec_t angle2unit( float angle ) {
    vec_t tmp ;
    tmp.x = cos( angle * D2R ) ;
    tmp.y = sin( angle * D2R ) ;
    return tmp ;
}
vec_t unitV( vec_t v ) {
    return mulV( 1/magV(v), v ) ;
}
float dotP( vec_t v1, vec_t v2 ) {
    return v1.x * v2.x + v1.y * v2.y ;
}
    // Angle
float orient( vec_t v ) {
    return atan2( v.y, v.x ) ;
}
    /////////////////////////////////

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

bool start=false;

bool up = false, down = false, right = false, left = false;

    //Declarations
vec_t pointer;
int magu;
int counter=0;
int x=-300;
typedef struct {
    float speed;
    vec_t vec;
    float r,g,b;
    float angle;
    bool appear;
    float radius;
}obj_t;
vec_t unit;
float angle;
obj_t obj[3];
obj_t fire={10,{0,0},1,0,0,0,0,4};



float distance(obj_t obj){	return sqrt( (obj.vec.x - fire.vec.x ) * (obj.vec.x - fire.vec.x ) +
                                        (obj.vec.y - fire.vec.y ) * (obj.vec.y - fire.vec.y ) );}
float findTimef(obj_t obj){
        //printf("%f      %f\n"distance(obj),obj.speed );
    
    return distance(obj)/fire.speed;
}

float findangular(obj_t obj){
    return (atan(obj.radius/(sqrt(distance(obj)*distance(obj)-obj.radius*obj.radius)))/D2R);
    
}

void drawObject(){
    for(int i = 0;i<3;i++){
        if(obj[i].appear)
        {
            obj[i].radius=15;
            obj[i].vec.x=(100+i*50)*cos(D2R*obj[i].angle);
            obj[i].vec.y=(100+i*50)*sin(D2R*obj[i].angle);
            glColor3ub(obj[i].r,obj[i].g,obj[i].b);
            circle(obj[i].vec.x,obj[i].vec.y,obj[i].radius);
            glColor3f(1,1,1);
            vprint(obj[i].vec.x-8,obj[i].vec.y-8,GLUT_BITMAP_8_BY_13,"%.f",obj[i].angle);
        }
    }
}
void drawLabel(){
    glColor3f(1,1,1);
    vprint(-250,220,GLUT_BITMAP_8_BY_13,"HOMEWORK #3");
    vprint(-250,210,GLUT_BITMAP_8_BY_13,"by BERKAY OZTURK");
}
void drawAxis(){
        //axis
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex2d(0,250);
    glVertex2d(0,-250);
    glEnd();
    glBegin(GL_LINES);
    glVertex2d(-250,0);
    glVertex2d(250,0);
    glEnd();
    
        //orbits
    circle_wire(0,0,200);
    circle_wire(0,0,150);
    circle_wire(0,0,100);
    
}
void drawText(){
    if(x>500) x-800;
    if(!start){
        
        print(x,7*sin(10*x*D2R)-200,"C",GLUT_BITMAP_8_BY_13);
        print(x+10,7*sin((10*x+10)*D2R)-200,"L",GLUT_BITMAP_8_BY_13);
        print(x+20,7*sin((10*x+20)*D2R)-200,"I",GLUT_BITMAP_8_BY_13);
        print(x+30,7*sin((10*x+30)*D2R)-200,"C",GLUT_BITMAP_8_BY_13);
        print(x+40,7*sin((10*x+40)*D2R)-200,"K",GLUT_BITMAP_8_BY_13);
        
        print(x+60,7*sin((10*x+60)*D2R)-200,"T",GLUT_BITMAP_8_BY_13);
        print(x+70,7*sin((10*x+70)*D2R)-200,"O",GLUT_BITMAP_8_BY_13);
        
        print(x+90,7*sin((10*x+90)*D2R)-200,"S",GLUT_BITMAP_8_BY_13);
        print(x+100,7*sin((10*x+100)*D2R)-200,"T",GLUT_BITMAP_8_BY_13);
        print(x+110,7*sin((10*x+110)*D2R)-200,"A",GLUT_BITMAP_8_BY_13);
        print(x+120,7*sin((10*x+120)*D2R)-200,"R",GLUT_BITMAP_8_BY_13);
        print(x+130,7*sin((10*x+130)*D2R)-200,"T",GLUT_BITMAP_8_BY_13);
        
        
    }
}
void drawPointer(){
    
    unit = unitV(pointer);
    unit = mulV(70,unit);
    glBegin(GL_LINES);
    glColor3d(1,0,0);
    glVertex2d(0,0);
    glVertex2d(unit.x,unit.y);
    glEnd();
    angle=orient(unit);
    
    angle=angle/D2R;
    if(angle<0)
        angle+=360;
    vprint(cos(angle*D2R)*220,sin(angle*D2R)*220,GLUT_BITMAP_8_BY_13,"%.f",angle);
    
}
bool checkCollision(obj_t t, obj_t f ) {
    float d = sqrt( (t.vec.x - f.vec.x ) * (t.vec.x - f.vec.x ) +
                   (t.vec.y - f.vec.y ) * (t.vec.y - f.vec.y ) );
    return d <= ( t.radius + 4 ) ;
}
void fire_f()
{
    
    if(fire.appear)
    {
        glColor3f(fire.r,fire.g,fire.b);
        circle(fire.vec.x,fire.vec.y,fire.radius);
        
        fire.vec.x+=fire.speed*cos(fire.angle*PI/180);
        fire.vec.y+=fire.speed*sin(fire.angle*PI/180);
        
        if ( fire.vec.x < -250 || fire.vec.x > 250
            || fire.vec.y < -250 || fire.vec.y > 250 )
        {
            fire.appear =false ;
            fire.vec.x=0;
            fire.vec.y=0;
        }
        
        
    }
}
void display()
{
    
    glClearColor( 0, 0 , 0 , 0 );
    glClear( GL_COLOR_BUFFER_BIT );
    
    drawLabel();
    drawAxis();
    drawPointer();
    drawText();
    fire_f();
    
    
    if(start)
    {
            //AI
            //*************************************************************
        
        
        for(int i = 0 ; i<3; i ++)
        {
            if(obj[i].appear)
            {
                int latency = findTimef(obj[i])*obj[i].speed;
                if( (angle-latency) < (obj[i].angle+findangular(obj[i])) && (angle-latency) > (obj[i].angle-findangular(obj[i])))
                {glColor3f(1,1,1);
                    circle(obj[i].vec.x,obj[i].vec.y,17);
                    
                }
            }
            
            drawObject();
        }
    }
    
        //*******************************************************************
    
    glutSwapBuffers();
    
    
}
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
void onClick( int button, int stat, int x, int y )
{
        // Write your codes here.
    if(button==GLUT_LEFT_BUTTON && stat== GLUT_DOWN)
    {
        if(start && !fire.appear)
        {fire.appear=true;
            fire.angle=angle;
        }
        
        if(!start)
        {
            start=true;
                //random color , random speed  , random place
            for(int i=0 ; i<3;i++){
                obj[i].angle=rand()%360;
                obj[i].r=rand()%255;
                obj[i].g=rand()%255;
                obj[i].b=rand()%255;
                obj[i].speed=rand()%4-2;
                obj[i].appear=true;
                if(obj[i].speed==0) obj[i].speed++;	}
        }
        
    }
        // to refresh the window it calls display() function
    glutPostRedisplay() ;
}
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
    pointer.x=x-250;
    pointer.y=250-y;
    magu=magV(pointer);
    
    
        // to refresh the window it calls display() function
        //glutPostRedisplay() ;
}

#if TIMER_ON == 1
void onTimer( int v ) {
    
    glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
        // Write your codes here.
    x++;
    if(x>250)
        x=-250;
    if (start)
    {		
        for(int i = 0 ; i <3 ; i++){
            if(obj[i].angle>=360)
                obj[i].angle-=360;
            else if(obj[i].angle<0)
                obj[i].angle+=360;
            obj[i].angle+=obj[i].speed;
        }
    }
    for(int i=0;i<3;i++){
        if(obj[i].appear)
            if(checkCollision(obj[i],fire))
            {
                obj[i].appear=false;
                fire.appear=false;
                fire.vec.x=0;
                fire.vec.y=0;
                counter++;
                printf("%d\n",counter);
            }
        
        
    }
    if(counter==3){
        start=false;
        counter=0;}
    
    
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
    return 0 ;
}
