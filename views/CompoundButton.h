/*
	CompoundButton.h - A button with two states, checked and unchecked.
	When the button is pressed or clicked, the state changes automatically.

	This methods implements the callback method onChange, that will be called
	if the state has changed

	Created by Ivan Seidel Gomes, May, 2013.
	Released into the public domain.
*/

#ifndef CompoundButton_h
#define CompoundButton_h

#include "View.h"

class CompoundButton: public View{
protected:
	bool _isPressing;
	bool value;

	callback_bool	_onChange;

public:

	CompoundButton(){
		// Call super method
		View::View();

		_onChange = NULL;
		value = false;
	}

	virtual void setValue(bool _value){
		value = _value;
		render(false);
	}

	virtual int getValue(){
		return value;
	}

	virtual void onChange(callback_bool callback){
		_onChange = callback;
	}

	virtual void onChange(){
		if(_onChange)
			_onChange(value);
	}

	virtual void onTouch(ActionEvent evt){
		static bool callOnChange;
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
				callOnChange = _isPressing;
				
				// Invert checkbox value
				if(callOnChange)
					value = !value;

				_isPressing = false;
				render(false);
				// invalidate();
				if(callOnChange)
					onChange();
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