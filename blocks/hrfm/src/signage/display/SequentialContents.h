#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>
#include "cinder/Xml.h"

#include "hrfm.h"
#include "Sequence.h"
#include "IEnableTransition.h"

namespace hrfm{ namespace signage{ namespace display{
    
    class Sequence;
    
    /*
     
     XML で設定した順番にコンテンツを切り替え表示するスケジューラークラスです.
     
     */
    class SequentialContents : public hrfm::display::DisplayNode, public IEnableTransition{
        
    public:
        
        SequentialContents();
        SequentialContents( ci::XmlTree &xml );
        ~SequentialContents();
        
        void init( ci::XmlTree &xml );
        
        void addContent( hrfm::display::DisplayNode * content, float time );
        void addContent( hrfm::display::DisplayNode * content, float time, string trigger );
        
        void play( int index = 0 );
        void stop();
        
        bool isPlaying();
        
        int numSequence();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        void _onComplete( hrfm::events::Event * event );
        
    private:
        
        bool _isPlaying;
        
        //! 現在表示しているコンテンツの _contentList 上の index.
        int _currentIndex;
        
        //! 現在表示しているコンテンツへのポインタ参照.
        Sequence * _currentSequence;
        
        //! 表示するコンテンツのリスト.
        vector<Sequence*> _sequenceList;
        
    };
    
}}}