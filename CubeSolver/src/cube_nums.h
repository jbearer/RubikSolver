#pragma once

#include <vector>
#include "cube.h"

namespace CubeSolver {

///////////////////////////////////
//// Cube nums/////////////////////
///////////////////////////////////

/**
 * @brief      Store a cube as a set of three shorts: edgeOrients,
 *             cornerOrients, and edgeOrbits.  These three numbers do not
 *             Uniquely identify a cube, but are sufficient to distinguish all
 *             cubes during step one of the algorithm.  They are used to compute
 *             turns more quickly (the results can be easily serialized) and
 *             because they are smaller in memory.
 */
struct CubeNumsStep1 {
    struct Hash;

    /// Constructors: Default initialize to a solved cube.
    CubeNumsStep1();
    CubeNumsStep1(const Cube& cube);
    CubeNumsStep1(ushort edgeOrients, ushort cornerOrients, ushort edgeOrbits);

    bool operator==(const CubeNumsStep1& rhs) const;
    bool operator!=(const CubeNumsStep1& rhs) const;

    void print();

    struct Hash : public std::unary_function<CubeNumsStep1, size_t> {
        size_t operator()(const CubeNumsStep1& cube) const;
    };

    /// Serialize cubenums.  Unused attribute gets rid of compiler warnings
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version __attribute__((unused)))
    {
        ar & edgeOrients_ & cornerOrients_ & edgeOrbits_;
    }

    static CubeNumsStep1 turn(const CubeNumsStep1& cube, int i);

private:

    ushort edgeOrients_;
    ushort cornerOrients_;
    ushort edgeOrbits_;
};

/**
 * @brief      Like CubeNumsStep1, store the cube as a set of three integers:
 *             cornerColors, edgeColors1, and edgeColors2.  Uniquely determine a
 *             cube for step 2.
 */
struct CubeNumsStep2{
    struct Hash;

    /// Default construct to a solved cube.
    CubeNumsStep2();
    CubeNumsStep2(const Cube& cube);
    CubeNumsStep2(ushort cornerColors, ushort edgeColors1, ushort edgeColors2);

    bool operator==(const CubeNumsStep2& rhs) const;
    bool operator!=(const CubeNumsStep2& rhs) const;


    struct Hash : public std::unary_function<CubeNumsStep2, size_t> {
        size_t operator()(const CubeNumsStep2& cube) const;
    };

    /// serialize cubenums
    template<typename Archive>
    void serialize(Archive& ar, const uint version __attribute__((unused)))
    {
        ar & cornerColors_ & edgeColors1_ & edgeColors2_;
    }

    static CubeNumsStep2 turn(const CubeNumsStep2& cube, int i);

private:

    ushort cornerColors_;
    ushort edgeColors1_;
    ushort edgeColors2_;

};

}
