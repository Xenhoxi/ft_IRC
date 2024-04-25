/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:06:25 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/25 10:23:28 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Channel::Channel(User *user, std::string name) : _name(name), _max_users(0), _need_pass(false), _topic_mode(TOPIC_ALL)
{
	_operators.push_back(user);
	add_user(user);
}

Channel::~Channel()
{
	return ;
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

bool    Channel::is_invited(std::string nick)
{
	std::list<std::string>::iterator it;

	for (it = _user_invited.begin(); it != _user_invited.end(); it++)
	{
		if ((*it) == nick)
			return (true);
	}
	return (false);
}
