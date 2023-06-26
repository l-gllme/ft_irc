#include <User.hpp>

User::User(std::string nickname, std::string username) :
	_nickname(nickname), _username(username), _operator(0)
{
	return ;
}

User::User(const User & src) : _nickname(src._nickname), _username(src._username),
	_operator(src._operator)
{
	return ;
}

User::~User(void)
{
	return ;
}

User	&User::operator=(const User &src)
{
	this->_nickname = src.getNickname();
	this->_username = src.getUsername();
	this->_operator = src.isOperator();
	return (*this);
}

std::string	User::getNickname(void) const
{
	return (this->_nickname);
}

std::string	User::getUsername(void) const
{
	return (this->_username);
}

void	User::setNickname(std::string str)
{
	this->_nickname = str;	
	return ;
}

void	User::setUsername(std::string str)
{
	this->_username = str;	
	return ;
}

bool	User::isOperator(void) const
{
	return (this->_operator);
}

void	User::setOperator(bool op)
{
	this->_operator = op;
	return ;
}
