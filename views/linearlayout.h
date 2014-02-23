#ifndef LinearLayout_h
#define LinearLayout_h

#include "ViewGroup.h"

class LinearLayout: public ViewGroup
{
protected:
	Orientation orient;

	void organizeViews(){
		int len = total_views;
		Serial.println(total_views);
		
		// Calculates the Relative point
		Point tRel = Point(x1(), y1());

		if(orient == HORIZONTAL){
			// Organize views horizontaly
			for(int i = 0; i < len; i++){
				View *v = views[i];
				// Set relative atribute of view, based on the current view position
				v->_rel = tRel;
				// Calculate next start point
				tRel.x += /*v->o.x + */v->w + margin;
			}
		}else{
			// Organize views verticaly
			for(int i = 0; i < len; i++){
				View *v = views[i];
				// Set relative atribute of view, based on the current view position
				v->_rel = tRel;
				// Calculate next start point
				tRel.y += /*v->o.y + */v->h + margin;
			}
		}
	}

public:
	int margin;

	LinearLayout(Orientation _orient = VERTICAL)
	{
		ViewGroup::ViewGroup();

		orient = _orient;
		margin = 10;
	}

	virtual void render(bool forceRender = false){
		if(forceRender || _invalidate)
			organizeViews();
		// _invalidate = false;

		// No callback set?
		if(_onRender)
			_onRender();

		// Render all views appended to the GroupView
		for(int i = 0; i < total_views; i++){
			if(forceRender || views[i]->shouldRender())  // Render if should render, or is forcing render
				views[i]->render(forceRender);
		}


	}
	
};

#endif