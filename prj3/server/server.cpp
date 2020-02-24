#include "config.h"
#include "sampo.h"
#include "netfunc.h"


cpptoml::local_date defaultStart = {2018, 12, 1};
shared_ptr<cpptoml::table> config;

int main ( int argc, char** argv )
{
#ifdef SYSLOG
    openlog ( "Pervak", LOG_PID, LOG_USER );
#endif
    int debug = 0;
    if ( argc == 2 ) {
        debug = stoi ( argv[1] );
#ifdef SYSLOG
        syslog ( LOG_NOTICE, "Debug class %d to localhost", debug );
#else
        clog << "Debug class " << debug << " to localhost" << endl;
#endif
    }
    try {
        config = cpptoml::parse_file ( CONFIG );
        auto tStart =
            config->get_as<cpptoml::local_date> ( "start" ).value_or ( defaultStart );
        tm tmStart {
            0, 0, 0,
            tStart.day,
            tStart.month - 1,
            tStart.year - 1900,
            0, 0, 0
        };
        time_t t = mktime ( &tmStart );
        localtime_r ( &t, &tmStart );

        startPoint = chrono::system_clock::from_time_t ( t );
        Sampo::printTime ( "The starting point of the schedule", startPoint );

        if ( tmStart.tm_wday != 1 )
            throw wrong_sampo ( "Starting point not a Monday" );

        pSampo = new Sampo ( debug );
        int ListenSocket = CreateListenSocket ( 0, PORT );
        while ( 1 ) {
            clientInfo WorkerInfo = WaitConnect ( ListenSocket );
            if ( threadCounter > THREAD_LIMIT ) {
                close ( WorkerInfo.Socket );
                delete WorkerInfo.pSocAddr;
                continue;
            }
            threadCounter++;
            thread* pThreap = new thread ( ServerWorker, WorkerInfo );
            pThreap->detach();
        }
        close ( ListenSocket );
        return 0;
    } catch ( int& e ) {
#ifdef SYSLOG
        syslog ( LOG_CRIT, "Error %d. Service stopped\n", e );
#else
        clog << "Error " << e << endl;
#endif
        if ( pSampo != nullptr )
            delete pSampo;
        exit ( e );
    } catch ( wrong_sampo& e ) {
#ifdef SYSLOG
        syslog ( LOG_CRIT, "[Error] %s. Service stopped\n", e.what() );
#else
        clog << "[Error] " << e.what() << endl;
#endif
        if ( pSampo != nullptr )
            delete pSampo;
        exit ( 255 );
    }
}
