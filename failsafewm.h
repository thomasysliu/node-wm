/***************************************************************************
                            failsafewm.h  -  description
                            ---------------------
    begin                : Sun Dec 25 18:12:15 CEST 2001
    copyright            : (C) 2000-2001 by Robert Sperling
    email                : sperling@small-window-manager.de
    version              : v0.0.2
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         * 
 ***************************************************************************/

#define failsafewmVERSION	"0.0.2"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <xcb/xcb.h>
#include <xcb/xcb_image.h>

#include <stdlib.h>
#include <stdio.h>

#define true        	  1
#define false       	  0

#define ChildMask        (SubstructureRedirectMask|SubstructureNotifyMask)
#define ButtonMask       (ButtonPressMask|ButtonReleaseMask)
#define MouseMask        (ButtonMask|PointerMotionMask)


/* X11 needed things */
static Display *	  display;
static char		 *displayOption = "";
static int		  screen;
static Window		  rootWindow;
Window  		  focusedWindow; 
unsigned int 			  dummy, sizeX, sizeY;

/* used atoms */
Atom                      xa_wm_state;
Atom                      xa_wm_change_state;
Atom                      xa_wm_protos;
Atom                      xa_wm_delete;
Atom			  xa_mwm_wm_hints;
int			  motionStartingX;
int			  motionStartingY;
int			  exitfailsafewm = false;

const int		  POINTERMOVEMODE   = 1;
const int		  POINTERRESIZEMODE = 2;
int			  pointerMode       = 0;
/*
void failsafewmQuitNicely();
void initfailsafewm();
void grabKeys();
void failsafewmEventLoop();
void keyEventHandler (XKeyEvent *event);
void mouseMotionEventHandler(XMotionEvent *event);
void buttonPressedEventHandler(XButtonEvent *event);
void sendExitClient(Window clientWindow);
*/
#define CONFDIR "$HOME"
 

#define UINT32_MAX  (0xffffffff)
