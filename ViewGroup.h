/*
  ViewGroup.h - Implementation to support multiple views in one single view.

  This class is responsable for Holding multiple views in one single View.

  Created by Ivan Seidel Gomes, May, 2013.
  Released into the public domain.
*/

#ifndef ViewGroup_h
#define ViewGroup_h

#include "View.h"

#define MAX_VIEWS 40

class ViewGroup: public View
{
protected:
	View* views[MAX_VIEWS];
	int total_views;

	virtual void fixArray();

public:

	ViewGroup();
	virtual bool addView(View* view);
	virtual View* getChildAt(int index);
	virtual int getChildCount();
	virtual void removeAllViews();
	virtual void removeChildAt(int index);
	virtual void removeChild(View* view);

	virtual View* hitTest(Point p);
	virtual View* hitTest(long x, long y);

	// Overrides methods for Render, since there are many views on Group
	virtual void onRender(onRenderHandler func);
	virtual void render(bool forceRender = false);
};


#endif