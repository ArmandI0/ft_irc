/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/28 11:18:24 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Class/Server.hpp"

int main(int ac, char** av)
{
    if(ac != 3)
    {
		std::cerr << "Error : usage : ./ft_irc <port> <password> " << std::endl;
        return 0;
    }
    try
    {   
        std::string port(av[1]);
        std::string password(av[2]);
        char* end;
        errno = 0;
        unsigned long p = strtoul(port.c_str(), &end, 10);
        if (end == port.c_str() || *end != '\0' || errno == ERANGE || p > 65535)
        {
            std::cerr << "Error: Invalid port." << std::endl;
            return 1;
        }
        if (password.size() > 20 || password.size() < 8 || password.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{}|;:',.<>/?") != std::string::npos)
        {
            std::cerr << "Error: Your password must be between 8 and 20 characters long and can only include letters, numbers, and valid special characters (!, @, #, $, %, ^, &, *)." << std::endl;
            return 1;
        }
        Server serv = Server(p, password);
        serv.listenSocket();
        serv.execServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << RED << e.what() << RESET << std::endl;
    }
    return 0;
}