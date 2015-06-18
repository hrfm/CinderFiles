#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>
#include "cinder/Xml.h"

#include "DisplayNode.h"
#include "ImageTexture.h"
#include "MovieTexture.h"

#include "Sequence.h"
#include "Transition.h"

namespace hrfm{ namespace signage{ namespace display{
    
    class Transition;
    
    class Sequence;
    
    /*
     
     XML で設定した順番にコンテンツを切り替え表示するスケジューラークラスです.
     
     */
    class SequentialContents : public hrfm::display::DisplayNode{
        
    public:
        
        SequentialContents();
        ~SequentialContents();
        
        int numSequence();
        
        bool isPlaying();
        
        virtual void setSize( int w, int h );
        
        void setLoop( bool flag = true );
        
        void setTransition( Transition * transition );
        
        void addContent( ci::fs::path filepath, float time, string trigger = "" );
        void addContent( hrfm::display::DisplayNode * content, float time, string trigger = "" );
        
        void play( int index = 0 );
        void stop();
        
        Sequence * getCurrentSequence();
        
        void clear();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        // ファイルパスから、それに応じたシーケンス用の DisplayNode を生成します.
        // ファイルがどのようなファイルかは、拡張子から自動的に判断されます.
        hrfm::display::DisplayNode * _createContent( ci::fs::path filepath, bool isLoop = true );
        // ファイルパスとタイプから、それに応じたシーケンス用の DisplayNode を生成します.
        hrfm::display::DisplayNode * _createContent( ci::fs::path filepath, string type, bool isLoop = true );
        
        void _onComplete( hrfm::events::Event * event );
        void _onTransitionComplete( hrfm::events::Event * event );
        
    private:
        
        bool _isLoop = false;
        
        //! 現在表示しているコンテンツの _contentList 上の index.
        int _currentIndex;
        //! 現在表示しているコンテンツへのポインタ参照.
        Sequence * _currentSequence = NULL;
        //! 表示するコンテンツのリスト.
        vector<Sequence*> _sequenceList;
        
        //! 表示切り替え時のトランジション.
        bool _runningTransition;
        Transition * _transition = NULL;
        
    
    public:
        // 消すかも
        SequentialContents( ci::XmlTree &xml );
        void init( ci::XmlTree &xml );
        
    };
    
}}}