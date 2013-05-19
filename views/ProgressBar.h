/*
	ProgressBar.h - Visual indicator of progress in some operation.
	Displays a bar to the user representing how far the operation
	has progressed; the application can change the amount of progress
	(modifying the size of the bar) as it moves forward.

	Use:
		setValue(long value), getValue()
		setMax(long max), getMax()
		setMin(long min), getMin()
		...


	Created by Ivan Seidel Gomes, May, 2013.
	Released into the public domain.
*/

#ifndef ProgressBar_h
#define ProgressBar_h

#include "View.h"

class ProgressBar: public View
{
protected:
	long 	_value,	// Current progress
			_min,	// Value representing minimum fill
			_max;	// Value representing the maximum fill

	int _lastSize;

	callback_int _onChange;

public:
	Color 	cBg,
			cProgress,
			cText;

	enum Type {
		LINEAR_UP,
		LINEAR_RIGHT
	};

	Type type;	// The type of ProgressBar
				// This is done, because we can implement
				// circular progressbar, point progressbar...

	ProgressBar(){
		// Call super method
		View::View();

		// Default values to colors
		cBg = cGRAY;
		cProgress = cYELLOW;
		cText = cYELLOW;

		type = LINEAR_RIGHT;

		w = 100;
		h = 20;

		_min = 0;
		_max = 100;
		_value = 0;

		_lastSize = 0;

		_onChange = NULL;

		_requestTouch = false;
	}

	virtual void render(bool forceRender = false){
		// Store the progress in pixels
		int progressPx,
			_x1 = x1(),
			_x2 = x2(),
			_y1 = y1(),
			_y2 = y2();

		// Fixes "left characters" on the side of the text
		static String out, text;
		static int dif, size;
		text = String(getValue());
		size = text.length();
		dif = _lastSize - size;
		_lastSize = size;
		out = text;
		for(int i = 0; i < dif; i++)
			out = out + String(" ");

		switch (type){
			case LINEAR_UP:
				/*
					Render a ProgressBar in the UP position
				*/
				progressPx = map(
				getValue(),
				_min, _max,0, h); // Map from _min and _max to Width

				// Fill the background
				ArdUI::LCD->setColor(cBg);
				ArdUI::LCD->fillRect(
					_x1,
					_y1,
					_x2,
					_y2-progressPx);

				// Draw progress
				ArdUI::LCD->setColor(cProgress);
				ArdUI::LCD->fillRect(
					_x1,
					_y2-progressPx,
					_x2,
					_y2);

				// Print name
				ArdUI::LCD->setBackColor(cBLACK);
				ArdUI::LCD->setColor(cText);
				ArdUI::LCD->print(out, _x2+3, (_y2-_y1)/2 + _y1 - 12);

				break;
			case LINEAR_RIGHT:
				/*
					Render a ProgressBar in the RIGHT position
				*/
				progressPx = map(
					getValue(),
					_min, _max,0, w); // Map from _min and _max to Width

				// Fill the background
				ArdUI::LCD->setColor(cBg);
				ArdUI::LCD->fillRect(
					_x1+progressPx,
					_y1,
					_x2,
					_y2);

				// Draw progress
				ArdUI::LCD->setColor(cProgress);
				ArdUI::LCD->fillRect(
					_x1,
					_y1,
					_x1+progressPx,
					_y2);

				// Print name
				ArdUI::LCD->setBackColor(cBLACK);
				ArdUI::LCD->setColor(cText);
				ArdUI::LCD->print(out, _x2+3, (_y2-_y1)/2 + _y1 - 12);
				break;
		}

		_invalidate = false;
	}


	virtual void setValue(long value, bool renderNow = true){
		_value = value;

		if(_onChange)
			_onChange(_value);

		if(renderNow){
			render(false);
		}else{
			invalidate();
		}
	}

	virtual void onChange(callback_int callback){
		_onChange = callback;
	}

	virtual long getValue(){
		return min(max(_value, _min),_max);
	}

	virtual void setMax(long max){
		_max = max;
	}

	virtual long getMax(){
		return _max;
	}

	virtual long getMin(){
		return _min;
	}



};

#endif