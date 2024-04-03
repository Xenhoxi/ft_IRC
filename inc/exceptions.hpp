/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:28:38 by smunio            #+#    #+#             */
/*   Updated: 2024/04/03 23:13:08 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_HPP
# define INCLUDE_HPP

#include <iostream>

class Error : public std::exception 
{
    const char *_msg;
    public :
        Error(const char *msg) : _msg(msg) {};  
        const char *    what() const throw() {
            return (_msg);
        }
};

#endif