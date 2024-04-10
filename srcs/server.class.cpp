/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:49:47 by smunio            #+#    #+#             */
/*   Updated: 2024/04/10 11:41:03 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Server::Server()
{
    time_t  t               = time(0);
    this->_datetime         = ctime(&t);
	this->_server_name		= "vitesse a la team de puants";
    return ;
}

Server::~Server()
{
    return ;
}

void	Server::socket_init(int port)
{
	int 	fd_socket;
    User    *server_socket  = new User();
    
	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Socket init: " << fd_socket << std::endl;
	if (fd_socket < 0) 
		std::cerr << "Socket exception !" << std::endl;
	this->set_addr(port, fd_socket);
	listen(fd_socket, 5);
    this->_usr_list.push_back(server_socket);
	this->_usr_list.front()->set_fds(fd_socket);
}

void    Server::set_addr(int port, int fd_socket)
{
    this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd_socket, (struct sockaddr *) &this->_addr, sizeof(this->_addr)) <= 0)
		perror("Bind info");
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

void	Server::join_channel(User *user, std::string ch_name)
{
	if (_channel_list.find(ch_name) != _channel_list.end())
		_channel_list[ch_name]->add_user(user);
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
		new_user->change_status(NEGOTIATION);
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
	return ("null");
}

void	Server::call_op_cmd(std::string line, User &caller)
{
	std::string ch_name = find_ch_name(line);
	char *cmd			= strtok((char *)line.c_str(), " ");
	const char *cmd_tab[] = {"KICK", "INVITE", "TOPIC", "MODE"};
	void (Channel::*functptr[])(std::string&, User&) = {&Channel::kick, &Channel::invite, &Channel::topic, &Channel::mode};

	if (ch_name == "null") // a securiser davantage
		return ;

	Channel *my_channel = this->_channel_list[ch_name];
	for (int i = 0; i < 4; i++)
	{	
		if (!strcmp(cmd, cmd_tab[i]))
		{
			std::cout << "line: " << line << std::endl;
			if (my_channel->is_operator(caller) == true)
			{
				std::cout << "found operator" << std::endl;
				(my_channel->*functptr[i])(line, caller);
			}
		}
	}
}

void	Server::broadcast(User *user, std::string line)
{
	std::string msg = line.substr(line.find(':') + 1, line.size() - line.find(':') + 1);
	std::string ch_name = line.substr(line.find('#'), line.find(':') - line.find('#') - 1);
	std::cout << "msg = " << msg << " | "<< "channel name = " << ch_name << std::endl;
	if (_channel_list.find(ch_name) != _channel_list.end())
	{
		std::cout << "Channel name = " << ch_name << " | " << "msg = " << msg << std::endl;
		_channel_list[ch_name]->send_to_all_user(msg, user, ch_name);
	}
}

void	Server::channel_part(std::string line, User *user)
{
	std::string ch_name = line.substr(line.find('#'), line.size() - line.find('#'));
	std::cout << ch_name << "|" << std::endl;
	if (_channel_list.find(ch_name) != _channel_list.end())
		_channel_list[ch_name]->disconnect(user, ch_name);
}
