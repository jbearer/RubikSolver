#pragma once
#include "boost/numeric/ublas/matrix.hpp"
#include <iostream>

template<int size, typename T>
class ColumnVector : public boost::numeric::ublas::matrix<T> {
public:
	ColumnVector();

	template<typename... Coords>
	ColumnVector(Coords... coords);

	ColumnVector(const ColumnVector<size, T>& other);

	bool operator==(const ColumnVector<size, T> other) const;

	bool operator!=(const ColumnVector<size, T> other) const;

	T& operator[](size_t index);

	const T& operator[](size_t index) const;
};

template<int size, typename T>
std::ostream& operator<<(std::ostream&, const ColumnVector<size, T>&);

#include "column-vector-private.h"
