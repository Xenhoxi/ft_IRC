/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelGeters.class.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:08:09 by smunio            #+#    #+#             */
/*   Updated: 2024/04/25 10:14:40 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

std::string Channel::get_password() const
{
	return (_password);
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

size_t Channel::get_size(void)
{
	return (_userInChannel.size());
}

size_t Channel::get_max_user(void)
{
	return (_max_users);
}

unsigned int  Channel::get_topic_mode(void)
{
	return (_topic_mode);
}

bool	Channel::get_pass_bool(void) const
{
	return (_need_pass);
}

unsigned int  Channel::get_invite_mode(void)
{
	return (_invite_mode);
}
