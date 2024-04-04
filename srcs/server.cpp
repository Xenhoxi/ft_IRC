/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:49:47 by smunio            #+#    #+#             */
/*   Updated: 2024/04/04 15:54:28 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

Server::Server()
{
    time_t  t               = time(0);
    this->_datetime         = ctime(&t);
    return ;
}

Server::~Server()
{
    return ;
}

void Server::socket_init(int port)
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

std::list<User *> Server::get_usr_list() const
{
    return (_usr_list);
}
