#ifndef CHECK_HPP
# define CHECK_HPP

# include "print.hpp"

bool	isdigit(const char c);
bool	check_port(const char *port);
bool	check_pass(const char *pass);
bool	errors(int ac, char **av);

#endif // CHECK_HPP
