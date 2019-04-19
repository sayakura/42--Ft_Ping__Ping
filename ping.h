/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:51:22 by qpeng             #+#    #+#             */
/*   Updated: 2019/04/19 09:49:07 by qpeng            ###   ########.fr       */
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
#include <netinet/icmp6.h>

# define XOR(a) a ^= a
# define PING_TTL 255
# define PING_PKG_SIZ 56
# define PING_TIMEOUT 4
# define BUFF_SIZE 1024
# define IP_HDR_LEN 20
# define ICMP_HDR_KEN 8
# define DATALEN 56
# define PCKSIZE(datalen) (DATALEN + IP_HDR_LEN + ICMP_HDR_KEN)
# define last_arg(index, ac) (index != ac - 1)
# define PING_FLAG_C 0
# define PING_FLAG_I 1
// # define FETAL(msg, arg) ({\
//             fprintf(stderr, msg); \
//             fprintf(stderr, arg); \
//             exit(EXIT_FAILURE);\
//         })

struct              s_pin_g
{
    struct sockaddr     *ssend;
    struct sockaddr     *srecv;
    struct iovec        iov;
    struct icmp         icmp;
    struct msghdr       msg;
    struct timeval      _tv_start;
    struct timeval      _tv_end;
    double              min;
    double              max;
    double              avg;
    double              total;
    void                (*ft_send)(void);
    void                (*ft_recv)(int, char *);
    
    int                 times;
    int                 duration;
   
    int                 ssendlen;                 
    unsigned int        seq;
    int                 datalen;
    int                 ttl_val;
    int                 pkg_siz;
    int                 time_out;
    int                 sockfd;
    unsigned int        pkg_received;
    unsigned int        msg_cnt;
    pid_t               pid;

    char                *r_host;
    char                ip[100];
    char                *host;
    char                sendbuf[BUFF_SIZE];
    char                recvbuf[BUFF_SIZE];
    char                ctrl_buf[BUFF_SIZE];

    bool                verbose;
    bool                r_ns_lookup;
    bool                bell;
    bool                quiet;
};

struct s_pin_g       _g;

char                *lookup_host (const char *host);
char                *reverse_dns_lookup(char *ip_addr);
u_int8_t            get_checksum(uint16_t *b, int len);
double              caltime(struct timeval end, struct timeval start);
struct addrinfo *   host_to_addrinfo(const char *host, const char *serv, int family, int socktype);
void                sig_alrm(int signo);
void                readloop(void);
void                stat_cnt(double rrt);
void                tv_sub(struct timeval *out, struct timeval *in);
void                send_v4(void);
void                send_v6(void);
void                readmsg_v4(int b_read, char *recvbuff);
void                readmsg_v6(int b_read, char *recvbuff);
void                sig_int(int signo);
void                readopt(int ac, char **av);
#endif