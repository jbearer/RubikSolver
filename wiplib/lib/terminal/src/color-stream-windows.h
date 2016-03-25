/**
 * \headerfile lib/terminal/src/color-stream-windows.h.
 * Declares a class that can be used to stream text to a terminal window
 *  in various colors.
 */

#include <string>
#include <ostream>
#include <unordered_map>
#include <vector>
#include <windows.h>

namespace wiplib {

    /**
     * \class ColorStreamWindows
     * \brief An object which can be used like std::cout, but conveniently
     *  supports printing text in various colors.
     * \warning Only works when printing to a terminal window.
     */
    class ColorStreamWindows {
    public:

        /**
         * \brief Construct a color stream object which outputs to standard out.
         */ 
        ColorStreamWindows();

        /**
         * \brief Restore default settings at destruction.
         */
        ~ColorStreamWindows();

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
         * \brief If the requested color is supported and not the same as the
         *	background color, set the color of subsequent text to that color.
         *	Otherwise, no sideeffects.
         * \param color The color to which to set the text.
         * \return True if the text color was set, false otherwise.
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
        ColorStreamWindows& operator<<(const T& output);

        ColorStreamWindows& operator<<(std::ostream&(*manipulator)(std::ostream&));

    private:

        /**
         * \brief Try to set the color to a color encoded by \param color.
         *	If the requested color is the same as the background color, do not
         *	set the color.
         * \return True if and only if the color was set.
         */
        bool setColor(char color);

        /// A handle for the standard console window.
        HANDLE console_;        

        /// Map strings which represent supported colors chars which code for
        /// them in the Windows environment.
        std::unordered_map<std::string, char> supportedColors_;

        /// The user's original console settings.
        CONSOLE_SCREEN_BUFFER_INFO defaults_;
    };
}

#include "color-stream-windows-private.h"