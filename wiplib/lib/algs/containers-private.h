#include <set>
#include <unordered_set>

template<template<class> class ContainerType, class ElementType>
bool wiplib::contains(
    ContainerType<ElementType> container, ElementType element)
{
    for (ElementType e : container) {
        if (e == element) return true;
    }
    return false;
}

template<class ElementType>
bool wiplib::contains(
    std::set<ElementType> container, ElementType element) 
{
    return container.find(element) != container.end();
}

template<class ElementType>
bool wiplib::contains(
    std::unordered_set<ElementType> container, ElementType element)
{
    return container.find(element) != container.end();
}

template<template<class, class> class MapType, class KeyType, class ValueType>
bool wiplib::contains(MapType<KeyType, ValueType> map, KeyType key) {
    return map.find(key) != map.end();
}
