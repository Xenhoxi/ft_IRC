/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:11:44 by smunio            #+#    #+#             */
/*   Updated: 2024/04/04 16:01:44 by ljerinec         ###   ########.fr       */
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

void    User::parsing(void)
{
	std::vector<std::string> parsed;
	std::string tmp;
	size_t closest;
	
	while (_data.size() && (_data.find('\r') != _data.npos || _data.find('\n') != _data.npos))
	{
		if (_data.find('\r') < _data.find('\n'))
			closest = _data.find('\r');
		else
			closest = _data.find('\n');
		tmp = _data.substr(0, closest);
		this->parse_registration(tmp);
		_data.erase(0, closest + 1);
	}
	_data.clear();
}

void    User::negotiation(void)
{

}

void    User::registration(void)
{
	
}

void    User::parse_registration(std::string line)
{
	if (!strncmp(line.c_str(), "CAP LS", strlen(line.c_str())))
		write(this->_fds->fd, "CAP * LS\n", 9);
	else if ("PASS" == line.substr(0, 4))
		this->_password = line.substr(5, strlen(line.c_str()) - 5);
	else if ("NICK" == line.substr(0, 4))
		this->_nickname = line.substr(5, strlen(line.c_str()) - 5);
	else if ("USER" == line.substr(0, 4))
	{
		char *tmp = strtok((char *)line.c_str(), " ");
		tmp = strtok(NULL, " ");
		this->_username = tmp;
	}
	else if (!strncmp(line.c_str(), "CAP END", strlen(line.c_str())))
		this->_status = REGISTRATION;
}

void User::store_pass(std::string line)
{
	this->_password = line.substr(5, strlen(line.c_str()) - 5);
}

void User::store_nickname(std::string line)
{
	this->_nickname = line.substr(5, strlen(line.c_str()) - 5);
}

void User::store_username(std::string line)
{
	char *tmp = strtok((char *)line.c_str(), " ");
		tmp = strtok(NULL, " ");
		this->_username = tmp; 
}

void    User::change_status(int status)
{
	if (this->_status == 0 && status == 2)
		throw Error("can't skip registration");
	if (this->_status == 2 && status == 1)
		throw Error("user is already registered");
	this->_status = status;
	std::cout << "Status change of " << this->get_fds()->fd << " to " << _status << std::endl;
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
	return (this->_status);
	return (this->_status);
}

std::string User::get_nickname() const
{
	return (this->_nickname);
}
