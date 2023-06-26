#include "Client.hpp"

void	Client::pass(std::vector<std::string> &args)
{
	if (this->_authorized == true)
		this->_response = err_alreadyregistered(CLIENT);
	else if (args.size() == 0)
		this->_response = err_needmoreparams(CLIENT, "PASS");
	else if (args.size() != 1 || args.front() != this->_pass)
		this->_response = err_passwdmismatch(CLIENT);
	else
	{
		_authorized = true;
		this->_response = "Pass accepted\r\n";
	}
}
