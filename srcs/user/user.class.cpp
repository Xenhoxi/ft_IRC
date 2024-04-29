/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.class.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:11:44 by smunio            #+#    #+#             */
/*   Updated: 2024/04/29 17:29:07 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

User::User() : _status(NEGOTIATION)
{
	this->_fds = new struct pollfd[1];
	_last_pong = time(0);
	return ;
}

User::~User() 
{
	return ;
}

void	User::parse_command(std::string line, Server &server)
{
	if ("JOIN" == line.substr(0, 4))
		server.join_channel(this, line);
	else if ("PRIVMSG" == line.substr(0, 7))
		server.broadcast(this, line);
	else if ("PING" == line.substr(0, 4))
		send_message("PONG diloragequit " + line.erase(0, 5) + "\r\n");
	else if ("PONG" == line.substr(0, 4))
		_last_pong = time(0);
	else if ("KICK" == line.substr(0, 4) || "INVITE" == line.substr(0, 6)
		|| "TOPIC" == line.substr(0, 5) || "MODE" == line.substr(0, 4))
		server.call_op_cmd(line, *this);
	else if ("PART" == line.substr(0, 4))
		server.channel_part(line, this);
	else if ("QUIT" == line.substr(0, 4))
		server.disconnect(this, line);
	else if ("NICK" == line.substr(0, 4))
	{
		this->_nickname = line.substr(5, strlen(line.c_str()) - 5);
		check_nick_validity(server);
	}
}

void	User::parsing(Server &server)
{
	std::vector<std::string> parsed;
	std::string tmp;
	size_t closest;
	
	if (_data.size() && (_data.find('\r') != _data.npos || _data.find('\n') != _data.npos))
	{
		if (_data.find('\r') < _data.find('\n'))
			closest = _data.find('\r');
		else
			closest = _data.find('\n');
		tmp = _data.substr(0, closest);
		try
		{
			parse_command(tmp, server);
		}
		catch (Error &e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
		_data.erase(0, closest + 1);
	}
	_data.clear();
}

void	User::check_nick_validity(Server &server)
{
	std::list<User *> 	user_list;
	std::stringstream	nb;
	std::string			nick;
	std::string			old_nick = _nickname;	
	int					i = 0;

	while (_nickname.find("#") != _nickname.npos)
		_nickname.erase(_nickname.find("#"), 1);
	if (_nickname.size() == 0 || nick_isalnum() == 0)
		_nickname = "Guest";
	nick = _nickname;
	while (is_nick_used(server))
	{
		nb.str(std::string());
		nb << ++i;
		_nickname = nick + nb.str();
	}
	send_message(":" + old_nick + " NICK :" + _nickname + "\r\n");	
}

void    User::parse_negotiation(std::string line, Server &server)
{
	if ("CAP LS" == line || "CAP LS 302" == line)
		write(this->_fds->fd, "CAP * LS\n", 9);
	else if ("PASS" == line.substr(0, 4))
		this->_password = line.substr(5, strlen(line.c_str()) - 5);
	else if ("NICK" == line.substr(0, 4))
	{
		if (!server.is_pass(_password))
		{
			send_message("ERROR :Password incorrect\r\n");
			server.disconnect(this, "NULL");
			throw Error("Password incorrect");
		}
		this->_nickname = line.substr(5, strlen(line.c_str()) - 5);
		check_nick_validity(server);
	}
	else if ("USER" == line.substr(0, 4))
	{
		char *tmp = strtok((char *)line.c_str(), " ");
		tmp = strtok(NULL, " ");
		this->_username = tmp;
		if (line.find(":") != line.npos)
			this->_realname = line.substr(line.find(":") + 1, line.size() - line.find(":") + 1);	
		std::cout << _username << std::endl;
		std::cout << _realname << std::endl;
	}
	else if (line == "CAP END")
	{
		this->registration(server);
	}
}
