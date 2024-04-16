/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:10:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/16 10:41:04 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "libs.hpp"

class Server;

class User
{
public:
	User();
	~User();
	int				get_status() const;
	std::string 	get_nick();
	struct pollfd   *get_fds() const;
	void			add_data(std::string new_data);
	
	void			parsing(Server &server);
	void			parse_negotiation(std::string line, Server &server);
	void			negotiation(Server &server);
	void			registration(Server &server);
	void			send_message(std::string msg);
	void			change_status(int status);
	void            set_fds(int server_socket);
	void			parse_command(std::string line, Server &server);
	void			parse_op_cmd(const char *cmd, Server &server, std::string ch_name) const;

private:
	std::string		_data;
	std::string     _nickname;
	std::string		_username;
	std::string		_password;
	int				_status;
	struct pollfd	*_fds;
};


#endif