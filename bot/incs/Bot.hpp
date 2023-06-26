#ifndef BOT_HPP
# define BOT_HPP

# include <string>
# include <string.h>
# include <iostream>
# include <signal.h>
# include <cstdlib>
# include <errno.h>
# include <sys/socket.h>
# include <stdio.h>
# include <netinet/in.h>
# include <vector>
# include <sys/epoll.h>

# define TRUE 1
# define MAX_EVENTS 64

extern int g_shutdown;

class Bot
{
	public:
		Bot(int const & server_ip, std::string const & server_password);
		~Bot(void);
		void	run(void);
		void	shutdown(void);

	private:
		std::string		recvMessage(void);
		void			sendMessage(std::string const & message);

		void	connectToServer(void);
		void	loginToServer(void);

		void	listenToServer(void);

		void	_exit(void);

		int const			_server_ip;
		std::string const	_server_password;
		int					_socket_fd;
		int					_epoll_fd;
		bool				_logged_in;
		std::vector<epoll_event>	_events;
};

# endif // BOT_HPP
