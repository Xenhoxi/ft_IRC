/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/03/28 14:25:42 by ljerinec         ###   ########.fr       */
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
	(void) argc;
	(void) argv;
	int fd_socket;
	struct sockaddr_in my_addr;
	char	*error_msg;
	
	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_socket < 0)
		std::cerr << "Socket exception !" << std::endl;
	my_addr.sin_family = AF_INET;
	//my_addr.sin_port = (unsigned short)atoi(argv[2]);
	if (bind(fd_socket, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0)
	{
		perror(error_msg);
		std::cerr << error_msg << std::endl;
	}
	std::cout << "fd socket : " << fd_socket << std::endl;
	// while (1)
	// {
		
	// }
	return (0);
}
