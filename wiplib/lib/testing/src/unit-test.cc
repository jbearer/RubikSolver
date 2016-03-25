#include "unit-test.h"
#include "color-stream.h"
#include <iostream>
#include <string>
#include <stdexcept>

///////////////////////////////////////////////////////////////////////////////
// Implementation of TestResult class
///////////////////////////////////////////////////////////////////////////////

wiplib::TestResult::TestResult()
    : status_{ INVALID }, expected_{nullptr}, actual_{nullptr} {}

wiplib::TestResult::TestResult(const std::string& name)
    : name_{ name }, status_{ INVALID }, expected_{nullptr}, actual_{nullptr} {}

wiplib::TestResult::TestResult(const TestResult& other)
    : name_{ other.name_ }, status_{ other.status_ },
    expected_{ other.expected_ ? new std::string(*(other.expected_)) : nullptr },
    actual_{ other.actual_ ? new std::string(*(other.actual_)) : nullptr } {
    // Nothing else to do
}

wiplib::TestResult& wiplib::TestResult::operator=(
    const wiplib::TestResult& other) 
{
    name_ = other.name_;
    status_ = other.status_;

    delete expected_;
    delete actual_;
    expected_ = other.expected_ ? new std::string(*(other.expected_)) : nullptr;
    actual_ = other.actual_ ? new std::string(*(other.actual_)) : nullptr;

    return *this;
}

wiplib::TestResult::~TestResult() {
    delete expected_;
    delete actual_;
}

void wiplib::TestResult::pass() {
    status_ = PASSED;
    expected_ = nullptr;
    actual_ = nullptr;
}

void wiplib::TestResult::fail(const std::string& condition) {
    status_ = FAILED;
    expected_ = new std::string{condition};
    actual_ = nullptr;
}

void wiplib::TestResult::fail(const std::string& expected, const std::string& actual) {
    status_ = FAILED;
    expected_ = new std::string{expected};
    actual_ = new std::string{actual};
}

void wiplib::TestResult::except(const std::string& errorMessage) {
    status_ = INCOMPLETE;
    expected_ = new std::string{errorMessage};
    actual_ = nullptr;
}

bool wiplib::TestResult::isValid() const {
    return !(status_ == INVALID);
}

wiplib::TestResult::Status wiplib::TestResult::status() const {
    return status_;
}

void wiplib::TestResult::colorPrint() const {
    if (!isValid()) throw std::runtime_error(
        "Attempted to read invalid TestResult. Run test before attempting to read result.");

    ColorStream stream;

    if (status_ == INCOMPLETE) {
        stream.setColor("red");
        stream << "The test \"" << name_ 
            << "\" failed to run. Reason:" << std::endl
            << "\t" << *expected_;
        return;
    }

    if (status_ == PASSED) stream.setColor("green");
    else stream.setColor("red");
    stream << "The test \"" << name_ << "\" "
        << (status_ == TestResult::PASSED ? "passed" : "failed") << ".";

    if (expected_)
        stream << std::endl << "\tExpected:\t" << *expected_;
    if (actual_)
        stream << std::endl << "\tActual:\t" << *actual_;

}

std::ostream& wiplib::operator<<(std::ostream& stream, const TestResult& result) {
    result.colorPrint();
    return stream;
}
