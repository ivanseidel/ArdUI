/*
	Button.h - A button to be clicked (Yes, just that)
	
	This methods implements the callback method onClick, that will be called
	if the button has ben Clicked, and followed those rules:
		The button received a ACTION_DOWN
		The button received 0 or more ACTION_MOVE
		The button received a ACTION_UP

	If those requirements are satisfied, the button sends the onClick event;


	Created by Ivan Seidel Gomes, May, 2013.
	Released into the public domain.
*/

#ifndef Button_h
#define Button_h

#include "View.h"

#define DEF_cBg 		cGRAY
#define DEF_cBgHit		cYELLOW
#define DEF_cText 		cBLACK
#define DEF_cTextHit	cBLACK
#define DEF_cBorder		cGREEN

extern uint8_t OCR_A_Extended_M[];	// Font

class Button: public View{
protected:
	bool _isPressing;
	long _onDownTime;	// Keeps the button down timestamp

	callback 	//_onLongPress,	// Called if long press is detected
				_onClick;		// Called if a click (ACTION_DOWN and ACTION_UP)
								// is detected within a certain time.

public:

	Color 	cBg,		// Color of background
			cBgHit,		// Color of background when hitted
			cText,		// Color of texts
			cTextHit;	// Color of texts when hitted

	String name;

	Button(String _name = "Button"){
		// Call super method
		View::View();

		// Default values to colors
		cBg = DEF_cBg;
		cBgHit = DEF_cBgHit;
		cText = DEF_cText;
		cTextHit = DEF_cTextHit;

		_onClick = NULL;
		name = _name;
	}

	virtual void render(bool forceRender = false){
		// Force render usualy is set to true when activity changes,
		// so let's reset ispressing just in case...
		if(forceRender)
			_isPressing = false;

		// Set font
		ArdUI::LCD->setFont(OCR_A_Extended_M);

		// Render button bg for current state
		if(_isPressing){
			ArdUI::LCD->setColor(cBgHit);
			ArdUI::LCD->setBackColor(cBgHit);
		}else{
			ArdUI::LCD->setColor(cBg);
			ArdUI::LCD->setBackColor(cBg);
		}
		ArdUI::LCD->fillRect(	o.x + _rel.x,	//x1
						o.y + _rel.y,				//y1
						o.x + _rel.x + w, 			//x2
						o.y + _rel.y + h);			//y2

		ArdUI::LCD->setColor(_isPressing ? cTextHit: cText);
		ArdUI::LCD->print(name,		// String
					o.x + _rel.x + 10,	// x
					o.y + _rel.y + 10);	// y

		_invalidate = false;

	};

	virtual void onClick(callback callback){
		_onClick = callback;
	}

	virtual void onClick(){
		if(_onClick)
			_onClick();
	}

	virtual void onTouch(ActionEvent evt){
		static bool callOnClick;
		switch(evt.type){
				
			case ACTION_DOWN:
				_isPressing = true;
				render(false);
				// invalidate();
				break;
			case ACTION_MOVE:
				break;
			case ACTION_UP:
				// Save to read after render, to call onClick
				callOnClick = _isPressing;
				_isPressing = false;
				render(false);
				// invalidate();
				if(callOnClick)
					onClick();
				break;
			case ACTION_HOVER_EXIT:
				_isPressing = false;
				// invalidate();
				render(false);
				break;
	        default:
	        	_isPressing = false;
	    }
	};

};

#endif