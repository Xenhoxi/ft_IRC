/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:10:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/03 19:16:32 by smunio           ###   ########.fr       */
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
	void			parse_registration(std::string line);
	void			change_status(int status);
	void            set_nickname(std::string name);
	void            set_fds(int server_socket);
	struct pollfd   *get_fds() const;
	std::string		get_nickname() const;
	unsigned int	get_r_infos() const;
	std::string		_data;

private:
	unsigned int	_r_infos;
	std::string     _nickname;
	std::string		_username;
	std::string		_password;
	unsigned int	_status;
	struct pollfd	*_fds;
};

#endif