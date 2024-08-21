/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/21 16:44:23 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Class/Server.hpp"

int main(int ac, char** av)
{
    if(ac != 3)
    {
        std::cout << RED "Error : socket and password are required." RESET << std::endl;
        return 0;
    }
    try
    {
        Server serv = Server(av[1], av[2]);
        serv.listenSocket();
        serv.execServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

}