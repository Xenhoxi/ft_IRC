/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userCommands.class.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:22:51 by ljerinec          #+#    #+#             */
/*   Updated: 2024/05/13 12:04:11 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

// void    User::negotiation(Server &server)
// {
// 	std::vector<std::string> parsed;
// 	std::string tmp;
// 	size_t closest;
	
// 	while (_data.size() && (_data.find('\r') != _data.npos || _data.find('\n') != _data.npos))
// 	{
// 		if (_data.find('\r') < _data.find('\n'))
// 			closest = _data.find('\r');
// 		else
// 			closest = _data.find('\n');
// 		tmp = _data.substr(0, closest);
// 		this->parse_negotiation(tmp, server);
// 		_data.erase(0, closest + 1);
// 	}
// 	_data.clear();
// }

void	User::negotiation(Server &server)
{
	std::vector<std::string> parsed;
	std::string tmp;
	size_t closest;
	
	if (_data.size() > 0 && (_data.find('\r') != _data.npos || _data.find('\n') != _data.npos))
	{
		if (_data.find('\r') < _data.find('\n'))
			closest = _data.find('\r');
		else
			closest = _data.find('\n');
		tmp = _data.substr(0, closest);
		std::cout << "tmp :" << tmp << std::endl;
		this->parse_negotiation(tmp, server);
		_data.erase(0, closest + 1);
	}
}

void    User::registration(Server &server)
{
	if (_nickname.size() != 0 && _username.size() != 0 && _cap_passed == true)
	{
		send_message(":ft_irc 001 " + _nickname + " :Welcome to the 'ft_irc' Network, " + _nickname + "\r\n");
		send_message(":ft_irc 002 " + _nickname + " :Your host is " + server.get_servername() + ", running version 1.0" + "\r\n");
		send_message(":ft_irc 003 " + _nickname + " :This server was created " + server.get_dt() + "\r\n");
		send_message(":ft_irc 004 " + _nickname + " :" + server.get_servername() + " version 1.0\r\n");
		send_message(":ft_irc 005 " + _nickname + " NETWORK=FT_IRC NICKLEN=30 CHANNELLEN=32 CHANTYPES=# PREFIX=(o)@ :are available on this server\r\n");
		send_message(":ft_irc 005 " + _nickname + " CASEMAPPING=ascii CHANMODES=,ko,l,it :are available on this server\n");
		ascii_sam(this);
		change_status(CONNECTED);
	}
	else
	{
		send_message("Cannot register if negotiation is not done: will be disconnected\r\n");
		server.disconnect(this, "NULL");
		throw Error("Cannot register if negotiation is not done.");
	}
}

void	User::change_nick(std::string new_nick, Server &server)
{
	std::string old_nick = this->_nickname;
	if (new_nick.size() > 30)
		new_nick = new_nick.substr(0, 30);
	this->_nickname = new_nick;
	for (int i = 0; new_nick[i]; i++)
	{
		if (i == 0 && !isalpha(new_nick[i]))
		{
			send_message(": 432 " + old_nick + " " + this->_nickname + " :Errorneous nickname\r\n");
			this->_nickname = old_nick;
			throw Error("errorneous nickname");
		}
	}
	if (is_nick_used(server))
	{
		send_message(": NOTICE " + this->_nickname + " :This nick is owned by someone else.\r\n");
		send_message(": NOTICE " + this->_nickname + " :Your nick will be changed if you like it or not.\r\n");
		this->_nickname = old_nick;
		throw Error("errorneous nickname");
	}
	send_message(":" + old_nick + "!" + _username + "@localhost NICK :" + _nickname + "\r\n");
	for (std::list<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
		(*it)->send_to_others(":" + old_nick + "!" + _username + "@localhost NICK :" + _nickname + "\r\n", this);
}

void    User::change_status(int status)
{
	if (this->_status == 0 && status == 2)
		throw Error("can't skip registration");
	if (this->_status == 2 && status == 1)
		throw Error("user is already registered");
	this->_status = status;
}

void	User::send_message(std::string msg)
{
	write(this->get_fds()->fd, msg.c_str(), msg.size());
	std::cout << ">> " << msg.substr(0, msg.size() - 2) << std::endl;
}

void	User::ping(Server &server)
{
	time_t  t = time(0);

	if (t - _last_pong > 20 && _pinged == false)
	{
		send_message("PING ft_irc\r\n");
		_pinged = true;
	}
	if (t - _last_pong > 25 && _pinged == true)
	{
		server.disconnect(this, " TIMED OUT");
	}
}

