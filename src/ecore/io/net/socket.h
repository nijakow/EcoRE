#ifndef ECO_IO_NET_SOCKET_H
#define ECO_IO_NET_SOCKET_H

int Eco_Net_Socket_OpenServerV4(int, int);
int Eco_Net_Socket_AcceptClientV4(int);
unsigned int Eco_Net_Socket_Read(int, char*, unsigned int);

#endif
