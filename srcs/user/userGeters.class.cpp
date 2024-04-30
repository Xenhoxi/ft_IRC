/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userGeters.class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:22:54 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/30 22:54:18 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

bool	User::is_nick_used(Server &server)
{
	for (std::list<User *>::iterator it = server.get_usr_list().begin(); it != server.get_usr_list().end(); it++)
	{
		if (this != (*it) && this->get_nick() == (*it)->get_nick())
			return (true);
	}
	return (false);
}

int	User::nick_isalnum(void)
{
	for (int i = 0; _nickname[i]; i++)
	{
		if (!std::isalnum(_nickname[i]))
			return (0);
	}
	return (1);
}

std::string 	User::get_nick()
{
	return (_nickname);
}

void	User::add_data(std::string new_data)
{
	_data += new_data;
}

struct pollfd *User::get_fds() const
{
	return (this->_fds);
}

int User::get_status() const
{
	return (this->_status);
}

void    User::set_fds(int server_socket)
{
	this->_fds->fd = server_socket;
	this->_fds->events = POLLIN | POLLOUT;
	return ;
}