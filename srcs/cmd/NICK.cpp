#include "Client.hpp"

static bool	nick_in_use(std::list<std::string> &nicks, std::string nick, std::string old_nick)
{
	std::list<std::string>::iterator it = nicks.begin();

	it++;
	while (it != nicks.end())
	{
		if (nick == *it)
			return (true);
		it++;
	}
	nicks.push_back(nick);
	it = nicks.begin();
	it++;
	while (it != nicks.end())
	{
		if (old_nick == *it)
		{
			nicks.erase(it);
			break;
		}
		it++;
	}
	return (false);
}

static bool	nick_forbidden(std::string nick)
{
	if (nick[0] == '$' || nick[0] == ':' || nick[0] == '#')
		return (true);
	for (int i = 0; nick[i]; i++)
	{
		if (nick[i] == '.' || nick[i] == ',' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@')
			return (true);
	}
	return (false);
}

void	Client::nick(std::vector<std::string> &args, std::list<std::string> &nicks)
{
	if (this->_authorized == false)
		this->_response += err_noregistered(CLIENT);
	else if (args.size() == 0)
		this->_response += err_nonicknamegiven(CLIENT);
	else if (args.size() != 1 || nick_forbidden(args[0]))
		this->_response += err_erroneusnickname(CLIENT, args[0]);
	else
	{
		if (nick_in_use(nicks, args[0], _nick))
		{
			_response += err_nicknameinuse(CLIENT, args[0]);
			return ;
		}
		else if (this->_bnick == false)
		{
			this->_bnick = true;
			this->_response = ":Irssi NICK " + args[0] + "\r\n";
			_nick = args[0];
			if (this->_busername == true)
			{
				this->_authenticated = true;
				this->_response += rpl_welcome(_nick, _username);
			}
		}
		else
		{
			this->_response = ":" + this->_nick + " NICK " + args[0] + "\r\n";
			_nick = args[0];
		}
	}
}
