#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>
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

EasyCube::EasyCube(std::vector<Face> faces) :
    up_{faces[0]}, back_{faces[1]}, left_{faces[2]},
    right_{faces[3]}, front_{faces[4]}, down_{faces[5]}
{
    // Nothing more to do
}

std::ostream& operator<<(std::ostream& out, const Color& color)
{
    switch(color) {
        case Red:
            out << "Red"; break;
        case Orange:
            out << "Orange"; break;
        case Yellow:
            out << "Yellow"; break;
        case Green:
            out << "Green"; break;
        case Blue:
            out << "Blue"; break;
        case White:
            out << "Black"; break;
    }
    return out;
}


Cube translate(vector<vector<vector<Color>>> maybeCube)
{
    vector<vector<vector<Color>>> allCubes(1,vector<vector<Color>>(6,vector<Color>(8)));

    for (size_t face = 0; face < NUM_FACES; ++face) {
        for (size_t facelet = 0; facelet < NUM_FACELETS; ++facelet) {
            vector<Color> colors = maybeCube[face][facelet];
            // if we know what the color is
            if (colors.size() == 1) {
                // update all the cubes with the color
                for (auto& cube : allCubes) {
                    cube[face][facelet] = colors[0];
                }
            }

            else {
                // create a temporary new vector of cubes
                vector<vector<vector<Color>>> newCubes;
                for (auto cubeCopy : allCubes) {
                    for (auto color : colors) {
                        // copy a version of each cube with the new color
                        // into the new vector
                        cubeCopy[face][facelet] = color;
                        newCubes.push_back(cubeCopy);
                    }
                }
                allCubes = newCubes;
            }
        }
    }

    std::cout << "found " << allCubes.size() << " possibilities" << std::endl;
    vector<Cube> resultCubes;

    // try to convert each of the vectors into actual cubes
    for (auto& cube : allCubes) {
        EasyCube easyCube(cube);
        try {
            Cube result = translate(easyCube);
            resultCubes.push_back(result);
        }
        catch (std::exception&){}
    }


    if (resultCubes.size() > 1) {
        throw std::runtime_error("hey, there are too many cubes");
    }

    if (resultCubes.size() == 0) {
        throw std::runtime_error("hey, none of the cubes worked");
    }

    std::cout << "hey, it actually worked" << std::endl;

    return resultCubes[0];
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
    // create a solved cube to compare with
    const int solvedEdges[Cube::NUM_EDGES] =
        {0,1,2,3,4,5,6,7,8,9,10,11 };

    const int solvedCorners[Cube::NUM_CORNERS] =
        { 0,1,2,3,4,5,6,7 };

    if (!std::is_permutation(eColors, eColors + Cube::NUM_EDGES, solvedEdges)){
        throw std::runtime_error("edge appears twice");
    }
    if (!std::is_permutation(cColors, cColors + Cube::NUM_CORNERS, solvedCorners)){
        throw std::runtime_error("corner appears twice");
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
