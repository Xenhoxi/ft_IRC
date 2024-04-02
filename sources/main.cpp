/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/02 13:44:23 by ljerinec         ###   ########.fr       */
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

int main(int argc, char **argv)
{
	//(void) argv;
	if (argc < 3)
	{
		std::cout << "Not enought parameters !" << std::endl;
		return (1);
	}
	int fd_socket;
	struct sockaddr_in my_addr;

	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "fd socket : " << fd_socket << std::endl;
	if (fd_socket < 0) 
		std::cerr << "Socket exception !" << std::endl;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(fd_socket, (struct sockaddr *) &my_addr, sizeof(my_addr)) <= 0)
		perror("Bind info");
	
	listen(fd_socket, 5);
	std::cout << "Listening ......." << std::endl;
	char buff[30];
	memset(buff, 0, 30);
	
	int client_socket = 0;
	std::list <struct pollfd *> listfd;
	struct pollfd fdss[1];
	fdss[0].fd = fd_socket;
	fdss[0].events = POLLIN | POLLOUT;
	listfd.push_back(fdss);
	while (1)
	{
		for (std::list<struct pollfd *>::iterator it = listfd.begin(); it != listfd.end(); ++it)
		{
			struct pollfd *fds = *it;
			if (poll(fds, 1, 0) > 0)
			{
				if (fds->fd == fd_socket && fds->revents & POLLIN)
				{
					std::cout << "Ici" << std::endl;
					struct pollfd *tmp = new struct pollfd[1];
					tmp->fd = accept(fd_socket, NULL, NULL);
					tmp->events = POLLIN | POLLOUT;
					listfd.push_back(tmp);
					if (tmp->fd < 0)
						perror("Socket client");
					else
						std::cout << "Connection accepted on socket " << tmp->fd << std::endl;
				}
				else if (fds->revents & POLLIN)
				{
					int n_read = read(fds->fd, buff, 30);
					if (n_read > 0)
						std::cout << "Read: " << n_read << " Buff = " << buff << std::endl;
					write(fds->fd, "OK\n", 4);
					memset(buff, 0, 30);
				}
			}
		}
	}
	close(client_socket);
	close(fd_socket);
	return (0);
}
