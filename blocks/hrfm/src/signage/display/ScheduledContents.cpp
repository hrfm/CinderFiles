#include "ScheduledContents.h"

namespace hrfm{ namespace signage{ namespace display{
    
    //! public:
    
    ScheduledContents::ScheduledContents(){
        DisplayNode();
    }
    
    ScheduledContents::ScheduledContents( XmlTree &xml ){
        ScheduledContents();
        init(xml);
    }
    
    ScheduledContents::~ScheduledContents(){}
    
    void ScheduledContents::init( XmlTree &xml ){
        
        _fbo = new hrfm::gl::ExFbo( 256, 256 );
        
        mtx.x = 1;
        mtx.y = 1;
        
        // ---------
        
        cout << "- ScheduledContents::init()" << endl << endl;
        cout << xml << endl;
        
        if( xml.hasChild("settings") ){
            
            XmlTree settings = xml.getChild("settings");
            
            // --- Create Sequence.
            
            XmlTree list = settings.getChild("list");
            
            cout << "--------------------------------" << endl;
            
            // --- Create schedule from xml.
            
            for( XmlTree::Iter item = list.begin(); item != list.end(); ++item ){
                
                string pathStr = item->getAttribute("src").getValue<string>();
                ci::fs::path path;
                
                if( pathStr.find("~/") == 0 ){
                    pathStr.erase(0,2);
                    path = getDocumentsDirectory() / ".." / pathStr;
                }else{
                    path = pathStr;
                }
                
                try{
                    
                    const string type = item->getAttribute("type").getValue<string>();
                    
                    vector<string> pair;
                    string time = item->getAttribute("time").getValue<string>();
                    boost::split( pair, time, boost::is_any_of(":") );
                    
                    string hour = pair.at(0);
                    string min  = pair.at(1);
                    
                    if( type == "pic" ){
                        
                        // 設定されているコンテンツが画像の場合
                        
                        _contentList[time] = new hrfm::display::TextureNode( path );
                        
                    }else if( type == "mov" ){
                        
                        // 設定されているコンテンツが動画の場合
                        
                        ::hrfm::display::MovieTexture * mov = new ::hrfm::display::MovieTexture(path);
                        
                        // loop = true の場合はループ再生する.
                        if( item->hasAttribute("loop") && item->getAttribute("loop").getValue<string>() == "true" ){
                            mov->getMovieGlRef()->setLoop();
                        }else{
                            mov->addEventListener(hrfm::events::Event::COMPLETE, this, &ScheduledContents::_onMovieComplete );
                        }
                        
                        // silent = true の場合は音量を0にする.
                        if( item->hasAttribute("silent") && item->getAttribute("silent").getValue<string>() == "true" ){
                            mov->setSilent( true );
                        }
                        
                        //mov->setLetterbox(true);
                        
                        _contentList[time] = mov;
                        
                    }else if( type == "seq" ){
                        
                        // 設定されているコンテンツがシーケンスの場合
                        
                        XmlTree xml = hrfm::io::XmlLoader::load( path );
                        hrfm::signage::display::SequentialContents * seq = new SequentialContents( xml );
                        
                        _contentList[time] = seq;
                        
                    }else if( type == "blank" ){
                        
                        // 設定されているコンテンツが blank の場合
                        
                        _contentList[time] = new hrfm::display::DisplayNode();
                        
                    }else{
                        
                        // それ以外の場合は無視.
                        
                        continue;
                        
                    };
                    
                    cout << "[ " << hour << ":" << min << " ] " << path << endl;
                    
                    int h, m;
                    
                    if( hour == "*" ){
                        h = -1;
                    }else{
                        h = stoi(hour);
                    }
                    m = stoi(min);
                    
                    if( item->hasAttribute("trigger") ){
                        addSchedule( time, m, h, item->getAttributeValue<string>("trigger") );
                    }else{
                        addSchedule( time, m, h );
                    }
                    
                    if( item->hasAttribute("cols") ){
                        _splitList[time] = ivec2( item->getAttributeValue<int>("cols"), 1 );
                    }else{
                        _splitList[time] = ivec2( 1, 1 );
                    }
                    
                }catch(...){
                    
                    cout << "[ Error ] " << path << endl;
                    
                }
                
            }
            
            _currentContent = NULL;
            
            _enable = true;
            
            cout << "--------------------------------" << endl << endl;
            
        }
        
    }
    
    void ScheduledContents::addSchedule( const string key, int minutes ){
        addSchedule( key, minutes, -1 );
    }
    
    void ScheduledContents::addSchedule( const string key, int minutes, string trigger ){
        addSchedule( key, minutes, -1, trigger );
    }
    
    void ScheduledContents::addSchedule( const string key, int minutes, int hour, string trigger ){
        addSchedule( key, minutes, hour );
        _triggerList[key] = trigger;
    }
    
    void ScheduledContents::addSchedule( const string key, int minutes, int hour ){
        SiTimeUtil::getInstance().addTiming( key, new TimingData( minutes, hour ), this, &ScheduledContents::_onTimer );
    }
    
    int ScheduledContents::numSchedule(){
        return _contentList.size();
    }
    
    bool ScheduledContents::isPlaying(){
        return _isPlaying;
    }
    
    void ScheduledContents::play( string type ){
        
        if( !_enable || _contentList.size() == 0 ){
            _clear();
            return;
        }
        
        if( _contentList.find(type) == _contentList.end() || _currentContent == _contentList[type] ){
            return;
        }
        
        _isPlaying = true;
        
        _clear();
        
        _currentContent = _contentList[type];
        
        mtx.x = _splitList[type].x;
        mtx.y = _splitList[type].y;
        
        if( _currentContent ){
            
            if( ::hrfm::display::MovieTexture * mov = dynamic_cast<::hrfm::display::MovieTexture*>(_currentContent) ){
                mov->play();
            }else if( SequentialContents * seq = dynamic_cast<SequentialContents*>(_currentContent) ){
                seq->play();
            }
            
            //addChild( _currentContent );
            
        }
        
        if( _triggerList.find(type) != _triggerList.end() ){
            hrfm::events::TriggerEvent * evt = new hrfm::events::TriggerEvent( hrfm::events::TriggerEvent::TRIGGER, _triggerList[type] );
            dispatchEvent( evt );
            hrfm::events::SiEventDispatcher::getInstance().dispatchEvent( evt );
        }
        
    }
    
    void ScheduledContents::playRecentContent(){
        
        if( _contentList.size() == 0 ){
            return;
        }
        
        // TODO 設定されているもっとも時間が近いコンテンツを再生する.
        
        std::map<string,hrfm::display::DisplayNode*>::iterator it;
        std::map<string,hrfm::display::DisplayNode*>::iterator end = _contentList.end();
        
        tm * timeinfo = SiTimeUtil::getInstance().getTimeinfo();
        
        int currentTime = stoi( to_string(timeinfo->tm_hour) + to_string(timeinfo->tm_min) );
        int tmpTime = 0;
        string type = "";
        
        int latestTime    = 0;
        string latestType = "";
        
        // Iterate through all functions in the event, from high proproty to low
        for( it=_contentList.begin(); it!=end; ++it ){
            
            string time = it->first;
            vector<string> pair;
            boost::split( pair, time, boost::is_any_of(":") );
            
            int targetTime = stoi(pair.at(0)+pair.at(1));
            
            if( tmpTime < targetTime && targetTime < currentTime){
                tmpTime = targetTime;
                type = time;
            }
            
            if( latestTime < targetTime ){
                latestTime = targetTime;
                latestType = time;
            }
            
        }
        
        if( type != "" ){
            play( type );
        }else{
            play( latestType );
        }
        
    }
    
    void ScheduledContents::enabled(){
        _enable = true;
    }
    
    void ScheduledContents::disabled(){
        _enable = false;
        _clear();
    }
    
    //! protected:
    
    void ScheduledContents::_update(){
        if( _fbo->getSize() != (vec2)getSize() ){
            _fbo = new hrfm::gl::ExFbo( width, height );
        }
        _fbo->beginOffscreen(true);
        {
            if( _currentContent ){
                _currentContent->setSize( width, height);
                _currentContent->update();
                _currentContent->draw();
            }
        }
        _fbo->endOffscreen();
    }
    
    void ScheduledContents::_draw(){
        float w = width / (float)mtx.x;
        float h = height / (float)mtx.y;
        for( int i=0; i<mtx.x; i++ ){
            for( int j=0; j<mtx.y; j++ ){
                ci::gl::draw( _fbo->getTexture(), Rectf( w*i, h*j, w*(i+1), h*(j+1) ) );
            }
        }
    }
    
    void ScheduledContents::_onTimer( events::TimeUtilEvent * event ){
        cout << "SequantialContents::_onTimer("+event->type()+")" << endl;
        play(event->type());
    }
    
    /**
     * If movie loop setting is false.
     * Remove content from DisplayNode.
     */
    void ScheduledContents::_onMovieComplete( hrfm::events::Event * event ){
        if( _currentContent && _currentContent->getParent() == this ){
            //removeChild(_currentContent);
            _currentContent = NULL;
        }
        _fbo->beginOffscreen(true)->endOffscreen();
        this->dispatchEvent(new hrfm::events::Event("moviecomplete"));
    }
    
    //! private
    
    void ScheduledContents::_clear(){
        if( _currentContent ){
            if( ::hrfm::display::MovieTexture * mov = dynamic_cast<::hrfm::display::MovieTexture*>(_currentContent) ){
                mov->stop();
            }else if( SequentialContents * seq = dynamic_cast<SequentialContents*>(_currentContent) ){
                seq->stop();
            }
            //removeChild(_currentContent);
            _currentContent = NULL;
        }
        _fbo->beginOffscreen(true)->endOffscreen();
    }
    
}}}