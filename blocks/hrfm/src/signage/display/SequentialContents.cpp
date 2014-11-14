#include "SequentialContents.h"

using namespace ci;
using namespace std;
using namespace hrfm::events;
using namespace hrfm::utils;

namespace hrfm{ namespace signage{ namespace display{
    
    //! public:
    
    SequentialContents::SequentialContents(){
        DisplayNode();
        IEnableTransition();
        _currentIndex = 0;
    }
    SequentialContents::SequentialContents( XmlTree &xml ){
        SequentialContents();
        init(xml);
    }
    
    SequentialContents::~SequentialContents(){
        
    }
    
    void SequentialContents::init( XmlTree &xml ){
        
        cout << "- SequentialContents::init()" << endl << endl;
        cout << xml << endl;
        
        if( xml.hasChild("settings") ){
            
            XmlTree settings = xml.getChild("settings");
            
            // --- Create Sequence.
            
            XmlTree list = settings.getChild("list");
            
            cout << "--------------------------------" << endl;
            
            int index = 0;
            
            for( XmlTree::Iter item = list.begin(); item != list.end(); ++item ){
                
                string pathStr = item->getAttribute("src").getValue<string>();
                fs::path path;
                
                if( pathStr.find("~/") == 0 ){
                    pathStr.erase(0,2);
                    path = getDocumentsDirectory() / ".." / pathStr;
                }else{
                    path = pathStr;
                }
                
                try{
                    
                    const string type = item->getAttribute("type").getValue<string>();
                    float time = item->getAttribute("time").getValue<float>();
                    
                    hrfm::display::IDrawable * content;
                    
                    if( type == "pic" ){
                        
                        // 設定されているものが画像の場合
                        
                        content = new hrfm::display::Texture( loadImage(path) );
                        
                    }else if( type == "mov" ){
                        
                        // 設定されている者が動画の場合
                        
                        hrfm::display::MovieTexture * mov = new hrfm::display::MovieTexture(path);
                        if( item->hasAttribute("loop") && item->getAttribute("loop").getValue<string>() == "true" ){
                            mov->getMovieGlRef()->setLoop();
                        }
                        content = mov;
                        
                    }else if( type == "seq" ){
                        
                        // 設定されているコンテンツがシーケンスの場合
                        
                        XmlTree xml = XmlLoader::load( path );
                        hrfm::signage::display::SequentialContents * seq = new SequentialContents( xml );
                        content = seq;
                        
                    }else if( type == "blank" ){
                        
                        // 設定されているコンテンツが blank の場合
                        
                        content = new hrfm::display::IDrawable();
                        
                    }else{
                        
                        // それ以外の場合は無視.
                        
                        continue;
                        
                    };
                    
                    cout << "[ " << index++ <<  " ] " << time << " -> " << path << endl;
                    
                    if( item->hasAttribute("trigger") ){
                        addContent( content, time, item->getAttributeValue<string>("trigger") );
                    }else{
                        addContent( content, time );
                    }

                }catch(...){}
                
            }
            
            _currentSequence = NULL;
            
            cout << "--------------------------------" << endl << endl;
            
        }
        
    }
    
    void SequentialContents::addContent( IDrawable * content, float time ){
        Sequence * seq = new Sequence( content, time );
        seq->addEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onComplete );
        _sequenceList.push_back( seq );
    }
    
    void SequentialContents::addContent( IDrawable * content, float time, string trigger ){
        Sequence * seq = new Sequence( content, time );
        seq->setTrigger( trigger );
        seq->addEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onComplete );
        _sequenceList.push_back( seq );
    }
    
    void SequentialContents::play( int index ){
        
        _isPlaying = true;
        
        cout << "SequentialContents::play(" << index << ")" << endl;
        
        cout << _sequenceList.size() << endl;
        
        if( _sequenceList.size() == 0 ){
            return;
        }
        
        if( _sequenceList.size() <= index ){
            _currentIndex = _sequenceList.size() - 1;
        }else if( index < 0 ){
            _currentIndex = 0;
        }
        
        if( _currentSequence ){
            _currentSequence->stop();
            removeChild(_currentSequence->getContentRef());
        }
        
        _currentSequence = _sequenceList.at(index);
        
        if( _currentSequence ){
            _currentSequence->play();
            addChild( _currentSequence->getContentRef() );
        }
        
        _currentIndex = index;
        
    }
    
    void SequentialContents::stop(){
        if( _currentSequence ){
            _currentSequence->stop();
            removeChild(_currentSequence->getContentRef());
        }
        _currentSequence = NULL;
        _isPlaying = false;
    }
    
    bool SequentialContents::isPlaying(){
        return _isPlaying;
    }
    
    int SequentialContents::numSequence(){
        return _sequenceList.size();
    }
    
    //! protected:
    
    void SequentialContents::_update(){
        if( _currentSequence ){
            _currentSequence->update();
            _currentSequence->setSize( _width, _height );
        }
    }
    
    void SequentialContents::_draw(){
        
    }
    
    void SequentialContents::_onComplete( hrfm::events::Event * event ){
        
        cout << "SequantialContents::_onTimer("+event->type()+")" << endl;
        
        int nextIndex = _currentIndex + 1;
        if( _sequenceList.size() <= nextIndex ){
            nextIndex = 0;
            this->dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
        }
        
        play( nextIndex );
        
    }
    
}}}