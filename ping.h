/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:51:22 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/03 20:54:30 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PING_H
# define PING_H

#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <netinet/ip_icmp.h> 
#include <time.h> 
#include <fcntl.h> 
#include <signal.h> 
#include <stdbool.h>
#include <errno.h>
#include <sys/time.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <resolv.h>
#include <netdb.h> 
#include <netinet/in.h>

bool g_running = true;
# define PING_TTL 51
# define PING_PKG_SIZ 56
# define PING_TIMEOUT 4
struct s_ping_conf
{
    int ttl_val;
    int pkg_siz;
    int time_out;
    bool run;
};

typedef struct s_ping_pkt 
{ 
    struct icmphdr  hdr; 
    char            msg[PING_PKG_SIZ - sizeof(struct icmphdr)]; 
}               t_ping_pkt;
 
struct s_ping_conf g_conf;
#endif