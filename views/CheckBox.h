/*
	CheckBox.h - A checkbox is a specific type of
	two-states button that can be either checked or unchecked.

	use:
		isChecked() to check if it's checked

		setChecked() to set it's current state

	Created by Ivan Seidel Gomes, May, 2013.
	Released into the public domain.
*/

#ifndef CheckBox_h
#define CheckBox_h
	
#include "View.h"
#include "views/CompoundButton.h"

class CheckBox: public CompoundButton{
public:

	Color 	cBg,		// Color of background
			cBgHit,		// Color of background when hitted
			cText,		// Color of texts
			cTextHit;	// Color of texts when hitted

	String name;

	CheckBox(String _name = "Button"){
		// Call super method
		CompoundButton::CompoundButton();

		// Default values to colors
		cBg = color(50,50,50);
		cBgHit = cYELLOW;
		cText = cYELLOW;
		cTextHit = cYELLOW;

		w = 35;
		h = 35;

		name = _name;
	}

	// Just to facilitate
	virtual bool isChecked(){
		return getValue();
	}

	virtual bool setChecked(bool value, bool renderNow = true){
		setValue(value, renderNow);
	}

	virtual void render(bool forceRender = false){
		static int _x1, _x2, _y1, _y2;
		_x1 = x1();
		_x2 = x2();
		_y1 = y1();
		_y2 = y2();
		// Force render usualy is set to true when activity changes,
		// so let's reset ispressing just in case...
		if(forceRender)
			_isPressing = false;
		
		ArdUI::LCD->setColor(cBgHit);
		ArdUI::LCD->drawRoundRect(_x1, _y1, _x2, _y2);

		ArdUI::LCD->setColor(_isPressing ^ value? cBgHit: cBg);
		ArdUI::LCD->fillRect(_x1 + 4, _y1 + 4, _x2 - 4, _y2 - 4);

		ArdUI::LCD->setBackColor(cBLACK);
		ArdUI::LCD->setColor(_isPressing ^ value? cTextHit: cText);
		ArdUI::LCD->print(name, _x2+3, (_y2-_y1)/2 + _y1 - 12);


		_invalidate = false;

	};

};

#endif