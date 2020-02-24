#include "sampo.h"

chrono::minutes one_para = 1h + 35min;
chrono::hours one_day = 24h;
chrono::minutes para[6] = {
    8h,
    9h + 50min,
    11h + 40min,
    13h + 45min,
    15h + 35min,
    17h + 25min
};
chrono::system_clock::time_point startPoint;
Sampo * pSampo = nullptr;

vector<string> weekdays = {"mon", "tue", "wed", "thu", "fri", "sat"};

Sampo::Sampo ( int debug )
{
    map<string, string> defAddr = {
        {"205", SUBNET_205},
        {"216", SUBNET_216},
        {"301", SUBNET_301},
        {"308", SUBNET_308}
    };
    for ( LabInfo & lab : compLabs ) {
        auto table = config->get_table ( lab.name );
        lab.addr =
            convAddr ( table->get_as<string> ( "addr" ).value_or ( defAddr[lab.name] ) );
        lab.mask =
            convAddr ( table->get_as<string> ( "mask" ).value_or ( MASK ) );
#ifdef SYSLOG
        syslog ( LOG_NOTICE, "[%s]\n", lab.name.data() );
        syslog ( LOG_NOTICE, "addr: 0x%08X\n", lab.addr );
        syslog ( LOG_NOTICE, "mask: 0x%08X\n", lab.mask );
#else
        clog << '[' << lab.name << ']' << endl;
        clog << hex;
        clog << "addr: 0x" << lab.addr << endl;
        clog << "mask: 0x" << lab.mask << endl;
        clog << dec;
#endif
        for ( size_t wday = 0; wday < weekdays.size(); ++wday ) {
            auto arr = table->get_array_of<int64_t> ( weekdays[wday] );
            for ( size_t i = 0; i < arr->size(); ++i )  {
                lab.shedule.push_back (
                    addPoint ( para[ arr->at ( i ) - 1 ] + one_day *  wday  )
                );
            }
        }
    }
    if ( debug != 0 ) {
        compLabs[debug - 1].addr = 0x7f000001;
        compLabs[debug - 1].mask = 0xffffffff;
    }
    printTime ( "Server start" );
}

void Sampo::printTime ( const char* about, chrono::system_clock::time_point t )
{
    time_t tt;
    tt = chrono::system_clock::to_time_t ( t );
#ifdef SYSLOG
    syslog ( LOG_NOTICE, "%s at %s\n", about, ctime ( &tt ) );
#else
    clog << about << " at " << ctime ( &tt );
#endif
}

Sampo::~Sampo()
{
    for ( LabInfo & lab : compLabs ) {
        for ( auto p : lab.shedule ) {
            delete p.t_begin;
            delete p.t_end;
        }
    }
}

Sampo::Period Sampo::addPoint ( chrono::minutes disp )
{
    Period p;
    p.t_begin = new chrono::system_clock::time_point ( startPoint + disp );
    p.t_end = new chrono::system_clock::time_point ( *p.t_begin + one_para );
    printTime ( "Sampo start", *p.t_begin );
    return p;
}

void Sampo::checkAddrAndTime ( uint32_t addr )
{
    for ( LabInfo lab : compLabs ) {
        if ( ( lab.mask & addr ) == lab.addr ) {
            checkTime ( lab.shedule );
            return;
        }
    }
    throw wrong_sampo ( "Wrong sampo ip address\n" );
}

void Sampo::checkTime ( list<Period> lPeriod )
{
    chrono::system_clock::time_point t = chrono::system_clock::now();
    for ( auto p : lPeriod ) {
        if ( t < * ( p.t_begin ) )
            throw wrong_sampo ( "Wrong sampo time\n" );
        else if ( t < *p.t_end )
            return;
    }
    throw wrong_sampo ( "Wrong sampo time\n" );
}

uint32_t Sampo::convAddr ( string s )
{
    in_addr tmp;
    inet_aton ( s.data(), &tmp );
    return ntohl ( tmp.s_addr );
}
