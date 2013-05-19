/*
	SeekBar.h - A SeekBar is an extension of ProgressBar that adds a
	draggable thumb. The user can touch the thumb and drag left or
	right to set the current progress level or use the arrow keys.

	Placing focusable widgets to the left or right of a SeekBar is discouraged.

	Minimum and maximum value is configurable, but CANNOT be floating points

	onChange(function) will set a callback for the View, when it's value
	has changed.

	Created by Ivan Seidel Gomes, May, 2013.
	Released into the public domain.
*/

#ifndef SeekBar_h
#define SeekBar_h

#include "View.h"
#include "views/ProgressBar.h"

class SeekBar: public ProgressBar
{
protected:

	/*
		This is the method responsable for "translating"
		from an hit on the screen at x and y, to a linear value
		that maches the position hitted.

		We could say, that this is what gets the position on the
		screen, and finds out what is the desired value selected.
	*/
	virtual long intersectActionEvent(ActionEvent *evt){
		long _1, _2, _v;// Two values, indicating the lowest
						// and the higgest possible evt.x or evt.y
						// (depends on the type of SeekBar)

		// This is a preview of what we are doing, on a numeric scale:
		// ------ (_1) ------- (_v) --------------------- (_2) ---
		// Should also be filtered, to bound _1 and _2;
		switch(type){
			case LINEAR_UP:
				_1 = y2();
				_2 = y1();
				_v = min(max(_2, evt->y), _1);
				break;

			case LINEAR_RIGHT:
				_1 = x1();
				_2 = x2();
				_v = min(max(_1, evt->x), _2);

				break;
		}

		// Now, we need to calculate "how much" _v is between _1 and _2
		return map(_v, _1, _2, getMin(), getMax());
	}


public:
	SeekBar(){
		ProgressBar::ProgressBar();
		// Increases bound sizes, to facilitate touch
		_left = 30;
		_bottom = 30;
		_right = 30;
		_top = 30;

		// Enable touch, disabled by ProgressBar
		_requestTouch = true;
	};

	virtual void onTouch(ActionEvent evt){
		static bool callOnChange;
		switch(evt.type){
				
			case ACTION_DOWN:
			case ACTION_MOVE:
	        	setValue(intersectActionEvent(&evt), true); // true to render now
	        	break;

	        case ACTION_HOVER_EXIT:
			case ACTION_UP:
				break;

	    }
	};

};

#endif