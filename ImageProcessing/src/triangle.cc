#include "triangle.h"

Triangle::Triangle()
: base_{nullptr, nullptr},
  leftLeg_{nullptr, nullptr},
  rightLeg_{nullptr, nullptr},
  apex_{nullptr}
 {}

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3) {
	// Choose the edge with the least slope to be the base
	// Guarantees that if one edge is horizontal, it is the base
	int diff12 = abs(p1[Y] - p2[Y]);
	int diff13 = abs(p1[Y] - p3[Y]);
	int diff23 = abs(p2[Y] - p3[Y]);

	if (diff12 < diff13 && diff12 < diff23) {
		// The base should be p1p2; set base_[0] to be the leftmost base vertex
		if (p1[X] < p2[X]) {
			// p1 is leftmost
			base_[0] = new Point(p1);
			base_[1] = new Point(p2);
		}
		else {
			// p2 is leftmost
			base_[0] = new Point(p2);
			base_[1] = new Point(p1);
		}
		apex_ = new Point(p3);
	}
	else if (diff13 < diff23) {
		// The base should be p1p3; set base_[0] to be the leftmost base vertex
		if (p1[X] < p3[X]) {
			// p1 is leftmost
			base_[0] = new Point(p1);
			base_[1] = new Point(p3);
		}
		else {
			// p3 is leftmost
			base_[0] = new Point(p3);
			base_[1] = new Point(p1);
		}
		apex_ = new Point(p2);
	}
	else {
		// The base should be p2p3; set base_[0] to be the leftmost base vertex
		if (p2[X] < p3[X]) {
			// p2 is leftmost
			base_[0] = new Point(p2);
			base_[1] = new Point(p3);
		}
		else {
			// p3 is leftmost
			base_[0] = new Point(p3);
			base_[1] = new Point(p2);
		}
		apex_ = new Point(p1);
	}

	if ((*base_[0])[X] < (*base_[1])[X]) {
		// The left side is base_[0]apex_
		leftLeg_[0] = base_[0];
		leftLeg_[1] = apex_;

		// The right side is base_[1]apex_
		rightLeg_[0] = base_[1];
		rightLeg_[1] = apex_;
	}
	else {
		// The left side is base_[1]apex_
		leftLeg_[0] = base_[1];
		leftLeg_[1] = apex_;

		// The right side is base_[0]apex_
		rightLeg_[0] = base_[0];
		rightLeg_[1] = apex_;
	}
}

Triangle::~Triangle() {

	// Only 3 distinct pointers
	delete base_[0];
	delete base_[1];
	delete apex_;

	// Legs are aliases for the above pointers, so no need to delete
}

bool Triangle::contains(const Point& p) const {
	if ((*apex_)[Y] > baseY((*apex_)[X])) {
		return p[Y] >= baseY(p[X]) && 
		p[X] >= leftLegX(p[Y]) && 
		p[X] <= rightLegX(p[Y]);
	}
	else {
		return p[Y] <= baseY(p[X]) && 
		p[X] >= leftLegX(p[Y]) && 
		p[X] <= rightLegX(p[Y]);
	}
}

Triangle::iterator Triangle::begin() const {
	// Point to the base vertex which is further from the apex, to guarantee we
	// hit every point as we move towards the apex
	int diff0 = abs((*base_[0])[Y] - (*apex_)[Y]);
	int diff1 = abs((*base_[1])[Y] - (*apex_)[Y]);

	if (diff0 < diff1) {
		return iterator((*base_[1]), *this);
	}
	else  {
		// If the base vertices are at the same Y, prefer the left most one
		return iterator((*base_[0]), *this);
	}
}

Triangle::iterator Triangle::end() const {
	iterator it((*apex_), *this);
	return ++it;
}

int Triangle::baseY(int x) const {
	// Compute slope, use point-slope equation, and round to the nearest integer
	double m =	(double)((*base_[1])[Y] - (*base_[0])[Y]) / 
				(double)((*base_[1])[X] - (*base_[0])[X]);

	return (int)((*base_[0])[Y] + m*((double)(x - (*base_[0])[X])) + 0.5);
}

int Triangle::leftLegX(int y) const {
	// Compute slope, use point-slope equation, and round to the nearest integer
	double m = 	(double)((*leftLeg_[1])[X] - (*leftLeg_[0])[X]) /
				(double)((*leftLeg_[1])[Y] - (*leftLeg_[0])[Y]);

	return (int)((*leftLeg_[0])[X] + m*((double)(y - (*leftLeg_[0])[Y])) + 0.5);
}

int Triangle::rightLegX(int y) const {
	// Compute slope, use point-slope equation, and round to the nearest integer
	double m = 	(double)((*rightLeg_[1])[X] - (*rightLeg_[0])[X]) /
				(double)((*rightLeg_[1])[Y] - (*rightLeg_[0])[Y]);

	return (int)((*rightLeg_[0])[X] + m*((double)(y - (*rightLeg_[0])[Y])) + 0.5);
}

////////////////////////////////////////////////////////////////////////////////
// Implementation of Triangle::InteriorIterator
////////////////////////////////////////////////////////////////////////////////

Triangle::InteriorIterator::InteriorIterator(const Point& p, const Triangle& container) 
:point_{p}, container_{container}
{}

Triangle::InteriorIterator& Triangle::InteriorIterator::operator++() {
	++point_[X];
	if (container_.contains(point_)) {
		// We are done
		return *this;
	}

	// We've gone past the edge of the triangle, need to wrap around

	if ((*container_.apex_)[Y] > container_.baseY((*container_.apex_)[X])) {
		++point_[Y];
	}
	else {
		--point_[Y];
	}
	point_[X] = container_.leftLegX(point_[Y]);
	return *this;
}

bool Triangle::InteriorIterator::operator==(
	const Triangle::InteriorIterator& other) const
{
	return point_ == other.point_;
}

bool Triangle::InteriorIterator::operator!=(
	const Triangle::InteriorIterator& other) const
{
	return !(*this == other);
}

Triangle::InteriorIterator::reference Triangle::InteriorIterator::operator*() {
	return point_;
}
