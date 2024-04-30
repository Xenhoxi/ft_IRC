/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 18:32:01 by smunio            #+#    #+#             */
/*   Updated: 2024/04/30 15:01:35 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP

# define DISCONNECTED 	0
# define NEGOTIATION 	1
# define CONNECTED 		2
# define LEAVING 		3
# define MAIN_SOCKET    4

# define TOPIC_ALL		0
# define TOPIC_OP		1

# define NO_INVITE      0
# define ON_INVITE      1

#include <stdio.h>
#include <stdlib.h>
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
#include <time.h>
#include <ctime>
#include <map>
#include <sstream>
#include <signal.h>
#include "user.hpp"
#include "exceptions.hpp"
#include "server.hpp"
#include "channel.hpp"

#endif