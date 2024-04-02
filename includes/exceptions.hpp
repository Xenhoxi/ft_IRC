/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smunio <smunio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:28:38 by smunio            #+#    #+#             */
/*   Updated: 2024/04/02 17:36:14 by smunio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_HPP
# define INCLUDE_HPP

#include <iostream>

class Error : public std::exception {
    public :
        const char *    what() const throw() {
            return ("error");
        }
};

#endif