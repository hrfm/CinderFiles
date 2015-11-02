#include "SequentialContents.h"

namespace hrfm{ namespace signage{ namespace display{
    
    //! public:
    
    SequentialContents::SequentialContents(){
        DisplayNode();
    }
    SequentialContents::~SequentialContents(){}
    
    int SequentialContents::numSequence(){
        return _sequenceList.size();
    }
    
    bool SequentialContents::isPlaying(){
        return _currentSequence != NULL || _runningTransition;
    }
    
    void SequentialContents::setSize( int w, int h ){
        DisplayNode::setSize( w, h );
        if( _transition && ( width != w || height != h ) ){
            _transition->setSize( w, h );
        }
        if( _currentSequence != NULL ){
            _currentSequence->setSize(w,h);
        }
    }
    
    void SequentialContents::setLoop( bool flag ){
        _isLoop = flag;
    }
    
    void SequentialContents::setTransition( hrfm::signage::display::Transition * transition ){
        _transition = transition;
    }
    
    void SequentialContents::addContent( ci::fs::path filepath, float time, string trigger ){
        hrfm::display::DisplayNode * content = _createContent(filepath);
        if( content != NULL ){
            cout << "SequencialContents[" << numSequence() << "] " << filepath.native() << endl;
            addContent( content, time, trigger );
        }
    }
    
    void SequentialContents::addContent( DisplayNode * content, float time, string trigger ){
        Sequence * seq = new Sequence( content, time );
        if( trigger != "" ){
            seq->setTrigger( trigger );
        }
        _sequenceList.push_back( seq );
    }
    
    void SequentialContents::play( int index ){
        
        cout << "SequentialContents::play(" << index << ")" << endl;
        
        int num = numSequence();
        // シーケンスの設定自体が存在しない場合はリターン.
        if( num == 0 ){ return; }
        // index の値がはみ出ないように丸め込む.
        if( num <= index ){
            _currentIndex = _sequenceList.size() - 1;
        }else if( index < 0 ){
            _currentIndex = 0;
        }
        
        cout << "play" << endl;
        
        // Transition の設定の有無に応じて修正.
        if( _transition != NULL ){
            
            // トランジションの準備.
            _transition->prepare();
            
            // 現在表示中のコンテンツがある場合,描画ツリーから外しつつ Transition に委譲.
            if( _currentSequence ){
                _currentSequence->stop();
                _currentSequence->removeEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onComplete );
                removeChild(_currentSequence->getContentRef());
                _transition->setCurrent( _currentSequence->getContentRef() );
            }
            
            // 次に表示をするコンテンツを設定し Transition に委譲する.Transition が完了するまで addChild はしない.
            _currentSequence = _sequenceList.at(index);
            if( _currentSequence ){
                _currentSequence->setSize( width, height );
                _currentSequence->addEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onComplete );
                _currentSequence->play();
                _transition->setNext( _currentSequence->getContentRef() );
            }
            _currentIndex = index;
            
            // Transition の終了を Listen しつつトランジションを開始.
            _transition->addEventListener( hrfm::events::Event::COMPLETE , this, &SequentialContents::_onTransitionComplete );
            _transition->start();
            
        }else{
            
            if( _currentSequence ){
                _currentSequence->stop();
                _currentSequence->removeEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onComplete );
                removeChild(_currentSequence->getContentRef());
            }
            
            _currentSequence = _sequenceList.at(index);
            
            if( _currentSequence ){
                _currentSequence->setSize( width, height );
                _currentSequence->addEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onComplete );
                _currentSequence->play();
                addChild( _currentSequence->getContentRef() );
                this->dispatchEvent( new SequenceEvent(SequenceEvent::PLAY) );
            }
            
            _currentIndex = index;
            
        }
        
    }
    
    void SequentialContents::stop(){
        if( _currentSequence ){
            _currentSequence->stop();
            removeChild(_currentSequence->getContentRef());
        }
        _currentSequence = NULL;
    }
    
    void  SequentialContents::setVolume( float volume ){
        _volume = volume;
    }
    float SequentialContents::getVolume(){
        return _volume;
    }
    
    Sequence * SequentialContents::getCurrentSequence(){
        return _currentSequence;
    }
    
    void SequentialContents::clear(){
        stop();
        _sequenceList.clear();
        _sequenceList.empty();
        _currentIndex    = 0;
        _currentSequence = NULL;
    }
    
    //! protected:
    
    void SequentialContents::_update(){
        if( _currentSequence ){
            _currentSequence->setSize( width, height );
            _currentSequence->setVolume( _volume );
        }
        if( _transition && _transition->running() ){
            cout << "update transition" << endl;
            _transition->update();
        }else if( _currentSequence ){
            _currentSequence->update();
        }
    }
    
    void SequentialContents::_draw(){
        if( _transition && _transition->running() ){
            ci::gl::draw( _transition->getTexture(), getDrawBounds() );
        }
    }
    
    hrfm::display::DisplayNode * SequentialContents::_createContent( ci::fs::path filepath, bool isLoop, bool isSilent ){
        string type = hrfm::utils::getFileType( filepath );
        if( type == hrfm::utils::FILE_TYPE_PIC || type == hrfm::utils::FILE_TYPE_MOV ){
            return _createContent( filepath, type, isLoop, isSilent );
        }else{
            return NULL;
        }
    }
    
    hrfm::display::DisplayNode * SequentialContents::_createContent( ci::fs::path filepath, string type, bool isLoop, bool isSilent ){
        
        string   pathStr = filepath.string();
        fs::path path;
        
        try{
            
            if( pathStr.find("~/") == 0 ){
                pathStr.erase(0,2);
                path = getDocumentsDirectory() / ".." / pathStr;
            }else{
                path = pathStr;
            }
            
            if( type == "pic" ){
                // 設定されているものが画像の場合
                hrfm::display::ImageTexture * imgTex = new hrfm::display::ImageTexture(path);
                imgTex->setLetterbox(true);
                return imgTex;
            }else if( type == "mov" ){
                // 設定されている者が動画の場合
                hrfm::display::MovieTexture * mov = new hrfm::display::MovieTexture(path);
                if( isLoop ){ mov->getMovieGlRef()->setLoop(); }
                if( isSilent ){ mov->setSilent(true); }
                mov->setLetterbox(true);
                return mov;
            }else if( type == "seq" ){
                // 設定されているコンテンツがシーケンスの場合
                XmlTree xml = XmlLoader::load( path );
                hrfm::signage::display::SequentialContents * seq = new SequentialContents( xml );
                //if( isLoop ){ seq->setLoop(); }
                return seq;
            }else if( type == "blank" ){
                // 設定されているコンテンツが blank の場合
                return new hrfm::display::DisplayNode();
            }
            
        }catch(...){}
        
        return NULL;
        
    }
    
    void SequentialContents::_onComplete( hrfm::events::Event * event ){
        cout << "SequantialContents::_onComplete("+event->type()+")" << endl;
        int nextIndex = _currentIndex + 1;
        if( numSequence() <= nextIndex ){
            if( _isLoop == true ){
                nextIndex = 0;
                this->dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
            }else{
                stop();
                return;
            }
        }
        play( nextIndex );
    }
    
    void SequentialContents::_onTransitionComplete( hrfm::events::Event * event ){
        cout << "SequentialContents::_onTransitionComplete()" << endl;
        _transition->removeEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onTransitionComplete );
        addChild( _currentSequence->getContentRef() );
        this->dispatchEvent( new SequenceEvent(SequenceEvent::PLAY) );
    }
    
    
    
    
    
    
    
    
    
    
    // --- 消すかも
    
    SequentialContents::SequentialContents( XmlTree &xml ){
        SequentialContents();
        init(xml);
    }
    
    void SequentialContents::init( XmlTree &xml ){
        
        clear();
        
        cout << "- SequentialContents::init(xml)" << endl << endl;
        
        cout << xml << endl;
        
        if( xml.hasChild("settings") ){
            
            XmlTree settings = xml.getChild("settings");
            
            // --- Create Sequence.
            
            XmlTree list = settings.getChild("list");
            
            cout << "--------------------------------" << endl;
            
            int index = 0;
            
            for( XmlTree::Iter item = list.begin(); item != list.end(); ++item ){
                
                string pathStr    = item->getAttribute("src").getValue<string>();
                const string type = item->getAttribute("type").getValue<string>();
                float time        = item->getAttribute("time").getValue<float>();
                bool isLoop       = item->hasAttribute("loop") && item->getAttribute("loop").getValue<string>() == "true";
                bool isSilent     = item->hasAttribute("silent") && item->getAttribute("silent").getValue<string>() == "true";
                
                hrfm::display::DisplayNode * content = _createContent( pathStr, type, isLoop, isSilent );
                
                if( content != NULL ){
                    cout << "[ " << index++ <<  " ] " << time << " -> " << pathStr << endl;
                    if( item->hasAttribute("trigger") ){
                        addContent( content, time, item->getAttributeValue<string>("trigger") );
                    }else{
                        addContent( content, time );
                    }
                }
                
            }
            
            _currentSequence = NULL;
            
            cout << "--------------------------------" << endl << endl;
            
        }
        
    }
    
}}}