/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/03 12:00:08 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void socket_init(std::list <User *> &listfd, int port)
{
	int 				fd_socket;
	struct sockaddr_in 	my_addr;
	User				*server_socket = new User();
	// struct pollfd 		*fds = new struct pollfd[1];

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
		std::cout << "Connection accepted on socket " << new_user->get_fds()->fd << std::endl;
}

void	read_socket(User *user)
{
	char buff[30];

	memset(buff, 0, 30);
	int n_read = read(user->get_fds()->fd, buff, 30);
	if (n_read > 0)
		std::cout << buff;
	write(user->get_fds()->fd, "OK\n", 4);
	user->_data += buff;
	memset(buff, 0, 30);
}

void	running_server(std::list <User *> &user_list)
{
	for (std::list<User *>::iterator it = user_list.begin(); it != user_list.end(); ++it)
	{
		struct pollfd *fds = (*it)->get_fds();
		if (poll(fds, 1, 0) > 0)
		{
			if (fds->fd == user_list.front()->get_fds()->fd && fds->revents & POLLIN)
				accept_connection(user_list);
			else if (fds->revents & POLLIN)
				read_socket(*it);
		}
	}
}

int main(int argc, char **argv)
{
	std::list <User *> user_list;
	
	try
	{
		if (argc != 3)
			throw Error();
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
