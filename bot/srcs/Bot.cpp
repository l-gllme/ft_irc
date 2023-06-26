#include "Bot.hpp"

Bot::Bot(int const & server_ip, std::string const & server_password)
	: _server_ip(server_ip), _server_password(server_password), _logged_in(false), _events(MAX_EVENTS)
{
	connectToServer();
	loginToServer();

	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
	{
		perror("epoll_create1");
		_exit();
	}

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.data.fd = _socket_fd;
	event.events = EPOLLIN | EPOLLRDHUP;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _socket_fd, &event))
	{
		perror("epoll_ctl");
		_exit();
	}

	return ;
}

Bot::~Bot(void)
{
	return ;
}

void	Bot::run(void)
{
	std::cout << "Bot running..." << std::endl;
	while (g_shutdown)
	{
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
			if (_events[i].events)
				listenToServer();
		}
	}
	shutdown();
}

void	Bot::shutdown(void)
{
	sendMessage("QUIT :Shutting down...\r\n");
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, _socket_fd, NULL);
	close(this->_socket_fd);
	close(this->_epoll_fd);
}

std::string	Bot::recvMessage(void)
{
	std::string message;
	char buffer[1024];

	if (recv(this->_socket_fd, buffer, 1024, 0) < 0)
	{
		perror("recv");
		_exit();
	}
	message = buffer;
	message.append("\0");
	return (message);
}

void	Bot::sendMessage(std::string const & message)
{
	if (send(this->_socket_fd, message.c_str(), message.size(), 0) < 0)
	{
		perror("send");
		_exit();
	}
	return ;
}

void	Bot::connectToServer(void)
{
	// connect to the server socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd < 0)
	{
		perror("socket");
		_exit();
	}
	int op = TRUE;
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&op, sizeof(op)) < 0)
	{
		perror("setsockopt");
		_exit();
	}

	struct sockaddr_in address;
	memset(&address, '0', sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->_server_ip);

	if (connect(this->_socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("connect");
		_exit();
	}
	return ;
}

void Bot::_exit(void)
{
	_events.~vector();
	exit(EXIT_FAILURE);
}

void	Bot::loginToServer(void)
{
	sendMessage("PASS " + this->_server_password + "\r\n");
	if (recvMessage() != "Pass accepted\r\n")
	{
		std::cout << "Password incorrect" << std::endl;
		_exit();
	}
	this->_logged_in = true;
	// finish login
	sendMessage("NICK bot\r\n");
	recvMessage();
	sendMessage("USER bot 0 localhost :bot\r\n");
	recvMessage();
	return ;
}

void	Bot::listenToServer(void)
{
	std::string message;
	std::string command;

	message = recvMessage();
	// std::cout << message;
	command = "PRIVMSG " + message.substr(1, message.find('!') - 1);
	if (message.find("PRIVMSG") != std::string::npos && message.find("PING") != std::string::npos)
		sendMessage(command + " :PONG\r\n");
	else if (message.find("PRIVMSG") != std::string::npos && message.find("PONG") != std::string::npos)
		sendMessage(command + " :PING\r\n");
	return ;
}
