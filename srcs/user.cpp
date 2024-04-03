/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:11:44 by smunio            #+#    #+#             */
/*   Updated: 2024/04/03 21:28:55 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

User::User() : _status(DISCONNECTED)
{
    this->_fds = new struct pollfd[1];
    return ;
}

User::~User() 
{
    return ;
}

void    User::parse_registration(std::string line)
{
    if (!strncmp(line.c_str(), "CAP LS", strlen(line.c_str()))) {
        write(this->_fds->fd, "CAP * LS\n", 9);
    }
    else if (!strncmp(line.c_str(), "NICK", strlen(line.c_str()))) {
        this->_nickname = line.substr(5, strlen(line.c_str()) - 5);
        std::cout << "nickname: " << this->_nickname << std::endl;
    }
    // USER username hostname servername :Real Name
    else if (!strncmp(line.c_str(), "USER", strlen(line.c_str()))) {
        char *tmp = strtok((char *)line.c_str(), " ");
        tmp = strtok(NULL, " ");
        this->_username = tmp; 
        std::cout <<"username: " << this->_username << std::endl;
    }
    else if (!strncmp(line.c_str(), "PASS", strlen(line.c_str()))) {
		this->_password = line.substr(5, strlen(line.c_str()) - 5);
		std::cout << "password: " << this->_password << std::endl;
	}
    else if (!strncmp(line.c_str(), "CAP END", strlen(line.c_str()))) {
		this->_status = REGISTRATION;
	}
	else
		write(this->_fds->fd, line.c_str(), strlen(line.c_str()));
}

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

unsigned int User::get_status() const
{
	return (this->get_status());
}

std::string User::get_nickname() const
{
    return (this->_nickname);
}
