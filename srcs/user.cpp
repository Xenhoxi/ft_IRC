/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:11:44 by smunio            #+#    #+#             */
/*   Updated: 2024/04/03 17:46:16 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

User::User() : _status(0)
{
    this->_fds = new struct pollfd[1];
    return ;
}

User::~User() 
{
    return ;
}

// void    User::parse_registration(std::string line)
// {
//     if (strncmp(line, "CAP LS"))
// }

void    User::change_status(int status)
{
    if (this->_status == 0 && status == 2)
        throw Error("can't skip registration");
    if (this->_status == 2 && status == 1)
        throw Error("user is already registered");
    this->_status = status;
}

void    User::set_fds(int server_socket)
{
    if (server_socket < 0)
    {
        throw Error("failed to accept");
    }
    this->_fds->fd = server_socket;
    this->_fds->events = POLLIN | POLLOUT;
    return ;
}

void    User::set_nickname(std::string name)
{
    this->_nickname = name;
    return ;
}

struct pollfd *User::get_fds() const
{
    return (this->_fds);
}

std::string User::get_nickname() const
{
    return (this->_nickname);
}
