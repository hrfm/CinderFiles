#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>

#include "cinder/app/App.h"
#include "cinder/Xml.h"

#include "sgmnt.h"
#include "SequentialContents.h"
#include "IEnableTransition.h"
#include "SiEventDispatcher.h"

namespace sgmnt{ namespace signage{ namespace display{
    
    /*
     
     XML で設定した時刻にコンテンツを切り替え表示するスケジューラークラスです.
     
     */
    class ScheduledContents : public sgmnt::display::DisplayNode, public IEnableTransition{
        
    public:
        
        ScheduledContents();
        ScheduledContents( ci::XmlTree &xml );
        ~ScheduledContents();
        
        void init( ci::XmlTree &xml );
        
        void addSchedule( const string key, int minutes );
        void addSchedule( const string key, int minutes, string trigger );
        
        void addSchedule( const string key, int minutes, int hour );
        void addSchedule( const string key, int minutes, int hour, string trigger );
        
        int numSchedule();
        
        void play( string type );
        
        void playRecentContent();
        
        bool isPlaying();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        void _onTimer( sgmnt::events::TimeUtilEvent * event );
        
        bool _isPlaying;
        
    private:
        
        sgmnt::display::IDrawable * _currentContent;
        
        map<string,sgmnt::display::IDrawable*> _contentList;
        map<string,string> _triggerList;
        
    };
    
}}}