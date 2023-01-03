#include <netinet/in.h> 
#include <netinet/ip.h>
#include <sys/socket.h>
/* for socket, internet domain address AF_INET, in_port_t, sockaddr_in? */
#include <arpa/inet.h> /* for inet_pton, hton */
#include <string.h> /* for memsest */

#include "../smkl/string.h" /* for mk_atoi */
#include "practical_socket.h"

#define SOCKET_OK 0

typedef struct TCPClient
{
	/* public */
	int socket;
	int target_ip;
	int target_port;
	/* private */
	SocketAddress address;
} TCPClient;

static void
init_tcpclient (TCPClient *server, int port)
{

}

int
main (int argc, char *argv[])
{
	char *server_ip = argv[1];
	char *echo_msg = argv[2];
	in_port_t target_port = (argc == 4) ? mk_atoi (argv[3]) : 7;

	int client_socket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); /* Address Family: IPv4 (INET) */
	
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons (server_port);
	inet_pton (AF_INET, server_ip, server_addr.sin_family.s_addr); /* convert IPv4/6 string into binary */
	
	connect (tcp_socket, (struct sockaddr *) &server_addr, );


}