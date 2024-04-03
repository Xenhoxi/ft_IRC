/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/03 15:14:24 by ljerinec         ###   ########.fr       */
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
		std::cout << "Connection accepted on socket " << new_user->get_fds()->fd << std::endl;
}

void	read_socket(User *user)
{
	char buff[100];

	memset(buff, 0, 100);
	int n_read = read(user->get_fds()->fd, buff, 100);
	if (n_read > 0)
		std::cout << buff;
	write(user->get_fds()->fd, "OK\n", 4);
	user->_data += buff;
	std::cout << user->_data << std::endl;
	// usleep(10000000);
	memset(buff, 0, 100);
}

void	parse_data(std::string to_parse, std::vector<std::string> &vec_parsed)
{
	(void) to_parse;
	(void) vec_parsed;
	
	std::cout << "test : " << std::endl;
	// while (to_parse.find("\r") != to_parse.npos)
	// {
	// 	std::string test = to_parse.substr(0, to_parse.find("\r\n"));
	// 	std::cout << test << std::endl;
	// 	vec_parsed.push_back(test);
	// 	to_parse.erase(0, to_parse.find("\r\n"));
	// }
}

void	parsing(User *user)
{
	std::vector<std::string> parsed;
	
	parse_data(user->_data, parsed);
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
			{
				read_socket(user);
				std::cout << "User->data : " << user->_data.c_str() << std::endl;
			}
		}
		if (user_list.size() >= 2 && fds->fd != user_list.front()->get_fds()->fd)
		{
			std::cout << "ok" << user_list.size() << std::endl;
			std::cout << user->_data.c_str() << std::endl;
			sleep(1);
			parsing(user);
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
