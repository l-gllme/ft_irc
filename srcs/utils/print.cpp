#include "print.hpp"

void	print_error(std::string const &str, int const style)
{
	// set style
	if (style & 1)
		std::cerr << BOLD;
	if (style & 2)
		std::cerr << UNDERLINE;	
	if (style & 4)
		std::cerr << BLINK;
	if (style & 8)
		std::cerr << REVERSE;

	std::cerr << RED << "Error: " << str << RESET << std::endl;
}

void	print_str(std::string const &str, int const color, int const style)
{
	// set style
	if (style & 1)
		std::cout << BOLD;
	if (style & 2)
		std::cout << UNDERLINE;	
	if (style & 4)
		std::cout << BLINK;
	if (style & 8)
		std::cout << REVERSE;

	// set color
	if (color == 1)
		std::cout << BLACK;
	if (color == 2)
		std::cout << RED;
	if (color == 3)
		std::cout << GREEN;
	if (color == 4)
		std::cout << YELLOW;
	if (color == 5)
		std::cout << BLUE;

	std::cout << str << RESET << std::endl;
}
