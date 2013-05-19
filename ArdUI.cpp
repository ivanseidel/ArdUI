#include "ArdUI.h"
#include "Graphics.h"
#include "View.h"
#include "ViewGroup.h"

#include "ActivityManager.h"

#ifdef DEBUG
	#pragma message("ArdUI DEBUG Mode is ON")
#endif

// Default Setup
bool ArdUI::enabled = false;
UTFT* ArdUI::LCD = NULL;

View* ArdUI::rootView = 0; 	// NULL

ArdUI::TouchTriggerMode ArdUI::touchMode = ArdUI::INTERRUPT_TIMER;
UTouch* ArdUI::touchObject = 0; 	// NULL

int ArdUI::touchInterrupt = -1;

long ArdUI::touchTimerPeriod = 70000; // 70ms is the default
DueTimer* ArdUI::touchTimer = 0;	// NULL

void ArdUI::enableTouch(){
	debug("ArdUI::enableTouch()");
	enabled = true;
	setup();
}

void ArdUI::disableTouch(){
	debug("ArdUI::disableTouch()");
	enabled = false;
	setup();
}

void ArdUI::setup(){
	debug("ArdUI::setup()");
	if(enabled && touchMode != NONE){

		if(touchMode == INTERRUPT_TIMER){

			// Enable interrupt on Touch pin
			#ifdef DEBUG
				if(touchInterrupt < 0)
					debug("$ ArdUI: You didn't setup touchInterrupt!");
			#endif
			detachInterrupt(touchInterrupt);
			attachInterrupt(touchInterrupt, touchHandler, FALLING);

			// Stop Timer
			#ifdef DEBUG
				if(!touchTimer)
					debug("$ ArdUI: You didn't setup touchTimer!");
			#endif
			touchTimer->attachInterrupt(touchHandler);
			touchTimer->setPeriod(touchTimerPeriod);
			// touchTimer->stop();	// This will bug, if changes View and is touching

		}else if(touchMode == INTERRUPT_ONLY){

			// Enable interrupt on Touch pin
			#ifdef DEBUG
				if(touchInterrupt < 0)
					debug("$ ArdUI: You didn't setup touchInterrupt!");
			#endif
			attachInterrupt(touchInterrupt, touchHandler, LOW);

			// Stop Timer
			if(touchTimer)
				touchTimer->stop();

		}else if(touchMode == TIMER_ONLY){

			// Disable interrupt
			if(touchInterrupt >= 0)
				detachInterrupt(touchInterrupt);

			// Enable Timer
			#ifdef DEBUG
				if(!touchTimer)
					debug("$ ArdUI: You didn't setup touchTimer!");
			#endif
			touchTimer->attachInterrupt(touchHandler);
			touchTimer->setPeriod(touchTimerPeriod);
			touchTimer->start();

		}else{
			debug("$ ArdUI: _touchMode not configured! ", touchMode);
		}
	}else{
		// Disable interrupt
		if(touchInterrupt >= 0)
			detachInterrupt(touchInterrupt);

		// Stop Timer
		if(touchTimer)
			touchTimer->stop();
	}
}

// Method that handles touch on screen and send Events
void ArdUI::touchHandler(){
	static bool is_touching = false, same_spot = false;
	static int lastRootViewID = 0, x, y;
	static ActionEvent event;	// Static to not spend memory
	static View* currView = 0;		// Current view being touched
	static View* currRootView = 0;  // Root of the views

	if(touchMode == INTERRUPT_TIMER){
		touchTimer->start();
		detachInterrupt(touchInterrupt);
	}

	if(touchObject->dataAvailable()){

		touchObject->read();
		x = touchObject->getX();
		y = touchObject->getY();

		// Check if is the same spot touched
		same_spot = false;
		if(event.x == x && event.y == y)
			same_spot = true;

		//We copy, to avoid changing while processing
		currRootView = rootView;
		
		// Checks if there is a current activity
		if(currRootView){
			event.x = x;
			event.y = y;

			// hit test the same last view? (and is the same root View)
			if(	currRootView->ViewID == lastRootViewID
				&& currView
				&& currView->hitTest(event.x, event.y)){

				// Did it moved?
				if(!same_spot){
					// Prepare event
					event.type = ACTION_MOVE;
					// Send callback
					currView->onTouch(event);
				}
			}else if(!same_spot){
				// Check if was over another object, if yes
				// send a ACTION_HOVER_EXIT to it
				if(currView){
					event.type = ACTION_HOVER_EXIT;
					currView->onTouch(event);
				}

				// This will return the first view on the Tree
				// that has been hitted. Starting from currRootView
				currView = currRootView->hitTest(event.x, event.y);

				if(currView){
					// If was touching, then is a Hover_enter,
					// otherwise, it's a Down event
					if(is_touching)
						event.type = ACTION_HOVER_ENTER;
					else
						event.type = ACTION_DOWN;
					// Send callback
					currView->onTouch(event);
				}
			}

			is_touching = true; // is touching screen
		}
		// Save this activity ID as last, so we can compare next time
		lastRootViewID = (currRootView? currRootView->ViewID: 0);
	}else{
		// Checks if released button
		// Was there a view being touched?
		if(currView){
			// is the same root view?
			if(rootView->ViewID == lastRootViewID){
				// Prepare event
				event.type = ACTION_UP;
				// Send callback
				currView->onTouch(event);
			}
		}
		// Reset flags
		if(is_touching){
			is_touching = false;
			currView = NULL;
		}

		// If, trigger mode is INTERRUPT_TIMER:
		// * Disable timer
		// * Enable interrupt
		if(touchMode == INTERRUPT_TIMER){
			touchTimer->stop();
			attachInterrupt(touchInterrupt, touchHandler, LOW);
		}
	}
}