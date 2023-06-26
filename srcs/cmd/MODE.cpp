#include <Client.hpp>

static bool	verify_channel_exist(std::list<Channel> &channels, std::string &s, bool inv)
{
	std::list<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		if (s == it->get_name())
		{
			it->set_invite_only(inv);
			return (true);
		}
		it++;
	}
	return (false);
}

static bool	verify_user_op(std::list<Channel> &channels, std::string &s, int &id)
{
	std::list<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		if (s == it->get_name())
			break ;
		it++;
	}
	if (it == channels.end())
		return (false);

	std::map<int, t_user> users = it->get_users();
	
	if (!users[id].op)
		return (false);
	return (true);
}


/* MODE <channel> +i/-i */
/* :<server_name> MODE <channel> +i/-i */
void    Client::mode(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels)
{
	bool inv;
	(void) res;

	if (args.size() != 2 || args[0].empty() || args[1].empty())
		this->_response += "Incorect parameters\r\n";
	else if (args[0][0] != '#')
		this->_response += "";
	else if (args[1] != "+i" && args[1] != "-i")
		this->_response += "Invalid parameters\r\n";
	else
	{
		if (!verify_user_op(channels, args[0], this->_fd))
		{
			this->_response += "User is not in the channel/not op\r\n";
			return ;
		}
		if (args[1] == "+i")
			inv = true;
		else
			inv = false;
		if (!verify_channel_exist(channels, args[0], inv))
		{
			this->_response += ":ft_irc 403 no such channel\r\n";
			return ;
		}
		this->_response = ":ft_irc MODE " + args[0] + " " + args[1] + "\r\n";
	}
}
