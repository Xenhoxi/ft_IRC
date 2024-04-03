/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 18:32:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/03 20:05:53 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP

# define DISCONNECTED 0
# define NEGOTIATION 1
# define REGISTRATION 2
# define CONNECTED 3

#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <string.h>
#include <list>
#include <vector>
#include <exception>
#include <signal.h>
#include "exceptions.hpp"
#include "user.hpp"

#endif