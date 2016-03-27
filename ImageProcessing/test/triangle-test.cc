#define TEST_SUITE TriangleTest
#include "unit-test.h"
#include "../src/triangle.h"
#include <vector>
#include <sstream>
using std::vector;
using std::stringstream;

TEST(IsoscelesRight) {
	Triangle test(Point(0, 0), Point(2, 0), Point(0, 2));
	vector<Point> interior;
	for (auto p : test) {
		interior.push_back(p);
	}

	vector<Point> expected;
	expected.emplace_back(0, 0);
	expected.emplace_back(1, 0);
	expected.emplace_back(2, 0);
	expected.emplace_back(0, 1);
	expected.emplace_back(1, 1);
	expected.emplace_back(0, 2);

	if (expected.size() != interior.size()) {
		stringstream error;
		error << "Size mismatch.\nInterior points: ";
		for (auto p : interior) error << p << " ";
		error << "\nExpected points: ";
		for (auto p : expected) error << p << " ";
		FAIL(error.str());
	}

	// Make sure every point we expect is actually in the interior
	for (auto p : expected) {
		if (std::find(interior.begin(), interior.end(), p) == interior.end()) {
			stringstream error;
			error << "Expected point " << p << " in interior.";
			FAIL(error.str());
		}
	}

	// Make sure every point in the interior is one we expect
	for (auto p : interior) {
		if (std::find(expected.begin(), expected.end(), p) == expected.end()) {
			stringstream error;
			error << "Found unexpected point " << p << " in interior.";
			FAIL(error.str());
		}
	}
}

TEST(IsoscelesRightPermutation1) {
	Triangle test(Point(2, 0), Point(0, 2), Point(0, 0));
	vector<Point> interior;
	for (auto p : test) {
		interior.push_back(p);
	}

	vector<Point> expected;
	expected.emplace_back(0, 0);
	expected.emplace_back(1, 0);
	expected.emplace_back(2, 0);
	expected.emplace_back(0, 1);
	expected.emplace_back(1, 1);
	expected.emplace_back(0, 2);

	if (expected.size() != interior.size()) {
		stringstream error;
		error << "Size mismatch.\nInterior points: ";
		for (auto p : interior) error << p << " ";
		error << "\nExpected points: ";
		for (auto p : expected) error << p << " ";
		FAIL(error.str());
	}

	// Make sure every point we expect is actually in the interior
	for (auto p : expected) {
		if (std::find(interior.begin(), interior.end(), p) == interior.end()) {
			stringstream error;
			error << "Expected point " << p << " in interior.";
			FAIL(error.str());
		}
	}

	// Make sure every point in the interior is one we expect
	for (auto p : interior) {
		if (std::find(expected.begin(), expected.end(), p) == expected.end()) {
			stringstream error;
			error << "Found unexpected point " << p << " in interior.";
			FAIL(error.str());
		}
	}
}

TEST(IsoscelesRightPermutation2) {
	Triangle test(Point(0, 2), Point(0, 0), Point(2, 0));
	vector<Point> interior;
	for (auto p : test) {
		interior.push_back(p);
	}

	vector<Point> expected;
	expected.emplace_back(0, 0);
	expected.emplace_back(1, 0);
	expected.emplace_back(2, 0);
	expected.emplace_back(0, 1);
	expected.emplace_back(1, 1);
	expected.emplace_back(0, 2);

	if (expected.size() != interior.size()) {
		stringstream error;
		error << "Size mismatch.\nInterior points: ";
		for (auto p : interior) error << p << " ";
		error << "\nExpected points: ";
		for (auto p : expected) error << p << " ";
		FAIL(error.str());
	}

	// Make sure every point we expect is actually in the interior
	for (auto p : expected) {
		if (std::find(interior.begin(), interior.end(), p) == interior.end()) {
			stringstream error;
			error << "Expected point " << p << " in interior.";
			FAIL(error.str());
		}
	}

	// Make sure every point in the interior is one we expect
	for (auto p : interior) {
		if (std::find(expected.begin(), expected.end(), p) == expected.end()) {
			stringstream error;
			error << "Found unexpected point " << p << " in interior.";
			FAIL(error.str());
		}
	}
}

TEST(Scalene) {
	Triangle test(Point(0, 0), Point(2, 1), Point(1, 4));
	vector<Point> interior;
	for (auto p : test) {
		interior.push_back(p);
	}

	vector<Point> expected;
	expected.emplace_back(0, 0);
	expected.emplace_back(1, 1);
	expected.emplace_back(2, 1);

	expected.emplace_back(0, 1);
	expected.emplace_back(1, 2);
	expected.emplace_back(2, 2);

	expected.emplace_back(1, 3);
	expected.emplace_back(1, 4);

	if (expected.size() != interior.size()) {
		stringstream error;
		error << "Size mismatch.\nInterior points: ";
		for (auto p : interior) error << p << " ";
		error << "\nExpected points: ";
		for (auto p : expected) error << p << " ";
		FAIL(error.str());
	}

	// Make sure every point we expect is actually in the interior
	for (auto p : expected) {
		if (std::find(interior.begin(), interior.end(), p) == interior.end()) {
			stringstream error;
			error << "Expected point " << p << " in interior.";
			error << "Size mismatch.\nInterior points: ";
			for (auto p : interior) error << p << " ";
			error << "\nExpected points: ";
			for (auto p : expected) error << p << " ";
			FAIL(error.str());
		}
	}

	// Make sure every point in the interior is one we expect
	for (auto p : interior) {
		if (std::find(expected.begin(), expected.end(), p) == expected.end()) {
			stringstream error;
			error << "Found unexpected point " << p << " in interior.";
			error << "Size mismatch.\nInterior points: ";
			for (auto p : interior) error << p << " ";
			error << "\nExpected points: ";
			for (auto p : expected) error << p << " ";
			FAIL(error.str());
		}
	}
}

int main() {
	RUN_TESTS();
}

#undef TEST_SUITE