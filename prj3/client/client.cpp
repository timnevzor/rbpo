#include <iostream>
#include <string>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "decryptflag.h"
#include "getflag.h"

#define OPT_DECRYPT "decrypt-flag"
#define OPT_GETFLAG "get-flag"
#define OPT_HELP "help"

using namespace std;

class wrong_option : public logic_error
{
public:
    explicit wrong_option ( const string& what_arg ) : logic_error ( what_arg ) {}
    explicit wrong_option ( const char* what_arg ) : logic_error ( what_arg ) {}
};

int main ( int argc, char **argv )
{
    static option long_opts[] = {
        {OPT_GETFLAG, 1, 0, 'g'},
        {OPT_DECRYPT, 1, 0, 'd'},
        {OPT_HELP, 0, 0, 'h'},
        {0, 0, 0, 0}
    };

    int optIdx;
    int rc;
    try {
        while ( true ) {
            rc = getopt_long ( argc, argv, ":g:d:h", long_opts, &optIdx );
            if ( rc = -1 )
                break;
            switch ( rc ) {
            case 'h':
                cout << "Program for the extraction of the flag. V0.99 (c) IBST" << endl;
                cout << "Usage: pervak option[=arg]" << endl;
                cout << "Options:" << endl;
                cout << "  -h, --help                 this help" << endl;
                cout << "  -g, --get-flag=host:port   get the encrypted flag from the remote host," << endl;
                cout << "                             where port is a tcp port for connection" << endl;
                cout << "  -d, --decrypt-flag=flag    decrypt encrypted flag" << endl;
                exit ( EXIT_SUCCESS );
            case 'g':
                cout << getFlag ( optarg ) << endl;;
                exit ( EXIT_SUCCESS );
            case 'd':
                cout << decryptFlag ( optarg ) << endl;
                exit ( EXIT_SUCCESS );
            case '?':
                throw wrong_option ( "unknown option" );
            case ':':
                throw wrong_option ( "missing argument" );
            }
        }

        throw wrong_option ( "no options" );

    } catch ( invalid_argument e ) {
        string what = e.what();
        string msg;
        if ( rc == 'd' )
            msg = "wrong symbol in the encrypted flag";
        else if ( rc == 'g' )
            msg = "wrong symbol in port number";
        cerr << "Function " << what << " reported: " << msg << endl;
        exit ( EXIT_FAILURE );
    } catch ( length_error e ) {
        cerr << "Encrypted flag error: " << e.what() << endl;
        exit ( EXIT_FAILURE );
    } catch ( domain_error e ) {
        cerr << "Fatal error: " << e.what() << endl;
        exit ( EXIT_FAILURE );
    } catch ( wrong_option e ) {
        cerr << "Options error: " << e.what() << endl;
        cerr << "Try `pervak --help' for more information." << endl;
        exit ( EXIT_FAILURE );
    } catch ( int e ) {
        cerr << "getaddrinfo: " << gai_strerror ( e ) << endl;
        exit ( EXIT_FAILURE );
     } catch ( system_error e ) {
        cerr << e.what() << endl;
        exit ( EXIT_FAILURE );
    }
    return 0;
}
