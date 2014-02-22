/*
	ArdUI.h - Responsable for handeling touch actions and
	dispaching event to the corresponding views.
	  
	This Library requires:
		+ UTFT	 	(https://github.com/ivanseidel/UTFT)
		+ UTouch 	(http://bit.ly/10FnSpC)
		+ DueTimer 	(https://github.com/ivanseidel/DueTimer)

	For more information about this library, go to:
	https://github.com/ivanseidel/ArdUI

	Created by Ivan Seidel Gomes, May, 2013.
	Released into the public domain.
*/

#ifndef ArdUI_h
#define ArdUI_h

#include <inttypes.h>

// SubIncludes
#include "ArdUITypes.h"
#include "View.h"
#include "ViewGroup.h"

// External Includes
#include <DueTimer.h>
#include <UTouch.h>
#include <UTFT.h>

#ifndef DEBUG_FUNCS
//#define DEBUG
#define DEBUG_FUNCS 1
#ifdef DEBUG
    inline void debug(const char *s) { Serial.println(s); }
    inline void debug(const int s) { Serial.println(s); }
    inline void debug(const char *s, const char *t) { Serial.print(s);Serial.println(t); }
    inline void debug(const char *s, const int t) { Serial.print(s);Serial.println(t); }
#else
    inline void debug(const char *s) {  }
    inline void debug(const int i) {  }
    inline void debug(const char *s, const char *t) { }
    inline void debug(const char *s, const int t) { }
#endif
#endif

/*
	This class provides static methods, usefull for many situations.

	Use this class to controll the Display touch and send callbacks
	to current views.

	Note: Rendering is not done here, but in each view, or, by
	your main program.

	TouchTriggerMode explanation:
		Since many people will be using this library, they might want
		to  change the way touch is handled. We provide a few, basic
		modes, that requires or not Interrupt pins AND/OR Timer.

		TouchTriggerMode.NONE
			Set to NONE when you will call handleTouch yourself,
			externaly to the class

		TouchTriggerMode.INTERUPT_ONLY
			Will call handleTouch every time that the signal is LOW
			Note that: When no touch is detected, the callback
			will NOT run, so, detecting ACTION_UP will be imposible,
			since we require to read a NO TOUCH action

		TouchTriggerMode.TIMER_ONLY
			This will start a Timer with the callback to handleTouch.
			Note that: Even when NO touch is received, the action will
			continuously be runned, spending few, but constantly CPU

		TouchTriggerMode.INTERRUPT_TIMER 
			!!! THE BEST ONE, AND DEFAULT !!!
			This is a MIX of both INTERRUPT_ONLY and TIMER_ONLY.
			Here is how it works:
				After calling initialize (just for the first time),
				this will configure the INTERRUPT PIN to detect FALLING edge
				and to call touchHandler when interrupted.

				Once the Interrupt is detected, we will DISABLE the interrupt,
				and start the Timer to call touchHandler.

				After touchHandler detecting NO TOUCH on screen	(will be
				possible, since Timer will continue to run, even when 
				the finger is not anymore on the screen), he will DISABLE
				the Timer, and Initialize the Interrupt pin.

				This mode is "the perfec" mode, spending CPU just when it needs
*/

class ArdUI
{
protected:
	static bool enabled;
	
public:

	// Touch modes
	enum TouchTriggerMode{
		NONE,				// No touch is enabled
		INTERRUPT_ONLY,		// Interrupt mode is enabled (Consumes LOT of CPU)
		TIMER_ONLY,			// Consumes less, but frequently CPU
		INTERRUPT_TIMER 	// Intercalates bettwen interrupt and Timer (BEST)
	};

	enum TouchPerception{
		NORMAL,
		INVERTED_X,
		INVERTED_Y,
		INVERTED_X_Y
	};

	/*
		This is the CURRENT ROOT View object.

		Imagine, that each screen is a View (or ViewGroup).
		This is where the touchHandler will start seeking the touch,
		and will ONLY send Events to views WITHIN this View.

		Remember to change this, everytime you change between screens
	*/
	static View* rootView;

	static UTFT* LCD;	// Default renderer to use within views
						// This is what Views will call to render
						// things, so MAKE SHURE IT's SETUP correcly

	static long lcdWidth;  // use this to allow a global lcd width value
	static long lcdHeight; // use this to allow a global lcd height value

	// Trigger mode. Best is INTERRUPT_TIMER
	static TouchTriggerMode touchMode;
	// Touch perception mode.
	static TouchPerception touchPerception;

	// Touch object (read data from touch)
	static UTouch* touchObject;

	// INTERRUPT_ONLY and INTERRUPT_TIMER
	// Touch interrupt (used to detect touch Falling edges)
	static int  touchInterrupt;

	// TIMER_ONLY and INTERRUPT_TIMER
	// Period of timer in uS (Period of update, after first touch is triggered)
	static long  touchTimerPeriod;
	// Timer to run touch listner (Timer responsable for running touchHandler)
	static DueTimer* touchTimer;

	// Methods to enable and Disable touch
	static void enableTouch();
	static void disableTouch();

	// Call this method to initialize Timers, Interrupts...
	static void setup();

	/*
		Method responsable for actualy DOING the hard work,
		Preparing and Sending Events to Views;

		Should be runned every time the finger is on the screen,
		and once after the finger is not anymore on the screen

		Should be PROTECTED, but since this will be called
		from external interrupts, we need it to be public.
	*/
	static void touchHandler();

	/*
		This is the callback method for ALL views,
		called everytime a invalidate() is done over a view.

		Setup properly this method, to receive this events.
	*/
	static callback onInvalidateView;
};

#endif