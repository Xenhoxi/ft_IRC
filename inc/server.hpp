/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:39:53 by smunio            #+#    #+#             */
/*   Updated: 2024/04/09 00:19:14 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "libs.hpp"

class User;
class Channel;

class Server
{
public: 
	Server();
	~Server();
	char                *get_dt() const;
	std::list<User *>	&get_usr_list();
	void				socket_init(int port);
	void				set_addr(int port, int fd_socket);\
	std::string         get_servername() const;
	void				join_channel(User *user, std::string ch_name);
	void				add_user(void);
	void				broadcast(User *user, std::string msg, std::string ch_name);
	std::map<std::string, Channel *>	get_channel_list() const;
	std::string 			find_ch_name(std::string line);
	void					call_op_cmd(std::string line, User &caller);
private:
	std::list<User *>       			_usr_list;
	std::string         				_server_name;
	std::map<std::string, Channel *>    _channel_list;
	char                				*_datetime;
	struct	sockaddr_in					_addr;
};

#endif