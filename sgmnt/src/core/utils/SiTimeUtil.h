#pragma once

#include <time.h>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

#include "Singleton.h"
#include "TimeUtilEvent.h"

using namespace std;

namespace sgmnt{ namespace utils{
    
    struct TimingData{
    public:
        
        TimingData( int hour, int minutes = 0 );
        ~TimingData(){}
        
        //! 引数に指定した時刻と比較しタイミングと合致した場合 true を返します.
        bool check( int hour, int minutes );
        
    private:
        
        int _hour;
        int _minutes;
        int _beforeHour;
        int _beforeMinutes;
        
    };
    
    class SiTimeUtil : public Singleton<SiTimeUtil>, public sgmnt::events::EventDispatcher{
        
    public:
        
        /*
         
         現在時刻の更新と TimingData を調べてのイベントの発行を行います.
         
         */
        virtual void update();
        
        //! 現在時刻の情報を取得します.
        virtual tm * getTimeinfo();
        
        //! 現在の日を取得します.
        virtual int getDay();
        
        //! 現在の時刻(時)を取得します.
        virtual int getHour();
        
        //! 現在の時刻(分)を取得します.
        virtual int getMinutes();
        
        //! 現在の時刻(秒)を取得します.
        virtual int getSeconds();
        
        //! 現在時刻を引数で指定したフォーマットの文字列にして取得します.
        virtual string getTimeString(const char *format);
        
        //! イベントを発行するタイミングを追加します.
        virtual void addTiming( const string type, TimingData * timing );
        
        virtual void removeTiming( const string type );
        
        virtual void removeTiming( const string type, TimingData * timing );
        
    private:
        
        friend class Singleton<SiTimeUtil>;
        
        SiTimeUtil(){
            
            sgmnt::events::EventDispatcher();
            
            time_t rawtime = time(NULL);
            time(&rawtime);
            _timeinfo = localtime(&rawtime);
            
            update();
            
        }
        
        struct tm * _timeinfo;
        struct tm * _beforeTimeinfo;
        
        map<const string,list<TimingData*>> _timingList;
        
    };
    
}}