/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:29:14 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/30 18:46:31 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void	read_socket(User *user)
{
	char	buff[100];
	int		u;

	memset(buff, 0, 100);
	u = read(user->get_fds()->fd, buff, 100);
	user->add_data(buff);
	if (u > 0)
		std::cout << "<< " << buff;
	memset(buff, 0, 100);
}

void	running_server(Server &server)
{
	std::list<User *> &user_list = server.get_usr_list();

	for (std::list<User *>::iterator it = user_list.begin(); it != user_list.end(); it++)
	{
		try
		{
			struct pollfd *fds = (*it)->get_fds();
			User *user = *it;
			if (poll(fds, 1, 0) > 0)
			{
				if (fds->fd == user_list.front()->get_fds()->fd && fds->revents & POLLIN)
					server.add_user();
				else if (fds->revents & POLLIN)
					read_socket(user);
			}
			if (fds->fd != user_list.front()->get_fds()->fd && user->get_status() == NEGOTIATION)
			{
				user->negotiation(server);
				user->ping(server);	
			}
			else if (user->get_status() == CONNECTED)
			{
				user->parsing(server);
				user->ping(server);
			}
			// else if (user->get_status() != DISCONNECTED && user->get_status() != MAIN_SOCKET)
			else if (user->get_status() == DISCONNECTED)
				it = user_list.erase(it);
		}
		catch (Error &e)
		{
			std::cout << "souris inverser pendant 3j" << std::endl;
			std::cout << e.what() << std::endl;
		}
	}
}

void	stop_server(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "catched a SIGINT" << std::endl;
		throw Stop("catched a SIGINT");
	}
	return ;
}

int main(int argc, char **argv)
{
	Server	*server = new Server();
	try
	{
		signal(SIGINT, stop_server);
		if (argc != 3)
			throw Error("wrong args amount");
		server->socket_init(atoi(argv[1]), argv[2]);
		while (1)
			running_server(*server);
	}
	catch (Stop &e)
	{
		std::cout << "Stoping: " << e.what() << std::endl;
	}
	catch (Error &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	delete server;
	return (0);
}
