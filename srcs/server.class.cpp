/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:49:47 by smunio            #+#    #+#             */
/*   Updated: 2024/04/24 13:09:35 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Server::Server()
{
    time_t  t               = time(0);
    this->_datetime         = ctime(&t);
	this->_server_name		= "Vitesse a la team de puants";
    return ;
}

Server::~Server()
{
    return ;
}

void	Server::socket_init(int port, std::string pass)
{
	int 	fd_socket;
    User    *server_socket  = new User();

	this->_password = pass;
	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Socket init: " << fd_socket << std::endl;
	if (fd_socket < 0) 
		std::cerr << "Socket exception !" << std::endl;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd_socket, (struct sockaddr *) &this->_addr, sizeof(this->_addr)) <= 0)
		perror("Bind info");
	listen(fd_socket, 5);
    this->_usr_list.push_back(server_socket);
	this->_usr_list.front()->set_fds(fd_socket);
}

std::list<User *> &Server::get_usr_list()
{
    return (_usr_list);
}

char	*Server::get_dt() const
{
	return (this->_datetime);
}

std::string	Server::get_servername() const
{
	return (this->_server_name);
}

void	Server::join_channel(User *user, std::string &line)
{
	/* 	<< JOIN #chezmoi
		>> :choopa.nj.us.dal.net 475 smunio__ #chezmoi :Cannot join channel (+k)
		<< JOIN #chezmoi chezmoi 
		>> :smunio__!~smunio@b42f-2f71-26c7-79bc-fd70.abo.wanadoo.fr JOIN :#chezmoi
	*/
	char *ch_name 	= strtok((char *)line.c_str(), " ");
	ch_name			= strtok(NULL, " ");
	std::cout << "line: " << line << " ch_name: " << ch_name << std::endl;
	if (_channel_list.find(ch_name) != _channel_list.end())
	{
		if (_channel_list[ch_name]->get_size() >= _channel_list[ch_name]->get_max_user() && _channel_list[ch_name]->get_max_user() != 0)
			user->send_message(":ft_irc 471 " + user->get_nick() + " " + ch_name + " :Cannot join channel (+l)\r\n");
		else if (_channel_list[ch_name]->get_invite_mode() == ON_INVITE && !_channel_list[ch_name]->is_invited(user->get_nick()))
			user->send_message(":ft_irc 473 " + user->get_nick() + " " + ch_name + " :Cannot join channel (+i)\r\n");
		if (_channel_list[ch_name]->get_pass_bool() != false)
		{
			std::cout << "need pass: " << _channel_list[ch_name]->get_password() << std::endl;
			if (line.size() < line.find(ch_name) + strlen(ch_name) + 1)
				user->send_message(":ft_irc 475 " + user->get_nick() + " " + ch_name + " :Cannot join channel (+k)\r\n");
			else
			{
				std::string pass = line.substr(line.find(ch_name) + strlen(ch_name) + 1, line.size());
				std::cout << "pass: " << pass << std::endl;
				if (pass == _channel_list[ch_name]->get_password())
					_channel_list[ch_name]->add_user(user);
				else
					user->send_message(":ft_irc 475 " + user->get_nick() + " " + ch_name + " :Cannot join channel (+k)\r\n");
			}
		}
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
	User *new_user = new User();
	new_user->set_fds(accept(_usr_list.front()->get_fds()->fd, NULL, NULL));
	if (new_user->get_fds()->fd < 0)
		throw Error("failed to accept connection");
	else
	{
		_usr_list.push_back(new_user);
		std::cout << "Connection accepted on socket " << new_user->get_fds()->fd << std::endl;
	}
}

std::string Server::find_ch_name(std::string line)
{
	char *tok = strtok((char *)line.c_str(), " ");
	while (tok != NULL)
	{
		if (tok[0] == '#')
			return (static_cast<std::string>(tok));
		tok = strtok(NULL, " ");
	}
	throw Error("can't find channel");
}

void	Server::call_op_cmd(std::string line, User &caller)
{
	std::string ch_name = find_ch_name(line);
	char *cmd			= strtok((char *)line.c_str(), " ");
	const char *cmd_tab[] = {"KICK", "INVITE", "TOPIC", "MODE"};
	void (Channel::*functptr[])(std::string&, User&, Server&) = {&Channel::kick, &Channel::invite, &Channel::topic, &Channel::mode};

	Channel *my_channel = this->_channel_list[ch_name];
	for (int i = 0; i < 4; i++)	
		if (!strcmp(cmd, cmd_tab[i]))
			(my_channel->*functptr[i])(line, caller, *this);
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

User &Server::get_user(std::string nick)
{
    std::list<User *>::iterator it;
    for (it = _usr_list.begin(); it != _usr_list.end(); it++)
	{
        if ((*it)->get_nick() == nick)
            return (*(*it));
    }
    throw Error("can't find target in user list");
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
			_channel_list.erase(_channel_list.find(ch_name));
			std::cout << "Channel supprimer !" << std::endl;
		}
	}
}

bool	Server::is_pass(std::string password)
{
	if (password == _password)
		return (true);
	return (false);
}

int		Server::get_nb_client()
{
	return (_nb_client);
}

void	Server::disconnect(User *user, std::string line)
{
	std::list<User *>::iterator it;
	std::map<std::string, Channel *>::iterator it2;

	if (line.find(":"))
		line = line.substr(line.find(":"), line.size() - line.find(":"));
    for (it = _usr_list.begin(); it != _usr_list.end(); it++)
	{
        if ((*it) == user)
		{
			close((*it)->get_fds()->fd);
			for (it2 = _channel_list.begin(); it2 != _channel_list.end(); it2++)
			{
				if (it2->second->is_connected(user))
				{
					it2->second->delete_ops(user);
					it2->second->disconnect(user, "QUIT", ":Quit" + line);
					if (it2->second->get_size() == 0)
						it2 = _channel_list.erase(it2);
				}
			}
			(*it)->change_status(DISCONNECTED);
			break ;
		}
    }
}
