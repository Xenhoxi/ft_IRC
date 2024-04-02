/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/03/31 22:37:42 by ljerinec         ###   ########.fr       */
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
	struct pollfd fds[1];
	fds[0].fd = fd_socket;
	fds[0].events = POLLIN | POLLOUT;
	while (1)
	{
		if (poll(fds, 1, 0) > 0)
		{
			client_socket = accept(fd_socket, NULL, NULL);
			if (client_socket < 0)
				perror("Socket client");
			else
				std::cout << "Connection accepted !" << client_socket << std::endl;
		}
		if (client_socket)
		{
			int n_read = read(client_socket, buff, 30);
			if (n_read > 0)
				std::cout << "Read: " << n_read << " Buff = " << buff << std::endl;
		}
	}
	close(client_socket);
	close(fd_socket);
	return (0);
}
