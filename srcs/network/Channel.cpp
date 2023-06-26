#include "Channel.hpp"

Channel::Channel(void) : _users(), _invite_only(false)
{
	return ;
}

Channel::Channel(std::string const &name) : _name(name), _users(), _invite_only(false)
{
}

Channel::~Channel(void)
{
}

std::string const &Channel::get_name(void) const
{
	return (this->_name);
}

std::map<int, t_user> const &Channel::get_users(void) const
{	
	return (this->_users);
}

std::string	Channel::get_user_list(void)
{
	std::string ret;
	std::map<int, t_user>::iterator it = this->_users.begin();

	while (it != this->_users.end())
	{
		ret += it->second.nick;
		if (it->second.op)
			ret += "@";
		ret += " ";
		it++;
	}
	ret += "\r\n";
	return (ret);
}

std::list<std::string>	&Channel::get_invited_list(void)
{
	return (this->_invited);
}

void	Channel::push_user_invited(std::string name)
{
	this->_invited.push_back(name);
}

void Channel::add_user(int const &fd, std::string const &nick, bool const &op, bool const &invited)
{
	t_user t;

	t.nick = nick;
	t.op = op;
	t.invited = invited;
	this->_users.insert(std::pair<int, t_user>(fd, t));
}

void Channel::remove_user(int const &fd_user)
{
	this->_users.erase(fd_user);
}
#include <iostream>
bool Channel::chan_empty(void)
{
	if (_users.size() == 0)
		return (true);
	return (false);
}

bool Channel::is_in_channel(int const &fd_user)
{
	std::map<int, t_user>::iterator it = this->_users.begin();

	while (it != this->_users.end())
	{
		if (it->first == fd_user)
			return (true);
		it++;
	}
	return (false);
}

void Channel::op(int const &fd_user)
{
	this->_users[fd_user].op = true;
}

void Channel::set_invite_only(bool a)
{
	this->_invite_only = a;
}

bool Channel::get_invite_only(void)
{
	return (this->_invite_only);
}

bool Channel::operator==(Channel const &rhs)
{
	return (this->_name == rhs._name);
}
