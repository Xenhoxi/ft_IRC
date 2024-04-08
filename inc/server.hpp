/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:39:53 by smunio            #+#    #+#             */
/*   Updated: 2024/04/08 13:06:56 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "libs.hpp"

class User;

class Server
{
    public: 
        Server();
        ~Server();
        char                    *get_dt() const;
        std::list<User *>		&get_usr_list();
		void					socket_init(int port);
		void					set_addr(int port, int fd_socket);\
        std::string             get_servername() const;
    private:
        std::list<User *>   _usr_list;
        std::string         _server_name;
        char                *_datetime;
		struct	sockaddr_in	_addr;
};

#endif