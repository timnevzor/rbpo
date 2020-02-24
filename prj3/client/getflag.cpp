#include <string>
#include <stdexcept>
#include <system_error>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "getflag.h"


std::string getFlag ( std::string server )
{
    size_t leftColon = server.find_first_of ( ':' );
    if ( leftColon == std::string::npos )
        throw std::domain_error ( "port missing" );
    size_t rightColon = server.find_last_of ( ':' );
    if ( leftColon != rightColon )
        throw std::domain_error ( "wrong host:port argument" );
    std::string host = server.substr ( 0, leftColon );
    std::string port = server.substr ( leftColon + 1 );
    addrinfo *res, hint = {
        AI_NUMERICSERV,
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP,
        0, 0, 0, 0
    };
    int rc = getaddrinfo ( host.data(), port.data(), &hint, &res );
    if ( rc != 0 ) {
        if ( rc == EAI_SYSTEM )
            throw std::system_error ( std::error_code ( errno, std::generic_category() ),
                                 "getaddrinfo system error" );
        else
            throw rc;
    }
    sockaddr_in self_addr = {0};

    int Socket = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( Socket == -1 )
        throw std::system_error ( std::error_code ( errno, std::generic_category() ),
                                  "socket" );

    rc = bind ( Socket, ( const sockaddr* ) &self_addr, sizeof self_addr );
    if ( rc == -1 )
        throw std::system_error ( std::error_code ( errno, std::generic_category() ),
                                  "bind" );

    rc = connect ( Socket, res->ai_addr, res->ai_addrlen );
    if ( rc == -1 )
        throw std::system_error ( std::error_code ( errno, std::generic_category() ),
                                  "connect" );

    std::string msg = "get flag\n";
    rc = send ( Socket, ( const void* ) msg.data(), msg.size(), 0 );
    if ( rc == -1 )
        throw std::system_error ( std::error_code ( errno, std::generic_category() ),
                                  "send" );

    char answer[1024];
    rc = recv ( Socket, ( void * ) answer, 1023, 0 );
    if ( rc == -1 )
        throw std::system_error ( std::error_code ( errno, std::generic_category() ),
                                  "recv" );
    answer[rc] = '\0';
    close ( Socket );
    freeaddrinfo ( res );
    return answer;
//   return "41baaa356aeb9c536e2f64551d5461a338796ff69f24faf7";
}

