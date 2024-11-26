

void IRC_Server::handle_MODE(std::vector<std::string>& message, int fd) {
    if (message.size() < 3) {
        clientList[fd].reply(ERR_NEEDMOREPARAMS(clientList[fd].getNickname(), "MODE"), fd);
        return;
    }

    std::string channelName = message[1];
    std::string modeString = message[2];
    std::string parameter = (message.size() > 3) ? message[3] : "";

    Channel& channel = channelList[channelName];

    bool add = true;
    for (std::string::size_type i = 0; i < modeString.size(); ++i) {
        switch (modeString[i]) {
            case '+':
                add = true;
                break;
            case '-':
                add = false;
                break;
            case 'i':
                handleInviteOnly(channel, add);
                break;
            case 't':
                handleTopicRestrict(channel, add);
                break;
            case 'k':
                handleChannelKey(channel, add, parameter);
                break;
            case 'o':
                handleOperatorPrivilege(channel, add, parameter);
                break;
            case 'l':
                handleUserLimit(channel, add, parameter);
                break;
            default:
                clientList[fd].reply(ERR_UNKNOWNMODE(clientList[fd].getNickname(), std::string(1, modeString[i])), fd);
                break;
        }
    }
}

void IRC_Server::handleInviteOnly(Channel& channel, bool add) {
    if (add) {
        channel.setInviteOnly(true);
    } else {
        channel.setInviteOnly(false);
    }
}

void IRC_Server::handleTopicRestrict(Channel& channel, bool add) {
    if (add) {
        channel.setTopicRestrict(true);
    } else {
        channel.setTopicRestrict(false);
    }
}

void IRC_Server::handleChannelKey(Channel& channel, bool add, const std::string& key) {
    if (add) {
        channel.setKey(key);
    } else {
        channel.removeKey();
    }
}

void IRC_Server::handleOperatorPrivilege(Channel& channel, bool add, const std::string& user) {
    if (add) {
        channel.addOperator(user);
    } else {
        channel.removeOperator(user);
    }
}

void IRC_Server::handleUserLimit(Channel& channel, bool add, const std::string& limit) {
    if (add) {
        channel.setUserLimit(std::atoi(limit.c_str()));
    } else {
        channel.removeUserLimit();
    }
}
