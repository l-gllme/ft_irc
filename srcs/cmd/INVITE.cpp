#include <Client.hpp>

static bool	verify_channel_exist(std::list<Channel> &channels, std::string &s)
{
	std::list<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		if (s == it->get_name())
			return (true);
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

static void	add_user_to_invited_list(std::list<Channel> &channels, std::string user, std::string &s)
{
	std::list<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		if (s == it->get_name())
		{
			it->push_user_invited(user);
			break ;
		}
		it++;
	}
}

/* INVITE <nickname> <channel> */
/* :<nickname>~!<username>@<host> INVITE <nickname> <channel> */
void	Client::invite(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels)
{
	if (args.size() < 2 || args[0].empty() || args[1].empty())
		this->_response += err_needmoreparams(this->_nick, "INVITE");
	else if (args[1][0] != '#' || args.size() != 2)
		this->_response += "Invalid parameters\r\n";
	else
	{
		if (!verify_channel_exist(channels, args[1]))
		{
			this->_response += "403 no such channel\r\n";
			return ;
		}
		if (!verify_user_op(channels, args[1], this->_fd))
		{
			this->_response += "User is not in the channel/not op\r\n";
			return ;
		}
		this->_response += ":" + _nick + "!" + _username + "@localhost INVITE " + args[0] + " " + args[1] + "\r\n";
		res.push_back(args[0]);
		add_user_to_invited_list(channels, args[0], args[1]);
	}
}
