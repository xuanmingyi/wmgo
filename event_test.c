#include <stdio.h>
#include <X11/Xlib.h>

static Display *dpy;
static int screen;
static int sw, sh;
static Window root;
static int running = 1;

typedef void (*handler)(XEvent *);

static void *handlers[LASTEvent];

void default_handler(XEvent *ev)
{
    printf("default event handler: ev.type = %d\n", ev->type);
    return;
}

void motionnotify(XEvent *e)
{
    XMotionEvent *ev = &e->xmotion;
    if (ev->state & Button2Mask)
    {
        printf("button 2 motion\n");
    }
    printf("motion notify event: state=%d , x y = %d %d\n", ev->state, ev->x, ev->y);
    return;
}

void createnotify(XEvent *e)
{
    XCreateWindowEvent *ev = &e->xcreatewindow;

    printf("==========createnotify=========\n");
    printf("parent=%ld window=%ld\n", ev->parent, ev->window);
    printf("x,y = %d,%d  width, height=%d, %d\n", ev->x, ev->y, ev->width, ev->height);
    printf("===============================\n");
    return;
}

void maprequest(XEvent *e)
{
    XWindowAttributes wa;
    XMapRequestEvent *ev = &e->xmaprequest;
    if (!XGetWindowAttributes(dpy, ev->window, &wa))
        return;

    XMapWindow(dpy, ev->window);
    printf("map request\n");
    return;
}

void mapnotify(XEvent *e)
{
    printf("map notify\n");
    return;
}

void destroynotify(XEvent *e)
{
    printf("destroy notify\n");
    return;
}

void init()
{
    dpy = XOpenDisplay(NULL);

    XSetWindowAttributes wa;

    screen = DefaultScreen(dpy);
    sw = DisplayWidth(dpy, screen);
    sh = DisplayHeight(dpy, screen);
    root = RootWindow(dpy, screen);

    // wa.event_mask = SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask | PropertyChangeMask;
    wa.event_mask = SubstructureRedirectMask;

    XChangeWindowAttributes(dpy, root, CWEventMask, &wa);
    XSelectInput(dpy, root, wa.event_mask);

    for (int i = 0; i < LASTEvent; i++)
    {
        handlers[i] = default_handler;
    }

    //    handlers[CreateNotify] = createnotify;
    //    handlers[DestroyNotify] = destroynotify;
    handlers[MapRequest] = maprequest;
    //    handlers[MapNotify] = mapnotify;
}

void run()
{
    XEvent ev;
    XSync(dpy, False);
    while (running && !XNextEvent(dpy, &ev))
    {
        handler caller = handlers[ev.type];
        if (caller != NULL)
        {
            caller(&ev);
        }
    }
}

int main()
{
    init();
    run();
}