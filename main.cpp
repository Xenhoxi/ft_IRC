/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/03/27 14:01:21 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	int fd_socket;
	
	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << fd_socket << std::endl;
	while (1)
	{
		
	}
	return (0);
}
