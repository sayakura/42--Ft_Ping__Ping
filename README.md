# 42--Ft_Ping__Ping
Recode linux command ping. 

# Usage

#### Pre
```sh
$ make
```
#### ping a host (result not shown)
```sh
$ ./ft_ping google.com 
$ ./ft_ping localhost
```

#### ping a address (result not shown)
```sh
$ ./ft_ping 127.0.0.1
$ ./ft_ping ::1
```

### Flags
| Command | Description |
| ------ | ------ |
| -h | [Print usage] |
| -v | [Verbose mode, print received non-icmp packet] |
| -c | [Ping a host a specific number of times] |
| -i | [Ping `host`, specifying the interval in `seconds` between requests (default is 1 second)] |
| -n | [Ping `host` without trying to lookup symbolic names for addresses] |
| -a | [Ping `host` and ring the bell when a packet is received (if your terminal supports it)] |
| -t | [Set the ping ttl value] |

### Bonus Feature
- [x] 6 extra flags. 
- [x] IP6 support.
