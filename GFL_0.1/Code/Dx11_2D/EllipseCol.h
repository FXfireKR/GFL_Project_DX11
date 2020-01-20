#pragma once

/* Ellipse-circle collision detection
 *
 * by Olli Niemitalo in 2012-08-06.
 * This work is placed in the public domain.
 */

class EllipseCol {
private:
	double *innerPolygonCoef;
	double *outerPolygonCoef;
	int maxIterations;

	bool iterate(double x, double y, double c0x, double c0y, double c2x, double c2y, double rr) const;

public:

	//두 원간의 충돌판정
	bool collide(double x0, double y0, double wx0, double wy0, double hw0,
		double x1, double y1, double wx1, double wy1, double hw1) const;

	// Test for collision between an ellipse of horizontal radius w0 and vertical radius h0 at (x0, y0) and
	// an ellipse of horizontal radius w1 and vertical radius h1 at (x1, y1)
	//장, 단축을가진 타원간의 충돌체크
	bool collide(double x0, double y0, double w0, double h0, double x1, double y1, double w1, double h1) const;

	// Test for collision between an ellipse of horizontal radius w and vertical radius h at (x0, y0) and
	// a circle of radius r at (x1, y1)
	//비 수평적 타원에 대한 충돌 판정
	bool collide(double x0, double y0, double w, double h, double x1, double y1, double r) const;


	EllipseCol();
	EllipseCol(int maxIterations);		//maxIterations :: 충돌에 대해 얼마나 디테일한 판정을 할것인가 [ 5 ]
	~EllipseCol();
};