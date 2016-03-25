/**
 * \headerfile containers.h
 * Declares several methods useful for manipulating containers of all sorts.
 */

#include <set>
#include <unordered_set>

namespace wiplib {

/**
 * \brief Determine if a given element exists in a container.
 */
template<template<class> class ContainerType, class ElementType>
bool contains(ContainerType<ElementType> container, ElementType element);

template<class ElementType>
bool contains(std::set<ElementType>, ElementType element);

/// Unordered set specialization
template<class ElementType>
bool contains(std::unordered_set<ElementType>, ElementType element);

/// Set specialization
template<class ElementType>
bool contains(std::set<ElementType>, ElementType element);

/// Map specialization
template<template<class, class> class MapType, class KeyType, class ValueType>
bool contains(MapType<KeyType, ValueType>, KeyType key);

}

#include "containers-private.h"
