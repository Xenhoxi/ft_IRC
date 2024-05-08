/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverCommands.class.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:16:10 by ljerinec          #+#    #+#             */
/*   Updated: 2024/05/08 14:07:51 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void	Server::join_channel(User *user, std::string &line)
{
	std::string ch_name;
	std::string	pass;

	if (line.size() > 5)
	{
		ch_name = line.substr(line.find(" ") + 1, line.size() - (line.find(" ") + 1) - (line.size() - line.rfind(" ")));
		if (line.find(" ", line.find(" ") + 1) != line.npos)
			pass = line.substr(line.find(" ", line.find(" ") + 1) + 1, line.size() - line.rfind(" ") + 1);
		std::cout << "Pass :" << pass << "|" << std::endl;
	}
	else
		return ;
	if (ch_name[0] != '#')
		throw Error("Need # to join channel");
	if (_channel_list.find(ch_name) != _channel_list.end())
	{
		if (_channel_list[ch_name]->get_size() >= _channel_list[ch_name]->get_max_user() && _channel_list[ch_name]->get_max_user() != 0)
			user->send_message(":ft_irc 471 " + user->get_nick() + " " + ch_name + " :Cannot join channel (+l)\r\n");
		else if (_channel_list[ch_name]->get_invite_mode() == ON_INVITE && !_channel_list[ch_name]->is_invited(user->get_nick()))
			user->send_message(":ft_irc 473 " + user->get_nick() + " " + ch_name + " :Cannot join channel (+i)\r\n");
		else if ((_channel_list[ch_name]->get_pass_bool() == true && !_channel_list[ch_name]->is_invited(user->get_nick()) && pass.empty() == true)
				|| (_channel_list[ch_name]->get_pass_bool() == true && !_channel_list[ch_name]->is_invited(user->get_nick()) && pass.empty() == false
				&& pass != _channel_list[ch_name]->get_password()))
			user->send_message(":ft_irc 475 " + user->get_nick() + " " + ch_name + " :Cannot join channel (+k)\r\n");
		else
			_channel_list[ch_name]->add_user(user);
	}
	else
	{
		Channel *newChannel = new Channel(user, ch_name);
		_channel_list.insert(std::pair<std::string, Channel *>(ch_name, newChannel));
	}
}

void	Server::add_user(void)
{
	User	*new_user = new User();
	int		newfd;
	newfd = accept(_usr_list.front()->get_fds()->fd, NULL, NULL);
	if (newfd < 0)
	{
		new_user->set_fds(-1);
		delete new_user;
		throw Error("Failed to accept connection");
	}
	else
	{
		new_user->set_fds(newfd);
		_usr_list.push_back(new_user);
		std::cout << "Connection accepted on socket " << new_user->get_fds()->fd << std::endl;
	}
}

void	Server::broadcast(User *user, std::string line)
{
	std::string msg = line.substr(line.find(':') + 1, line.size() - line.find(':') + 1);
	std::string ch_name;

	if (line.find('#') != line.npos)
		ch_name = line.substr(line.find('#'), line.find(':') - line.find('#') - 1);
	else
		ch_name = line.substr(line.find(' ') + 1, line.find(':') - line.find(' ') - 2);
	if (ch_name.find("#") != ch_name.npos && _channel_list.find(ch_name) != _channel_list.end())
	{
		if (_channel_list[ch_name]->is_connected(user))
		{
			msg = ":" + user->get_nick() + " PRIVMSG " + ch_name + " :" + msg + "\r\n";
			_channel_list[ch_name]->send_to_others(msg, user);
			return ;
		}
	}
	else if (ch_name.find("#") == ch_name.npos)
	{
		std::list<User *>::iterator it;
		for (it = _usr_list.begin(); it != _usr_list.end(); it++)
		{
			std::cout << (*it)->get_nick() << ":" << ch_name << "|" << std::endl;
			if ((*it)->get_nick() == ch_name)
			{
				msg = ":" + user->get_nick() + " PRIVMSG " + ch_name + " :" + msg + "\r\n";
				(*it)->send_message(msg);
				return ;
			}
		}
	}
	user->send_message(":ft_irc 401 " + user->get_nick() + "\r\n");
}

void	Server::disconnect(User *user, std::string line)
{
	std::list<User *>::iterator it;
	std::map<std::string, Channel *>::iterator it2;

	if (line.find(":") != line.npos)
		line = line.substr(line.find(":"), line.size() - line.find(":"));
    for (it = _usr_list.begin(); it != _usr_list.end(); it++)
	{
        if ((*it) == user)
		{
			close((*it)->get_fds()->fd);
			(*it)->change_status(DISCONNECTED);
			for (it2 = _channel_list.begin(); it2 != _channel_list.end(); it2++)
			{
				if (it2->second->is_connected(user))
				{
					it2->second->delete_ops(user);
					it2->second->disconnect(user, "QUIT", ":Quit" + line);
					std::cout << it2->second->get_size() << std::endl;
					if (it2->second->get_size() == 0)
					{
						delete it2->second;
						_channel_list.erase(it2);
						break ;
					}
				}
			}
			break ;
		}
    }
}

void	Server::channel_part(std::string line, User *user)
{
	std::string ch_name;
	std::string	reason = "";

	if (line.find("#") != line.npos && line.find(":") != line.npos)
		ch_name = line.substr(line.find('#'), line.find(":") - 1 - line.find('#'));
	else if (line.find("#") != line.npos)
		ch_name = line.substr(line.find('#'), line.size() - line.find('#'));
	else
		ch_name = "#";
	if (line.find(":") != line.npos)
		reason += " " + line.substr(line.find(':'), line.size() - line.find(':'));
	if (_channel_list.find(ch_name) != _channel_list.end())
	{
		_channel_list[ch_name]->disconnect(user, "PART", reason);
		if ((*(_channel_list.find(ch_name))).second->get_size() == 0)
		{
			delete _channel_list.find(ch_name)->second;
			_channel_list.erase(_channel_list.find(ch_name));
			std::cout << "Channel supprimer !" << std::endl;
		}
	}
}

void	Server::call_op_cmd(std::string line, User &caller)
{
	std::string ch_name = find_ch_name(line);
	char *cmd			= strtok((char *)line.c_str(), " ");
	const char *cmd_tab[] = {"KICK", "INVITE", "TOPIC", "MODE"};
	void (Channel::*functptr[])(std::string&, User&, Server&) = {&Channel::kick, &Channel::invite, &Channel::topic, &Channel::mode};

	if (_channel_list.find(ch_name) != _channel_list.end())
	{
		Channel *my_channel = this->_channel_list[ch_name];
		for (int i = 0; i < 4; i++)	
			if (!strcmp(cmd, cmd_tab[i]))
				(my_channel->*functptr[i])(line, caller, *this);
	}
}
