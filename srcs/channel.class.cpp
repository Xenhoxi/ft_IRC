/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:06:25 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/17 11:53:34 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Channel::Channel(User *user, std::string name) : _name(name), _max_users(0)
{
    _operators.push_back(user);
    add_user(user);
    std::cout << "Channel created !" << std::endl;
}

Channel::~Channel()
{
    std::cout << "Channel " << _name << "deleted !" << std::endl;
	return ;
}

void    Channel::add_user(User *user)
{
    std::string msg = ":ft_irc 353 " + user->get_nick() + " = " + _name + " :";
    _userInChannel.push_back(user);
    send_to_all_user(":" + user->get_nick() + " JOIN " + _name + "\r\n");
    for (std::list<User *>::iterator it = _userInChannel.begin(); it != _userInChannel.end(); ++it)
    {
        if (is_operator((*it)->get_nick()))
            msg += "@";
        msg += (*it)->get_nick() + " ";
    }
    user->send_message(msg + "\r\n" + ":ft_irc 366 " + user->get_nick() + " " + _name + " :End of /NAMES list.\r\n");
    std::cout << user->get_nick() << " add to channel: " << _name << std::endl;
}

bool    Channel::is_operator(std::string nick) const
{
    std::list<User *>::const_iterator it;
    for (it = this->_operators.begin(); it != this->_operators.end(); it++)
    {
        if ((*it)->get_nick() == nick)
            return (true);
    }
    std::cout << "Don't have op privileges" << std::endl;
    return (false);
}

void Channel::kick(std::string &line, User &caller, Server &server)
{
    (void)server;
    // CLIENT NE RECOIE PAS MSG DISANT QUIL NEST PAS OP
    std::string reason = line.substr(line.find(':') + 1, line.size());

    char *tar = (char *)line.c_str();
    tar = strtok(NULL, " ");
    tar = strtok(NULL, " ");

    User    &target = this->get_user(tar);

    std::string msg = ":" + caller.get_nick() + " KICK " + this->_name + " " + tar + "\r\n";
    this->send_to_all_user(msg);
    
    std::list<User *>::iterator it;
    for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
    {
        if (*it == &target)
        {
            _userInChannel.erase(it);
            break ;
        }
    }
}

void Channel::invite(std::string &line, User &caller, Server &server)
{
    char *tar = (char *)line.c_str();
    tar = strtok(NULL, " "); 
    User    &target = server.get_user(tar);

    target.send_message(":" + caller.get_nick() + " INVITE " + tar + " " + this->_name + "\r\n");
}

void Channel::topic(std::string &line, User &caller, Server &server)
{
    (void)server;
    (void)line;
    (void)caller;

    std::cout << "topic called" << std::endl;
}

void Channel::mode(std::string &line, User &caller, Server &server)
{
    (void)server;
    if (line.find('-') == std::string::npos && line.find('+') == std::string::npos)
    {
        throw Error("no MODE opt");
        return ;
        //should display channel characteristics
    }
	std::string opt = line.substr(line.find("MODE") + 6 + this->_name.size(), 2);

	if (opt[1] != 'o' && opt[1] != 'i' && opt[1] != 'l' && opt[1] != 't' && opt[1] != 'k'
        && opt[0] != '+' && opt[0] != '-')
		throw Error("wrong MODE opt");
	if (opt[1] == 'o')
        this->mode_o(line, opt, caller);
    else if (opt[1] == 'l')
        this->mode_l(line, opt, caller);
}

void Channel::mode_o(std::string &line, std::string &opt, User &caller)
{
    (void)caller;
	std::string	tar = line.substr(line.find("MODE") + 9 + this->_name.size(), line.size());
    std::list<User *>::iterator it;

    if (opt[0] == '-')
        for (it = this->_operators.begin(); it != this->_operators.end(); it++)
        {
            if ((*it)->get_nick() == tar)
            {
                    this->_operators.erase(it);
                    this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + tar + "\r\n");
                    break ;
            }
        }
    else
        for (it = this->_userInChannel.begin(); it != this->_userInChannel.end(); it++)
        {
            if ((*it)->get_nick() == tar)
            {
                    this->_operators.push_back(*it);
                    this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + tar + "\r\n");
                    break ;
            }
        }
}

void    Channel::mode_l(std::string &line, std::string &opt, User &caller)
{
    (void)caller;
    if (opt[0] == '+')
    {
        std::string count = line.substr(line.find(opt) + 3, line.size());
        this->_max_users += atoi(count.c_str());
        this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + count + "\r\n");
    }
    if (opt[0] == '-')
    {
        this->_max_users = 0;
        this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + "\r\n");
    }
}

User &Channel::get_user(std::string nick)
{
    std::list<User *>::iterator it;
    for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
        if ((*it)->get_nick() == nick)
            return (*(*it));
    }
    throw Error("can't find target in user list");
}

void    Channel::disconnect(User *leaving_user, std::string ch_name)
{
    std::list<User *>::iterator it;
    for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *user = *it;
		if (leaving_user == user)
		{
			send_to_all_user(":" + leaving_user->get_nick() + " PART " + ch_name + " " + leaving_user->get_nick() + "\r\n");
            _userInChannel.erase(it);
			std::cout << "User leave the channel: " << ch_name << std::endl;
            break ;
		}
	}
}

void    Channel::send_to_others(std::string msg, User *send_user)
{
	std::list<User *>::iterator it;

	for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *user_receiver = *it;
		if (send_user != user_receiver)
			user_receiver->send_message(msg);
	}
}

void    Channel::send_to_all_user(std::string msg)
{
	std::list<User *>::iterator it;

	for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *user_receiver = *it;
        user_receiver->send_message(msg);
	}
}

bool    Channel::is_connected(User *user)
{
    std::list<User *>::iterator it;

    for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *channel_user = *it;
		if (user == channel_user)
			return (true);
	}
    return (false);
}

size_t Channel::get_size(void)
{
    return (_userInChannel.size());
}

size_t Channel::get_max_user(void)
{
    return (_max_users);
}
