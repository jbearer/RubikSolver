/**
 * \headerfile lib/testing/src/unit-test.h
 * Defines macros and declares functions and types that can be used to quickly
 *  and easily write unit tests for a project.
 */

#pragma once
#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>

/// Defines a test and prepares the result of the test
#define TEST(NAME)                                                  \
void NAME(wiplib::TestResult&);                             \
template <>                                                         \
struct Test<__COUNTER__, TEST_SUITE_TEMPLATE_ARG> {                 \
    static void run(wiplib::TestResult* result) {           \
        wiplib::TestResult newResult(#NAME);                \
        try {                                                       \
            NAME(newResult);                                        \
        } catch(std::exception e) {                                 \
            newResult.except(e.what());                             \
        }                                                           \
        if (!newResult.isValid()) newResult.pass();                 \
        *result = newResult;                                        \
    }                                                               \
};                                                                  \
void NAME(wiplib::TestResult& __result)

/// If condition is false, exit the test with a failure, and save the condition
/// that was failed.
#define ASSERT(CONDITION)                                           \
    if(!(CONDITION)) {                                              \
        __result.fail(#CONDITION);                                  \
        return;                                                     \
    }

/// If expected != actual, exit the test with a failure. 
#define ASSERT_EQUAL(expected, actual)                              \
    if(!(expected == actual)) {                                     \
        std::stringstream expectedStr;                              \
        std::stringstream actualStr;                                \
        expectedStr << expected;                                    \
        actualStr << actual;                                        \
        __result.fail(expectedStr.str(), actualStr.str());          \
        return;                                                     \
    }

/// If arg1 == arg2, fail and exit the test.
#define ASSERT_NOT_EQUAL(arg1, arg2) ASSERT(arg1 != arg2)

/// If condition is false, exit with a failure
#define ASSERT_TRUE(condition) ASSERT(condition)

/// If condition is true, exit with a failure
#define ASSERT_FALSE(condition) ASSERT(!condition)

/// \todo PASS
#define PASS return;

/// \todo FAIL(message)
#define FAIL(message) __result.fail(message); return;

#define ASSERT_EXCEPT(ExceptionType, expression)                    \
    try {                                                           \
        expression;                                                 \
        std::string error = "Expected exception \'";                \
        error += typeid(ExceptionType).name();                      \
        error += "\'.";                                             \
        FAIL(error);                                                \
    }                                                               \
    catch(ExceptionType) {}
    
namespace wiplib {

    /**
    * \class TestResult
    * \brief Encapsulates data about a test and the result after running
    *   that test.
    */
    class TestResult {

    public:

        enum Status {
            /// Has not yet been assigned a valid state.
            INVALID,

            /// Ran and passed the test.
            PASSED,

            /// Ran and failed the test.
            FAILED,

            /// Ran and encountered an error, or otherwise failed to
            ///complete the test.
            INCOMPLETE
        };

        /**
         * \brief Construct an empty test result with undefined data. The test
         *  result is invalid until copy constructed.
         */
        TestResult();

        /**
         * \brief Construct an empty test result with a given name.
         * \details The TestResult must be run using pass() or fail()
         *  before any other operations are valid.
         * \post The TestResult is in an invalid state.
         *
         * \param name The name of the corresponding test.
         */
        TestResult(const std::string& name);

        /**
         * \brief Make a deep copy of the TestResult.
         */
        TestResult(const TestResult& other);

        TestResult& operator=(const TestResult& other);

        /// Free allocated memory.
        ~TestResult();

        /**
         * \brief Put the TestResult in the passed state.
         * \post The TestResult is in a valid state for reading with <<.
         */
        void pass();

        /**
         * \brief Put the TestResult in a failed state.
         * \param condition A string representing the condition that
         *  caused the test to fail. Used for output via <<.
         * \post The TestResult is in a valid state for reading with <<.
         */
        void fail(const std::string& condition);

        /**
        * \brief Put the TestResult in a failed state.
        * \param expected A string representing the expected, correct
        *   output of the test.
        * \param actual A string representing the output that was actually
        *   encountered when running the test.
        * \post The TestResult is in a valid state for reading with <<.
        */
        void fail(const std::string& expected, const std::string& actual);

        /**
         * \brief Call when the test threw an exception or otherwise failed to
         *  run completely.
         * \param errorMessage A string describing the cause of the failure.
         */
        void except(const std::string& errorMessage);

        /**
         * \brief Check if the TestResult is in a valid state for reading.
         * \return True if and only if the TestResult is in a valid state.
         */
        bool isValid() const;

        /**
         * \brief Get the status of the test.
         */
        Status status() const;

        /**
         * \brief Print the status to standard out with color coding.
         */
        void colorPrint() const;

    private:

        /// The name of the test, for output.
        std::string name_;

        Status status_;

        /// The expected, correct value for the test.
        std::string* expected_;

        /// The value actually encountered when running the test.
        std::string* actual_;

        /**
        * \brief Send information about the status of a test to stream.
        * \pre The TestResult is in a valid state for reading.
        * \warning Throws a runtime_error if invalid.
        */
        friend std::ostream& operator<<(
            std::ostream& stream, const TestResult& result);

    };

    std::ostream& operator<<(std::ostream& stream, const TestResult& result);                                                                 
}

template<int testNum, class testSuite>
struct Test {
    static void run(wiplib::TestResult* result);
};

template<int testNum, class testSuite>
struct RunTests {
    static void run(
        unsigned int& passedAccum, unsigned int& failedAccum, 
        unsigned int& incompleteAccum) 
    {
        wiplib::TestResult result;
        Test<testNum, testSuite>::run(&result);

        std::cout << result << std::endl;
        switch (result.status()) {
        case wiplib::TestResult::PASSED:
            ++passedAccum;
            break;
        case wiplib::TestResult::FAILED:
            ++failedAccum;
            break;
        case wiplib::TestResult::INCOMPLETE:
            ++incompleteAccum;
            break;
        default:
            throw std::runtime_error(
                "An error occured during testing. \
                Bad data in test result. Terminating program.");
        }
        RunTests<testNum - 1, testSuite>::run(
            passedAccum, failedAccum, incompleteAccum);
    }
};

template<class testSuite>
struct RunTests<-1, testSuite> {
    static void run(unsigned int&, unsigned int&, unsigned int&) {
        return;
    }
};

#define PREPEND_TMP(s) tmp##s
#define XPREPEND_TMP(s) PREPEND_TMP(s)
namespace XPREPEND_TMP(TEST_SUITE){
    class TEST_SUITE;
}
typedef XPREPEND_TMP(TEST_SUITE)::TEST_SUITE TEST_SUITE_TEMPLATE_ARG;

#define RUN_TESTS() {                                                       \
                                                                            \
    unsigned int testsPassed = 0;                                           \
    unsigned int testsFailed = 0;                                           \
    unsigned int testsIncomplete = 0;                                       \
                                                                            \
    const int numTests = __COUNTER__;                                       \
                                                                            \
    /* Execute each test and store the results.  */                         \
    RunTests<numTests-1, TEST_SUITE_TEMPLATE_ARG>::run(                     \
        testsPassed, testsFailed, testsIncomplete);                         \
                                                                            \
    std::cout << std::endl << testsPassed << " out of " << numTests         \
        << " tests passed." << std::endl                                    \
        << testsFailed << " tests failed." << std::endl                     \
        << testsIncomplete << " tests failed to run." << std::endl;         \
}      
