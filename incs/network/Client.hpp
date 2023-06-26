#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <list>
# include <vector>
# include <cstring>
# include "User.hpp"
# include "Channel.hpp"
# include "reply.hpp"

// Class
class Client
{
	public:
		Client(void);
		Client(int const fd, std::string const &pass);
		~Client(void);

		// void	set_username(std::string const &username);
		std::string const &get_nick(void) const;
		std::string const &get_username(void) const;
		std::string const &get_realname(void) const;
		int const & get_fd(void) const;
		void set_op(bool a);
		bool get_op(void);

		/* Auth proccess */
		//std::string	get_username(void);

		/* Command */
		void	parse_cmd(std::string line);
		std::vector<std::string> get_response(std::list<std::string> &nicks, std::list<Channel> &channels);
		bool	nick_exist(std::list<std::string> &nicks, std::string nick);
		bool	channel_exist(std::list<Channel> &channels, std::string channel);

		void	pass(std::vector<std::string> &args);
		void	nick(std::vector<std::string> &args, std::list<std::string> &nicks);
		void	user(std::vector<std::string> &args);
		void	privmsg(std::vector<std::string> &args, std::vector<std::string> &res, std::list<std::string> &nicks, std::list<Channel> &channels);
		void	notice(std::vector<std::string> &args, std::vector<std::string> &res, std::list<std::string> &nicks, std::list<Channel> &channels);
		void	join(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels);
		void	part(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels);
		void	kick(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels, std::list<std::string> &nicks);
		void	invite(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels);
		void	mode(std::vector<std::string> &res, std::vector<std::string> &args, std::list<Channel> &channels);
		void	quit(std::vector<std::string> &args);

	private:
		bool 		_op;
		int 		_fd;
		std::string	_pass;
		std::string	_username;
		std::string	_realname;
		std::string	_nick;

		/* Auth proccess */
		bool	_authorized;
		bool	_bnick;
		bool	_busername;
		bool	_authenticated;

		/* Command */
		std::vector<std::string>	_line;
		std::string			_response;

		std::string			get_cmd(size_t i);
		std::vector<std::string>	get_args(size_t i, std::string delim);
		void	split(std::string package, std::string delim);


};

#endif // CLIENT_HPP
