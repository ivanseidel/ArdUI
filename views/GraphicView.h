#ifndef GraphicView_h
#define GraphicView_h

#include "View.h"
#include "DrawableLine.h"

#include "LinkedList.h"

class GraphicView: public View{
protected:

	LinkedList<DrawableLine*>* lines;
	int measures;
	double min, max;

	long pos;

public:

	GraphicView(int _measures = 10, double _min = 0.0, double _max = 100.0){
		measures = measures;

		min = _min;
		max = _max;

		pos = 0;

		lines = new LinkedList<DrawableLine*>();
	}

	void render(bool forceRender = false){
		long _x1 = x1();
		long _x2 = x2();
		long _y1 = y1();
		long _y2 = y2();
		if(forceRender){
			// Render Background stuff
			ArdUI::LCD->setColor(cWHITE);
			ArdUI::LCD->setBackColor(cWHITE);

			ArdUI::LCD->drawLine(_x1, _y2, _x2, _y2);
			ArdUI::LCD->drawLine(_x1, _y1, _x1, _y2);

		}

		int size = lines->size();

		DrawableLine *lastLine = lines->get(0);
		DrawableLine *thisLine;

		for(int i = 1; i < size; i++){
			thisLine = lines->get(i);

			thisLine->line.p1 = lastLine->line.p2;

			thisLine->render(forceRender);

			lastLine = thisLine;
		}

	}

	void addValue(long value){
		long size = lines->size();
		if(size >= measures){
			lines->clear();
			size = 0;
		}

		long _x1 = x1();
		long _x2 = x2();
		long _y1 = y1();
		long _y2 = y2();

		DrawableLine *creatingLine = new DrawableLine();

		value = max(min, min(max, value));

		creatingLine->line.p2 = Point(
			_x1 + (_x2 - _x1)/(1.0 * measures) * size,
			_y2 - map(value, min, max, _y1, _y2) );

		lines->add(creatingLine);

		invalidate();
	}

};

#endif