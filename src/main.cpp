/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/20 22:08:13 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Class/Server.hpp"

int main(int ac, char** av)
{
    if(ac != 3)
    {
        std::cout << "Error : usage : ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    Server serv = Server(av[1], av[2]);
    serv.listenSocket();
    serv.execServer();
}