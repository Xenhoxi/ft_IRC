/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:06:25 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/08 14:14:31 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Channel::Channel(User *user, std::string name) : _name(name)
{
    _UserInChannel.push_back(user);
    std::cout << "Create Channel and add user to it: " << _name << std::endl;
}

Channel::~Channel()
{
    return ;
}

void    Channel::add_user(User *user)
{
    _UserInChannel.push_back(user);
    std::cout << "User add to channel: " << _name << std::endl;
}