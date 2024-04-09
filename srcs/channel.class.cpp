/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:06:25 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/09 15:18:30 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Channel::Channel(User *user, std::string name) : _name(name)
{
    _userInChannel.push_back(user);
    _operators.push_back(user);
    user->send_message(":" + user->get_nick() + " PRIVMSG " + name + " :has joined the channel\r\n");
	std::cout << "Create Channel and add user to it: " << _name << std::endl;
}

Channel::~Channel()
{
	return ;
}

void    Channel::add_user(User *user)
{
    _userInChannel.push_back(user);
    user->send_message(":" + user->get_nick() + " PRIVMSG " + this->_name + " :has joined the channel\r\n");
    std::cout << "User add to channel: " << _name << std::endl;
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

void Channel::kick(std::string line)
{
    (void)line;
    std::cout << "kicking ..." << std::endl;
}

void Channel::invite(std::string line)
{
    (void)line;
    std::cout << "inviting ..." << std::endl;
}

void Channel::topic(std::string line)
{
    (void)line;
    std::cout << "topic called" << std::endl;
}

void Channel::mode(std::string line)
{
    (void)line;
    std::cout << "mode called" << std::endl;
}

void    Channel::disconnect(User *leaving_user, std::string ch_name)
{
    std::list<User *>::iterator it;
    for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *user = *it;
		if (leaving_user == user)
		{
			leaving_user->send_message("PART\r\n");
            _userInChannel.erase(it);
			std::cout << "User leave the channel: " << ch_name << std::endl;
            break ;
		}
	}
}

void    Channel::send_to_all_user(std::string msg, User *send_user, std::string ch_name)
{
	std::list<User *>::iterator it;

	for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *user_receiver = *it;
		if (send_user != user_receiver)
		{
			user_receiver->send_message(":" + send_user->get_nick() + " PRIVMSG " + ch_name + " :" + msg + "\r\n");
			std::cout << "send to all user" << std::endl;
		}
	}
}
