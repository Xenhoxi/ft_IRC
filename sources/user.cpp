/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:11:44 by smunio            #+#    #+#             */
/*   Updated: 2024/04/02 17:26:53 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

User::User() 
{
    this->fds = new struct pollfd[1];
    return ;
}

User::User() 
{
    return ;
}

void    User::set_fds(int server_socket)
{
    this->fds->fd = server_socket;
    this->fds->events = POLLIN | POLLOUT;
    return ;
}

void    User::set_nickname(std::string name)
{
    this->nickname = name;
    return ;
}
