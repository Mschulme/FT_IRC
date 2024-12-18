#include <string>
#include <stdexcept>


bool    isPortNumeric(std::string port)
{
    for (size_t i = 0; i < port.length(); i++)
        if (!isdigit(port[i]))
            return (false);
    return (true);
}


bool    getPortLength(std::string port)
{
    if (port.length() != 4)
        return (false);
    return (true);
}


bool    getPasswordLength(std::string password)
{
    if (password.length() < 4 || password.length() > 8)
        return (false);
    return (true);
}


void    validateInput(std::string port, std::string password)
{
    if (!isPortNumeric(port))
        throw (std::runtime_error("Port argument should only contain digits!"));
    if (!getPortLength(port))
        throw (std::runtime_error("Port argument should contain 4 digits!"));
    if (!getPasswordLength(password))
        throw (std::runtime_error("Password argument should be between 4 and 8 characters long!"));
}
