#ifndef GUI_H
#define GUI_H

#include "rendering.h"
const int SLIDER_RADIUS=10;

template <class T>
class Button {
private:
    int x,y;
    float w,h;
    bool highlighted;
    T val;
public:
    Button();
    Button(int,int,float,float,T);
    ~Button();
    void init(int,int,float,float,T);
    bool checkMouse(int,int);
    int getX();
    int getY();
    float getWidth();
    float getHeight();
    bool isHighlighted();
    T getValue();
};

template <class T>
class Slider {
private:
    int x,y;        // original position
    int offsetx;    // slid position
    int lastX;
    T value;
    T min,max;
    T inc;
    bool clicked;
    bool hovering;
public:
    Slider();
    Slider(int,int,T,T,T);    // x, y, min, max, inc
    ~Slider();
    bool checkMouse(int,int);
    bool slide(int);
    void render(Window&);
    void setClicked(bool);
    void setHovering(bool);
    T getValue();
    int getClicked();
    int getLastX();
    int getHovering();
};

#endif