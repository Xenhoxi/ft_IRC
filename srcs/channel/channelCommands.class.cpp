/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCommands.class.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:18:50 by smunio            #+#    #+#             */
/*   Updated: 2024/05/08 14:25:23 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void Channel::kick(std::string &line, User &caller, Server &server)
{
	(void)server;
	if (is_operator(caller.get_nick()) != false)
	{
		std::string reason;
		if (line.find(':') != line.npos)
			reason = line.substr(line.find(':') + 1, line.size() - (line.find(':') + 1));
		char *tar = (char *)line.c_str();
		tar = strtok(NULL, " ");
		tar = strtok(NULL, " ");

		User    &target = this->get_user(tar);

		std::string msg = ":" + caller.get_host_info() + " KICK " + this->_name + " " + tar + " :" + reason + "\r\n";
		this->send_to_all_user(msg);
		
		std::list<User *>::iterator it;
		for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
		{
			if (*it == &target)
			{
				_userInChannel.erase(it);
				break ;
			}
		}
	}
	else
		caller.send_message(":ft_irc 482 " + caller.get_nick() + " " + _name + " :You're not channel operator\r\n");
}

void Channel::invite(std::string &line, User &caller, Server &server)
{
	char *tar = (char *)line.c_str();
	tar = strtok(NULL, " ");
	User	*target = NULL;
	if (!tar)
		return ;
	if (!is_operator(caller.get_nick()))
	{
		caller.send_message(":ft_irc 482 " + caller.get_nick() + " " + _name + " :You're not channel operator 1\r\n");
		return ;
	}
	try
	{
		target = &server.get_user(tar);
	}
	catch (Error &e)
	{
		caller.send_message(":ft_irc 401 " + caller.get_nick() + " " + tar + " :No such nick/channel\r\n");
		return ;
	}
	if (this->is_connected(target))
	{
		caller.send_message(":ft_irc 443" + caller.get_nick() + " " + _name + " " + target->get_nick() + " :is already on channel\r\n");
		return ;
	}
	target->send_message(":" + caller.get_host_info() + " INVITE " + tar + " " + this->_name + "\r\n");
	if (!is_invited(target->get_nick()))
		_user_invited.push_front(target->get_nick());
}

void Channel::topic(std::string &line, User &caller, Server &server)
{
	(void)server;
	std::string topic;
	
	if (line.find("#") != line.npos && line.find(":") != line.npos)
		topic = line.substr(line.find(":") + 1, line.size()- line.find(":") + 2);
	_topic = topic;
	if (_topic_mode == TOPIC_ALL)
		send_to_all_user(":" + caller.get_nick() + " TOPIC " + _name + " :" + _topic + "\r\n");
	else
	{
		if (_topic_mode == TOPIC_OP && is_operator(caller.get_nick()))
		   send_to_all_user(":" + caller.get_nick() + " TOPIC " + _name + " :" + _topic + "\r\n");
		else
			caller.send_message(":ft_irc 482 " + caller.get_nick() + " " + _name + " :You're not channel operator\r\n");
	}
}

void    Channel::disconnect(User *leaving_user, std::string type, std::string reason)
{
	std::list<User *>::iterator it;

	for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *user = *it;
		if (leaving_user == user)
		{
			if (type == "PART")
				send_to_all_user(":" + leaving_user->get_host_info() + " " + type + " " + _name + reason + "\r\n");
			else if (type == "QUIT")
				send_to_all_user(":" + leaving_user->get_host_info() + " " + type + " " + reason + "\r\n");
			_userInChannel.erase(it);
			delete_ops(leaving_user);
			std::cout << "User leave the channel: " << _name << std::endl;
			break ;
		}
	}
}

void    Channel::send_to_others(std::string msg, User *send_user)
{
	std::list<User *>::iterator it;

	for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *user_receiver = *it;
		if (send_user != user_receiver)
			user_receiver->send_message(msg);
	}
}

void    Channel::send_to_all_user(std::string msg)
{
	std::list<User *>::iterator it;

	for (it = _userInChannel.begin(); it != _userInChannel.end(); it++)
	{
		User *user_receiver = *it;
		user_receiver->send_message(msg);
	}
}

void	Channel::delete_ops(User *user)
{
	std::list<User *>::iterator it;

	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if ((*it)->get_nick() == user->get_nick())
		{
			_operators.erase(it);
			break ;
		}
	}
}

void    Channel::add_user(User *user)
{
	std::list<std::string>::iterator it;
	std::string msg = ":ft_irc 353 " + user->get_nick() + " = " + _name + " :";

	_userInChannel.push_back(user);
	send_to_all_user(":" + user->get_host_info() + " JOIN " + _name + "\r\n");
	if (_topic.size() > 0)
		user->send_message("TOPIC " + _name + " :" + _topic + "\r\n");
	for (std::list<User *>::iterator it = _userInChannel.begin(); it != _userInChannel.end(); ++it)
	{
		if (is_operator((*it)->get_nick()))
			msg += "@";
		msg += (*it)->get_nick() + " ";
	}
	user->send_message(msg + "\r\n" + ":ft_irc 366 " + user->get_nick() + " " + _name + " :End of /NAMES list.\r\n");
	for (it = _user_invited.begin(); it != _user_invited.end(); it++)
		if (*it == user->get_nick())
			it = _user_invited.erase(it);
}
