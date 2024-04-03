/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/03 19:17:33 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void socket_init(std::list <User *> &listfd, int port)
{
	int 				fd_socket;
	struct sockaddr_in 	my_addr;
	User				*server_socket = new User();

	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Socket init: " << fd_socket << std::endl;
	if (fd_socket < 0) 
		std::cerr << "Socket exception !" << std::endl;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(fd_socket, (struct sockaddr *) &my_addr, sizeof(my_addr)) <= 0)
		perror("Bind info");
	listen(fd_socket, 5);
	server_socket->set_fds(fd_socket);
	listfd.push_back(server_socket);
}

void	accept_connection(std::list <User *> &userlist)
{
	User *new_user = new User();
	new_user->set_fds(accept(userlist.front()->get_fds()->fd, NULL, NULL));
	userlist.push_back(new_user);
	if (new_user->get_fds()->fd < 0)
		perror("Socket client");
	else
	{
		std::cout << "Connection accepted on socket " << new_user->get_fds()->fd << std::endl;
		new_user->change_status(1);
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
	// std::cout << user->_data << std::endl;
	memset(buff, 0, 100);
}

void	parsing(User *user)
{
	std::vector<std::string> parsed;
	std::string tmp;
	size_t closest;
	
	// (void) user;
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
	for (std::vector<std::string>::iterator it = parsed.begin(); it != parsed.end(); ++it)
	{
		std::cout << *it << std::endl;
		user->parse_registration(*it);
	}
	// if (user->get_r_infos() < 4)
	// 	throw Error("Missing some registrations infos");
	// send rpl messages;
}

void	send_rpl_msgs(User	*usr)
{
	write(usr->get_fds()->fd, usr->get_nickname().c_str(), strlen(usr->get_nickname().c_str()));
}

void	running_server(std::list <User *> &user_list)
{
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
	std::list <User *> user_list;
	
	try
	{
		if (argc != 3)
			throw Error("wrong args amount");
		socket_init(user_list, atoi(argv[1]));
		while (1)
			running_server(user_list);
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	return (0);
}
