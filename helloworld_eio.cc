/* This code is PUBLIC DOMAIN, and is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND. See the accompanying 
 * LICENSE file.
 */

#include <v8.h>
#include <node.h>
#include <string.h>
#include <unistd.h>
#include "failsafewm.h"

using namespace node;
using namespace v8;

#define REQ_FUN_ARG(I, VAR)                                             \
  if (args.Length() <= (I) || !args[I]->IsFunction())                   \
    return ThrowException(Exception::TypeError(                         \
                  String::New("Argument " #I " must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

class HelloWorldEio: ObjectWrap
{
private:
  int m_count;
	/*  
	void failsafewmQuitNicely();
	void initfailsafewm();
	void grabKeys();
	void failsafewmEventLoop();
	void keyEventHandler (XKeyEvent *event);
	void mouseMotionEventHandler(XMotionEvent *event);
	void buttonPressedEventHandler(XButtonEvent *event);
	void sendExitClient(Window clientWindow);
    void dump_image_json(xcb_image_t const* img);
  */
  
// write a pixmap to stdout
/*
void
dump_image(xcb_image_t const* img)
{
    int x, y;
    for( y=0; y!=img->height; ++y)
    {
        for( x=0; x!=img->width; ++x)
        {
            switch(xcb_image_get_pixel( img, x, y))
            {
            case 0:
                putc( '_', stdout);
                break;
            case 1:
                putc( '*', stdout);
                break;
            default:
                putc( '?', stdout);
            }
        }
        putc( '\n', stdout);
    }
}
*/
// write a pixmap to stdout
char*  
dump_image_json(xcb_image_t * img)
{
    int x, y;
    char  buff[100];
    char  json[20000];
    unsigned int color;
    sprintf(json,"data=[\n");
    for( y=0; y!=img->height; ++y)
    {
        for( x=0; x!=img->width; ++x)
        {
            
            /*switch(xcb_image_get_pixel( img, x, y))
            {
            case 0:
                putc( '_', stdout);
                break;
            case 1:
                putc( '*', stdout);
                break;
            default:
                putc( '?', stdout);
            }*/
            color = xcb_image_get_pixel( img, x, y);
            color = color & 0xffffff;
            sprintf(buff,"{\"x\":%d,\"y\":%d,\"c\":\"#%06x\"},\n",x,y,color);
            printf("%s\n",buff);
            strcat (json, buff);
            //break;
            //putc( '\n', stdout);
        }
        //putc( '\n', stdout);
    }
    //sprintf(json,
    strcat (json, "];\n");
    //json = "1234";
    return json;
    //exit(0);
}



/***************************************************************************/

/* grabKeys
   grabs keys only to root window
 
*/
void grabKeys() 
{
  XGrabKey(display,XKeysymToKeycode(display,XStringToKeysym("Tab")),
           Mod1Mask,rootWindow,True,GrabModeAsync,GrabModeAsync);
  XGrabKey(display,XKeysymToKeycode(display,XStringToKeysym("F2")),
           Mod1Mask,rootWindow,True,GrabModeAsync,GrabModeAsync);
  XGrabKey(display,XKeysymToKeycode(display,XStringToKeysym("F3")),
           Mod1Mask,rootWindow,True,GrabModeAsync,GrabModeAsync);
  XGrabKey(display,XKeysymToKeycode(display,XStringToKeysym("F4")),
           Mod1Mask,rootWindow,True,GrabModeAsync,GrabModeAsync);
  XGrabKey(display,XKeysymToKeycode(display,XStringToKeysym("F5")),
           Mod1Mask,rootWindow,True,GrabModeAsync,GrabModeAsync);	   
  XGrabKey(display,XKeysymToKeycode(display,XStringToKeysym("F6")),
           Mod1Mask,rootWindow,True,GrabModeAsync,GrabModeAsync);	   
}/* end grabKeys */        

/***************************************************************************/

/* keyEventHandler
   manages all incoming key events for failsafewm, keys are usually grabbed to 
   the rootWindow   
*/

void keyEventHandler (XKeyEvent *event)
{ unsigned int     dummyInt; 
  int     dummyInts; 

  Window  dummyWin;

  XQueryPointer(display, rootWindow, &dummyWin, &focusedWindow, 
                &dummyInts, &dummyInts, &dummyInts, &dummyInts, &dummyInt); 
  if (event->keycode == XKeysymToKeycode(display, XStringToKeysym("Tab")) 
      && event->state == Mod1Mask) 
  { XCirculateSubwindowsUp(display,rootWindow);
  }/* end if Alt-Tab */

  if (event->keycode == XKeysymToKeycode(display, XStringToKeysym("F2")) 
      && event->state == Mod1Mask) 
  { pointerMode=POINTERMOVEMODE;
    XGrabPointer(display, focusedWindow, False, 
                 PointerMotionMask | ButtonPressMask, GrabModeAsync, 
		 GrabModeAsync, None, None, CurrentTime); 
  }/* end if Alt-F2 */
  if (event->keycode == XKeysymToKeycode(display, XStringToKeysym("F3")) 
      && event->state == Mod1Mask) 
  { pointerMode=POINTERRESIZEMODE;
    XGrabPointer(display, focusedWindow, False, 
                 PointerMotionMask | ButtonPressMask, GrabModeAsync, 
		 GrabModeAsync, None, None, CurrentTime); 
    XGetGeometry(display,focusedWindow, &rootWindow, &dummyInts, &dummyInts, 
                 &sizeX, &sizeY, &dummy, &dummy);
  }/* end if Alt-F3 */
  if (event->keycode==XKeysymToKeycode(display,XStringToKeysym("F4")) 
      && event->state == Mod1Mask)		
  { sendExitClient(focusedWindow);
  }/* end if Alt-F4 */         
  if (event->keycode==XKeysymToKeycode(display,XStringToKeysym("F5")) 
      && event->state == Mod1Mask)		
  { XRaiseWindow(display,focusedWindow);
  printf("Alt-F5 is pressed\n");
  }/* end if Alt-F5 */    
  if (event->keycode==XKeysymToKeycode(display,XStringToKeysym("F6")) 
      && event->state == Mod1Mask)		
  { //XRaiseWindow(display,focusedWindow);
  printf("//Alt-F6 is pressed\n");
  
  xcb_connection_t   *c;
  int              screen_nbr;
  //Window * win = &focusedWindow;
  /* Open the connexion to the X server and get the first screen */
  c = xcb_connect (NULL, &screen_nbr);
  
    XGetGeometry(display,focusedWindow, &rootWindow, &dummyInts, &dummyInts, 
                 &sizeX, &sizeY, &dummy, &dummy);
    xcb_image_t *window_image = xcb_image_get(c ,
                        rootWindow,
                        0, 0,
                        sizeX,
                        sizeY,
                        UINT32_MAX, XCB_IMAGE_FORMAT_Z_PIXMAP);
  
  dump_image_json(window_image);
  //focusedWindow->pixmap,
  }/* end if Alt-F6 */         

//  }/* end if rootWindow */
}/* end keyEventHandler */

/*************************************************************************/

/* buttonPressedEventHandler
   handles button presses after move/resize
*/
void buttonPressedEventHandler(XButtonEvent *event)
{ while (XCheckTypedEvent(display,ButtonPress,(XEvent *) event));

  if (event->window == rootWindow) 
  { switch (event->button) 
    { case Button1: system(CONFDIR "/.failsafewm/button1" "&"); break;
      case Button2: system(CONFDIR "/.failsafewm/button2" "&"); break;
      case Button3: system(CONFDIR "/.failsafewm/button3" "&"); break; 
    }	
  } 
  else 
  { XUngrabPointer(display,CurrentTime);
    switch (event->button) 
    { case Button1: pointerMode=0;
           break;
      case Button2: 
           break;
      case Button3: 
	   break;
    }/* end switch button */
  }/* end else if rootwin */
}/* end buttonPressEventHandler */

/*************************************************************************/

/* mouseMotionEventHandler
   handles mouse movement while moveing or resizing clients
*/

void mouseMotionEventHandler(XMotionEvent *event)
{ 

  if (motionStartingX<0) motionStartingX=event->x=event->x_root;
  if (motionStartingY<0) motionStartingY=event->y=event->y_root;     

  while (XCheckTypedEvent (display, MotionNotify, (XEvent *) event));

  if (pointerMode==POINTERMOVEMODE)
  { XMoveWindow(display, focusedWindow, event->x_root,event->y_root);
  }
  if (pointerMode==POINTERRESIZEMODE)
  { XResizeWindow(display, focusedWindow, 
                  sizeX+event->x_root-motionStartingX,
		  sizeY+event->y_root-motionStartingY);
  }
}/* end mouseMotionEventHandler */


/***************************************************************************/

/* sendXMessage 
   sends a xwindow event to the window "givenWin" 
*/

int sendXMessage(Window givenWin, Atom xatom, long value)
{ XEvent event;
  event.type 			= ClientMessage;
  event.xclient.window 		= givenWin;
  event.xclient.message_type 	= xatom;
  event.xclient.format 		= 32;
  event.xclient.data.l[0] 	= value;
  event.xclient.data.l[1] 	= CurrentTime;

  return XSendEvent(display, givenWin, False, NoEventMask, &event);
}// end sendXmessage 

/***************************************************************************/

/* sendExitClient
   sends the client a message to quit itself nicely, if it doesn`t listen to
   xa_wm_delete, we try to kill it as nicely as possible.
*/

void sendExitClient(Window clientWindow)
{ int count, maxcount, found = 0;
  Atom *protocols;

  if (XGetWMProtocols(display, clientWindow, &protocols, &maxcount))
  { for (count=0; count<maxcount; count++) 
    { if (protocols[count] == xa_wm_delete) found++;
    }
    if (protocols) XFree(protocols);
  }
  if (found) 
  { sendXMessage(clientWindow, xa_wm_protos, xa_wm_delete);
  }
  else 
  { XKillClient(display, clientWindow);
  }/* end else if found */
}/* end sendExitClient */

/***************************************************************************/

void failsafewmEventLoop()
{ XEvent event;

  while (!exitfailsafewm) 
  { 
      XNextEvent(display, &event); 
      switch (event.type) 
      { 
        case ButtonPress  : buttonPressedEventHandler  	(&event.xbutton); 
             break;
        case MotionNotify : mouseMotionEventHandler    	(&event.xmotion);    
             break; 
	case KeyPress  	  : keyEventHandler  		(&event.xkey); 
             break;
      }
  }
}/* end failsafewmEventLoop */




  
public:

  static Persistent<FunctionTemplate> s_ct;
  static void Init(Handle<Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("HelloWorldEio"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "hello", Hello);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "get_screen", Get_Screen);

             printf("NODE_SET_PROTOTYPE_METHOD...\n\n");

    target->Set(String::NewSymbol("HelloWorldEio"),
                s_ct->GetFunction());
 
                
                
                
  }

  HelloWorldEio() :
    m_count(0)
  {
	  
	                  
    //////
    
    XSetWindowAttributes attr; 
  display = XOpenDisplay(displayOption); 

  if (!display) 
  { fprintf(stderr,"failsafewm: cannot open display %s, giving up\n", displayOption);
    exit(1);
  }
  screen = DefaultScreen(display);
  rootWindow = RootWindow(display, screen);

//  xa_wm_change_state = XInternAtom(display, "WM_CHANGE_STATE", False);
  xa_wm_protos       = XInternAtom(display, "WM_PROTOCOLS", False);
  xa_wm_delete       = XInternAtom(display, "WM_DELETE_WINDOW", False);

  //this->grabKeys();
  //this->failsafewmEventLoop();   
             //printf("123");
	  
  }


  char* dump_screen()
  {
	  unsigned int     dummyInt; 
  int     dummyInts; 

  Window  dummyWin;

  XQueryPointer(display, rootWindow, &dummyWin, &focusedWindow, 
                &dummyInts, &dummyInts, &dummyInts, &dummyInts, &dummyInt); 
	  
  printf("//Alt-F6 is pressed\n");
  
  xcb_connection_t   *c;
  int              screen_nbr;
  Window * win = &focusedWindow;
  /* Open the connexion to the X server and get the first screen */
  
  c = xcb_connect (NULL, &screen_nbr);
  
    XGetGeometry(display,focusedWindow, &rootWindow, &dummyInts, &dummyInts, 
                 &sizeX, &sizeY, &dummy, &dummy);
    xcb_image_t *window_image = xcb_image_get(c ,
                        rootWindow,
                        0, 0,
                        sizeX,
                        sizeY,
                        UINT32_MAX, XCB_IMAGE_FORMAT_Z_PIXMAP);
  
  return dump_image_json(window_image);
	}


  ~HelloWorldEio()
  {
  }

  static Handle<Value> New(const Arguments& args)
  {
    HandleScope scope;
    HelloWorldEio* hw = new HelloWorldEio();
    hw->Wrap(args.This());
    return args.This();
  }

  struct hello_baton_t {
    HelloWorldEio *hw;
    int increment_by;
    int sleep_for;
    Persistent<Function> cb;
  };

  static Handle<Value> Hello(const Arguments& args)
  {
    HandleScope scope;

	printf("hellooooooooooo\n\n\n");
    REQ_FUN_ARG(0, cb);

    HelloWorldEio* hw = ObjectWrap::Unwrap<HelloWorldEio>(args.This());

    hello_baton_t *baton = new hello_baton_t();
    baton->hw = hw;
    baton->increment_by = 2;
    baton->sleep_for = 1;
    baton->cb = Persistent<Function>::New(cb);

    hw->Ref();

    eio_custom(EIO_Hello, EIO_PRI_DEFAULT, EIO_AfterHello, baton);
    ev_ref(EV_DEFAULT_UC);

    return Undefined();
  }

  static Handle<Value> Get_Screen(const Arguments& args)
  {
    HandleScope scope;
    printf("Get_Screen\n\n");
    REQ_FUN_ARG(0, cb);

   // HelloWorldEio* hw = ObjectWrap::Unwrap<HelloWorldEio>(args.This());
    //hw->dump_screen();
/**/
    //REQ_FUN_ARG(0, cb);

    HelloWorldEio* hw = ObjectWrap::Unwrap<HelloWorldEio>(args.This());

    hello_baton_t *baton = new hello_baton_t();
    baton->hw = hw;
    baton->increment_by = 2;
    baton->sleep_for = 1;
    baton->cb = Persistent<Function>::New(cb);

    hw->Ref();

    eio_custom(EIO_Hello, EIO_PRI_DEFAULT, EIO_AfterHello, baton);

    ev_ref(EV_DEFAULT_UC);

    return Undefined();
  }


  static int EIO_Hello(eio_req *req)
  {
    hello_baton_t *baton = static_cast<hello_baton_t *>(req->data);

    sleep(baton->sleep_for);

    baton->hw->m_count += baton->increment_by;

    return 0;
  }

  static int EIO_AfterHello(eio_req *req)
  {
	  
	  
	  
	  
    
	  unsigned int     dummyInt; 
  int     dummyInts; 

  Window  dummyWin;

  XQueryPointer(display, rootWindow, &dummyWin, &focusedWindow, 
                &dummyInts, &dummyInts, &dummyInts, &dummyInts, &dummyInt); 
	  
  printf("//Alt-F6 is pressed\n");
  
  xcb_connection_t   *c;
  int              screen_nbr;
  Window * win = &focusedWindow;
  /* Open the connexion to the X server and get the first screen */
  
  c = xcb_connect (NULL, &screen_nbr);
  
    XGetGeometry(display,rootWindow, &rootWindow, &dummyInts, &dummyInts, 
                 &sizeX, &sizeY, &dummy, &dummy);
    xcb_image_t *window_image = xcb_image_get(c ,
                        rootWindow,
                        0, 0,
                        sizeX,
                        sizeY,
                        UINT32_MAX, XCB_IMAGE_FORMAT_Z_PIXMAP);
  printf("sizeX = %d sizeY = %d \n",sizeX,sizeY);
  //return dump_image_json(window_image);
    
    
    HandleScope scope;


    hello_baton_t *baton = static_cast<hello_baton_t *>(req->data);

    ev_unref(EV_DEFAULT_UC);
    baton->hw->Unref();

    int x, y;
    char  color_str[100];
    unsigned int color;
    //sprintf(json,"data=[\n");
    //for( y=0; y!=window_image->height; ++y)
    for( y=0; y!=200; ++y)
    {
    //    for( x=0; x!=window_image->width; ++x)
        for( x=0; x!=200; ++x)
        {
            
            /*switch(xcb_image_get_pixel( img, x, y))
            {
            case 0:
                putc( '_', stdout);
                break;
            case 1:
                putc( '*', stdout);
                break;
            default:
                putc( '?', stdout);
            }*/
            
            

    Local<Value> argv[3];
            
            color = xcb_image_get_pixel( window_image, x, y);
            color = color & 0xffffff;
            
            sprintf(color_str,"#%06x",color);

    argv[0] = Integer::New(x);
    argv[1] = Integer::New(y);
    //argv[2] = Integer::New(color);
    argv[2] = String::New(color_str);
    
    
    
    TryCatch try_catch;

    baton->cb->Call(Context::GetCurrent()->Global(), 3, argv);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

            //sprintf(buff,"{\"x\":%d,\"y\":%d,\"c\":\"#%06x\"},\n",x,y,color);
            //printf("%s\n",buff);
            //strcat (json, buff);
           // break;
            //putc( '\n', stdout);
        }
        //putc( '\n', stdout);
    }
    
    

    baton->cb.Dispose();
    
    delete baton;
            
    //}
    //argv[0] = String::New("Hello World Thomas");
    //argv[0] = String::New( baton->hw->dump_screen() );
	// ;
    return 0;
  }
  
    static int EIO_AfterDump(eio_req *req)
  {
    HandleScope scope;
    hello_baton_t *baton = static_cast<hello_baton_t *>(req->data);
    ev_unref(EV_DEFAULT_UC);
    baton->hw->Unref();

    Local<Value> argv[1];

    //argv[0] = String::New("Hello World Thomas");
    argv[0] = String::New( baton->hw->dump_screen() );
	// ;
    TryCatch try_catch;

    baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

    baton->cb.Dispose();

    delete baton;
    return 0;
  }
  
  

};

Persistent<FunctionTemplate> HelloWorldEio::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
    HelloWorldEio::Init(target);
  }

  NODE_MODULE(helloworld_eio, init);
}
