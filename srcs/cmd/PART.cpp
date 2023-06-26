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

#include <sstream>

void	Client::part(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels)
{
	if (args.size() == 0 || args[0].empty())
		this->_response = err_needmoreparams(CLIENT, "PART");
	else
	{
		if (!f_t_v_i_c_e_a(channels, args[0]))
		{
			this->_response += err_nosuchchannel(args[0]);
			return ;
		}
		else if (!f_t_v_i_t_u_i_a_i_t_c_g_a_a_p(this->_fd, channels, args[0]))
		{
			this->_response += err_notonchannel(args[0]);
			return ;
		}
		this->_response += ":" + _nick + "!~" + _username + "@localhost PART " + args[0] + " ";
		if (args.size() > 1)
		{
			this->_response.append(args[1].begin()+1, args[1].end());
			for (size_t i = 2; i < args.size(); i++)
			{
				this->_response += " ";
				this->_response += args[i];
			}
		}
		this->_response += "\r\n";
		res.push_back(args[0]);
		std::stringstream str;
		str << _fd;
		std::string aaa = str.str();
		res.push_back(aaa);
	}
}
