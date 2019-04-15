#include "ping.h"


void    stat_cnt(double rrt)
{
    _g.pkg_received++;
    if (rrt != 0)
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