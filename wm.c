#include <stdio.h>
#include <X11/Xlib.h>

static Display *dpy;
static int screen;
static int sw, sh;
static Window root;
static int running = 1;

void init()
{
    dpy = XOpenDisplay(NULL);

    XSetWindowAttributes wa;

    screen = DefaultScreen(dpy);
    sw = DisplayWidth(dpy, screen);
    sh = DisplayHeight(dpy, screen);
    root = RootWindow(dpy, screen);

    wa.event_mask = SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask | PropertyChangeMask;
    XChangeWindowAttributes(dpy, root, CWEventMask, &wa);
    XSelectInput(dpy, root, wa.event_mask);
}

void manage(Window w, XWindowAttributes *wa)
{
}

void maprequest(XEvent *e)
{
    static XWindowAttributes wa;
    XMapRequestEvent *ev = &e->xmaprequest;
    XGetWindowAttributes(dpy, ev->window, &wa);

    manage(ev->window, &ev);
}

void run()
{
    XEvent ev;
    XSync(dpy, False);
    while (running && !XNextEvent(dpy, &ev))
    {
        switch (ev.type)
        {
        case CreateNotify: // 16
            printf("CreateNotify\n");
            break;
        case MapRequest: // 20
            maprequest(&ev);
            break;
        default:
            printf("ev.type = %d\n", ev.type);
        }
    }
}

int main()
{
    init();
    run();
}