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

#include <netinet/in.h>
char    *reverse_dns_lookup(char *ip_addr) 
{ 
    struct sockaddr_i   _temp;     
    socklen_t           len; 
    char                buf[NI_MAXHOST];
    char                *ret_buf; 
  
    _temp.sin_family = AF_INET; 
    _temp.sin_addr.s_addr = inet_addr(ip_addr); 
    len = sizeof(struct sockaddr_in); 
    if (getnameinfo((struct sockaddr *) &_temp, len, buf,  
                    sizeof(buf), NULL, 0, NI_NAMEREQD))  
    { 
        perror("getnameinfo");
        return NULL; 
    }
    ret_buf = (char*)malloc((strlen(buf) +1)*sizeof(char) ); 
    strcpy(ret_buf, buf); 
    return ret_buf; 
} 

char    *lookup_host (const char *host)
{
    struct addrinfo   hints;
    struct addrinfo   *res;
    static char       addrstr[100];
    void              *ptr;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;
    if (getaddrinfo(host, NULL, &hints, &res) < 0)
    {
        perror ("getaddrinfo");
        return (NULL);
    }
    if (res)
    {
        inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);
        if (res->ai_family == AF_INET)
            ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
        else if (res->ai_family == AF_INET6)
            ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
        inet_ntop (res->ai_family, ptr, addrstr, 100);
        printf ("IPv%d address: %s (%s)\n", res->ai_family == PF_INET6 ? 6 : 4,
              addrstr, res->ai_canonname);
        return (addrstr);
    }
    else
        return (NULL);
}

int     main(int ac, char **av)
{
    char    *ip;

    if (ac != 2)
    {
        printf("usage: ping [-v] host [-h]\n");
        exit(EXIT_SUCCESS);
    }
    ip = lookup_host(av[1]);
    if (ip == NULL)
       printf("ping: %s: Name or service not known\n", av[1]);
    printf("%s\n", ip);
    return (0);
}