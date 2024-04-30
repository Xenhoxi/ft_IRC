/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:39:53 by smunio            #+#    #+#             */
/*   Updated: 2024/04/30 18:12:24 by smunio           ###   ########.fr       */
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
	/*server.class.cpp*/
	Server();
	~Server();
	void				socket_init(int port, std::string pass);
	std::string 		find_ch_name(std::string line);	

	/*serverCommands.class.cpp*/
	void				join_channel(User *user, std::string &line);
	void				add_user(void);
	void				broadcast(User *user, std::string line);
	void				disconnect(User *user, std::string line);
	void				channel_part(std::string line, User *user);
	void				call_op_cmd(std::string line, User &caller);

	/*serverGeters.class.cpp*/
	bool				is_pass(std::string password);
	int					get_nb_client();
	char                *get_dt() const;
	User				&get_user(std::string nick);
	std::list<User *>	&get_usr_list();
	std::string         get_servername() const;

private:
	std::list<User *>       			_usr_list;
	std::string         				_server_name;
	std::map<std::string, Channel *>    _channel_list;
	char                				*_datetime;
	struct	sockaddr_in					_addr;
	std::string							_password;
	int									_nb_client;
};

#endif