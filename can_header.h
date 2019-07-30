#ifndef can_header
#define can_header
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include "curl/curl.h"
#include <string.h>


 
void canserver(void);
#endif







