
#ifndef AngleView_h
#define AngleView_h

#include "View.h"
#include "views/DrawableLine.h"
#include <math.h>

class AngleView: public View
{
protected:
	double angle, lastAngle;

	DrawableLine arrow;

public:
	Color color;

	AngleView(){
		View::View();

		angle = 0;
		lastAngle = -1;

		color = cWHITE;

		arrow = DrawableLine();
	}

	void setAngle(double _angle, bool renderNow = false){
		angle = _angle;

		if(renderNow) render(true);
		else invalidate();
	}

	// void setAngle(long _angle, bool renderNow = false){
	// 	setAngle(_angle*PI/180);
	// }

	void render(bool forceRender = false)
	{
		// Serial.println("#");
		if(forceRender){
			ArdUI::LCD->setColor(color);
			ArdUI::LCD->drawCircle(x1(), y2(), w);
		}

		// Serial.println("\t#");
		if(forceRender || angle != lastAngle){
			long x = x1();
			long y = y2();

			arrow.line.set(x, y, x + cos(angle)*(w-5), y + sin(angle)*(w-5));
			arrow.render(forceRender);

			lastAngle = angle;
			// Serial.println("\t\t#");
		}

		_invalidate = false;
		// Serial.println("\t\t\t#");
	}
};

#endif
