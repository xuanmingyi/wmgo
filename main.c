#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void test_or_die(int rc)
{
    // #define Success		   0	/* everything's okay */
    // #define BadRequest	   1	/* bad request code */
    // #define BadValue	   2	/* int parameter out of range */
    // #define BadWindow	   3	/* parameter not a Window */
    // #define BadPixmap	   4	/* parameter not a Pixmap */
    // #define BadAtom		   5	/* parameter not an Atom */
    // #define BadCursor	   6	/* parameter not a Cursor */
    // #define BadFont		   7	/* parameter not a Font */
    // #define BadMatch	   8	/* parameter mismatch */
    // #define BadDrawable	   9	/* parameter not a Pixmap or Window */
    // #define BadAccess	  10	/* depending on context:
    //			 - key/button already grabbed
    //			 - attempt to free an illegal
    //			   cmap entry
    //			- attempt to store into a read-only
    //			   color map entry.
    //			- attempt to modify the access control
    //			   list from other than the local host.
    //			*/
    // #define BadAlloc	  11	/* insufficient resources */
    // #define BadColor	  12	/* no such colormap */
    // #define BadGC		  13	/* parameter not a GC */
    // #define BadIDChoice	  14	/* choice not in range or already used */
    // #define BadName		  15	/* font or color name doesn't exist */
    // #define BadLength	  16	/* Request length incorrect */
    // #define BadImplementation 17	/* server is defective */

    // #define FirstExtensionError	128
    // #define LastExtensionError	255
    switch (rc)
    {
    case Success:
        printf("rc=%d Success\n", rc);
        return;
    case BadRequest:
        printf("rc=%d BadRequest\n", rc);
        break;
    case BadValue:
        printf("rc=%d BadValue\n", rc);
        break;
    default:
        printf("rc=%d Unknown error\n", rc);
        break;
    }
    exit(1);
}

void setimage(Display *X, Window win, GC gc)
{
    XImage *ximage;
    int depth = DefaultDepth(X, 0);
    int width, height, n;
    int rc;
    unsigned char *idata;
    unsigned char *buffer;

    idata = stbi_load("wld.jpg", &width, &height, &n, 0);
    buffer = malloc(width * height * 4);

    for (int i = 0; i < width * height; i++)
    {
        buffer[i * 4] = idata[i * 3 + 2];
        buffer[i * 4 + 1] = idata[i * 3 + 1];
        buffer[i * 4 + 2] = idata[i * 3];
        buffer[i * 4 + 3] = 0;
    }
    stbi_image_free(idata);

    ximage = XCreateImage(X, DefaultVisual(X, 0), depth, ZPixmap,
                          0, (char *)buffer, width, height, 32, 0);
    rc = XPutImage(X, win, gc, ximage, 0, 0, 0, 0, width, height);
    test_or_die(rc);

    free(buffer);
}

int main()
{
    Display *X;
    Window win;
    GC gc;
    XSetWindowAttributes attributes;
    XKeyEvent event;

    X = XOpenDisplay(NULL);

    attributes.background_pixel = 0x4433ff;
    attributes.border_pixel = 0x3344ff;

    win = XCreateWindow(X,
                        XRootWindow(X, 0),
                        0, 0, 556, 720,
                        15,
                        DefaultDepth(X, 0),
                        InputOutput,
                        DefaultVisual(X, 0),
                        CWBackPixel | CWBorderPixel,
                        &attributes);

    XDefineCursor(X, win, XCreateFontCursor(X, XC_right_side));

    XSelectInput(X, win, KeyPressMask | ExposureMask);

    gc = XCreateGC(X, win, 0, NULL);

    XMapWindow(X, win);
    while (1)
    {
        XNextEvent(X, (XEvent *)&event);
        switch (event.type)
        {
        case Expose:
            setimage(X, win, gc);
            break;
        case KeyPress:
        {
            XFreeGC(X, gc);
            XCloseDisplay(X);
            exit(0);
        }
        break;

        default:
        {
            printf("%p\n", &event);
        }
        break;
        }
    }
    return 0;
}