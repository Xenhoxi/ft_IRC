/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/02 17:32:12 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <string.h>
#include <list>
#include <exception>

void socket_init(std::list <struct pollfd *> &listfd, int port)
{
	int fd_socket;
	struct sockaddr_in my_addr;
	struct pollfd *fds = new struct pollfd[1];

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
	fds->fd = fd_socket;
	fds->events = POLLIN | POLLOUT;
	listfd.push_back(fds);
}

void	accept_connection(std::list <struct pollfd *> &listfd)
{
	struct pollfd *tmp = new struct pollfd[1];
	tmp->fd = accept(listfd.front()->fd, NULL, NULL);
	tmp->events = POLLIN | POLLOUT;
	listfd.push_back(tmp);
	if (tmp->fd < 0)
		perror("Socket client");
	else
		std::cout << "Connection accepted on socket " << tmp->fd << std::endl;
}

void	read_socket(struct pollfd *fds)
{
	char buff[30];

	memset(buff, 0, 30);
	int n_read = read(fds->fd, buff, 30);
	if (n_read > 0)
		std::cout << "Read: " << n_read << " Buff = " << buff << std::endl;
	write(fds->fd, "OK\n", 4);
	memset(buff, 0, 30);
}

void	running_server(std::list <struct pollfd *> &listfd)
{
	for (std::list<struct pollfd *>::iterator it = listfd.begin(); it != listfd.end(); ++it)
	{
		struct pollfd *fds = *it;
		if (poll(fds, 1, 0) > 0)
		{
			if (fds->fd == listfd.front()->fd && fds->revents & POLLIN)
				accept_connection(listfd);
			else if (fds->revents & POLLIN)
				read_socket(fds);
		}
	}
}

int main(int argc, char **argv)
{
	std::list <struct pollfd *> listfd;
	
	try
	{
		if (argc < 3)
		{
			std::cout << "Not enought parameters !" << std::endl;
			return (1);
		}
		socket_init(listfd, atoi(argv[1]));
		while (1)
			running_server(listfd);
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	return (0);
}
