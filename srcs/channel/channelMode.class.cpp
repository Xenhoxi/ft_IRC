/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelMode.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 10:22:07 by smunio            #+#    #+#             */
/*   Updated: 2024/04/29 09:26:53 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void Channel::mode(std::string &line, User &caller, Server &server)
{
	(void)server;
	if (is_operator(caller.get_nick()) == false)
		caller.send_message(":ft_irc 482 " + caller.get_nick() + " " + _name + " :You're not channel operator\r\n");
	if (line.find('-') == std::string::npos && line.find('+') == std::string::npos)
	{
		std::cout << "line: " << line << " //should display channel characteristics" << std::endl;
		throw Error("no MODE opt");
		return ;
		//should display channel characteristics
	}
	std::string opt = line.substr(line.find("MODE") + 6 + this->_name.size(), 2);

	if (opt[1] != 'o' && opt[1] != 'i' && opt[1] != 'l' && opt[1] != 't' && opt[1] != 'k'
		&& opt[0] != '+' && opt[0] != '-')
		throw Error("wrong MODE opt");
	if (opt[1] == 'o')
		this->mode_o(line, opt);
	else if (opt[1] == 'l')
		this->mode_l(line, opt);
	else if (opt[1] == 't')
		this->mode_t(opt);
	else if (opt[1] == 'i')
		this->mode_i(opt);
	else if (opt[1] == 'k')
		this->mode_k(line);
}

void Channel::mode_o(std::string &line, std::string &opt)
{
	std::string	tar = line.substr(line.find("MODE") + 9 + this->_name.size(), line.size());
	std::list<User *>::iterator it;

	if (opt[0] == '-')
		for (it = this->_operators.begin(); it != this->_operators.end(); it++)
		{
			if ((*it)->get_nick() == tar)
			{
					this->_operators.erase(it);
					this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + tar + "\r\n");
					break ;
			}
		}
	else
		for (it = this->_userInChannel.begin(); it != this->_userInChannel.end(); it++)
		{
			if ((*it)->get_nick() == tar)
			{
					this->_operators.push_back(*it);
					this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + tar + "\r\n");
					break ;
			}
		}
}

void    Channel::mode_l(std::string &line, std::string &opt)
{
	if (opt[0] == '+')
	{
		std::string count = line.substr(line.find(opt) + 3, line.size());
		this->_max_users += atoi(count.c_str());
		this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + count + "\r\n");
	}
	if (opt[0] == '-')
	{
		this->_max_users = 0;
		this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + "\r\n");
	}
}

void    Channel::mode_t(std::string &opt)
{
	if (opt[0] == '+')
	{
		this->_topic_mode = TOPIC_ALL;
		this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + "\r\n");
	}
	if (opt[0] == '-')
	{
		this->_topic_mode = TOPIC_OP;
		this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + "\r\n");
	}

}

void    Channel::mode_i(std::string &opt)
{
	if (opt[0] == '+')
	{
		this->_invite_mode = ON_INVITE;
		this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + "\r\n");
	}
	if (opt[0] == '-')
	{
		this->_invite_mode = NO_INVITE;
		this->send_to_all_user(": MODE " + this->_name + " " + opt + " " + "\r\n");
	}
}

void    Channel::mode_k(std::string &line)
{
	// si 
	std::string opt = line.substr(line.find("MODE") + 6 + this->_name.size(), 2);
	if (opt[0] == '+')
	{
		std::string pass = line.substr(line.find(opt) + 3, line.size());
		this->_password = pass;
		std::cout << this->_password << std::endl;
		this->_need_pass = true;
		send_to_all_user(": MODE " + this->_name + " " + opt + " " + pass + "\r\n");
	}
	if (opt[0] == '-')
	{
		std::string pass = line.substr(line.find(opt) + 3, line.size());
		if (pass != this->_password)
			throw Error("can't remove password: Wrong password");
		this->_password.clear();
		this->_need_pass = false;
		send_to_all_user(": MODE " + this->_name + " " + opt + " " + pass + "\r\n");
	}
}
