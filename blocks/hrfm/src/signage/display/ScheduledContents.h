#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>
#include "cinder/Xml.h"

#include "DisplayNode.h"
#include "SiEventDispatcher.h"
#include "SequentialContents.h"

#include "ExFbo.h"
#include "SplitFilter.h"

namespace hrfm{ namespace signage{ namespace display{
    
    /*
     
     XML で設定した時刻にコンテンツを切り替え表示するスケジューラークラスです.
     
     */
    class ScheduledContents : public hrfm::display::DisplayNode{
        
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
        
        bool isPlaying();
        
        void play( string type );
        
        void playRecentContent();
        
        void enabled();
        void disabled();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        void _onTimer( hrfm::events::TimeUtilEvent * event );
        
        bool _isPlaying = false;
        
        void _onMovieComplete( hrfm::events::Event * event );
        
    private:
        
        void _clear();
        
        hrfm::display::DisplayNode * _currentContent = NULL;
        
        map<string,hrfm::display::DisplayNode*> _contentList;
        map<string,string> _triggerList;
        
        map<string,Vec2i> _splitList;
        
        bool _enable;
        
        hrfm::gl::ExFbo * _fbo;
        Vec2i mtx;
        
    };
    
}}}