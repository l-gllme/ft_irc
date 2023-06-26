#ifndef REPLY_HPP
# define REPLY_HPP

# define SERVER "ircserv"
# define CLIENT "Irssi"
# define VERSION "v0.1"
# define USERMODE "iow"
# define CHANMODE "o"

# include <iostream>
# include <string>
# include <cstdlib>

// Use a namespace to avoid name conflicts

// Errors replies
std::string err_nosuchnick(std::string const & source, std::string const & nickname);
std::string err_nosuchchannel(std::string const & source);
std::string err_unknowncommand(std::string const & source, std::string const & command);
std::string err_nonicknamegiven(std::string const & source);
std::string err_erroneusnickname(std::string const & source, std::string const & nickname);
std::string err_nicknameinuse(std::string const & source, std::string const & nickname);
std::string err_nickcollision(std::string const & source, std::string const & nickname);

std::string err_notonchannel(std::string const & source);

std::string err_noregistered(std::string const & source);

std::string err_needmoreparams(std::string const & source, std::string const & command);
std::string err_alreadyregistered(std::string const & source);
std::string err_passwdmismatch(std::string const & source);

// Numeric replies
//#define RPL_WELCOME(source)	"001 " source " :Welcome " source " to the ft_irc network\r\n"

std::string rpl_welcome(std::string const & nick, std::string const & user);
std::string rpl_yourhost(std::string const & source);
std::string rpl_created(std::string const & source);

// Command replies

#endif // REPLY_HPP
