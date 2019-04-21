/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:09 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/20 22:11:11 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

const char   *g_err_msg_fmt[] = {
    "ping: invalid timing interval:", 
    "ping: invalid count of packets to transmit:"
};

const char   g_flag_lookup[] = {
    'c',
    'i'
};

uint16_t    in_cksum (uint16_t * addr, int len)
{
    uint32_t    sum;
    uint16_t    answer;
    
    answer = 0;
    sum = 0;
    while (len > 1)
    {
        sum += *addr++;
        len -= 2;
    }
    if (len == 1)
    {
        * (unsigned char *) (&answer) = * (unsigned char *) addr;
        sum += answer;
     }
    sum = (sum >> 16) + (sum & 0xffff); 
    sum += (sum >> 16); 
    answer = ~sum; 
    return (answer);
}

void    stat_cnt(double rrt)
{
    _g.pkg_received++;
    if (rrt != NOT_ECHO)
    {
        _g.total += rrt;
        if (rrt < _g.min || _g.min == 0.0000)
            _g.min = rrt;
        if (rrt > _g.max)
            _g.max = rrt;
    }
}

char* reverse_dns_lookup(char *ip_addr) 
{ 
    struct sockaddr_in  temp_addr;     
    socklen_t           len; 
    char                buf[100];
    static char         ret_buf[200]; 
  
    temp_addr.sin_family = AF_INET; 
    temp_addr.sin_addr.s_addr = inet_addr(ip_addr); 
    len = sizeof(struct sockaddr_in); 
    if (getnameinfo((struct sockaddr *) &temp_addr, len, buf,  
                    sizeof(buf), NULL, 0, NI_NAMEREQD))  
        return (ip_addr); 
    strcpy(ret_buf, buf); 
    return ret_buf; 
}

void    arg_check(int rcx, int ac, char **av, int flag)
{
    if (!last_arg(rcx, ac))
    {
        if (atoi(av[rcx + 1]) != 0)
        {
            if (flag == PING_FLAG_C)
                _g.times = atoi(av[rcx + 1]);
            else if (flag == PING_FLAG_I)
                _g.duration = atoi(av[rcx + 1]);
        }
        else
        {
            fprintf(stderr, "%s `%s`", g_err_msg_fmt[flag], av[rcx + 1]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "ping: option requires an argument -- %c\n"\
        , g_flag_lookup[flag]);
        PRINT_USAGE;
    }
}

void    readopt(int ac, char **av)
{
    int     rcx;

    rcx = 0;
    while(++rcx < ac)
    {
        if (!strcmp(av[rcx], "-v"))
            _g.verbose = true;
        else if (!strcmp(av[rcx], "-h"))
            PRINT_USAGE;
        else if (!strcmp(av[rcx], "-c"))
            arg_check(rcx, ac, av, PING_FLAG_C);
        else if (!strcmp(av[rcx], "-i"))
            arg_check(rcx, ac, av, PING_FLAG_I);
        else if (!strcmp(av[rcx], "-n"))
            _g.r_ns_lookup = false;
        else if (!strcmp(av[rcx], "-a"))
            _g.bell = true;
        else if (!strcmp(av[rcx], "-q"))
            _g.quiet = true;
        else if ((strcmp(av[rcx - 1], "-c") && strcmp(av[rcx - 1], "-i")))
            _g.host = strcmp(av[rcx], "0") == 0 ? "localhost" : av[rcx];
    }
}