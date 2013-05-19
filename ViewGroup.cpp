#include <Arduino.h>

#include "View.h"
#include "ViewGroup.h"

ViewGroup::ViewGroup(){
	View::View();

	total_views = 0;
	_invalidate = true; // Should always be true on GroupView
}

void ViewGroup::fixArray(){
	int curRole = 0;

	for(int i = 0; i < MAX_VIEWS; i++)
	{
		if(!views[i])
		{
			for(int j = i+1; j < MAX_VIEWS; j++)
			{
				if(views[j])
				{
					views[i] = views[j];
					break;
				}
			}
		}
	}
}

bool ViewGroup::addView(View* view){
	if(total_views >= MAX_VIEWS || !view)
		return false;

	// Checks if Current view is not already on the array
	for(int i = 0; i < total_views; i++)
		if(views[i]->ViewID == view->ViewID)
			// Already added
			return true;

	// Otherwise, add it and increment total_views
	views[total_views] = view;
	total_views++;

	// Set flag to request touch
	_requestTouch = true;
}

View* ViewGroup::getChildAt(int position){
	if(position >= 0 && position < total_views)
		return views[position];

	return 0; // NULL
}

int ViewGroup::getChildCount(){
	return total_views;
}

void ViewGroup::removeAllViews(){
	for(int i = 0; i < MAX_VIEWS; i++){
		views[i] = 0; // NULL
	}
	total_views = 0;
}

void ViewGroup::removeChildAt(int index){
	if(index >= total_views)
		return;

	views[index] = 0; // NULL
	total_views--;
	fixArray();
}

void ViewGroup::removeChild(View* view){
	// Checks if Current view is on the array
	for(int i = 0; i < total_views; i++)
		if(views[i]->ViewID == view->ViewID){
			views[i] = 0; // NULL
			total_views--;
			fixArray();
			return;
		}
}

/*
	CallBack for onRender
*/
void ViewGroup::render(bool forceRender){
	// No callback set?
	if(_onRender)
		_onRender();

	// Calculates the Relative point
	Point tRel = Point(_rel.x+o.x, _rel.y+o.y);

	// Render all views appended to the GroupView
	for(int i = 0; i < total_views; i++){
		views[i]->_rel = tRel;	// Set relative atribute of view, based on the current view position
		if(forceRender || views[i]->shouldRender())  // Render if should render, or is forcing render
			views[i]->render(forceRender);
	}
}

void ViewGroup::onRender(onRenderHandler func){
	_onRender = func;
}

/*
	hitTest, is responsable for "checking" if a view is hit by a touch.
	on GroupView, this is done by checking each of the views appended to
	this group.
*/

View* ViewGroup::hitTest(Point p){
	return hitTest(p.x, p.y);
}

View* ViewGroup::hitTest(long x, long y){
	View* test;
	for(int i = 0; i < total_views; i++){
		test = views[i]->hitTest(x, y);
		if(test)
			return test;
	}
	return 0; // NULL
}