/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:32:05 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/17 13:43:03 by ljerinec         ###   ########.fr       */
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
	size_t	get_size(void);
	size_t	get_max_user(void);
	bool	get_topic_restriction(void);

private:
    std::string         			_name;
	std::string						_topic;
    std::list<User *>  				_userInChannel;
	std::list<User *>				_operators;
	size_t							_max_users;
	bool							_topic_restriction;
};

#endif