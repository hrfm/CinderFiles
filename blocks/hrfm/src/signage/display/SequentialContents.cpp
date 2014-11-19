#include "SequentialContents.h"

using namespace ci;
using namespace std;
using namespace hrfm::events;
using namespace hrfm::utils;

namespace hrfm{ namespace signage{ namespace display{
    
    //! public:
    
    SequentialContents::SequentialContents(){
        DisplayNode();
    }
    SequentialContents::SequentialContents( XmlTree &xml ){
        SequentialContents();
        init(xml);
    }
    
    SequentialContents::~SequentialContents(){
        
    }
    
    void SequentialContents::init( XmlTree &xml ){
        
        _currentIndex      = 0;
        _isSetTransition   = false;
        
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
                    
                    hrfm::display::DisplayNode * content;
                    
                    if( type == "pic" ){
                        
                        // 設定されているものが画像の場合
                        
                        content = new hrfm::display::ImageTexture( loadImage(path) );
                        
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
                        
                        content = new hrfm::display::DisplayNode();
                        
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
    
    void SequentialContents::addContent( DisplayNode * content, float time ){
        Sequence * seq = new Sequence( content, time );
        seq->addEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onComplete );
        _sequenceList.push_back( seq );
    }
    
    void SequentialContents::addContent( DisplayNode * content, float time, string trigger ){
        Sequence * seq = new Sequence( content, time );
        seq->setTrigger( trigger );
        seq->addEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onComplete );
        _sequenceList.push_back( seq );
    }
    
    void SequentialContents::setTransition( hrfm::signage::display::Transition * transition ){
        cout << "SequentialContents::setTransition" << endl;
        _isSetTransition = true;
        _transition = transition;
    }
    
    void SequentialContents::setSize( int w, int h ){
        if( _isSetTransition && ( width != w || height != h ) ){
            DisplayNode::setSize( w, h );
            _transition->setSize( w, h );
        }
    }
    
    void SequentialContents::play( int index ){
        
        _isPlaying = true;
        
        cout << "SequentialContents::play(" << index << ")" << endl;
        
        // シーケンスの設定自体が存在しない場合はリターン.
        if( _sequenceList.size() == 0 ){
            return;
        }
        
        // index の値がはみ出ないように丸め込む.
        if( _sequenceList.size() <= index ){
            _currentIndex = _sequenceList.size() - 1;
        }else if( index < 0 ){
            _currentIndex = 0;
        }
        
        // Transition の設定の有無に応じて修正.
        if( _isSetTransition == true ){
            
            // トランジションの準備.
            _transition->prepare();
            
            // 現在表示中のコンテンツがある場合,描画ツリーから外しつつ Transition に委譲.
            if( _currentSequence ){
                _currentSequence->stop();
                removeChild(_currentSequence->getContentRef());
                _transition->setCurrent( _currentSequence->getContentRef() );
            }
            
            // 次に表示をするコンテンツを設定し Transition に委譲する.Transition が完了するまで addChild はしない.
            _currentSequence = _sequenceList.at(index);
            if( _currentSequence ){
                _currentSequence->play();
                _transition->setNext( _currentSequence->getContentRef() );
            }
            _currentIndex = index;
            
            // Transition の終了を Listen しつつトランジションを開始.
            _transition->addEventListener( hrfm::events::Event::COMPLETE , this, &SequentialContents::_onTransitionComplete );
            
            _transition->start(1.3f);
            
        }else{
            
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
            _currentSequence->setSize( width, height );
        }
        if( _isSetTransition && _transition->running() ){
            _transition->update();
        }
    }
    
    void SequentialContents::_draw(){
        if( _isSetTransition && _transition->running() ){
            ci::gl::draw( _transition->getTexture(), getDrawBounds() );
        }
    }
    
    void SequentialContents::_onComplete( hrfm::events::Event * event ){
        
        cout << "SequantialContents::_onComplete("+event->type()+")" << endl;
        
        int nextIndex = _currentIndex + 1;
        if( _sequenceList.size() <= nextIndex ){
            nextIndex = 0;
            this->dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
        }
        
        play( nextIndex );
        
    }
    
    void SequentialContents::_onTransitionComplete( hrfm::events::Event * event ){
        
        cout << "SequentialContents::_onTransitionComplete()" << endl;
        
        _transition->removeEventListener( hrfm::events::Event::COMPLETE, this, &SequentialContents::_onTransitionComplete );
        addChild( _currentSequence->getContentRef() );
        
    }
    
}}}