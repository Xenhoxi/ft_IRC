/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:06:25 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/08 20:20:02 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Channel::Channel(User *user, std::string name) : _name(name)
{
	_userInChannel.push_back(user);
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