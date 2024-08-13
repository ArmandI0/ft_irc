/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:29 by aranger           #+#    #+#             */
/*   Updated: 2024/08/13 21:37:59 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string port, std::string password) :  _password(password)
{
    this->_server_infos.sin_port = htons(std::atoi(port.c_str()));
    this->_server_infos.sin_family = AF_INET;
    this->_server_infos.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

Server::~Server()
{
    
}

void    Server::listenSocket()
{
    int status;
	int log = 10;	//nb max de connexion

    /* CREER LE SOCKET D'ECOUTE */

    this->_listen_socket = socket(this->_server_infos.sin_family, SOCK_STREAM, 0);
    if (this->_listen_socket == -1)
    {
        std::cerr << "ERREUR : il faut throw une execption" << std::endl;
        return ;
    }

    /* ECOUTE SUR LE SOCKET CREE */
    
    status = bind(this->_listen_socket, (struct sockaddr *)&(this->_server_infos), sizeof this->_server_infos);
    if (status != 0)
    {
        std::cerr << "ERREUR : il faut throw une execption" << std::endl;
        return ;
    }
	status = listen(this->_listen_socket, log);
    if (status != 0)
	{
        std::cerr << "ERREUR : il faut throw une execption" << std::endl;
        return ;
    } 

    /* CREATION STRUCT POUR EPOLL*/
    struct epoll_event ev;
    ev.events = EPOLLIN;  // Par exemple, surveiller les événements de lecture (EPOLLIN)
    ev.data.fd = this->_listen_socket;

    /* CREATION EPOLL*/
    this->_epoll_socket = epoll_create1(0);
    if (this->_epoll_socket == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE); 
    }

    /* AJOUT FD A SURVEILLER TO EPOLL */
    if (epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, this->_listen_socket, &ev) == -1)
    {
        perror("epoll_ctl: EPOLL_CTL_ADD");
        exit(EXIT_FAILURE);
    }

    struct epoll_event evs[10];
    int ret;
    while(1)
    {
        ret = epoll_wait(this->_epoll_socket, evs, 10, -1);
        if (ret == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        /* gestion des events */
        for (int i = 0; i < ret; i++)
        {
            if (evs[i].events & EPOLLIN)
            {
				// Gérer la lecture sur le socket_fd
				// Par exemple : accepter une nouvelle connexion ou lire des données
				int client_fd = accept(this->_listen_socket, NULL, NULL);
				if (client_fd == -1) {
					perror("accept");
					continue;
				}
				std::cout << "connexion etablie fd =" << client_fd << std::endl;
        	}
    	}
	}
	std::cout << "CA MARCHE PAS SI TU ES LA " <<std::endl;
}

void	Server::execServer()
{

    // epoll_wait()
    
    // struct sockaddr_storage client_addr; 
    // socklen_t addr_size;
    // int client_fd;
    
    // addr_size = sizeof client_addr;
    // client_fd = accept(this->_listen_socket, (struct sockaddr *)&client_addr, &addr_size);
    // if (client_fd == -1) 
    // {
    //     return ;
    // }
    // printf("Accepted new connection on client socket fd: %d\n", client_fd);

}

