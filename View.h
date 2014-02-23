/*
  View.h - This is the BASE class for ALL Views.
  
  Keeps the basic view flags and methods.
  
  All others Views should Extend this class, in order
  to connect properly with others.

  Created by Ivan Seidel Gomes, May, 2013.
  Released into the public domain.
*/

#ifndef View_h
#define View_h

#include "ArdUITypes.h"
#include "Graphics.h"

#define cBLACK		0x0000
#define cWHITE		0xFFFF
#define cRED		0xF800
#define cGREEN		0x0400
#define cBLUE		0x001F
#define cSILVER		0xC618
#define cGRAY		0x8410
#define cMAROON		0x8000
#define cYELLOW		color(255,200,0)
#define cOLIVE		0x8400
#define cLIME		0x07E0
#define cAQUA		0x07FF
#define cTEAL		0x0410
#define cNAVY		0x0010
#define cFUCHSIA	0xF81F
#define cPURPLE		0x8010

class View
{
public:
	int ViewID;					// The View ID (Created on the instantiation)

	/*
	 
	While rendering, _rel assumes the relative position acordingly
	to the previews rendering item on the Three.

	This is a simple way to understand what each value is.

	_rel assumes values from the parent view, for example:
		If a view is inside the other, then you change the
		parent view position, ivalidates

	 _________________________________________________________
	|(0,0)               |                                    |
	|                    | _rel.y                             |
	|              ______|__________________________          |
	|             |             |                   |         |
	|             |             | o.y               |         |
	|             |         ____|_____              |         |
	|             |        |x1(),y1() |             |         |
	|             |        |          |             |         |  SCREEN
	|_____________|        |   VIEW   | h           |         |
	|   _rel.x    |________|          |             |         |
	|             |   o.x  |__________|             |         |
	|             |             w      x2(),y2()    |         |
	|             |_________________________________|         |
	|_________________________________________________________|
	

	*/


	Point 	_rel, 				// The Point with the x an y position relative
								// to the parent view, used by Render and hitTest
			o;					// X and Y Start Origin

	int w, h;					// Width and Height of object
	int _left,					// Extra size on left, to check for touch
		_top,					// Extra size on top, to check for touch
		_right,					// Extra size on right, to check for touch
		_bottom;				// Extra size on bottom, to check for touch

	View();
	// Sets the callback function
	virtual void onTouch(onEventHandler func);
	virtual void onRender(onRenderHandler func);
	// Execute the callback function (if exists)
	virtual void onTouch(ActionEvent evt);
	virtual void render(bool forceRender = false);

	// Do a hit test on the view
	virtual View* hitTest(Point p);
	virtual View* hitTest(long x, long y);

	virtual void setArea(Point _origin, int _width, int _height);
	virtual void setArea(int x, int y, int _width, int _height);

	// Returns if the View object should be rendered
	virtual bool shouldRender();

	/*
		Helper methods, to use within view
		x1() and y1(), are the ABSOLUTE x and y of the START View.
		Math is done like this: o.x + _rel.x
	*/
	virtual int x1();
	virtual int y1();
	/*
		Helper methods, to use within view
		x2() and y2(), are the ABSOLUTE x and y of the END View.
		Math is done like this: x1() + w
	*/
	virtual int x2();
	virtual int y2();

	// Public flags
	/*
		Does this view request touch events?

		If set to false, onHitTest will NOT perform test,
		and will return NULL without any computation
	*/
	bool _requestTouch; 
	/*
		This flag indicates wheater the View needs or not
		to be rendered.

		Since Checking Views constantly for this flag consumes
		LOTS of CPU usage, prefer to notify BOTH the View, and your
		ViewController (Such as ArduinOS::requestRender = true);
	*/
	bool _invalidate; // Does this view needs to be rendered?
	virtual void invalidate();

protected:
	onEventHandler _onTouch;
	onRenderHandler _onRender;
};

#endif
