#ifndef COLOR_HPP
# define COLOR_HPP

// Macro text style
# define S_BOLD			1
# define S_UNDERLINE	2
# define S_BLINK		4
# define S_REVERSE		8

// Define text style
# define RESET		"\033[0m"
# define BOLD		"\033[1m"
# define UNDERLINE	"\033[4m"
# define BLINK		"\033[5m"
# define REVERSE	"\033[7m"
# define CONCEALED	"\033[8m"

// Macro text color
# define C_BLACK		1
# define C_RED			2
# define C_GREEN		3
# define C_YELLOW		4
# define C_BLUE			5

// Define text color
# define BLACK		"\033[30m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"

#endif // COLOR_HPP
