#pragma once

#include <vector>
#include <tuple>
#include <map>
#include <list>
#include <memory>
#include <exception>
#include <stdexcept>
#include "cube.h"
//#include "cubeSolver.h"

namespace CubeSolver {

    /*
         * 2 4 0 1 3 5
         * 0 2 4 3 5 1
         * 1 2 3 4 5 0
         * 2 3 1 0 4 5
         * 3 0 5 2 1 4
         * 4 0 2 5 1 3
         * 5 3 0 1 4 2
         * 0 5 3 4 2 1
         * 3 1 2 5 0 4
     */
    enum Color {Red, Orange, Yellow, Green, Blue, White};

    typedef std::list<Color> Corner;
    typedef std::list<Color> Edge;
    typedef std::vector<Color> Face;

    /* Represents a cube by the facelets, as opposed to the cubies */
    struct EasyCube {
    public:
        EasyCube() = default;
        EasyCube(Face up, Face back, Face left, Face right, Face front, Face down);
        EasyCube(std::vector<Face> faces);

        // default to a solved easy cube;
        Face up_ = std::vector<Color>(8, Color::Yellow);
        Face back_ = std::vector<Color>(8, Color::Blue);
        Face left_ = std::vector<Color>(8, Color::Red);
        Face right_ = std::vector<Color>(8, Color::Orange);
        Face front_ = std::vector<Color>(8, Color::Green);
        Face down_ = std::vector<Color>(8, Color::White);

        const std::vector<std::vector<Color*>> edges {
            {&up_[1], &back_[1]},
            {&left_[1], &up_[3]},
            {&right_[1], &up_[4]},
            {&up_[6], &front_[1]},

            {&left_[3], &back_[4]},
            {&right_[4], &back_[3]},
            {&left_[4], &front_[3]},
            {&right_[3], &front_[4]},

            {&down_[1], &back_[6]},
            {&left_[6], &down_[3]},
            {&right_[6], &down_[4]},
            {&down_[6], &front_[6]}
        };

        const std::vector<std::vector<Color*>> corners {
            {&left_[0], &back_[2], &up_[0]},
            {&right_[2], &up_[2], &back_[0]},
            {&left_[2], &up_[5], &front_[0]},
            {&right_[0], &front_[2], &up_[7]},

            {&left_[7], &down_[5], &back_[7]},
            {&right_[7], &back_[5], &down_[7]},
            {&left_[7], &front_[5], &down_[0]},
            {&right_[5], &down_[2], &front_[7]}
        };
    };



    /**
     * @brief      Translate from the facelet representation of rubik's
     *             cube to the Cubie representation
     *
     * @param[in]  colors  The colors, organized by faces Yellow,
     *             Blue, Red, Orange, Green, White.  Left to right,
     *             then Back to Front, then Top to Bottom
     *
     *
     * @return     The corresponding cube object
     */
    Cube translate(EasyCube colors);

    static const std::map<Cube::Corner_t, Corner> corner_map {
        {Cube::YRB, Corner{Red, Blue, Yellow}},
        {Cube::YOB, Corner{Orange, Yellow, Blue}},
        {Cube::YRG, Corner{Red, Yellow, Green}},
        {Cube::YOG, Corner{Orange, Green, Yellow}},
        {Cube::WRB, Corner{Red, White, Blue}},
        {Cube::WOB, Corner{Orange, Blue, White}},
        {Cube::WRG, Corner{Red, Green, White}},
        {Cube::WOG, Corner{Orange, White, Green}}
    };

    static const std::map<Cube::Edge_t, Edge> edge_map {
        {Cube::YB, Edge{Yellow, Blue}},
        {Cube::YR, Edge{Red, Yellow}},
        {Cube::YO, Edge{Orange, Yellow}},
        {Cube::YG, Edge{Yellow, Green}},
        {Cube::RB, Edge{Red, Blue}},
        {Cube::OB, Edge{Orange, Blue}},
        {Cube::RG, Edge{Red, Green}},
        {Cube::OG, Edge{Orange, Green}},
        {Cube::WB, Edge{White, Blue}},
        {Cube::WR, Edge{Red, White}},
        {Cube::WO, Edge{Orange, White}},
        {Cube::WG, Edge{White, Green}}
    };

    bool contains(Corner corner, Color color);
    bool same_set(Corner c1, Corner c2);
    int permutation(std::list<Color> c1, std::list<Color> c2);
    void cycle(std::list<Color>& cubie);
    std::tuple<Cube::Corner_t, size_t> translate_corner(Corner c);
    std::tuple<Cube::Edge_t, size_t> translate_edge(Corner c);

}
