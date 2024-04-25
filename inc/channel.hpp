/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 12:32:05 by ljerinec          #+#    #+#             */
/*   Updated: 2024/04/25 11:34:17 by smunio           ###   ########.fr       */
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
	bool			is_operator(std::string nick) const;
	bool			is_connected(User *user);
	bool			is_invited(std::string nick);

	/*channelCommands.class.cpp*/
	void	kick(std::string &line, User &caller, Server &server);
	void	invite(std::string &line, User &caller, Server &server);
	void	topic(std::string &line, User &caller, Server &server);
    void    		add_user(User *user);
    void    		send_to_all_user(std::string msg);
    void   			send_to_others(std::string msg, User *user);
	void			disconnect(User *user, std::string type, std::string reason);
	void			delete_ops(User *user);

	/*channelMode.class.cpp*/
	void	mode(std::string &line, User &caller, Server &server);
	void	mode_o(std::string &line, std::string &opt);
	void	mode_l(std::string &line, std::string &opt);
	void	mode_t(std::string &opt);
	void	mode_i(std::string &opt);
	void	mode_k(std::string &line);

	/*channelGeters.class.cpp*/
	User			&get_user(std::string nick);
	bool			get_pass_bool(void) const;
	size_t			get_size(void);
	size_t			get_max_user(void);
	unsigned int	get_topic_mode(void);
	unsigned int	get_invite_mode(void);
	std::string		get_password(void) const;

private:
    std::string         			_name;
	std::string						_topic;
	std::string						_password;
    std::list<User *>  				_userInChannel;
	std::list<User *>				_operators;
	std::list<std::string>			_user_invited;
	size_t							_max_users;
	bool							_need_pass;

	unsigned int					_topic_mode;
	unsigned int					_invite_mode;
};

#endif