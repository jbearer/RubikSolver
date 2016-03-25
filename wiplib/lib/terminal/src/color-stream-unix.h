/**
 * \headerfile lib/terminal/src/color-stream-unix.h.
 * Declares a class that can be used to stream text to a terminal window
 *  in various colors.
 */

#include <string>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace wiplib {

    /**
     * \class ColorStreamUnix
     * \brief An object which can be used like std::cout, but conveniently
     *  supports printing text in various colors.
     * \warning Only works when printing to a terminal window.
     */
    class ColorStreamUnix {
    public:

        /**
         * \brief Construct a color stream object which outputs to standard out.
         */ 
        ColorStreamUnix();

        /**
         * \brief Restore default settings at destruction.
         */
        ~ColorStreamUnix();

        /**
         * \return True if the requested color is supported by the terminal.
         *	False otherwise.
         */
        bool isSupported(std::string color) const;

        /**
         * \return A vector containing strings representing the colors supported
         *	by the terminal.
         */
        std::vector<std::string> supportedColors() const;

        /**
         * \brief If the requested color is supported, set the color of
         *  subsequent text to that color. Otherwise, no sideeffects.
         * \param color The color to which to set the text.
         * \return True if the requested color is supported, false otherwise.
         */
        bool setColor(std::string color);

        /**
         * \brief Restore the color to the user's default setting.
         */
        void resetColor();

        /**
         * \brief Print to the standard output.
         * \param output Data to be printed. Can be any type which can stream to
         *  cout.
         */
        template <class T>
        ColorStreamUnix& operator<<(const T& output);

        ColorStreamUnix& operator<<(std::ostream&(*manipulator)(std::ostream&));

    private:

        /// Set the color to a color encoded by \param color.
        void setColor(char color);     

        /// Map strings which represent supported colors to chars which code for
        /// them in the ANSI standard.
        std::unordered_map<std::string, char> supportedColors_;
    };
}

#include "color-stream-unix-private.h"