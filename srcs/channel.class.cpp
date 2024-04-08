/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:06:25 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/09 00:20:39 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Channel::Channel(User *user, std::string name) : _name(name)
{
    _userInChannel.push_back(user);
    _operators.push_back(user);
	std::cout << "Create Channel and add user to it: " << _name << std::endl;
}

Channel::~Channel()
{
	return ;
}

void    Channel::add_user(User *user)
{
    _userInChannel.push_back(user);
    std::cout << "User add to channel: " << _name << std::endl;
}

bool    Channel::is_operator(User &user) const
{
    std::list<User *>::const_iterator it;
    for (it = this->_operators.begin(); it != this->_operators.end(); it++)
    {
        std::cout << (*it)->get_nick() << std::endl;
        User    *op_user = *it;
        if (op_user == (&user))
            return (true);
    }
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

