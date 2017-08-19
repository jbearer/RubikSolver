#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include "translate.h"

using std::tuple;
using std::get;

namespace CubeSolver {

EasyCube::EasyCube(Face up, Face back, Face left, Face right, Face front, Face down) :
    up_{up}, back_{back}, left_{left}, right_{right}, front_{front},
    down_{down}
    {
        // Nothing more to do
    }


Cube translate(EasyCube cube) {

    Cube::Edge_t eColors[Cube::NUM_EDGES];
    int eOrients[Cube::NUM_EDGES];
    Cube::Corner_t cColors[Cube::NUM_CORNERS];
    int cOrients[Cube::NUM_CORNERS];

    int i = 0;
    for (auto edge_ptr : cube.edges) {
        Edge e{*edge_ptr[0], *edge_ptr[1]};
        tuple<Cube::Edge_t, size_t> result = translate_edge(e);
        eColors[i] = get<0>(result);
        eOrients[i] = get<1>(result);
        ++i;
    }
    i=0;
    for (auto corner_ptr : cube.corners) {
        Corner c{*corner_ptr[0], *corner_ptr[1], *corner_ptr[2]};
        tuple<Cube::Corner_t, size_t> result = translate_corner(c);
        cColors[i] = get<0>(result);
        cOrients[i] = get<1>(result);
        ++i;
    }

    int sumEdgeOrients = 0;
    for (size_t i = 0; i < Cube::NUM_EDGES; ++i)
        sumEdgeOrients += eOrients[i];
    if (sumEdgeOrients % 2 != 0) {
        throw std::exception();
    }

    int sumCornerOrients = 0;
    for (size_t i = 0; i < Cube::NUM_CORNERS; ++i)
        sumCornerOrients += cOrients[i];
    if (sumCornerOrients % 3 != 0) {
        throw std::exception();
    }


    return Cube(eColors, eOrients, cColors, cOrients);
}

tuple<Cube::Corner_t, size_t> translate_corner(Corner c) {
    for (auto pair : corner_map) {
        if (same_set(c, pair.second)) {
            int perm = permutation(c, pair.second);
            return std::make_tuple(pair.first, perm);
        }
    }
    throw std::invalid_argument("received bad corner");
}

tuple<Cube::Edge_t, size_t> translate_edge(Edge e) {
    for (auto pair : edge_map) {
        if (same_set(e, pair.second)) {
            int perm = permutation(e, pair.second);
            return std::make_tuple(pair.first, perm);
        }
    }
    throw std::invalid_argument("received bad edge");
}

int permutation(std::list<Color> c1, std::list<Color> c2) {
    int perm = 0;
    while (c1 != c2) {
        cycle(c2);
        ++perm;
        if (perm > 3) {
            throw std::invalid_argument("mirror image corner");
        }
    }
    return perm;
}

void cycle(std::list<Color>& cubie) {
    Color last = cubie.back();
    cubie.pop_back();
    cubie.push_front(last);
}

bool same_set(Corner c1, Corner c2) {
    for (auto color : c1)
        if (!contains(c2, color))
            return false;
    return true;
}

bool contains(Corner corner, Color color) {
    for (auto c : corner)
        if (c == color)
            return true;
    return false;
}

}