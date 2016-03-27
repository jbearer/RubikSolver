#pragma once
#include "column-vector.h"

typedef ColumnVector<2, int> Point;
const size_t X = 0;
const size_t Y = 1;

/**
 * \class Triangle
 * Represents a triangle in a discrete 2-dimensional space with
 * integer coordinates.
 */
class Triangle {
private:
	class InteriorIterator;

public:

	using iterator = InteriorIterator;

	Triangle();

	/**
	 * \brief Construct a representation of the triangle described by the
	 *	given three points.
	 * \pre The points are not colinear.
	 */
	Triangle(const Point& p1, const Point& p2, const Point& p3);

	~Triangle();

	/** 
	 * \brief Determine if a point lies in the interior of the triangle.
	 */
	bool contains(const Point& p) const;

	/// An iterator to the leftmost corner of the base
	iterator begin() const;

	/// An iterator to one past the end. If iterator it starts at begin and
	/// reaches end by incrementing, it has gone through every point within the
	/// triangle.
	iterator end() const;

private:

	/**
	 * \class Triangle::InteriorIterator.
	 * Used to iterate over all points with integer coordinates contained in the
	 * triangle.
	 */
	class InteriorIterator {
	public:
		typedef std::input_iterator_tag iterator_category;
		typedef Point value_type;
		typedef value_type& reference;
		typedef ptrdiff_t difference_type;

		InteriorIterator& operator++();

		bool operator==(const InteriorIterator& other) const;

		bool operator!=(const InteriorIterator& other) const;

		reference operator*();

	private:
		InteriorIterator(const Point& p, const Triangle& container);

		// Current coordinates of the iterator
		Point point_;

		// Offset in the y-direction between the base and the current point
		int yOffset_;

		// The Triangle that this iterator points to
		const Triangle& container_;

		friend class Triangle;
	};

	/**
	 * \brief A linear function which describes the base of the triangle.
	 * \param x The x-coordinate describing the point of interest on the edge.
	 * \return The y-coordinate such that the point (x, y) is on the base
	 *	of the triangle (or as near as possible with integer coordinates).
	 * \note The base is guaranteed not to be vertical.
	 */
	int baseY(int x) const;

	/**
	 * \brief A linear function which describes the left leg of the triangle.
	 * \param y The y-coordinate describing the point of interest on the edge.
	 * \return The x-coordinate such that the point (x, y) is on the leg (or as
	 *	near as possible with integer coordinates).
	 * \note The legs are guaranteed not to be horizontal.
	 */
	int leftLegX(int y) const;

	/**
	 * \brief A linear function which describes the right leg of the triangle.
	 * \param y The y-coordinate describing the point of interest on the edge.
	 * \return The x-coordinate such that the point (x, y) is on the leg (or as
	 *	near as possible with integer coordinates).
	 * \note The legs are guaranteed not to be horizontal.
	 */
	int rightLegX(int y) const;

	// Point pointers instead of just points are used here to avoid calling the
	// default constructor for Point in the constructor for Triangle. This is
	// necessary because the fairly complex logic involved in initializing
	// the points makes the use of initializer lists impractical.

	// The points that define the base of the triangle.
	Point *base_[2];

	// The points that define the left leg of the triangle.
	Point *leftLeg_[2];

	// The points that define the right leg of the triangle.
	Point *rightLeg_[2];

	// The vertex not adjacent to the base.
	Point *apex_;
};