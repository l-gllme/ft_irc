#include "Client.hpp"

Client::Client(void) : _op(false), _fd(-1), _pass("")
{
	// Empty
}

Client::Client(int const fd, std::string const &pass) : _fd(fd), _pass(pass), _username(""), _realname(""), _nick("*")
{
	_authorized = false;
	_bnick = false;
	_busername = false;
	_authenticated = false;
}

Client::~Client(void)
{
	// Empty
}

int const & Client::get_fd(void) const
{
	return (_fd);
}

std::string const &Client::get_nick(void) const
{
	return (_nick);
}

std::string const &Client::get_username(void) const
{
	return (_username);
}

std::string const &Client::get_realname(void) const
{
	return (_realname);
}

void	Client::parse_cmd(std::string line)
{
	this->_response = "";
	this->_line.clear();
	this->split(line, "\r\n");
}

void	Client::split(std::string package, std::string delim)
{
	char *tmp;

	tmp = strtok((char *)package.c_str(), delim.c_str());
	while (tmp)
	{
		this->_line.push_back(std::string(tmp));
		tmp = strtok(NULL, delim.c_str());
	}
}

void	Client::set_op(bool a)
{
	this->_op = a;
}

bool	Client::get_op(void)
{
	return (this->_op);
}

std::string	Client::get_cmd(size_t i)
{
	std::string tmp = _line[i];
	std::string res;

	for (int j = 0; tmp[j] && tmp[j] != ' '; j++)
		res += tmp[j];
	return (res);
}

std::vector<std::string>	Client::get_args(size_t i, std::string delim)
{
	std::vector<std::string>	vec;
	char *tmp;

	tmp = strtok((char *)_line[i].c_str(), delim.c_str());
	while (tmp)
	{
		vec.push_back(std::string(tmp));
		tmp = strtok(NULL, delim.c_str());
	}
	vec.erase(vec.begin());
	return (vec);
}

bool	Client::nick_exist(std::list<std::string> &nicks, std::string nick)
{
	std::list<std::string>::iterator it = nicks.begin();

	it++;
	while (it != nicks.end())
	{
		if (nick == *it)
			return (true);
		it++;
	}
	return (false);
}

bool	Client::channel_exist(std::list<Channel> &channels, std::string channel)
{
	std::list<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		if (channel == it->get_name())
			return (true);
		it++;
	}
	return (false);
}

std::vector<std::string> Client::get_response(std::list<std::string> &nicks, std::list<Channel> &channels)
{
	std::string	cmd;
	std::vector<std::string> args;
	std::vector<std::string> res;

	for (size_t i = 0; i < _line.size(); i++)
	{
		cmd = this->get_cmd(i);
		args = this->get_args(i, " ");
		if (cmd == "PING")
			this->_response = "PONG\r\n";
		else if (cmd == "CAP")
			;
		else if (cmd == "MODE")
			mode(res, args, channels);
		else if (cmd == "WHOIS")
			;
		else if (cmd == "PASS")
			pass(args);
		else if (_authorized == false)
			this->_response = err_noregistered(_nick);
		else if (cmd == "NICK")
			nick(args, nicks);
		else if (cmd == "USER")
			user(args);
		else if (cmd == "AUTHENTICATE")
		{
			if (this->_authenticated == true)
				this->_response = "Authenticated\r\n";
			else
				this->_response = "Not authenticated\r\n";
		}
		else if (_authenticated == false)
			this->_response = err_noregistered(CLIENT);
		else if (cmd == "JOIN")
			join(res, args, channels);
		else if (cmd == "PRIVMSG")
			privmsg(args, res, nicks, channels);
		else if (cmd == "NOTICE")
			notice(args, res, nicks, channels);
		else if (cmd == "PART")
			part(res, args, channels);
		else if (cmd == "KICK")
			kick(res, args, channels, nicks);
		else if (cmd == "INVITE")
			invite(res, args, channels);
		else if (cmd == "QUIT")
			quit(args);
		else
			this->_response += err_unknowncommand(_nick, cmd);
		args.clear();
	}
	res.insert(res.begin(), this->_response);
	return (res);
}
