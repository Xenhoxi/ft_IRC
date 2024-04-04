/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/04 15:56:13 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void	accept_connection(std::list <User *> &userlist)
{
	User *new_user = new User();
	new_user->set_fds(accept(userlist.front()->get_fds()->fd, NULL, NULL));
	if (new_user->get_fds()->fd < 0)
		throw Error("failed to accept connection");
	else
	{
		userlist.push_back(new_user);
		std::cout << "Connection accepted on socket " << new_user->get_fds()->fd << std::endl;
		new_user->change_status(NEGOTIATION);
	}
}

void	read_socket(User *user)
{
	char buff[100];

	memset(buff, 0, 100);
	read(user->get_fds()->fd, buff, 100);
	// if (n_read > 0)
	// 	std::cout << buff;
	// write(user->get_fds()->fd, "OK\n", 4);
	user->_data += buff;
	std::cout << buff;
	// std::cout << user->_data << std::endl;
	memset(buff, 0, 100);
}

void	parsing(User *user)
{
	std::vector<std::string> parsed;
	std::string tmp;
	size_t closest;
	
	// (void) user;
	std::cout << "parsing user..." << std::endl;
	while (user->_data.find('\r') != user->_data.npos || user->_data.find('\n') != user->_data.npos)
	{
		if (user->_data.find('\r') < user->_data.find('\n'))
			closest = user->_data.find('\r');
		else
			closest = user->_data.find('\n');
		tmp = user->_data.substr(0, closest);
		parsed.push_back(tmp);
		user->_data.erase(0, closest + 1);
	}
	user->_data.clear();
	for (std::vector<std::string>::iterator it = parsed.begin(); it != parsed.end(); ++it)
	{
		std::cout << *it << std::endl;
		user->parse_registration(*it);
	}
	// send rpl messages;
}

void	running_server(Server &server)
{
	std::list<User *> user_list = server.get_usr_list();
	for (std::list<User *>::iterator it = user_list.begin(); it != user_list.end(); ++it)
	{
		struct pollfd *fds = (*it)->get_fds();
		User *user = *it;
		if (poll(fds, 1, 0) > 0)
		{
			if (fds->fd == user_list.front()->get_fds()->fd && fds->revents & POLLIN)
				accept_connection(user_list);
			else if (fds->revents & POLLIN)
				read_socket(user);
		}
		if ((user->_data.find('\r') != user->_data.npos || user->_data.find('\n') != user->_data.npos) && fds->fd != user_list.front()->get_fds()->fd)
			parsing(user);
	}
}

int main(int argc, char **argv)
{
	Server	*server = new Server();	
	try
	{
		if (argc != 3)
			throw Error("wrong args amount");
		server->socket_init(atoi(argv[1]));
		while (1)
			running_server(*server);
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	return (0);
}
