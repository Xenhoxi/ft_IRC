/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:39:53 by smunio            #+#    #+#             */
/*   Updated: 2024/04/04 15:49:20 by smunio           ###   ########.fr       */
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
        std::list<User *>		get_usr_list() const;
		void					socket_init(int port);
		void					set_addr(int port, int fd_socket);
    private:
        std::list<User *>   _usr_list;
        char                *_datetime;
		struct	sockaddr_in	_addr;
};

#endif