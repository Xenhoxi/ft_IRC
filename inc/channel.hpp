/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:32:05 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/16 17:15:40 by smunio           ###   ########.fr       */
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

	void	kick(std::string &line, User &caller, Server &server);
	void	invite(std::string &line, User &caller, Server &server);
	void	topic(std::string &line, User &caller, Server &server);
	void	mode(std::string &line, User &caller, Server &server);

	void	mode_o(std::string &line, std::string &opt, User &caller);
	void	mode_l(std::string &line, std::string &opt, User &caller);

    void    send_to_all_user(std::string msg);
    void    send_to_others(std::string msg, User *user);
	void	disconnect(User *user, std::string ch_name);
	User	&get_user(std::string nick);
	bool	is_operator(std::string nick) const;
	bool	is_connected(User *user);
	int		get_size(void);

private:
    std::string         			_name;
	std::string						_topic;
    std::list<User *>  				_userInChannel;
	std::list<User *>				_operators;
	unsigned int					_max_users;
};

#endif