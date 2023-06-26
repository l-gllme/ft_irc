#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <map>

typedef struct	s_user
{
	std::string	nick;
	bool		op;
	bool		invited;
}				t_user;

class Channel
{
	public:
		Channel(void);
		Channel(std::string const &name);
		~Channel(void);

		std::string const &get_name(void) const;
		std::map<int, t_user> const &get_users(void) const;
		std::string	get_user_list(void);
		void add_user(int const &fd, std::string const &nick, bool const &op = false, bool const &invited = false);
		void remove_user(int const &fd_user);
		bool chan_empty(void);
		bool is_in_channel(int const &fd_user);
		void op(int const &fd_user);
		void set_invite_only(bool a);
		bool get_invite_only(void);
		std::list<std::string> &get_invited_list(void);
		void	push_user_invited(std::string name);

		bool operator==(Channel const &rhs);

	private:
		std::string _name;
		std::map<int, t_user> _users;
		bool	_invite_only;
		std::list<std::string>	_invited;
};

#endif // CHANNEL_HPP
