#include "triangle.h"

Triangle::Triangle(Point p1, Point p2, Point p3) {
	// There is at most one horizontal edge. To guarantee the legs are not
	// horizontal, we need the base to be horizontal if possible.
	if (p1[Y] == p2[Y]) {
		base_[0] = p1; base_[1] = p2;
		apex_ = p3;
	}
	else if (p2[Y] == p3[Y]) {
		base_[0] = p2; base_[1] = p3;
		apex_ = p1;
	}
	else {
		if (p1[X] == p3[X]) {
			// If p1p3 is horizontal, any of the other edges will work
			base_[0] = p1; base_[1] = p2;
			apex_ = p3;
		}
		else {
			// If p1p3 is not horizontal, use it for the base
			base_[0] = p1; base_[1] = p3;
			apex_ = p2;
		}
	}

	if (base_[0][X] < base_[1][X]) {
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

bool Triangle::contains(const Point& p) const {
	if (apex_[Y] > baseY(apex_[X])) {
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
	// Find the leftmost vertex in base
	if (base_[0][X] < base_[1][X]) {
		return iterator(base_[0], *this);
	}
	else return iterator(base_[1], *this);
}

Triangle::iterator Triangle::end() const {
	iterator it(apex_, *this);
	return ++it;
}

int Triangle::baseY(int x) const {
	// Compute slope, interpolate to find y(x), and round to the nearest integer
	return (int) ( ( (double)(base_[1][Y] - base_[0][Y]) /
		(double)(base_[1][X] - base_[0][X]) ) * x + 0.5);
}

int Triangle::leftLegX(int y) const {
	// Compute slope, interpolate to find x(y), and round to the nearest integer
	return (int) ( ( (double)(leftLeg_[1][X] - leftLeg_[0][X]) /
		(double)(leftLeg_[1][Y] - leftLeg_[0][Y]) ) * y + 0.5);
}

int Triangle::rightLegX(int y) const {
	// Compute slope, interpolate to find x(y), and round to the nearest integer
	return (int) ( ( (double)(rightLeg_[1][X] - rightLeg_[0][X]) /
		(double)(rightLeg_[1][Y] - rightLeg_[0][Y]) ) * y + 0.5);
}

////////////////////////////////////////////////////////////////////////////////
// Implementation of Triangle::InteriorIterator
////////////////////////////////////////////////////////////////////////////////

Triangle::InteriorIterator::InteriorIterator(const Point& p, const Triangle& container) 
:point_{p}, yOffset_{point_[Y] - container.baseY(point_[X])}, container_{container}
{}

Triangle::InteriorIterator& Triangle::InteriorIterator::operator++() {
	++point_[X];
	point_[Y] = yOffset_ + container_.baseY(point_[X]);
	if (container_.contains(point_)) {
		// We are done
		return *this;
	}

	// We've gone past the edge of the triangle, need to wrap around

	if (container_.apex_[Y] > container_.baseY(container_.apex_[X])) {
		++yOffset_;
	}
	else {
		--yOffset_;
	}

	// Go back to the leftmost corner and increment one "row"
	if (container_.base_[0][X] < container_.base_[1][X]) {
		point_[Y] = container_.base_[0][Y] + yOffset_;
	}
	else {
		point_[Y] = container_.base_[1][Y] + yOffset_;
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
