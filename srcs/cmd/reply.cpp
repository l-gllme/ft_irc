#include "reply.hpp"

// Errors replies
std::string err_nosuchnick(std::string const & source, std::string const & nickname) { return ("401 " + source + " " + nickname + " :No such nick/channel\r\n");}
std::string err_nosuchchannel(std::string const & source) { return ("403 " + source + " :No such channel\r\n");}
std::string err_unknowncommand(std::string const & source, std::string const & command) { return ("421 " + source + "" + command + " :Unknown command\r\n");}
std::string err_nonicknamegiven(std::string const & source) { return ("431 " + source + " :No nickname given\r\n");}
std::string err_erroneusnickname(std::string const & source, std::string const & nickname) { return ("432 " + source + " " + nickname + " :Erroneus nickname\r\n");}
std::string err_nicknameinuse(std::string const & source, std::string const & nickname) { return ("433 " + source + " " + nickname + " :Nickname is already in use\r\n");}
std::string err_nickcollision(std::string const & source, std::string const & nickname) { return ("436 " + source + " " + nickname + " :Nickname collision KILL\r\n");}

std::string err_notonchannel(std::string const & source) { return ("442 " + source + " :You're not on that channel\r\n");}

std::string err_noregistered(std::string const & source) { return ("451 " + source + " :Register first\r\n");}

std::string err_needmoreparams(std::string const & source, std::string const & command) { return ("461 " + source + " " + command + " :Not enough parameters\r\n");}
std::string err_alreadyregistered(std::string const & source) { return ("462 " + source + " :Unauthorized command (already registered)\r\n");}
std::string err_passwdmismatch(std::string const & source) { return ("464 " + source + " :Password incorrect\r\n");}

// Numeric replies
//#define RPL_WELCOME(source)	"001 " source " :Welcome " source " to the ft_irc network\r\n"

std::string rpl_welcome(std::string const & nick, std::string const & user) { return ("001 " + nick + " :Welcome to the ft_irc network " + nick + "!" + user + "@localhost \r\n");}
std::string rpl_yourhost(std::string const & source) { return ("002 " + source + " :Your host is " + source + ", running version " + VERSION + "\r\n");}
std::string rpl_created(std::string const & source) { return ("003 " + source + " :This server was created " + __DATE__ + "\r\n");}
