/*
 * endian_conv.c
 * Written by SW. YOON
 */

#include <stdio.h>

int main(int argc, char **argv)
{
	short host_port_order = 0x1234;
	short net_port_order;
	
	long host_add_order = 0x12345678;
	long net_add_order;
	
	net_port_order = htons(host_port_order);
	net_add_order = htonl(host_add_order);
	
	printf(" Host ordered port :%x \n", host_port_order);
	printf(" Network ordered port : %x \n\n", net_port_order);
	
	printf(" Host ordered Address : %x \n", host_add_order);
	printf(" Network ordered Address : %x \n\n", net_add_order);
	return 0;
}
