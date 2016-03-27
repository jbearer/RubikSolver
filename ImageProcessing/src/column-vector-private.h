#include "boost/numeric/ublas/matrix.hpp"
#include <vector>
#include <stdexcept>
#include <sstream>

using boost::numeric::ublas::matrix;

template<int size, typename T>
ColumnVector<size, T>::ColumnVector() : matrix<T>(size, 1) {}

template<int size, typename T> template<typename... Coords>
ColumnVector<size, T>::ColumnVector(Coords... coords) : matrix<T>(size, 1){
	std::vector<T> v{coords...};
	if (v.size() != size) {
		std::stringstream error;
		error << "Attempted to construct column vector of size "
			<< size
			<< " with "
			<< v.size()
			<< " coordinate arugments.";
		throw std::logic_error(error.str());
	}

	for (size_t i = 0; i < size; ++i) {
		(*this)[i] = v[i];
	}
}

template<int size, typename T>
ColumnVector<size, T>::ColumnVector(const ColumnVector<size, T>& other) 
: matrix<T>(other.size1(), 1) 
{
	for (size_t i = 0; i < size; ++i) {
		(*this)[i] = other[i];
	}
}

template<int size, typename T>
bool ColumnVector<size, T>::operator==(const ColumnVector<size, T> other) const {
	for (size_t row = 0; row < size; ++row) {
		if ((*this)[row] != other[row]) return false;
	}
	return true;
}

template<int size, typename T>
bool ColumnVector<size, T>::operator!=(const ColumnVector<size, T> other) const {
	return !(*this == other);
}

template<int size, typename T>
T& ColumnVector<size, T>::operator[](size_t index) {
	return (*this)(index, 0);
}

template<int size, typename T>
const T& ColumnVector<size, T>::operator[](size_t index) const {
	return (*this)(index, 0);
}

template<int size, typename T>
std::ostream& operator<<(std::ostream& out, const ColumnVector<size, T>& v) {
	out << "(" << v[0];
	for (size_t i = 1; i < size; ++i) {
		out << ", " << v[i];
	}
	out << ")";
	return out;
}
