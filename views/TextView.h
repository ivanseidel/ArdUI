/*
	TextView.h - Simple class to render Text.

	This also takes care of "fixing" left chars on the
	right of the string.

	For example: If you print "100" to the screen, and then print "99",
	the last char will remain, and will apear as "990".
	This class implements a simple way to fix this, by cleaning left chars.

	Created by Ivan Seidel Gomes, May, 2013.
	Released into the public domain.
*/

#ifndef TextView_h
#define TextView_h

#include "View.h"

class TextView: public View
{
protected:
	String text;
	int _lastSize;

public:

	Color 	cText,	// Color of the text
			cBg;	// Color of the background

	TextView(String _text = ""){
		View::View();

		text = _text;
		cText = cYELLOW;
		cBg = cBLACK;
		_lastSize = 0;

		_requestTouch = false;
	}

	virtual void render(bool forceRender = false){
		static String out;
		static int dif;

		dif = _lastSize - text.length();
		_lastSize = text.length();
		out = text;
		for(int i = 0; i < dif; i++)
			out = out + String(" ");

		ArdUI::LCD->setColor(cText);
		ArdUI::LCD->setBackColor(cBg);
		ArdUI::LCD->print(out, x1(), y1());

		_invalidate = false;
	}

	virtual void setValue(String _text, bool renderNow = false){
		text = _text;
		if(renderNow){
			render(false);
		}else{
			invalidate();
		}
	}

	void setValue(long _text, bool renderNow = false){
		setValue(String(_text));
	}

	void setValue(int _text, bool renderNow = false){
		setValue(String(_text));
	}

	virtual String getValue(){
		return text;
	}


};

#endif