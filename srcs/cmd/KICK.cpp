#include "Client.hpp"

/* funtion to verify if channel exist already */
static bool	f_t_v_i_c_e_a(std::list<Channel> &channels, std::string s)
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

/* function to verify if the user is already in the channel given as a parameter */
static bool	f_t_v_i_t_u_i_a_i_t_c_g_a_a_p(int fd, std::list<Channel> &channels, std::string s)
{
	std::list<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		if (s == it->get_name())
		{
			std::map<int, t_user> users = it->get_users();
			std::map<int, t_user>::iterator it = users.begin();

			while (it != users.end())
			{
				if (it->first == fd)
					return (true);
				it++;
			}
			return (false);
		}
		it++;
	}
	return (false);
}

static bool	user_in_channel(std::string nick, std::list<Channel> &channels, std::string s)
{
	std::list<Channel>::iterator it = channels.begin();
	std::map<int, t_user>	users;

	while (it != channels.end())
	{
		if (s == it->get_name())
		{
			users = it->get_users();
			for (std::map<int, t_user>::iterator itt = users.begin(); itt != users.end(); ++itt)
			{
				if (nick == itt->second.nick)
					return (true);
			}
			return (false);
		}
		it++;
	}
	return (false);
}

#include <sys/types.h>
#include <sys/socket.h>

/* function to remove the user in the right channel given in the parameters */
static void	f_t_r_t_u_i_t_r_c_g_i_t_p(std::string nick, std::list<Channel> &channels, std::string s, std::string &_response)
{
	std::list<Channel>::iterator it = channels.begin();
	std::map<int, t_user>	users;

	while (it != channels.end())
	{
		if (s == it->get_name())
		{
			users = it->get_users();
			for (std::map<int, t_user>::iterator itt = users.begin(); itt != users.end(); ++itt)
			{
				if (nick == itt->second.nick)
				{
					int privatefd = itt->first;
					if (privatefd != -1)
						send(privatefd, _response.c_str(), _response.size(), 0);
					it->remove_user(itt->first);
					return ;
				}
			}
			return ;
		}
		it++;
	}
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

std::string err_chanoprivsneeded(std::string const & nick, std::string const & source) { return ("482 " + nick + " " + source + " :You're not channel operator\r\n");}
std::string err_usernotinchannel(std::string const & source, std::string const & nick, std::string const & channel) { return ("441 " + source + " " + nick + " " + channel + " :They aren't on that channel\r\n");}

//KICK <channel> <user> :REASON
//response
//:<operatornick>!<username>@localhost KICK <channel> <user> :REASON

void	Client::kick(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels, std::list<std::string> &nicks)
{
	if (args.size() < 2 || args[0].empty())
		this->_response += err_needmoreparams(CLIENT, "KICK");
	else if (!f_t_v_i_c_e_a(channels, args[0]))
		this->_response += err_nosuchchannel(args[0]);
	else if (!nick_exist(nicks, args[1]))
		this->_response += err_nosuchnick(this->_nick, args[1]);
	else if (!verify_user_op(channels, args[0], this->_fd))
		this->_response += err_chanoprivsneeded(this->_nick, args[0]);
	else if (!f_t_v_i_t_u_i_a_i_t_c_g_a_a_p(this->_fd, channels, args[0]))
		this->_response += err_notonchannel(args[0]);
	else if (!user_in_channel(args[1], channels, args[0]))
		this->_response += err_usernotinchannel(this->_nick, args[1], args[0]);
	else
	{
		this->_response += ":" + _nick + "!~" + _username + "@localhost KICK " + args[0] + " " + args[1] + " ";
		this->_response.append(args[2].begin()+1, args[2].end());
		for (size_t i = 3; i < args.size(); i++)
		{
			this->_response += " ";
			this->_response += args[i];
		}
		this->_response += "\r\n";
		f_t_r_t_u_i_t_r_c_g_i_t_p(args[1], channels, args[0], this->_response);
		res.push_back(args[0]);
	}
}
