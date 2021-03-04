## ABOUT
> Small graphical application written in C++ demonstrating elastic collisions between circle objects.
## BUILD
* SDL2 developer library needs to be installed
	* **Linux**: *sudo apt-get install libsdl2-dev*
	* **Windows**: [Tutorial](https://www.matsson.com/prog/sdl2-mingw-w64-tutorial.php)
---
Then in cloned directory:
* **Build**: *make*
---
Execution: ***./main***
## FEATURES
* Amount of balls and ball radii can be tweaked near the top of main.cpp
* Toggle gravity by pressing 'g'
* Push balls by bumping the mouse into them
* Left-click to create a force that pushes all nearby balls outward
* Quick program termination by pressing 'Esc'
