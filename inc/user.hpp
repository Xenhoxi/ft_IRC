/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:10:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/08 13:06:47 by smunio           ###   ########.fr       */
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
	void			negotiation(void);
	void			registration(Server server);
	int				get_status() const;
	void			parse_negotiation(std::string line);
	void			change_status(int status);
	void            set_fds(int server_socket);
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