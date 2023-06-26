#ifndef SERVER_HPP
# define SERVER_HPP

# define MAX_EVENTS 64
# define BUF_SIZE 1024

# define DEBUG 0

# define EVENTMODE (EPOLLIN | EPOLLRDHUP)

# define TRUE 1
# define FALSE 0

# include <sys/epoll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <string>
# include <cstdlib>
# include <stdio.h>
# include <errno.h>

# include <vector>
# include <map>
# include <list>

# include "Client.hpp"
# include "Channel.hpp"
# include "reply.hpp"

extern int g_shutdown;

// Class
class Server
{
	public:
		Server(void);
		Server(int const port, std::string const pass);
		~Server(void);

		void	run(void);

	private:
		void	shutdown(void);
		void	_exit(void);

		int		create_socket(void);
		void	set_nonblocking(int const sockfd);
		void	add_epoll(int const epollfd, int const sockfd, uint32_t const eventmode);
		void	handle_new_connection(void);
		void	handle_client_data(int const clientfd);
		void	remove_client(int const clientfd);

		void	send_message(int const clientfd, std::string const & message);

		void	join_channel(int const clientfd, std::string const & channel);
		void	leave_channel(int const clientfd, std::string const & channel);

		int		_port;
		int		_epoll_fd;
		int		_server_fd;
		std::string _pass;
		std::map<int, Client>	_clients;
		std::map<int, std::string> _msg;
		std::vector<epoll_event>	_events;
		std::list<Channel>	_channels;
		std::list<std::string> _nicks;
};

#endif // SERVER_HPP
