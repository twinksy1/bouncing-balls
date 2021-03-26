#include "gui.h"
#include <iostream>
#include <cmath>

template <class T>
Slider<T>::Slider() {}
template <class T>
Slider<T>::~Slider() {}
template <class T>
Slider<T>::Slider(int x, int y, T min, T max, T inc) {
    this->x = x;
    offsetx = x;
    this->y = y;
    this->min = min;
    this->max = max;
    this->inc = inc;
    hovering = clicked = false;
    lastX = (max / inc) * SLIDER_RADIUS;
    value = min;
}
template <class T>
bool Slider<T>::checkMouse(int mousex, int mousey) {
    float xdiff = mousex - offsetx;
    float ydiff = mousey - y;
    float dist = sqrt(xdiff*xdiff + ydiff*ydiff);
    if(dist <= SLIDER_RADIUS) {
        hovering = true;
        return true;
    } else {
        hovering = false;
        return false;
    }
}
template <class T>
bool Slider<T>::slide(int mousex) {
    if(clicked) {
        if(mousex > offsetx) {
            // Sliding right
            while(offsetx + SLIDER_RADIUS < mousex && value < max) {
                offsetx += SLIDER_RADIUS;
                value += inc;
            }
        } else if(mousex < offsetx) {
            // Sliding left
            while(offsetx - SLIDER_RADIUS > mousex && value > min) {
                offsetx -= SLIDER_RADIUS;
                value -= inc;
            }
        }
        return true;
    }
    return false;
}
template <class T>
void Slider<T>::render(Window& win) {
    win.setColor(255,255,255);
    win.drawLine(x, y, x + lastX, y);
    win.setColor(0,255,0);
    win.fillCircle(offsetx, y, SLIDER_RADIUS);
    std::string val = std::to_string(value);
    win.setColor(0,0,0);
}
template <class T>
void Slider<T>::setClicked(bool b) {
    clicked = b;
}
template <class T>
void Slider<T>::setHovering(bool b) {
    hovering = b;
}
template <class T>
T Slider<T>::getValue() {
    return value;
}
template <class T>
int Slider<T>::getClicked() {
    return clicked;
}
template <class T>
int Slider<T>::getHovering() {
    return hovering;
}
template <class T>
int Slider<T>::getLastX() {
    return lastX;
}

template <class T>
Button<T>::Button() {
    this->highlighted = false;
}
template <class T>
Button<T>::Button(int x,int y,float w,float h,T val) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->val = val;
    this->highlighted = false;
}
template <class T>
Button<T>::~Button() {

}
template <class T>
void Button<T>::init(int x,int y,float w,float h,T val) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->val = val;
    this->highlighted = false;
}
template <class T>
bool Button<T>::checkMouse(int mousex,int mousey) {
    if(mousex >= x && mousex <= x + w && mousey >= y && mousey <= y + h) {
        this->highlighted = true;
        return true;
    } else {
        this->highlighted = false;
        return false;
    }
}
template <class T>
int Button<T>::getX() {
    return x;
}
template <class T>
int Button<T>::getY() {
    return y;
}
template <class T>
T Button<T>::getValue() {
    return val;
}
template <class T>
bool Button<T>::isHighlighted() {
    return highlighted;
}
template <class T>
float Button<T>::getWidth() {
    return w;
}
template <class T>
float Button<T>::getHeight() {
    return h;
}

template class Button<int>;
// template class Slider<int>;
template class Slider<float>;