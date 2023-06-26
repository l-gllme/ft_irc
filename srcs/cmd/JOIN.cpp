#include "Client.hpp"

static std::vector<std::string>	get_channels(std::string arg, std::string delim)
{
	std::vector<std::string>	vec;
	char *tmp;

	tmp = strtok((char *)arg.c_str(), delim.c_str());
	while (tmp)
	{
		vec.push_back(std::string(tmp));
		tmp = strtok(NULL, delim.c_str());
	}
	return (vec);
}

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

/* function to add the user in the right channel given in the parameters */
static void	f_t_a_t_u_i_t_r_c_g_i_t_p(std::string nick, int fd, std::list<Channel> &channels, std::string s, bool const &op)
{
	std::list<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		if (s == it->get_name())
		{
			it->add_user(fd, nick, op);
			return ;
		}
		it++;
	}
}

static std::string	diplay_channels_users(std::list<Channel> &channels, std::string s)
{
	std::list<Channel>::iterator it = channels.begin();
	std::string res;

	while (it != channels.end())
	{
		if (s == it->get_name())
		{
			std::map<int, t_user> users = it->get_users();
			std::map<int, t_user>::iterator it = users.begin();

			while (it != users.end())
			{
				res += it->second.nick;
				res += " ";
				it++;
			}
		}
		it++;
	}
	res += "\r\n";
	return (res);
}

static bool	is_invited(std::list<Channel> &channels, std::string nick, std::string &s)
{
	std::list<Channel>::iterator it = channels.begin();

	while (it != channels.end())
	{
		if (s == it->get_name())
		{
			if (!it->get_invite_only())
				return (true);
			std::list<std::string> inv = it->get_invited_list();
			for (std::list<std::string>::iterator ite = inv.begin(); ite != inv.end(); ++ite)
			{
				if (nick == *ite)
					return (true);
			}
		}
		it++;
	}
	return (false);
}

/* JOIN <channel> */
/* :<nickname>!<username>@<host> JOIN :<channel> */
void	Client::join(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels)
{
	std::vector<std::string> 	chans;
	std::string			tmp;
	bool				new_chan = false;

	if (args.size() == 0 || args[0].empty())
		this->_response += err_needmoreparams(CLIENT, "JOIN");
	else
	{
		chans = get_channels(args[0], ",");
		for (std::vector<std::string>::iterator it = chans.begin() ; it != chans.end(); it++)
		{
			if (it->at(0) != '#' && !(it->size() == 1 && it->at(0) == '!'))
			{
				tmp = "#";
				tmp += *it;
				*it = tmp;
			}
			if (!f_t_v_i_c_e_a(channels, *it))
			{
				channels.push_back(*it);
				new_chan = true;
			}
			else if (f_t_v_i_t_u_i_a_i_t_c_g_a_a_p(this->_fd, channels, *it))
			{
				this->_response += "User already in the channel\r\n";
				continue;
			}
			else if (!is_invited(channels, _nick, *it))
			{
				this->_response +=":ft_irc 473 " +  _nick + " " + *it + " :Cannot join channel (+i)\r\n";
				continue;
			}
			f_t_a_t_u_i_t_r_c_g_i_t_p(_nick, _fd, channels, *it, new_chan);
			this->_response += ":" + _nick + "!" + _username + "@localhost JOIN :" + *it + "\r\n";
			this->_response += "Users in the channel: ";
			this->_response += diplay_channels_users(channels, *it);
			res.push_back(*it);
		}	
	}
}
