/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:06:25 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/10 13:55:04 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Channel::Channel(User *user, std::string name) : _name(name)
{
    add_user(user);
    _operators.push_back(user);
    std::cout << "Create channel " << _name << " and add " << user->get_nick() << std::endl;
}

Channel::~Channel()
{
    std::cout << "Channel " << _name << "deleted !" << std::endl;
	return ;
}

void    Channel::add_user(User *user)
{
    _userInChannel.push_back(user);
    send_to_all_user(":" + user->get_nick() + " JOIN " + _name + "\r\n");
    for (std::list<User *>::iterator it = _userInChannel.begin(); it != _userInChannel.end(); it++)
        user->send_message(user->get_nick() + " = " + _name + ":" + (*it)->get_nick()  + "\r\n");
     user->send_message(user->get_nick() + " " + _name + " :End of /NAMES list\r\n");
    std::cout << user->get_nick() << " add to channel: " << _name << std::endl;
}

bool    Channel::is_operator(User &user) const
{
    std::list<User *>::const_iterator it;
    for (it = this->_operators.begin(); it != this->_operators.end(); it++)
    {
        std::cout << "Checking if "<< (*it)->get_nick() << " is operator" << std::endl;
        User    *op_user = *it;
        if (op_user == (&user))
            return (true);
    }
    std::cout << "Don't have op privileges" << std::endl;
    return (false);
}

void Channel::kick(std::string &line, User &caller)
{
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

void Channel::invite(std::string &line, User &caller)
{
    (void)line;
        (void)caller;

    std::cout << "inviting ..." << std::endl;
}

void Channel::topic(std::string &line, User &caller)
{
    (void)line;
        (void)caller;

    std::cout << "topic called" << std::endl;
}

void Channel::mode(std::string &line, User &caller)
{
    (void)line;
        (void)caller;

    std::cout << "mode called" << std::endl;
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