/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:51:51 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/03 22:38:55 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"


// char    *dns_lookup(char *addr_host, struct sockaddr_in *addr_con) 
// { 
//     struct hostent *host_entity; 

//     char *ip = (char*)malloc(NI_MAXHOST*sizeof(char)); 
//     int i; 

//     printf("\nResolving DNS..\n"); 
//     if ((host_entity = gethostbyname(addr_host)) == NULL) 
//         return NULL; 
//     strcpy(ip, inet_ntoa(*(struct in_addr *) 
//                         host_entity->h_addr)); 
//     (*addr_con).sin_family = host_entity->h_addrtype; 
//     (*addr_con).sin_port = htons(PORT_NO); 
//     (*addr_con).sin_addr.s_addr  = *(long*)host_entity->h_addr; 
//     return ip; 
// } 

int     dns_lookup(char *addr_host, struct sockaddr_in *addr_con)
{
    int status;

    if ((status = getaddrinfo(addr_host, NULL, NULL, &addr_con)) < 0)
    {
        printf("Error!\n");
        return status; 
    }
    return status; 
}

int     main(int ac, char **av)
{
    struct sockaddr_in6 addr_con; 

    if (ac != 2)
    {
        printf("usage: ping [-v] host [-h]\n");
        exit(EXIT_SUCCESS);
    }
    int status = dns_lookup(av[0], &addr_con);
    printf("%d\n", status);
    return (0);
}