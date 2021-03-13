#include "rendering.h"
#include "vec.h"
#include "circle.h"
#include "gui.h"

const float MAX_CIRCLE_SIZE = 50.0f;
const float MIN_CIRCLE_SIZE = 20.0f;
bool intersecting = false;
#define rnd() (float)rand() / (float)RAND_MAX
enum {TOGGLE_GRAVITY, ADD_CIRCLE, REM_CIRCLE, NUM_BUTTONS};
class Global {
public:
	Window w;
	Circle* c;
	Slider<float>* gravityAmount;
	Button<int> buttons[NUM_BUTTONS];
	Button<int>* curButton;
	int xres, yres;
	int mousex, mousey;
	int numCircles;
	bool toggleGravity;
	float gravity;
	Global()
	{
		gravityAmount = NULL;
		curButton = NULL;
		c = NULL;
		toggleGravity = false;
		gravityAmount = new Slider<float>(10, 10, 0.1f, 2.0f, 0.1f);
		gravity = gravityAmount->getValue();
		numCircles = 2;
		int x = gravityAmount->getLastX() + 20;
		float w = 40;
		c = new Circle[numCircles];
		for(int i=0; i<NUM_BUTTONS; i++) {
			buttons[i].init(x, 0, w, w, i);
			x += (w/2) + w;
		}
	}
	~Global()
	{
		if(c != NULL) {
			delete c;
			c = NULL;
		}
		delete gravityAmount;
		gravityAmount = NULL;
	}
	void addCircle()
	{
		Circle* tmp = c;
		c = new Circle[numCircles+1];
		for(int i=0; i<numCircles; i++) {
			c[i] = tmp[i];
		}
		float xpos = (float)(rand() % (int)(xres-MIN_CIRCLE_SIZE) + MIN_CIRCLE_SIZE);
		float ypos = (float)(rand() % (int)(yres-MIN_CIRCLE_SIZE) + MIN_CIRCLE_SIZE);
		float radius = rand() % (int)MAX_CIRCLE_SIZE + MIN_CIRCLE_SIZE;
		c[numCircles] = Circle(xpos, ypos, radius);
		numCircles++;
	}
	void remCircle()
	{
		if(numCircles == 0) return;
		if(numCircles == 1) {
			delete c;
			c = NULL;
			numCircles = 0;
		} else {
			Circle* tmp = c;
			c = new Circle[numCircles-1];
			for(int i=0; i<numCircles-1; i++) {
				c[i] = tmp[i];
			}
			numCircles--;
		}
	}
	void initCircles()
	{
		c = new Circle[numCircles];
		for(int i=0; i<numCircles; i++) {
			float xpos = (float)(rand() % (int)(xres-MIN_CIRCLE_SIZE) + MIN_CIRCLE_SIZE);
			float ypos = (float)(rand() % (int)(yres-MIN_CIRCLE_SIZE) + MIN_CIRCLE_SIZE);
			float radius = rand() % (int)MAX_CIRCLE_SIZE + MIN_CIRCLE_SIZE;
			c[i] = Circle(xpos, ypos, radius);
		}
	}
	void processSettings()
	{
		if(gravityAmount->slide(mousex) && gravityAmount->getValue() != gravity) {
			gravity = gravityAmount->getValue();
		}
		if(curButton != NULL) {
			switch (curButton->getValue()) {
				case 0:
					toggleGravity ^= 1;
					break;
				case 1:
					// Add circle
					addCircle();
					break;
				case 2:
					// Remove last circle
					remCircle();
					break;
				default:
					break;
			}
			curButton = NULL;
		}
	}
	void render()
	{
		for(int i=0; i<numCircles; i++) {
			Circle* cur = &(c)[i];
			if(cur->onGround)
				w.setColor(0, 255, 0);
			else 
				w.setColor(255, 255, 0);
			w.fillCircle(cur->center.x, cur->center.y, cur->radius);
		}
		gravityAmount->render(w);
		for(int i=0; i<NUM_BUTTONS; i++) {
			Button<int>* cur = &buttons[i];
			switch (cur->getValue()) {
				case 0:
					w.setColor(200, 200, 200);
					break;
				case 1:
					w.setColor(0, 255, 0);
					break;
				case 2:
					w.setColor(255, 0, 0);
					break;
				default:
					break;
			}
			w.fillRect(cur->getX(), cur->getY(), cur->getWidth(), cur->getHeight());
		}
	}
} g;

void init()
{
	std::cout << "Creating window\n";
	g.xres = 800;
	g.yres = 800;
	char title[] = {"Circle Simulation"};
	(g.w).init(g.xres, g.yres, title);
	std::cout << "Initializing circles\n";
	g.initCircles();
	std::cout << "Finished initializing!\n";
}

void checkMouse(SDL_Event);
bool handleInput(SDL_Event);
bool keyboardInput(SDL_Event);
void render();
void physics();

int main()
{
	srand(time(NULL));
	init();
	bool leave = false;
	while(!leave) {
		SDL_Event e;
		checkMouse(e);
		g.processSettings();
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
	float gravity = g.gravity;
	if(toggle) {
		for(int i=0; i<g.numCircles; i++) {
			Circle* p1 = &(g.c)[i];
			p1->move(g.xres, g.yres, g.toggleGravity, gravity);
			for(int j=i+1; j<g.numCircles; j++) {
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
		for(int i=g.numCircles-1; i>=0; i--) {
			Circle* p1 = &(g.c)[i];
			p1->move(g.xres, g.yres, g.toggleGravity, gravity);
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
	g.render();
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
				g.toggleGravity ^= 1;
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
	static float mouseBang = 500.0f;
	static bool lbutton_down=false, rbutton_down=false;
	if(e.type == SDL_MOUSEMOTION) {
		// Mouse moved
		SDL_GetMouseState(&g.mousex, &g.mousey);
		for(int i=0; i<g.numCircles; i++) {
			Circle* p = &(g.c)[i];
			IntersectData id = p->intersectingPoint((float)g.mousex, (float)g.mousey);
			if(id.getIntersecting()) {
				// Mouse is touching a circle
			}
		}
	}

	if(e.type == SDL_MOUSEBUTTONDOWN) {
		// Mouse click
		if(e.button.button == SDL_BUTTON_LEFT) {
			g.gravityAmount->setClicked(g.gravityAmount->checkMouse(g.mousex, g.mousey));
			// Vec2f mousePos((float)mousex, (float)mousey);
			// for(int i=0; i<g.numCircles; i++) {
			// 	Circle* p = &(g.c)[i];
			// 	Vec2f dist = p->center - mousePos;
			// 	if(dist.length <= mouseBang) {
			// 		p->addForce(MAX_VEL*dist.x, MAX_VEL*dist.y);
			// 	}
			// }
		}
	}
	if(e.type == SDL_MOUSEBUTTONUP) {
		g.gravityAmount->setClicked(false);
		Button<int>* cur;
		for(int i=0; i<NUM_BUTTONS; i++) {
			cur = &g.buttons[i];
			if(cur->checkMouse(g.mousex, g.mousey)) {
				g.curButton = cur;
				break;
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

