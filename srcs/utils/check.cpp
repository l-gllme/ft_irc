#include "check.hpp"

bool	isdigit(const char c)
{
	return (c >= '0' && c <= '9');
}

bool	check_port(const char *port)
{
	int	i = 0;

	if (!port || !*port)
		return (false);
	while (port[i])
	{
		if (!isdigit(port[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	check_pass(const char *pass)
{
	int	i = 0;

	if (!pass[0])
		return (false);
	while (pass[i])
	{
		if (pass[i] == ' ')
			return (false);
		i++;
	}
	return (true);
}

bool	errors(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << RED << "Usage: " << av[0] << " <port> <password>" << RESET << std::endl;
		return (false);
	}
	if (!check_port(av[1]))
	{
		print_error("Invalid port", S_BOLD | S_UNDERLINE);
		return (false);
	}
	if (!check_pass(av[2]))
	{
		print_error("Invalid password", S_BOLD | S_UNDERLINE);
		return (false);
	}
	return (true);
}
