#include "rendering.h"
#include "vec.h"
#include "circle.h"

const int NUM_CIRCLES = 40;
const float CIRCLE_SIZE = 25.0f;
bool toggleGravity = false;
float gravity = 1.2f;
bool intersecting = false;
#define rnd() (float)rand() / (float)RAND_MAX

struct Global {
	Window w;
	Circle c[NUM_CIRCLES];
	int xres = 3000;
	int yres = 2000;
};

Global g;

void checkMouse(SDL_Event);
bool handleInput(SDL_Event);
bool keyboardInput(SDL_Event);
void render();
void physics();

int main()
{
	srand(time(NULL));
	for(int i=0; i<NUM_CIRCLES; i++) {
		(g.c)[i] = Circle (g.xres*rnd(), g.yres*rnd(), CIRCLE_SIZE);
				//(50*rnd())+20.0f);
	}
	char title[] = {"Circle Simulation"};
	(g.w).init(g.xres, g.yres, title);
	bool leave = false;
/*
	Circle tmp(10.0f, 10.0f, 5.0f);
	IntersectData p1 = tmp.intersectingPoint(5.0f, 10.0f);
	IntersectData p2 = tmp.intersectingPoint(0.0f, 0.0f);
	std::cout << "Point 1 intersecting: " << p1.getIntersecting() <<
		", Distance: " << p1.getDistance() << std::endl;
	std::cout << "Point 2 intersecting: " << p2.getIntersecting() <<
		", Distance: " << p2.getDistance() << std::endl;
*/
	while(!leave) {
		SDL_Event e;
		checkMouse(e);
		leave = handleInput(e);
		(g.w).preRender();
		render();
		(g.w).postRender();
		physics();
	}
/*
	Circle c1(Vec2f (25.0f, 30.0f), 20.0f);
	std::cout << c1.getCenter() << ", Radius: " << c1.getRadius() << std::endl;
	Circle c2(Vec2f (45.0f, 30.0f), 20.0f);
	std::cout << c2.getCenter() << ", Radius: " << c2.getRadius() << std::endl;
	Circle c3(Vec2f (25.0f, 300.0f), 20.0f);
	std::cout << c3.getCenter() << ", Radius: " << c3.getRadius() << std::endl;

	IntersectData c1ic2 = c1.intersectingCircle(c2);
	IntersectData c1ic3 = c1.intersectingCircle(c3);
	IntersectData c2ic3 = c2.intersectingCircle(c3);

	std::cout << "Circle1 intersect Circle2: " << c1ic2.getIntersecting() <<
		", Distance: " << c1ic2.getDistance() << std::endl;
	std::cout << "Circle1 intersect Circle3: " << c1ic3.getIntersecting() <<
		", Distance: " << c1ic3.getDistance() << std::endl;
	std::cout << "Circle2 intersect Circle3: " << c2ic3.getIntersecting() <<
		", Distance: " << c2ic3.getDistance() << std::endl;
*/
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
				if(id.getIntersecting())
					intersecting = true;
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
		if(c->getOnGround()) {
			(g.w).setColor(255, 255, 0);
		}
		else 
			(g.w).setColor(0, 255, 0);
		(g.w).drawCircle(c->getCenter().x, c->getCenter().y, c->getRadius());
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
				Vec2f dist = p->getCenter() - mousePos;
				if(dist.length <= mouseBang) {
					p->setDx(MAX_VEL*dist.x);
					p->setDy(MAX_VEL*dist.y);
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

