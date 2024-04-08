/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:39:53 by smunio            #+#    #+#             */
/*   Updated: 2024/04/08 13:11:06 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "libs.hpp"

class Server
{
	public: 
		Server();
		~Server();
		std::list<User *>		&get_usr_list();
		void					socket_init(int port);
		void					set_addr(int port, int fd_socket);
		void					join_channel(User *user, std::string ch_name);
		void					add_user(void);
	private:
		std::list<User *>       			_usr_list;
		std::map<std::string, Channel *>    _channel_list;
		char                				*_datetime;
		struct	sockaddr_in					_addr;
};

#endif