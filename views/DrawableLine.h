
#ifndef DrawableLine_h
#define DrawableLine_h

#include "View.h"
#include "Graphics.h"

class DrawableLine: public View
{
protected:
	Color _color, _foreground;

	Line _lastLine;

	void clear(){
		ArdUI::LCD->setColor(_foreground);
		ArdUI::LCD->drawLine(
			_lastLine.p1.x,
			_lastLine.p1.y,
			_lastLine.p2.x,
			_lastLine.p2.y);
	}

	void draw(long x, long y){
		// Erase last circle position
		ArdUI::LCD->setColor(_color);
		ArdUI::LCD->drawLine(
			line.p1.x + x,
			line.p1.y + y,
			line.p2.x + x,
			line.p2.y + y);
	}

public:
	Line line;

	DrawableLine(){
		View::View();
		
		_foreground = cBLACK;
		_color = cYELLOW;

		line = Line();
		_lastLine = line;
	}

	void render(bool forceRender = false){
		if(forceRender || !(_lastLine == line)){
			long x = x1();
			long y = y1();

			clear();
			_lastLine = line;
			_lastLine = _lastLine + Point(x, y);
			draw(x, y);
		}

		_invalidate = false;
	}

	void setColor(Color color, bool renderNow = false){
		_color = color;

		if(renderNow) render(true);
		else invalidate();
	}

	void setForeground(Color color, bool renderNow = false){
		_foreground = color;

		if(renderNow) render(true);
		else invalidate();
	}
};

#endif