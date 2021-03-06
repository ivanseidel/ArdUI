/*
  Graphics.h - Class with basic trigonometry utility

  Created by Ivan Seidel Gomes, May, 2013.
  Releasedlongo the public domain.
*/

#ifndef Graphics_h
#define Graphics_h

#include "math.h"

/*
	Point

	Contains information about x and y values
*/
class Point
{
public:
	long x, y;

	Point(){
		x=0; y=0;
	}

	Point(long _x,long _y){
		x = _x;
		y = _y;
	}

	bool operator==(Point other){
		return other.x == x && other.y == y;
	}

	void operator=(Point other){
		x = other.x;
		y = other.y;
	}

	Point operator+(Point other){
		x += other.x;
		y += other.y;
		return *this;
	}
};

/*
	TwoPoint

	Base class to use with Rectangles, Elipses...
*/
class TwoPoint{
public:
	Point p1, p2;
	
	TwoPoint(long x1 = 0, long y1 = 0, long x2 = 0, long y2 = 0){
		p1 = Point(x1, y1);
		p2 = Point(x2, y2);
	}

	TwoPoint(Point _p1, Point _p2){
		p1 = _p1;
		p2 = _p2;
	}
	long getWidth(){
		return (p1.x > p2.x? p1.x - p2.x: p2.x - p1.x);
	}
	long getHeight(){
		return (p1.y > p2.y? p1.y - p2.y: p2.y - p1.y);
	}
};

/*
	Line
*/
class Line: public TwoPoint
{
public:
	explicit Line(): TwoPoint(){};
	explicit Line(Point p1, Point p2): TwoPoint(p1, p2){};
	explicit Line(long x1,long y1,long x2,long y2): TwoPoint(x1,y1,x2,y2){};

	void set(long x1,long y1,long x2,long y2){
		p1.x = x1;
		p1.y = y1;
		p2.x = x2;
		p2.y = y2;
	}

	bool operator==(Line other){
		return other.p1 == p1 && other.p2 == p2;
	}

	Line operator+(Point other){
		p1 = p1 + other;
		p2 = p2 + other;
		return *this;
	}

	void operator=(Line other){
		p1 = other.p1;
		p2 = other.p2;
	}

	double getLength(){
		return sqrt(pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2));
	}

	double getAngle(){
		return atan2(p1.y - p2.y, p2.x - p1.x);
	}

};


/*
	Rectangle
*/
class Rectangle: public TwoPoint
{
public:
	explicit Rectangle(): TwoPoint(){};
	explicit Rectangle(Point p1, Point p2): TwoPoint(p1, p2){};
	explicit Rectangle(long x1,long y1,long x2,long y2): TwoPoint(x1,y1,x2,y2){};

	double getArea(){
		return (p1.x-p2.x)*(p1.x-p2.x) * (p1.y-p2.y)*(p1.y-p2.y);
	}
};

#endif
