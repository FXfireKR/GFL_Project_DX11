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

	//�� ������ �浹����
	bool collide(double x0, double y0, double wx0, double wy0, double hw0,
		double x1, double y1, double wx1, double wy1, double hw1) const;

	// Test for collision between an ellipse of horizontal radius w0 and vertical radius h0 at (x0, y0) and
	// an ellipse of horizontal radius w1 and vertical radius h1 at (x1, y1)
	//��, ���������� Ÿ������ �浹üũ
	bool collide(double x0, double y0, double w0, double h0, double x1, double y1, double w1, double h1) const;

	// Test for collision between an ellipse of horizontal radius w and vertical radius h at (x0, y0) and
	// a circle of radius r at (x1, y1)
	//�� ������ Ÿ���� ���� �浹 ����
	bool collide(double x0, double y0, double w, double h, double x1, double y1, double r) const;


	EllipseCol();
	EllipseCol(int maxIterations);		//maxIterations :: �浹�� ���� �󸶳� �������� ������ �Ұ��ΰ� [ 5 ]
	~EllipseCol();
};