/**
 * \file benchmark.h
 * \brief Declares methods for gathering information about the runtime of
 *  arbitrary function calls.
 * \details Data can be obtained via the return value of a function, by printing
 *  to standard out, or by generating a plot of the results.
 *
 * \todo Implement this interface.
 */

#include <string>

namespace wiplib {

    /**
     * \brief Time execution of a number of calls to a given function.
     * \param function The function to benchmark.
     * \param iterations The number of times to execute function.
     * \return The total elapsed time in millisecond.
     */
    int timeExecution(void* function, int iterations);

    /**
     * \brief Execute a given function a number of times and print information
     *  about the elapsed time to standard out.
     * \param function The function to benchmark.
     * \param iterations The number of times to execute function.
     */
    void benchmark(void* function, int iterations);

    /**
     * \brief Benchmark two functions and print information comparing the
     *  results for each function to standard out.
     */
    void compare(void* function1, void* function2, int iterations);

    /**
     * \brief Execute a given function a number of times, and produce a plot
     *  of the elapsed time after each iteration.
     * \param function The function to benchmark.
     * \param maxIterations The number of executions over which data should be
     *  gathered.
     * \param outputFile The file path to where the resulting plot should be 
     *  saved.
     * \param outputFilename The name of the file (without extension) in which
     *  the plot should be saved.
     */
    void plot(void* function, int maxIterations,
        std::string outputPath, std::string outputFilename);

}
