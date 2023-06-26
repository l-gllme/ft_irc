#include "Client.hpp"

void	Client::user(std::vector<std::string> &args)
{
	if (this->_authorized == false)
		this->_response = err_noregistered(CLIENT);
	else if (args.size() == 0 || args[0].empty())
		this->_response = err_needmoreparams(CLIENT, "PASS");
	else if (this->_busername == true)
		this->_response = err_alreadyregistered(CLIENT);
	else
	{
		if (args.size() < 4 || !args[1][0] || !args[2][0] || args[3][0] != ':' || !args[3][1])
		{
			this->_response = "Wrong USER format\r\n"; //Message error change ?
			return ;
		}
		else
		{
			if (this->_bnick == false)
			{
				this->_username.append("~");
				this->_username.append(args[0]);
			}
			else
				this->_username = this->_nick;
			this->_realname.append(args[3].begin()+1, args[3].end());
			for (size_t i = 4; i < args.size(); i++)
			{
				this->_realname += " ";
				this->_realname += args[i];
			}
		}
		this->_busername = true;
		this->_response += "Username set as " + _username + " and realname set as " + _realname + "\r\n";
		if (this->_bnick == true)
		{
			this->_authenticated = true;
			this->_response += rpl_welcome(_nick, _username);
		}
	}
}
