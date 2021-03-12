#include "rendering.h"
Window::Window()
{
    window = NULL;
    screen_surface = NULL;
    rend = NULL;
    //for(int i=0; i<NUM_PARTICLES; i++)
    //        message[i] = NULL;
}
Window::~Window()
{
    SDL_FreeSurface(screen_surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Successful exit\n");
}
int Window::init(int xres, int yres, char title[])
{
    this->xres = xres;
    this->yres = yres;
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("ERROR SDL_INIT: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            xres, yres, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("ERROR DISPLAYING WINDOW: %s\n", SDL_GetError());
        return 1;
    }

    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(rend == NULL) {
        printf("ERROR RENDERER: %s\n", SDL_GetError());
        return 1;
    }
    //TTF_Init();

    return 0;
}
void Window::setColor(int r, int g, int b)
{
    SDL_SetRenderDrawColor(rend, r, g, b, 255);
}
void Window::preRender()
{
    setColor(0, 0, 0);
    SDL_RenderClear(rend);
}
void Window::postRender()
{
    // Show renderer
    SDL_RenderPresent(rend);
}
// Drawing functions
void Window::drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(rend, x1, y1, x2, y2);
}
void Window::drawPoint(int x, int y)
{
    SDL_RenderDrawPoint(rend, x, y);
}
void Window::drawRect(int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_RenderDrawRect(rend, &rect);
}
void Window::fillRect(int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_RenderFillRect(rend, &rect);
}
void Window::drawArc(int xc, int yc, int x, int y) 
{ 
    drawPoint(xc+x, yc+y); 
    drawPoint(xc-x, yc+y); 
    drawPoint(xc+x, yc-y); 
    drawPoint(xc-x, yc-y); 
    drawPoint(xc+y, yc+x); 
    drawPoint(xc-y, yc+x); 
    drawPoint(xc+y, yc-x); 
    drawPoint(xc-y, yc-x); 
}
void Window::drawCircle(int xc, int yc, int r) 
{ 
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    drawArc(xc, yc, x, y); 
    while (y >= x) { 
        // for each pixel we will 
        // draw all eight pixels 
            
        x++; 

        // check for decision parameter 
        // and correspondingly  
        // update d, x, y 
        if (d > 0) { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
        drawArc(xc, yc, x, y); 
    } 
}
void Window::fillArc(int xc, int yc, int x, int y) 
{ 
    drawLine(xc+x, yc+y, xc-x, yc+y); 
    drawLine(xc+x, yc-y, xc-x, yc-y); 
    drawLine(xc+y, yc+x, xc-y, yc+x); 
    drawLine(xc+y, yc-x, xc-y, yc-x); 
}
void Window::fillCircle(int xc, int yc, int r) 
{ 
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    fillArc(xc, yc, x, y); 
    while (y >= x) { 
        // for each pixel we will 
        // draw all eight pixels 
            
        x++; 

        // check for decision parameter 
        // and correspondingly  
        // update d, x, y 
        if (d > 0) { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
        fillArc(xc, yc, x, y); 
    } 
}