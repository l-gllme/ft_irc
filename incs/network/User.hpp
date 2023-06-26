#ifndef USER_HPP
# define USER_HPP

# include <string>

class User
{
	public:
		User(std::string nickname, std::string username);
		User(const User & src);
		~User(void);

		User	&operator=(const User & src);

		std::string	getNickname(void) const;
		std::string	getUsername(void) const;

		void		setNickname(std::string str);
		void		setUsername(std::string str);

		bool		isOperator(void) const;
		void		setOperator(bool op);
	
	private:

		std::string	_nickname;
		std::string	_username;

		bool		_operator;
};

#endif
