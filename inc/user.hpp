/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:10:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/03 11:57:38 by ljerinec         ###   ########.fr       */
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
	struct pollfd   *get_fds() const;
	std::string		_data;

private:
	std::string     _nickname;
	struct pollfd	*_fds;
};

#endif