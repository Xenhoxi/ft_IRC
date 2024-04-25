/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverGeters.class.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:11:10 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/25 10:22:04 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

User &Server::get_user(std::string nick)
{
    std::list<User *>::iterator it;
    for (it = _usr_list.begin(); it != _usr_list.end(); it++)
	{
        if ((*it)->get_nick() == nick)
            return (*(*it));
    }
    throw Error("can't find target in user list");
}

bool	Server::is_pass(std::string password)
{
	if (password == _password)
		return (true);
	return (false);
}

std::list<User *> &Server::get_usr_list()
{
    return (_usr_list);
}

char	*Server::get_dt() const
{
	return (this->_datetime);
}

std::string	Server::get_servername() const
{
	return (this->_server_name);
}

int		Server::get_nb_client()
{
	return (_nb_client);
}
