/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:52:09 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/19 09:56:32 by qpeng            ###   ########.fr       */
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
void    stat_cnt(double rrt)
{
    char    bell_char;

    bell_char = 0x07;
    _g.pkg_received++;
    if (_g.bell)
        write(1, &bell_char, 1);
    if (rrt != -1)
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

void    print_usage(void)
{
    printf("ping: option requires an argument -- h\n");
    printf("usage: ping [-v][-i][-c][-a][-q] host [-h]\n");
    exit(EXIT_SUCCESS);
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
        print_usage();
    }
}

void    bonus_flag(int rcx, int ac, char **av)
{
    if (!strcmp(av[rcx], "-c"))
        arg_check(rcx, ac, av, PING_FLAG_C);
    else if (!strcmp(av[rcx], "-i"))
        arg_check(rcx, ac, av, PING_FLAG_I);
    else if (!strcmp(av[rcx], "-n"))
        _g.r_ns_lookup = false;
    else if (!strcmp(av[rcx], "-a"))
        _g.bell = true;
    else if (!strcmp(av[rcx], "-q"))
        _g.quiet = true;
}

void    readopt(int ac, char **av)
{
    int     rcx;

    rcx = -1;
    while(++rcx < ac)
    {
        if (!strcmp(av[rcx], "-v"))
            _g.verbose = true;
        else if (!strcmp(av[rcx], "-h"))
            print_usage();
        bonus_flag(rcx, ac, av);
        // {
        //     if (!last_arg(rcx, ac))
        //     {
        //         fprintf(stderr, "ping: invalid increment size: %s",\
        //         av[rcx + 1]);   
        //         exit(EXIT_FAILURE);
        //     }
        //     else
                
        // }
    }
}