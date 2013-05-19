#include <Arduino.h>

#include "View.h"
#include "Graphics.h"

View::callback View::onInvalidateView = NULL;

View::View(){
	_onTouch = NULL;
	_onRender = NULL;
	_invalidate = true;
	_rel = Point(0,0); // Relative position is 0
	_requestTouch = true;
	o = Point(0,0);
	w = 0;
	h = 0;
	_left = 0;
	_top = 0;
	_right = 0;
	_bottom = 0;
	ViewID = (int)this;
}

/*
	CallBack for onTouch
*/
void View::onTouch(ActionEvent evt){
	evt.x-=_rel.x;
	evt.y-=_rel.y;
	// No callback set?
	if(!_onTouch) return;
	_onTouch(evt);
}

void View::onTouch(onEventHandler func){
	_onTouch = func;
}

/*
	CallBack for onRender
*/
void View::render(bool forceRender){
	// This flag is reset before render, because the view
	// can invalidate itself on the rendering proccess, 
	// and invalidating after rendering will ovewrite _invalidate flag
	_invalidate = false;

	// No callback set?
	if(!_onRender) return;
		_onRender();
}

void View::onRender(onRenderHandler func){
	_onRender = func;
}

/*
	HitTest, to check if hitted the rectangle of view
*/
View* View::hitTest(Point p){
	return hitTest(p.x, p.y);
}

View* View::hitTest(long x, long y){

	// Don't even try to hitTest, if this view didn't requested touch
	if(!_requestTouch)
		return NULL;

	long x1 = o.x + _rel.x,
		x2 = x1 + w;
	long y1 = o.y + _rel.y,
		y2 = y1 + h;

	if (x >= x1 - _left && x <= x2 + _right &&
		y >= y1 - _top  && y <= y2 + _bottom){
		return this;
	}
	
	return NULL;
}

bool View::shouldRender(){
	return _invalidate;
}

void View::setArea(Point _origin, int _width, int _height){
	_requestTouch = true;
	o = _origin;
	w = _width;
	h = _height;
}
void View::setArea(int x, int y, int _width, int _height){
	setArea(Point(x, y), _width, _height);
}

int View::x1(){
	return o.x + _rel.x;
}
int View::y1(){
	return o.y + _rel.y;
}
int View::x2(){	
	return x1()+w;
}
int View::y2(){	
	return y1()+h;
}

void View::invalidate(){
	_invalidate = true;
	if(View::onInvalidateView)
		View::onInvalidateView();
}