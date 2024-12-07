#include "IRC_Server.hpp"

void IRC_Server::handle_KICK(int fd, std::vector<std::string> message) {
    if (message.size() != 3) {
        sendClientMessage(KICK_USAGE, fd);
        return;
    }

    std::string targetNickname = message[2];
    std::string channelName = message[1].substr(1);
    std::string membersList;
    std::string response;

    for (std::vector<IRC_Channel>::iterator channelIt = channelList.begin(); channelIt != channelList.end(); ++channelIt) {
        if (channelIt->getName() == channelName) {
            bool is_operator = false;
            std::vector<IRC_Client> operators = channelIt->getOperators();
            std::string clientNickname = clientList[fd].getNickname();

            for (size_t i = 0; i < operators.size(); ++i) {
                if (clientNickname == operators[i].getNickname()) {
                    is_operator = true;
                    break;
                }
            }

            if (!is_operator) {
                sendClientMessage(ERR_CHANOPRIVSNEEDED(clientList[fd].getNickname(), channelName) , fd);
                return;
            }

            if (targetNickname == clientNickname) {
                sendClientMessage(ERR_KICKYOURSELF, fd);
                return;
            }

            if (!channelIt->isMember(targetNickname)) {
                sendClientMessage(ERR_NOSUCHNICK(clientList[fd].getNickname(), targetNickname), fd);
                return;
            }

            IRC_Client* targetClient = channelIt->findClient(targetNickname);
            if (targetClient) {
                sendClientMessage(INFO_KICKEDOUT(channelName), targetClient->getFd());
                channelIt->removeMember(targetNickname);

                std::vector<IRC_Client> members = channelIt->getMembers();
                membersList.clear();
                for (size_t i = 0; i < members.size(); ++i) {
                    bool isMember = false;
                    for (size_t j = 0; j < operators.size(); ++j) {
                        if (operators[j].getNickname() == members[i].getNickname()) {
                            isMember = true;
                            break;
                        }
                    }
                    if (isMember) {
                        membersList += "@";
                    }
                    membersList += members[i].getNickname() + " ";
                }

                for (size_t i = 0; i < members.size(); ++i) {
					sendClientMessage(RPL_NAMREPLY(members[i].getNickname(), channelName, membersList), members[i].getFd());
					sendClientMessage(RPL_ENDOFNAMES(members[i].getNickname(), channelName), members[i].getFd());
                }

                sendClientMessage("You kicked " + targetNickname + " out of #" + channelIt->getName(), fd);
                return;
            }
        }
    }
     sendClientMessage(KICK_USAGE, fd);
}
