#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>

#include "cinder/Xml.h"

#include "DisplayNode.h"
#include "Texture.h"
#include "MovieTexture.h"
#include "EventDispatcher.h"
#include "TimeUtilEvent.h"
#include "Utils.h"

namespace sgmnt{ namespace signage{ namespace display{
    
    /*
     
     XML で設定した時刻にコンテンツを切り替え表示するスケジューラークラスです.
     
     */
    class ScheduledContents : public sgmnt::display::DisplayNode, public sgmnt::events::EventDispatcher{
        
    public:
        
        ScheduledContents();
        ScheduledContents( ci::XmlTree &xml );
        ~ScheduledContents();
        
        void init( ci::XmlTree &xml );
        
        void addSchedule( const string key, int hour, int minutes );
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        void _onTimer( sgmnt::events::TimeUtilEvent * event );
        
    private:
        
        sgmnt::display::IDrawable * _currentContent;
        
        map<string,sgmnt::display::IDrawable*> _contentList;
        
    };
    
}}}