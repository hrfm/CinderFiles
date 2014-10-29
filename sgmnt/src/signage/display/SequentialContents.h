#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>
#include "cinder/Xml.h"
#include "Sequence.h"
#include "TimeUtilEvent.h"

namespace sgmnt{ namespace signage{ namespace display{
    
    /*
     
     XML で設定した順番にコンテンツを切り替え表示するスケジューラークラスです.
     
     */
    class SequentialContents : public sgmnt::display::DisplayNode, public sgmnt::events::EventDispatcher{
        
    public:
        
        SequentialContents();
        SequentialContents( ci::XmlTree &xml );
        ~SequentialContents();
        
        void init( ci::XmlTree &xml );
        
        void addContent( sgmnt::display::IDrawable * content, float time );
        
        void play( int index = 0 );
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        void _onComplete( sgmnt::events::Event * event );
        
    private:
        
        //! 現在表示しているコンテンツの _contentList 上の index.
        int _currentIndex;
        
        //! 現在表示しているコンテンツへのポインタ参照.
        Sequence * _currentSequence;
        
        //! 表示するコンテンツのリスト.
        vector<Sequence*> _sequenceList;
        
    };
    
}}}