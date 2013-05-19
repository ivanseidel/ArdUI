/*
	ArdUITypes.h - Keeps all types used in ArdUI Views and Controllers

	Created by Ivan Seidel Gomes, May, 2013.
	Released into the public domain.
*/

#ifndef ArdUITypes_h
#define ArdUITypes_h

#include <inttypes.h>

/*
	List of common methods that could be generated.
	Some are generic, and some are used by the ScreenController
	to create Callbacks with the action type.
*/
enum ActionType {
	/*
		When a view is pressed, AND, the screen was not
		being touched, ACTION_DOWN will be sent to the view
	*/
	ACTION_DOWN,
	/*
		When the screen is released, AND, was touching some view,
		ACTION_UP will be sent to the view.
	*/
	ACTION_UP,
	/*
		When the screen IS ALREADY being touched, AND, is hitting
		a view. ACTION_MOVE will be sent to the view, indicating
		movement OVER the view
	*/
	ACTION_MOVE,
	/*
		When the screen IS ALREADY being touched, AND:
			There WAS a current view being touched,
			or, Nothing WAS being touched
		AND:
			There IS a view for the FIRST time hitted
			(Probably, the finger has "moved inside" a view)
		ACTION_HOVER_ENTER will be sent, indicating that the view
		has been "pressed", but the finger was already on the screen
	*/
	ACTION_HOVER_ENTER,
	/*
		When a view WAS being touched, AND, the finger STILL on the screen,
		ACTION_HOVER_EXIT will be sent.

		Note:
			If there are two close views, and the finger passes from View1
			to View2, then:
				[...]
				Send ACTION_HOVER_EXIT to View1
				Send ACTION_HOVER_ENTER to View2
				[...]
			This way, a view should always know that it is not being pressed
			anymore, by: ACTION_HOVER_EXIT or ACTION_UP
	*/
	ACTION_HOVER_EXIT,

	/*
		The rest of ActionTypes are made to be used with ArduinOS, but fell free
		to use if needed
	*/
	// Use this to notify a CONFIRM action
	ACTION_CONFIRM,
	// Use this to RETURN to the parent Activity (previews)
	ACTION_RETURN,

	// If you have more buttons, then use the default ones given
	ACTION_DEF_1,	// Default button 1
	ACTION_DEF_2,	// Default button 2
	ACTION_DEF_3,	// Default button 3
	ACTION_DEF_4,	// Default button 4
};


/*
	This will contain all sort of View alignment and gravity
	settings.
*/
enum Alignment{
	ALIGN_LEFT,	// Force alignment to the Left of orientation (o)
	ALIGN_RIGHT // Force alignment to the Right of orientation (o)
};


/*
	This is the struct that will be sent to the views whenever an
	action occurs. X and Y are values, used to handle Touch events,
	and ActionType holds the triggered type event.
*/
struct ActionEvent{
	int x;				// Holds x position of event
	int y;				// Holds y position of event
	ActionType type;	// Holds the Type of action
};

// Define default Color type
typedef uint16_t Color;

// Used to convert on compilation time, an RGB color to VGA
inline Color color(int r, int g, int b){
	return (((r&248)|g>>5) << 8) | ((g&28)<<3|b>>3);
}

#endif