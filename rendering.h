#ifndef RENDERING_H
#define RENDERING_H
#include <SDL2/SDL.h>

class Window {
	private:
	SDL_Window* window;
	SDL_Surface* screen_surface;
	SDL_Renderer* rend;
	int xres;
	int yres;
	//SDL_Texture* message[NUM_PARTICLES];
	SDL_Event e;
	
	public:
	Window();
	~Window();
	int init(int xres, int yres, char title[]);
	void setColor(int r, int g, int b);
	void preRender();
	void postRender();
	// Drawing functions
	void drawLine(int x1, int y1, int x2, int y2);
	void drawPoint(int x, int y);
	void drawRect(int x, int y, int w, int h);
	void fillRect(int x, int y, int w, int h);
	void drawArc(int xc, int yc, int x, int y);
	void drawCircle(int xc, int yc, int r);
	void fillArc(int xc, int yc, int x, int y);
	void fillCircle(int xc, int yc, int r);
};

#endif
