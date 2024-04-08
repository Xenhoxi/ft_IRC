/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.class.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:11:44 by smunio            #+#    #+#             */
/*   Updated: 2024/04/08 14:13:20 by ljerinec         ###   ########.fr       */
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

void	User::parsing(Server &server)
{
	(void)server;
}

void    User::negotiation(Server &server)
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
		this->parse_negotiation(tmp, server);
		_data.erase(0, closest + 1);
	}
	_data.clear(); 
}

void    User::registration(Server &server)
{
	std::string msg;
	msg = "001 " + _nickname + " :Welcome to the diloragequit Network, " + _nickname + "\r\n";
	write(this->_fds->fd, msg.c_str(), msg.size());
	msg = "002 " + _nickname + " :Your host is ft_IRC, running version 1.0" + "\r\n";
	write(this->_fds->fd, msg.c_str(), msg.size());
	msg = "003 " + _nickname + " :This server was created " + server.get_dt() + "\r\n";
	write(this->_fds->fd, msg.c_str(), msg.size());
	msg = "004" + _nickname + " " + server.get_servername() + " version 1.0\r\n";
	write(this->_fds->fd, msg.c_str(), msg.size());
	msg = "005" + _nickname + " <tokens>: nothing is supported by this server, fck you\r\n";
	write(this->_fds->fd, msg.c_str(), msg.size());
	change_status(CONNECTED);
}

void    User::parse_negotiation(std::string line, Server &server)
{
	if ("CAP LS" == line)
		write(this->_fds->fd, "CAP * LS\n", 9);
	else if ("PASS" == line.substr(0, 4))
	{
		this->_password = line.substr(5, strlen(line.c_str()) - 5);
		std::cout << "pass: " << this->_password << std::endl;
	}
	else if ("NICK" == line.substr(0, 4))
	{
		this->_nickname = line.substr(5, strlen(line.c_str()) - 5);
		std::cout << "nick: " << this->_nickname << std::endl;
	}
	else if ("USER" == line.substr(0, 4))
	{
		char *tmp = strtok((char *)line.c_str(), " ");
		tmp = strtok(NULL, " ");
		this->_username = tmp;
		std::cout << "user: " << this->_username << std::endl;
	}
	else if (line == "CAP END")
		this->registration(server);
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

struct pollfd *User::get_fds() const
{
	return (this->_fds);
}

int User::get_status() const
{
	return (this->_status);
}
