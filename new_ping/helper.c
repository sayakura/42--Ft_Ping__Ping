#include "ping.h"


void    stat_cnt(double rrt)
{
    _g.pkg_received++;
    if (rrt)
    {
        _g.total += rrt;
        if (rrt < _g.min)
            _g.min = rrt;
        else if (rrt > _g.max)
            _g.max = rrt;
    }
}