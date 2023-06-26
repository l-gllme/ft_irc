#include "Client.hpp"

void	Client::privmsg(std::vector<std::string> &args, std::vector<std::string> &res, std::list<std::string> &nicks, std::list<Channel> &channels)
{
	if (args.size() < 2 || args[0].empty() || args[1].empty())
		this->_response = err_needmoreparams(CLIENT, "PRIVMSG");
	else if (args[1][0] != ':') 
		this->_response = "Wrong PRIVMSG format: need ':'\r\n";
	else if (args[0][0] != '#' && !nick_exist(nicks, args[0]))
		this->_response = err_nosuchnick(CLIENT, args[0]);
	else if (args[0][0] == '#' && !channel_exist(channels, args[0]))
		this->_response = err_nosuchnick(CLIENT, args[0]);
	else
	{
		this->_response += (":" + this->_nick + "!~" + this->_username + "@localhost PRIVMSG ");
		this->_response += (args[0] + " :");
		this->_response.append(args[1].begin()+1, args[1].end());
		for (size_t i = 2; i < args.size(); i++)
		{
			this->_response += " ";
			this->_response += args[i];
		}
		this->_response += "\r\n";
		res.push_back(args[0]);
	}
}
