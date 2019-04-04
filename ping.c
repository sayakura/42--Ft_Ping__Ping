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

void intHandler(int __) 
{ 
    g_conf.run = false; 
} 

char    *reverse_dns_lookup(char *ip_addr) 
{ 
    struct sockaddr_in  _temp;     
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
        return (addrstr);
    }
    else
        return (NULL);
}

void    ping(int sockfd, struct sockaddr_in *ser_add, char *r_host, char *ip, char *host)
{
    t_ping_pkt pckt;

    g_conf.run = 1;
    printf("PING %s (%s) %d bytes of data.\n", host, ip, g_conf.pkg_siz);
    while (g_conf.run)
    {
        bzero(&pckt, sizeof(pckt));
        pckt.hdr.type = ICMP_ECHO; 
        pckt.hdr.un.echo.id = getpid(); 
        
    }
}

int     sock_init(int sock_id, struct sockaddr_in *ser_addr, char *ip)
{
    ser_addr->sin_family = AF_INET;
    ser_addr->sin_port = 0;
    ser_addr->sin_addr.s_addr = inet_addr(ip);
    if (setsockopt(sock_id, SOL_IP, IP_TTL,  
               &g_conf.ttl_val, sizeof(g_conf.ttl_val)) != 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // setsockopt(sock_id, SOL_SOCKET, SO_RCVTIMEO, 
    //                (const char*)&tv_out, sizeof tv_out); 
}

void   ping_init(char *host)
{
    static struct sockaddr_in   *ser_addr;
    char                        *ip;
    char                        *r_host;
    int                         sockfd;

    g_conf.pkg_siz = PING_PKG_SIZ;
    g_conf.time_out = PING_TIMEOUT;
    g_conf.ttl_val = PING_TTL;
    ip = lookup_host(host);
    if (ip == NULL)
       printf("ping: %s: Name or service not known\n", host);
    r_host = reverse_dns_lookup(ip);
    if (sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, intHandler);
    sock_init(sockfd, ser_addr, ip);
    ping(sockfd, &ser_addr, r_host, ip, host); 
}

int     main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("usage: ping [-v] host [-h]\n");
        exit(EXIT_SUCCESS);
    }
    ping_init(av[1]);
    return (0);
}