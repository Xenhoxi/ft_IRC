/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:10:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/08 14:23:08 by ljerinec         ###   ########.fr       */
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
	void			negotiation(Server &server);
	void			registration(Server &server);
	int				get_status() const;
	void			parse_negotiation(std::string line, Server &server);
	void			parsing(Server &server);
	void			change_status(int status);
	void            set_fds(int server_socket);
	void			parse_command(std::string line, Server &server);
	struct pollfd   *get_fds() const;
	std::string		_data;

private:
	std::string     _nickname;
	std::string		_username;
	std::string		_password;
	int				_status;
	struct pollfd	*_fds;
};


#endif