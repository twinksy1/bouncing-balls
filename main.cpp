#include "rendering.h"
#include "vec.h"
#include "circle.h"

const int NUM_CIRCLES = 4;
const float CIRCLE_SIZE = 50.0f;
bool toggleGravity = false;
float gravity = 1.0f;
bool intersecting = false;
#define rnd() (float)rand() / (float)RAND_MAX

struct Global {
	Window w;
	Circle c[NUM_CIRCLES];
	int xres;
	int yres;
};

Global g;

void checkMouse(SDL_Event);
bool handleInput(SDL_Event);
bool keyboardInput(SDL_Event);
void render();
void physics();

int main()
{
	g.xres = 800;
	g.yres = 800;
	srand(time(NULL));
	for(int i=0; i<NUM_CIRCLES; i++) {
		(g.c)[i] = Circle (g.xres*rnd(), g.yres*rnd(), CIRCLE_SIZE);
				//(50*rnd())+20.0f);
	}
	char title[] = {"Circle Simulation"};
	(g.w).init(g.xres, g.yres, title);
	bool leave = false;
	while(!leave) {
		SDL_Event e;
		checkMouse(e);
		leave = handleInput(e);
		(g.w).preRender();
		render();
		(g.w).postRender();
		physics();
	}
	return 0;
}

void physics()
{
	static bool toggle = true;
	if(toggle) {
		for(int i=0; i<NUM_CIRCLES; i++) {
			Circle* p1 = &(g.c)[i];
			p1->move(g.xres, g.yres, toggleGravity, gravity);
			for(int j=i+1; j<NUM_CIRCLES; j++) {
				Circle* p2 = &(g.c)[j];
				IntersectData id = p1->intersectingCircle(*p2);
				if(id.getIntersecting()) {
					intersecting = true;
				}
				else
					intersecting = false;
			}
		}
	} else {
		for(int i=NUM_CIRCLES-1; i>=0; i--) {
			Circle* p1 = &(g.c)[i];
			p1->move(g.xres, g.yres, toggleGravity, gravity);
			for(int j=0; j<i; j++) {
				Circle* p2 = &(g.c)[j];
				IntersectData id = p1->intersectingCircle(*p2);
				if(id.getIntersecting())
					intersecting = true;
				else
					intersecting = false;
			}
		}
	}

	toggle ^= 1;
}

void render()
{
	for(int i=0; i<NUM_CIRCLES; i++) {
		Circle* c = &(g.c)[i];
		if(c->onGround)
			(g.w).setColor(0, 255, 0);
		else 
			(g.w).setColor(255, 255, 0);
		(g.w).fillCircle(c->center.x, c->center.y, c->radius);
	}
}

bool keyboardInput(SDL_Event e)
{
	static bool shift_down = false;
	// Key up
	if(e.type == SDL_KEYUP) {
		switch(e.key.keysym.sym) {
			case SDLK_LSHIFT:
				shift_down = false;
				//printf("Shift key released\n");
				break;
		}
	}
	// Handle keyboard input
	switch(e.key.keysym.sym) {
		case SDLK_LSHIFT:
			shift_down = true;
			break;
		case SDLK_g: {
			static bool ignore = false;
			if(!ignore)
				toggleGravity ^= 1;
			ignore ^= 1;
			break;
			     }
		case SDLK_ESCAPE:
			return true;
			break;
	}
	return false;
}
void checkMouse(SDL_Event e)
{
	static int mousex, mousey;
	static float mouseBang = 500.0f;
	static bool lbutton_down=false, rbutton_down=false;
	if(e.type == SDL_MOUSEMOTION) {
		// Mouse moved
		SDL_GetMouseState(&mousex, &mousey);
		for(int i=0; i<NUM_CIRCLES; i++) {
			Circle* p = &(g.c)[i];
			IntersectData id = p->intersectingPoint((float)mousex, (float)mousey);
			if(id.getIntersecting()) {
				// Mouse is touching a circle
			}
		}
	}

	if(e.type == SDL_MOUSEBUTTONDOWN) {
		// Mouse click
		if(e.button.button == SDL_BUTTON_LEFT) {
			Vec2f mousePos((float)mousex, (float)mousey);
			for(int i=0; i<NUM_CIRCLES; i++) {
				Circle* p = &(g.c)[i];
				Vec2f dist = p->center - mousePos;
				if(dist.length <= mouseBang) {
					p->addForce(MAX_VEL*dist.x, MAX_VEL*dist.y);
				}
			}
		}
	}

	if(e.type == SDL_MOUSEBUTTONUP) {
		// Button release
		lbutton_down = false;
		rbutton_down = false;
	}
}
bool handleInput(SDL_Event e)
{
	// Handles all events in queue
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT)
			return true;
		else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			if(keyboardInput(e))
				return true;
		// Always check mouse
		checkMouse(e);
	}

	return false;
}

