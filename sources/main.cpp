/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/03/28 15:58:55 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		std::cout << "Not enought parameters !" << std::endl;
		return (1);
	}
	int fd_socket;
	struct sockaddr_in my_addr;
	struct sockaddr client_addr;

	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_socket < 0) 
		std::cerr << "Socket exception !" << std::endl;
	my_addr.sin_family = AF_INET;
	//my_addr.sin_port = htons(atoi(argv[2]));
	//my_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(fd_socket, (struct sockaddr *) &my_addr, sizeof(my_addr)) <= 0)
		perror("Bind info");
	std::cout << "fd socket : " << fd_socket << std::endl;
	listen(fd_socket, 5);
	int client_socket = accept(fd_socket, (struct sockaddr *) &client_addr, (socklen_t *)sizeof(client_addr));
	return (0);
}
