#include "Bot.hpp"

static void	shutdown(int sig)
{
	(void)sig;
	std::cout << " Shutting down..." << std::endl;
	g_shutdown = 0;
}

int g_shutdown = 1;

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage: ./bot <server_ip> <server_password>" << std::endl;
		return (EXIT_FAILURE);
	}
	Bot bot(atoi(av[1]), av[2]);

	signal(SIGINT, shutdown);
	signal(SIGQUIT, shutdown);
	bot.run();
	return (EXIT_SUCCESS);
}