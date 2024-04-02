/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:10:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/02 18:49:47 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <poll.h>

class User {
    public:
        User();
        ~User();
        void            set_nickname(std::string name);
		void            set_fds(int server_socket);
		struct pollfd   *get_fds() const;
    private:
        std::string     nickname;
        struct pollfd   *fds;
};

#endif