/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:10:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/03 14:51:24 by smunio           ###   ########.fr       */
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
	void            set_nickname(std::string name);
	void            set_fds(int server_socket);
	void			change_status(int status);
	struct pollfd   *get_fds() const;
	std::string		get_nickname() const;
	std::string		_data;

private:
	std::string     _nickname;
	std::string		_username;
	unsigned int	_status;
	struct pollfd	*_fds;
};

#endif