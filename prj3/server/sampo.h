#pragma once

#include "config.h"


class wrong_sampo: public logic_error
{
public:
    explicit wrong_sampo ( const string& what_arg ) : logic_error ( what_arg ) {}
    explicit wrong_sampo ( const char* what_arg ) : logic_error ( what_arg ) {}
};

class Sampo
{
private:
    struct Period {
        chrono::system_clock::time_point *t_begin;
        chrono::system_clock::time_point *t_end;
    };
    struct LabInfo {
        string   name;
        uint32_t addr;
        uint32_t mask;
        list<Period> shedule;
        LabInfo ( string _name ): name ( _name ) {};
    };
    enum Labs { lab205 = 0, lab216, lab301, lab308};

    vector <LabInfo> compLabs  = {
        LabInfo ( "205" ),
        LabInfo ( "216" ),
        LabInfo ( "301" ),
        LabInfo ( "308" )
    };

    Period addPoint ( chrono::minutes disp );
    void checkTime ( list<Period> lPeriod );
    static uint32_t convAddr(string s);

public:
    Sampo ( int debug = 0 );
    ~Sampo();
    void checkAddrAndTime ( uint32_t addr );
    static void printTime (
        const char* about = "",
        chrono::system_clock::time_point t = chrono::system_clock::now()
    );


};

extern chrono::system_clock::time_point startPoint;
extern Sampo * pSampo;
extern shared_ptr<cpptoml::table> config;
