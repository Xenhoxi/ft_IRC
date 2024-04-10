/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:32:05 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/10 13:03:59 by ljerinec         ###   ########.fr       */
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
    void    send_to_all_user(std::string msg);
    void    send_to_others(std::string msg, User *user);
	void	kick(std::string line);
	void	invite(std::string line);
	void	topic(std::string line);
	void	mode(std::string line);
	void	disconnect(User *user, std::string ch_name);
	bool	is_operator(User &user) const;
	bool	is_connected(User *user);

private:
    std::string         			_name;
	std::string						_topic;
    std::list<User *>  				_userInChannel;
	std::list<User *>				_operators;
};

#endif