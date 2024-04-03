/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:11:44 by smunio            #+#    #+#             */
/*   Updated: 2024/04/03 23:12:44 by smunio           ###   ########.fr       */
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
        return ;
    }
    else if (!strncmp(line.c_str(), "CAP END", strlen(line.c_str()))) {
		this->_status = REGISTRATION;
        return ;
	}
    void (User::*functptr[])(std::string) = {
        &User::store_pass,
        &User::store_nickname,
        &User::store_username
    };
    std::string str_tab[] = {"PASS", "NICK", "USER"};
    for (int i = 0; i < 3; i++)
    {
        if (str_tab[i] == line.substr(0, 4))
            (this->*functptr[i])(line);
    }
}

void User::store_pass(std::string line)
{
    this->_password = line.substr(5, strlen(line.c_str()) - 5);
	std::cout << "password: " << this->_password << std::endl;
}

void User::store_nickname(std::string line)
{
    this->_nickname = line.substr(5, strlen(line.c_str()) - 5);
        std::cout << "nickname: " << this->_nickname << std::endl;
}

void User::store_username(std::string line)
{
    char *tmp = strtok((char *)line.c_str(), " ");
        tmp = strtok(NULL, " ");
        this->_username = tmp; 
        std::cout <<"username: " << this->_username << std::endl;
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

int User::get_status() const
{
	return (this->get_status());
}

std::string User::get_nickname() const
{
    return (this->_nickname);
}
