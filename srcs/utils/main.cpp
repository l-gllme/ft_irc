#include "main.hpp"

void	shutdown(int sig)
{
	(void)sig;
	std::cout << " Shutting down..." << std::endl;
	g_shutdown = 0;
}

int g_shutdown = 1;

int	main(int ac, char **av)
{
	if (!errors(ac, av))
		return (EXIT_FAILURE);


	std::string pass = av[2];
	Server server(atoi(av[1]), pass);

	signal(SIGINT, shutdown);
	signal(SIGQUIT, shutdown);
	server.run();
	return (EXIT_SUCCESS);
}
