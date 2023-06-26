#include "Server.hpp"
#include <iostream>
#include <sstream>

/******************************************************************************/
/**********************************  Debug  ***********************************/
/******************************************************************************/

void	check_event(int fd)
{
	std::cout << "Event type : ";
	if (fd & EPOLLIN)
		std::cout << "EPOLLIN ";
	if (fd & EPOLLOUT)
		std::cout << "EPOLLOUT ";
	if (fd & EPOLLRDHUP)
		std::cout << "EPOLLRDHUP ";
	if (fd & EPOLLHUP)
		std::cout << "EPOLLHUP ";
	if (fd & EPOLLET)
		std::cout << "EPOLLET ";
	std::cout << std::endl;
}

/******************************************************************************/
/******************************  Constructor  *********************************/
/******************************************************************************/

Server::Server(void) : _port(0), _pass(""), _clients(), _events(MAX_EVENTS), _channels(), _nicks()
{
}

Server::Server(int const port, std::string const pass) : _port(port), _pass(pass), _clients(), _events(MAX_EVENTS)
{
	// Create server socket
	_server_fd = create_socket();

	// Set server socket to non-blocking
	set_nonblocking(_server_fd);

	// Create the epoll instance
	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
	{
		perror("epoll_create1");
		_exit();
	}

	// Add the server socket to the epoll instance
	add_epoll(_epoll_fd, _server_fd, EVENTMODE);
	_nicks.push_front("");
}

Server::~Server(void)
{
	shutdown();
}

/******************************************************************************/
/******************************  Member Func *********************************/
/******************************************************************************/

void	Server::run(void)
{
	while (g_shutdown)
	{
		if (DEBUG)
			std::cout << "Waiting for events..." << std::endl;
		int nread = epoll_wait(_epoll_fd, _events.data(), _events.size(), -1);
		if (nread == -1)
		{
			if (errno == EINTR)
				continue;
			else
			{
				perror("epoll_wait");
				_exit();
			}
		}

		for (int i = 0; i < nread; i++)
		{
			if (DEBUG)
				check_event(_events[i].events);
			if (_events[i].data.fd == _server_fd)
				handle_new_connection();
			else if (_events[i].events)
				handle_client_data(_events[i].data.fd);
			if (DEBUG)
				std::cout << std::endl;
		}
	}
}

/******************************************************************************/
/******************************  Private Func ********************************/
/******************************************************************************/

void	Server::shutdown(void)
{
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _server_fd, NULL);
	close(_server_fd);
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, it->first, NULL);
		close(it->first);
	}
	close(_epoll_fd);
	_clients.clear();
}

void	Server::_exit(void)
{
	_events.~vector();
	exit(EXIT_FAILURE);
}

int	Server::create_socket(void)
{
	// Create the socket
	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket failed");
		_exit();
	}

	// Set socket options
	int	opt = TRUE;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		_exit();
	}

	// Set the socket to non-blocking
	set_nonblocking(server_fd);

	// Set up the server address structure
	struct sockaddr_in	address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	// Bind the socket to the server address
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		_exit();
	}

	// Listen for incoming connections
	if (listen(server_fd, SOMAXCONN) < 0)
	{
		perror("listen");
		_exit();
	}

	return (server_fd);
}

void	Server::set_nonblocking(int const sockfd)
{
	int flags = fcntl(sockfd, F_GETFL, O_NONBLOCK);
	if (flags == -1)
	{
		perror("fcntl");
		_exit();
	}
}

void	Server::add_epoll(int const epollfd, int const sockfd, uint32_t const eventmode)
{
	struct epoll_event	event;

	memset(&event, 0, sizeof(event));
	event.data.fd = sockfd;
	event.events = eventmode;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event))
	{
		perror("epoll_ctl");
		_exit();
	}
}

void	Server::handle_new_connection(void)
{
	// Accept the new connection
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len = sizeof(client_addr);
	int const client_socket = accept(_server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_socket < 0)
	{
		perror("accept");
		_exit();
	}

	// Set the socket to non-blocking
	set_nonblocking(client_socket);

	// Add the socket to the epoll instance
	add_epoll(_epoll_fd, client_socket, EVENTMODE);

	// Add the socket to the list of clients
	if (DEBUG)
		std::cout << "New client connected: " << client_socket << std::endl;
	_clients[client_socket] = Client(client_socket, _pass);
}

void Server::handle_client_data(int const clientfd)
{
	int		bytes_read = 0;
	char	buffer[BUF_SIZE + 1];
	
	bytes_read = recv(clientfd, buffer, BUF_SIZE, MSG_DONTWAIT);
	if (bytes_read > 0)
	{
		std::string msg(buffer, bytes_read);
		if (DEBUG)
			std::cout << "\033[35mRead: \033[0m" << msg;
		_msg[clientfd] += msg;
		if (_msg[clientfd].find("\n") != std::string::npos)
		{
			if (DEBUG)
				std::cout << "\033[35mMessage: \033[0m" << _msg[clientfd];
			send_message(clientfd, _msg[clientfd]);
			_msg[clientfd].erase();
		}
	}
	else if (bytes_read == 0)
	{
		if (DEBUG)
			std::cout << "Client disconnected" << std::endl;
		remove_client(clientfd);
	}
	else if (bytes_read == -1)
	{
		perror("Error reading from client socket");
		remove_client(clientfd);
	}
}

void	Server::remove_client(int const clientfd)
{
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, clientfd, NULL);
	close(clientfd);
	for (std::list<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it != _channels.end())
		{
			if (it->is_in_channel(clientfd))
				it->remove_user(clientfd);
		}
	}
	std::string nick = _clients[clientfd].get_nick();
	for (std::list<std::string>::iterator it = _nicks.begin(); it != _nicks.end(); ++it)
	{
		if (it != _nicks.end())
		{
			if (nick == *it)
			{
				_nicks.erase(it);
				break;
			}
		}
	}
	_msg.erase(clientfd);
	_clients.erase(clientfd);
}

int	get_private_fd(std::string nick, std::map<int, Client> &_clients)
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.get_nick() == nick)
			return (it->second.get_fd());
	}
	return (-1);
}

bool	is_privmsg(std::string str)
{
	int	i;
	std::size_t	found;

	i = 0;
	while (str[i])
	{
		found = str.find("PRIVMSG", i);
		if ((int)found == i)
			return (true);
		i++;
	}
	return (false);
}

void	Server::send_message(int const clientfd, std::string const & msg)
{
	_clients[clientfd].parse_cmd(msg);
	std::vector<std::string> reply = _clients[clientfd].get_response(_nicks, _channels);
	int	privatefd;
	std::string	res;

	if (reply.size() == 1)
	{
		if (send(clientfd, reply[0].c_str(), reply[0].size(), 0) < 0)
		{
			perror("Error sending message to client");
			remove_client(clientfd);
		}
	}
	else if (reply.size() == 2 || reply.size() == 3)
	{
		if (reply[1][0] != '#')
		{
			privatefd = get_private_fd(reply[1], _clients);
			if (privatefd != -1)
			{
				if (send(privatefd, reply[0].c_str(), reply[0].size(), 0) < 0)
				{
					perror("Error sending message to client");
					remove_client(clientfd);
				}
			}
		}
		else
		{
			std::list<Channel>::iterator itchan = _channels.begin();

			while (itchan != _channels.end())
			{
				if (reply[1] == itchan->get_name())
					break ;
				itchan++;
			}
			if (itchan == _channels.end())
				return ;
			std::map<int, t_user> users = itchan->get_users();
			std::map<int, t_user>::iterator it = users.begin();
			
			while (it != users.end())
			{
				privatefd = it->first;
				if (clientfd == privatefd && is_privmsg(reply[0]))
				{
					it++;
					continue;
				}
				if (privatefd != -1)
				{
					if (send(privatefd, reply[0].c_str(), reply[0].size(), 0) < 0)
					{
						perror("Error sending message to client");
						remove_client(clientfd);
					}
				}
				it++;
			}
		}
		if (reply.size() == 3)
		{
			std::list<Channel>::iterator a = _channels.begin();

			while (a != _channels.end())
			{
				if (reply[1] == a->get_name())
				{
					int num;
					std::istringstream iss(reply[2]);
					iss >> num;
					a->remove_user(num);
					if (a->chan_empty())
						_channels.erase(a);
					break;
				}
				a++;
			}
		}
	}
	if (DEBUG)
		std::cout << "\033[34mSent: \033[0m" << *reply.begin() << std::endl;
}
