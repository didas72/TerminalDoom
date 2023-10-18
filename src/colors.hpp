//colors.hpp - 

#ifndef COLORS_HPP_
#define COLORS_HPP_

#include <string>

#ifdef __linux__
#define COLOR_RESET "\x1B[0m"
#define COLOR_BLACK "\x1B[30m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_CYAN "\x1B[36m"
#define COLOR_WHITE "\x1B[37m"
#endif

std::string colors[] = { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE };

enum class Color : int
{
	Black = 0,
	Red = 1,
	Green = 2,
	Yellow = 3,
	Blue = 4,
	Magenta = 5,
	Cyan = 6,
	White = 7
};

inline std::string getColorStr(Color c)
{
	return colors[(int)c % 7];
}

#endif
