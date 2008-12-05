/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <SDL.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "gui/ExplicitLayout.h"
#include "gui/Picture.h"
#include "gui/Screen.h"
using namespace hesp;

static GLboolean should_rotate = GL_TRUE;

void quit(int code)
{
	SDL_Quit();
	exit(code);
}

void handle_key_down(SDL_keysym *keysym)
{
	switch(keysym->sym)
	{
		case SDLK_ESCAPE:
			quit(0);
			break;
		case SDLK_SPACE:
			should_rotate = !should_rotate;
			break;
		default:
			break;
	}
}

void process_events()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				handle_key_down(&event.key.keysym);
				break;
			case SDL_QUIT:
				quit(0);
				break;
		}
	}
}

void draw_screen()
{
	/* Our angle of rotation. */
	static float angle = 0.0f;

	static GLfloat v0[] = { -1.0f, -1.0f,  1.0f };
	static GLfloat v1[] = {  1.0f, -1.0f,  1.0f };
	static GLfloat v2[] = {  1.0f,  1.0f,  1.0f };
	static GLfloat v3[] = { -1.0f,  1.0f,  1.0f };
	static GLfloat v4[] = { -1.0f, -1.0f, -1.0f };
	static GLfloat v5[] = {  1.0f, -1.0f, -1.0f };
	static GLfloat v6[] = {  1.0f,  1.0f, -1.0f };
	static GLfloat v7[] = { -1.0f,  1.0f, -1.0f };
	static GLubyte red[]    = { 255,   0,   0, 255 };
	static GLubyte green[]  = {   0, 255,   0, 255 };
	static GLubyte blue[]   = {   0,   0, 255, 255 };
	static GLubyte white[]  = { 255, 255, 255, 255 };
	static GLubyte yellow[] = {   0, 255, 255, 255 };
	static GLubyte black[]  = {   0,   0,   0, 255 };
	static GLubyte orange[] = { 255, 255,   0, 255 };
	static GLubyte purple[] = { 255,   0, 255,   0 };

	/* Clear the color and depth buffers. */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* We don't want to modify the projection matrix. */
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	/* Move down the z-axis. */
	glTranslatef( 0.0, 0.0, -5.0 );

	/* Rotate. */
	glRotatef( angle, 0.0, 1.0, 0.0 );

	if( should_rotate ) {

		if( ++angle > 360.0f ) {
			angle = 0.0f;
		}

	}

	/* Send our triangle data to the pipeline. */
	glBegin( GL_TRIANGLES );

	glColor4ubv( red );
	glVertex3fv( v0 );
	glColor4ubv( green );
	glVertex3fv( v1 );
	glColor4ubv( blue );
	glVertex3fv( v2 );

	glColor4ubv( red );
	glVertex3fv( v0 );
	glColor4ubv( blue );
	glVertex3fv( v2 );
	glColor4ubv( white );
	glVertex3fv( v3 );

	glColor4ubv( green );
	glVertex3fv( v1 );
	glColor4ubv( black );
	glVertex3fv( v5 );
	glColor4ubv( orange );
	glVertex3fv( v6 );

	glColor4ubv( green );
	glVertex3fv( v1 );
	glColor4ubv( orange );
	glVertex3fv( v6 );
	glColor4ubv( blue );
	glVertex3fv( v2 );

	glColor4ubv( black );
	glVertex3fv( v5 );
	glColor4ubv( yellow );
	glVertex3fv( v4 );
	glColor4ubv( purple );
	glVertex3fv( v7 );

	glColor4ubv( black );
	glVertex3fv( v5 );
	glColor4ubv( purple );
	glVertex3fv( v7 );
	glColor4ubv( orange );
	glVertex3fv( v6 );

	glColor4ubv( yellow );
	glVertex3fv( v4 );
	glColor4ubv( red );
	glVertex3fv( v0 );
	glColor4ubv( white );
	glVertex3fv( v3 );

	glColor4ubv( yellow );
	glVertex3fv( v4 );
	glColor4ubv( white );
	glVertex3fv( v3 );
	glColor4ubv( purple );
	glVertex3fv( v7 );

	glColor4ubv( white );
	glVertex3fv( v3 );
	glColor4ubv( blue );
	glVertex3fv( v2 );
	glColor4ubv( orange );
	glVertex3fv( v6 );

	glColor4ubv( white );
	glVertex3fv( v3 );
	glColor4ubv( orange );
	glVertex3fv( v6 );
	glColor4ubv( purple );
	glVertex3fv( v7 );

	glColor4ubv( green );
	glVertex3fv( v1 );
	glColor4ubv( red );
	glVertex3fv( v0 );
	glColor4ubv( yellow );
	glVertex3fv( v4 );

	glColor4ubv( green );
	glVertex3fv( v1 );
	glColor4ubv( yellow );
	glVertex3fv( v4 );
	glColor4ubv( black );
	glVertex3fv( v5 );

	glEnd( );

	SDL_GL_SwapBuffers( );
}

static void setup_opengl( int width, int height )
{
    float ratio = (float) width / (float) height;

    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );

    /* Setup our viewport. */
    glViewport( 0, 0, width, height );

    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );
}

int main( int argc, char* argv[] )
{
#if 1
	// SMG
	Screen& screen = Screen::instance();
	screen.layout().add(new Picture, Extents(10, 10, 20, 20));
	Container<ExplicitLayout> *cont = new Container<ExplicitLayout>;
	cont->layout().add(new Picture, Extents(5, 0, 20, 10));
	screen.layout().add(cont, Extents(100, 100, 200, 200));
	screen.fit(Extents(0, 0, 1024, 768));
	//return 0;
#endif

	if(SDL_Init(SDL_INIT_VIDEO) < 0) quit(EXIT_FAILURE);

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	if(!info) quit(EXIT_FAILURE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int width = 1024;
	int height = 768;
	int bpp = 32;
	int flags = SDL_OPENGL;// | SDL_FULLSCREEN;

	if(SDL_SetVideoMode(width, height, bpp, flags) == 0) quit(EXIT_FAILURE);

#if 0
	setup_opengl(width, height);
#endif

	for(;;)
	{
		process_events();
#if 0
		draw_screen();
#endif
		screen.render();
	}

	return 0;
}
