#define TEST_SUITE TriangleTest
#include "unit-test.h"
#include "../src/triangle.h"

TEST(IsoscelesRight) {
	Triangle test(Point(0, 0), Point(2, 0), Point(0, 2));
}

int main() {
	RUN_TESTS();
}

#undef TEST_SUITE