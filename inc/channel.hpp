/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:32:05 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/09 00:23:11 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "libs.hpp"

class User;

class Channel
{
public:
	Channel(User *user, std::string name);
    ~Channel(void);
    void    add_user(User *user);
    void    send_to_all_user(std::string msg, User *user, std::string ch_name);
	void	kick(std::string line);
	void	invite(std::string line);
	void	topic(std::string line);
	void	mode(std::string line);
	bool	is_operator(User &user) const;

private:
    std::string         			_name;
    std::list<User *>  				_userInChannel;
	std::list<User *>				_operators;
};

#endif