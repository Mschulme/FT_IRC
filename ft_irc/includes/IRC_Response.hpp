#ifndef IRC_RESPONSE_HPP
#define IRC_RESPONSE_HPP

#define INVITE_USAGE                                    "Usage: </INVITE> <USER> #<CHANNEL>"
#define KICK_USAGE                                      "Usage: </KICK> $<CHANNEL> <NICK>"
#define PART_USAGE                                      "Usage: </PART> #<CHANNEL> <exitMessage>"
#define JOIN_USAGE                                      "Usage: </JOIN> #<CHANNEL> <PASSWORD>"
#define NO_AUTH                                         ", couldn't authenticate!"
#define TO_RUN                                          "To use program : ./ircserv <4-digit PORT> <4-8 CHAR LONG PASSWORD>"

#define ERR_KICKYOURSELF                                "Can't kick yourself out. There is PART command for this."
#define INFO_KICKEDOUT(channel)                         "You have been kicked out of #" + channel 

#define ERR_UNKNOWNMODE(source, modechar)               std::string(":") + SERVER_IP + " 472 " + source + modechar + " :is unknown mode char to me."

#define ERR_NOTREGISTERED(source)                       std::string(":") + SERVER_IP + " 451 " + source + " :You have not registered"
#define ERR_ALREADYREGISTERED(source)                   std::string(":") + SERVER_IP + " 462 " + source + " :Unauthorized command (already registered)"
#define ERR_PASSWDMISMATCH(source)                      std::string(":") + SERVER_IP + " 464 " + source + " :Password is incorrect"
#define ERR_NONICKNAMEGIVEN(source)                     std::string(":") + SERVER_IP + " 431 " + source + " :Nickname not given"
#define ERR_ERRONEUSNICKNAME(source)                    std::string(":") + SERVER_IP + " 432 " + source + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(source)                       std::string(":") + SERVER_IP + " 433 " + source + " " + source  + " :Nickname is already in use"



#define ERR_UNKNOWNCOMMAND(source, command)             std::string(":") + SERVER_IP + " 421 " + source + " " + command + " :Unknown command"
#define ERR_NEEDMOREPARAMS(source, command)             std::string(":") + SERVER_IP + " 461 " + source + " " + command + " :Not enough parameters"

#define ERR_TOOMANYCHANNELS(source, channel)            std::string(":") + SERVER_IP + " 405 " + source + " #" + channel + " :You have joined too many channels"
#define ERR_NOTONCHANNEL(source, channel)               std::string(":") + SERVER_IP + " 442 " + source + " #" + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(source, channel)              std::string(":") + SERVER_IP + " 403 " + source + " #" + channel + " :No such channel"
#define ERR_BADCHANNELKEY(source, channel)              std::string(":") + SERVER_IP + " 475 " + source + " #" + channel + " :Cannot join channel (+k)"
#define ERR_CHANNELISFULL(source, channel)              std::string(":") + SERVER_IP + " 471 " + source + " #" + channel + " :Cannot join channel (+l)"
#define ERR_CANNOTSENDTOCHAN(source, channel)           std::string(":") + SERVER_IP + " 404 " + source + " #" + channel + " :Cannot send to channel"
#define ERR_CHANOPRIVSNEEDED(source, channel)           std::string(":") + SERVER_IP + " 482 " + source + " #" + channel + " :You're not channel operator"


#define ERR_BADCHANMASK(source)                std::string(":") + SERVER_IP + " 476 " + source + " :Bad Channel Name"


#define ERR_NOSUCHNICK(source, nickname)                std::string(":") + SERVER_IP + " 401 " + source + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel) std::string(":") + SERVER_IP + " 441 " + source + " " + nickname + " #" + channel + " :They aren't on that channel"


#define RPL_WELCOME(source)                             std::string(":") + SERVER_IP + " 001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_NAMREPLY(source, channel, users)            std::string(":") + SERVER_IP + " 353 " + source + " = #" + channel + " :" + users
#define RPL_TOPIC(source, channel, topic)               std::string(":") + SERVER_IP + " 332 " + source + " = #" + channel + " :" + topic
#define RPL_ENDOFNAMES(source, channel)                 std::string(":") + SERVER_IP + " 366 " + source + " #" + channel + " :End of /NAMES list."


#define RPL_JOIN(source, channel)                       ":" + source + " JOIN #:" + channel
#define RPL_PART(source, channel)                       ":" + source + " PART #:" + channel
#define RPL_PING(source, token)                         ":" + source + " PONG :" + token
#define RPL_PRIVMSG(source, target, message)            ":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)             ":" + source + " NOTICE " + target + " :" + message
#define RPL_QUIT(source, message)                       ":" + source + " QUIT :Quit: " + message
#define RPL_KICK(source, channel, target, reason)       ":" + source + " KICK #" + channel + " " + target + " :" + reason
#define RPL_MODE(source, channel, modes, args)          ":" + source + " MODE #" + channel + " " + modes + " " + args


#endif
