#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>

#include "cinder/app/App.h"
#include "cinder/Xml.h"

#include "sgmnt.h"
#include "SequentialContents.h"
#include "IEnableTransition.h"

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
        
        void addSchedule( const string key, int hour, int minutes );
        
        int numSchedule();
        
        void play( string type );
        
        void playRecentContent();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        void _onTimer( sgmnt::events::TimeUtilEvent * event );
        
    private:
        
        sgmnt::display::IDrawable * _currentContent;
        
        map<string,sgmnt::display::IDrawable*> _contentList;
        
    };
    
}}}