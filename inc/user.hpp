/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:10:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/04 14:44:19 by ljerinec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <poll.h>

class User
{
public:
	User();
	~User();
	void			parsing(void);
	void			negotiation(void);
	void			registration(void);
	int				get_status() const;
	void			store_pass(std::string line);
	void			store_nickname(std::string line);
	void			store_username(std::string line);
	void			parse_registration(std::string line);
	void			change_status(int status);
	void            set_nickname(std::string name);
	void            set_fds(int server_socket);
	struct pollfd   *get_fds() const;
	std::string		get_nickname() const;
	std::string		_data;

private:
	std::string     _nickname;
	std::string		_username;
	std::string		_password;
	int				_status;
	struct pollfd	*_fds;
};


#endif