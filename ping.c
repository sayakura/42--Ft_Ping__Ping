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
int     dns_lookup(char *addr_host, void **ptr)
{
    struct addrinfo *res;
    struct addrinfo hints;
  char addrstr[100];

    memset (&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_RAW;
    if (getaddrinfo(addr_host, NULL, &hints, &res) < 0)
    {
        perror("getaddrinfo");
        return (-1);
    }
    while (res)
    {
        inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);
        if (res->ai_family == AF_INET || res->ai_family == AF_INET6)
            memcpy(*ptr, (struct sockaddr *)res->ai_addr,\
            sizeof(struct sockaddr));
        inet_ntop (res->ai_family, ptr, addrstr, 100);
        printf ("IPv%d address: %s (%s)\n", res->ai_family == PF_INET6 ? 6 : 4,
              addrstr, res->ai_canonname);
        if (*ptr)
            return (res->ai_family == AF_INET ? 4 : 6);
        res = res->ai_next;
    }
    return (0); 
}

int     main(int ac, char **av)
{
    void    *ptr;
    int     int_type;

    if (ac != 2)
    {
        printf("usage: ping [-v] host [-h]\n");
        exit(EXIT_SUCCESS);
    }
    ptr = malloc(sizeof(struct sockaddr));
    int_type = dns_lookup(av[1], &ptr);
    if (status == 0)
       printf("ping: %s: Name or service not known\n", av[1]);
    printf("%d\n", status);
    return (0);
}