#include "Client.hpp"

void	Client::quit(std::vector<std::string> &args)
{
	(void)args;
	this->_response = "QUIT\r\n";
}
