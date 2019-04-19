/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:51:54 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/19 11:13:49 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

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


void    send_v4(void)
{
    int     len;
    struct  icmp *icmp;

    icmp = (struct icmp *)_g.sendbuf;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = _g.pid;
    icmp->icmp_seq = _g.msg_cnt++;
    memset(icmp->icmp_data, 0xa5, DATALEN);
    gettimeofday((struct timeval *)icmp->icmp_data, NULL);
    len = 8 + DATALEN;
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = in_cksum((u_short *)icmp, len);
    sendto(_g.sockfd, _g.sendbuf, len, 0, _g.ssend, _g.ssendlen);
}

void    send_v6(void)
{
    int                 len;
    struct  icmp6_hdr   *icmp;

    icmp = (struct icmp6_hdr *)_g.sendbuf;
    icmp->icmp6_type = ICMP6_ECHO_REQUEST;
    icmp->icmp6_code = 0;
    icmp->icmp6_id = _g.pid;
    icmp->icmp6_seq = _g.msg_cnt++;
    memset((icmp + 1), 0xa5, DATALEN);
    gettimeofday((struct timeval *)(icmp + 1), NULL);
    len = 8 + DATALEN;
    sendto(_g.sockfd, _g.sendbuf, len, 0, _g.ssend, _g.ssendlen);
}

// void    readmsg_v6(int b_read, char *recvbuff)
// {
//     struct icmp6_hdr    *icmp6;
//     struct timeval      tvrecv;
//     struct cmsghdr      *cmsg;
//     int                 hlim;
//     double              rrt;

//     gettimeofday(&tvrecv, NULL);
//     icmp6 = (struct ip *)recvbuff;
//     if (len < 8)
//         return ;
//     if (icmp6->icmp6_type == ICMP6_ECHO_REPLY)
//     {
//         if (icmp6->icmp6_id != _g.pid || len < 16)
//             return ;
//         tv_sub(&tvrecv, (struct timeval *)icmp6 + 1);
//         rrt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
//         hlim = -1;
//         cmsg = CMSG_FIRSTHDR(&_g.msg);
//         while (cmsg != NULL)
//         {
//              if (cmsg->cmsg_level == IPPROTO_IPV6
//                     && cmsg->cmsg_type == IPV6_HOPLIMIT)
//                 {
//                      hlim = * (u_int32_t *) CMSG_DATA (cmsg);
//                      break;
//                 }
//             cmsg = CMSG_NXTHDR(&msg, cmsg);
//         }
//         stat_cnt(rrt);
//         if (!_g.quiet)
//             printf("%d bytes from %s (%s): icmp_seq=%u, hlim=%d, time=%.3f ms\n",
//                 b_read, _g.r_host ? _g.r_host : _g.host, _g.ip,  icmp6->icmp6_seq, hlim, rrt);
//     }
//     else if (_g.verbose)
//     {
//         stat_cnt(0);
//         if (!_g.quiet)
//             printf(" %d bytes from %s (%s): type = %d, code = %d\n",
//                 b_read, _g.host, _g.ip, icmp6->icmp6_type, icmp6->icmp6_code);
//     }
// }

static void print_msg(int b_recv, struct icmp *icmp, double rrt, int ttl, bool is_echo)
{
    char    bell_char;

    bell_char = 0x07;
    stat_cnt(is_echo ? rrt : NOT_ECHO);
    if (!_g.quiet)
    {
        if (is_echo)
        {
            if (_g.r_ns_lookup)
                printf("%d bytes from %s (%s): icmp_seq=%u, ttl=%d, time=%.3f ms\n",
                        b_recv, _g.r_host ? _g.r_host : _g.host, _g.ip, icmp->icmp_seq, ttl, rrt);
            else
                printf("%d bytes from %s: icmp_seq=%u, ttl=%d, time=%.3f ms\n",
                        b_recv, _g.ip, icmp->icmp_seq, ttl, rrt);
        }
        else
            printf(" %d bytes from %s (%s): type = %d, code = %d\n",
                b_recv, _g.host, _g.ip, icmp->icmp_seq, icmp->icmp_code);
        if (_g.bell)
            write(1, &bell_char, 1);
    }
    if (!_g.times)
        sig_int(SIGINT);
}

void    readmsg_v4(int b_read, char *recvbuff)
{
    struct ip       *iphdr;
    struct icmp     *icmp;
    struct timeval  tvrecv;
    int             hdrlen;
    double          rrt;

    gettimeofday(&tvrecv, NULL);
    iphdr = (struct ip *)recvbuff;
    hdrlen = iphdr->ip_hl << 2;
    if (iphdr->ip_p != IPPROTO_ICMP)
        return ;
    icmp = (struct icmp *)(recvbuff + hdrlen);
    if ((b_read - hdrlen) < 8)
        return ;
    if (icmp->icmp_type == ICMP_ECHOREPLY)
    {
        if (icmp->icmp_id != _g.pid || (b_read - hdrlen) < 16)
            return ;
        tv_sub(&tvrecv, (struct timeval *)icmp->icmp_data);
        rrt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
        print_msg(b_read - hdrlen, icmp, rrt, iphdr->ip_ttl, true);
    }
    else if (_g.verbose)
        print_msg(b_read - hdrlen, icmp, 0, 0, false);
}


void    readmsg_v6(int b_read, char *recvbuff)
{
    struct ip       *iphdr;
    struct icmp     *icmp;
    struct timeval  tvrecv;
    int             hdrlen;
    double          rrt;

    gettimeofday(&tvrecv, NULL);
    iphdr = (struct ip *)recvbuff;
    hdrlen = iphdr->ip_hl << 2;
    if (iphdr->ip_p != IPPROTO_ICMP)
        return ;
    icmp = (struct icmp *)(recvbuff + hdrlen);
    if ((b_read - hdrlen) < 8)
        return ;
    if (icmp->icmp_type == ICMP_ECHOREPLY)
    {
        if (icmp->icmp_id != _g.pid || (b_read - hdrlen) < 16)
            return ;
        tv_sub(&tvrecv, (struct timeval *)icmp->icmp_data);
        rrt = tvrecv.tv_sec * 1000.0 + tvrecv.tv_usec / 1000.0;
        stat_cnt(rrt);
        if (!_g.quiet)
            printf("%d bytes from %s (%s): icmp_seq=%u, ttl=%d, time=%.3f ms\n",
                (b_read - hdrlen), _g.r_host ? _g.r_host : _g.host, _g.ip,  icmp->icmp_seq, iphdr->ip_ttl, rrt);
    }
    else if (_g.verbose)
    {
        stat_cnt(0);
        if (!_g.quiet)
            printf(" %d bytes from %s (%s): type = %d, code = %d\n",
                (b_read - hdrlen), _g.host, _g.ip, icmp->icmp_type, icmp->icmp_code);
    }
}