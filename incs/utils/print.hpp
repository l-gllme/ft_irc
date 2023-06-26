#ifndef PRINT_HPP
# define PRINT_HPP

# include "color.hpp"
# include <iostream>
# include <string>

// Errors
void	print_error(std::string const &str, int const style = 0);
void	print_str(std::string const &str, int const color = 0, int const style = 0);

#endif // PRINT_HPP
