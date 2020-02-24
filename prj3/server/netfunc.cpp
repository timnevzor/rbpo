#include "netfunc.h"
#include "sampo.h"

atomic_int threadCounter ( 0 );

int CreateListenSocket ( uint32_t addr, uint16_t port )
{
    int Socket = socket ( AF_INET, SOCK_STREAM, 0 );
    if ( Socket == -1 ) throw 1;

    int on = 1;
    int rc = setsockopt ( Socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on );
    if ( rc == -1 ) throw 200;

    sockaddr_in  *server_addr;
    server_addr = new ( sockaddr_in );
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons ( port );
    server_addr->sin_addr.s_addr = htonl ( addr );
    rc = bind ( Socket, ( const sockaddr* ) server_addr, sizeof ( sockaddr_in ) );
    if ( rc == -1 ) throw 2;

    rc = listen ( Socket, 10 );
    if ( rc == -1 ) throw 3;

    return Socket;
}
clientInfo WaitConnect ( int Socket )
{
    clientInfo cl;
    cl.pSocAddr = new ( sockaddr_in );
    socklen_t addrlen = sizeof ( sockaddr_in );
    cl.Socket = accept ( Socket, ( struct sockaddr* ) cl.pSocAddr, &addrlen );
    if ( cl.Socket == -1 ) throw 4;
    return cl;
}

void ServerWorker ( clientInfo cl )
{
    string s;
    char * buffer = new char [BUFLEN];
    timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    Sampo::printTime("Client connect");
#ifdef SYSLOG
    syslog ( LOG_NOTICE, "Connect from %s\n", inet_ntoa ( cl.pSocAddr->sin_addr ) );
#else
    clog << inet_ntoa ( cl.pSocAddr->sin_addr ) << endl;
#endif
    setsockopt ( cl.Socket, SOL_SOCKET, SO_RCVTIMEO, ( const char* ) &tv, sizeof tv );
    int len = 0;
    try {
        len = read ( cl.Socket, buffer, BUFLEN - 1 );
        pSampo->checkAddrAndTime ( ntohl ( cl.pSocAddr->sin_addr.s_addr ) );
        buffer[len] = '\0';
        if ( string ( buffer ) != string ( REQ_STRING ) )
            throw wrong_sampo ( "Invalid sampo request\n" );
        s = FLAG_MESSAGE;
    } catch ( wrong_sampo e ) {
        s = e.what();
    }
    if ( len > 0 )
        SendString ( cl.Socket, s );
#ifdef SYSLOG
    syslog ( LOG_NOTICE, "Send to client: %s", s.data() );
#else
    clog << s;
#endif

    close ( cl.Socket );
    delete cl.pSocAddr;
    threadCounter--;
    return ;
}


void SendString ( int Socket, string s )
{
    int rc = send ( Socket, ( const void* ) s.data(), s.size(), 0 );
    if ( rc == -1 ) throw 10;
}

