#include <netinet/in.h> 
#include <netinet/ip.h>
#include <sys/socket.h>
/* for socket, internet domain address AF_INET, in_port_t, sockaddr_in? */
#include <arpa/inet.h> /* for inet_pton, hton */
#include <string.h> /* for memset */

#include "../smkl/io.h" /* for mkdie */
#include "stdio.h" /* for FILE */

#include "../smkl/string.h" /* for mk_atoi, mk_strcat */

#include "practical_socket.h"

#define SOCKET_OK 0
#define DIR_DIST "/home/kinten/Garage/mates-vpk/dist"

typedef struct TCPServer
{
	/* public */
	int socket;
	unsigned short port;
	/* private */
	SocketAddress address;
} TCPServer;

static TCPServer *
init_tcpserver (unsigned short port)
{
	if (!(argc == 2))
		mkdie ("No port specified!");
	TCPServer *server = (TCPServer *) malloc (sizeof (TCPServer));
	server->socket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); /* Address Family: IPv4 (INET) */
	server->port = port;

	server->address.sin_family = AF_INET;
	server->address.sin_port = htons (server_port);
	server->address.sin_addr.s_addr = htons (INADDR_ANY);

	return server;
}

static void
start_tcpserver (TCPServer *server)
{
	bind (server->socket, (SocketAddress *) &server->address, sizeof (server->address));
	listen (server->socket, 5);
	int client_socket;
	while (1)
	{
		
	}
}

static void
close_tcpserver (TCPServer *server)
{

}

int
main (int argc, char *argv[])
{
	FILE *stream_html = fopen (DIR_DIST, "r");
	if (!stream_html)
		mkdie ("Couldn't open index markup file!");
	char stream_response[1024];
	fgets (stream_response, 1024, stream_html);
	char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
	mk_strcat(stream_response, http_header);

	TCPServer *server = init_tcpserver (NULL, NULL, mk_atoi (argv[1]));	
	start_tcpserver (server);
	
	close_tcpserver (server);
}