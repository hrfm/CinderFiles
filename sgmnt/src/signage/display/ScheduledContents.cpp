#include "ScheduledContents.h"

using namespace ci;
using namespace std;
using namespace sgmnt::display;
using namespace sgmnt::events;
using namespace sgmnt::utils;

namespace sgmnt{ namespace signage{ namespace display{
    
    //! public:
    
    ScheduledContents::ScheduledContents(){
        IEnableTransition();
        DisplayNode();
    }
    ScheduledContents::ScheduledContents( XmlTree &xml ){
        ScheduledContents();
        init(xml);
    }
    
    ScheduledContents::~ScheduledContents(){
        
    }
    
    void ScheduledContents::init( XmlTree &xml ){
        
        cout << "- ScheduledContents::init()" << endl << endl;
        cout << xml << endl;
        
        if( xml.hasChild("settings") ){
            
            XmlTree settings = xml.getChild("settings");
            
            // --- Create Sequence.
            
            XmlTree list = settings.getChild("list");
            
            cout << "--------------------------------" << endl;
            
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
                    
                    vector<string> pair;
                    string time = item->getAttribute("time").getValue<string>();
                    boost::split( pair, time, boost::is_any_of(":") );
                    
                    int hour = stoi(pair.at(0));
                    int min  = stoi(pair.at(1));
                    
                    if( type == "pic" ){
                        
                        // 設定されているコンテンツが画像の場合
                        
                        _contentList[time] = new sgmnt::display::Texture( loadImage(path) );
                        
                    }else if( type == "mov" ){
                        
                        // 設定されているコンテンツが動画の場合
                        
                        MovieTexture * mov = new MovieTexture(path);
                        if( item->hasAttribute("loop") && item->getAttribute("loop").getValue<string>() == "true" ){
                            mov->getMovieGlRef()->setLoop();
                        }
                        
                        _contentList[time] = mov;
                        
                    }else if( type == "seq" ){
                        
                        // 設定されているコンテンツがシーケンスの場合
                        
                        XmlTree xml = XmlLoader::load( path );
                        sgmnt::signage::display::SequentialContents * seq = new SequentialContents( xml );
                        
                        _contentList[time] = seq;
                        
                    }else if( type == "blank" ){
                        
                        // 設定されているコンテンツが blank の場合
                        
                        _contentList[time] = new sgmnt::display::IDrawable();
                        
                    }else{
                        
                        // それ以外の場合は無視.
                        
                        continue;
                        
                    };
                    
                    cout << "[ " << hour << ":" << min << " ] " << path << endl;
                    
                    addSchedule( time, hour, min );
                    
                }catch(Exception e){
                    
                    cout << "[ Error ] " << path << endl;
                    
                }
                
            }
            
            _currentContent = NULL;
            
            cout << "--------------------------------" << endl << endl;
            
        }
        
    }
    
    void ScheduledContents::addSchedule( const string key, int hour, int minutes ){
        SiTimeUtil::getInstance().addTiming( key, new TimingData( hour, minutes ), this, &ScheduledContents::_onTimer );
    }
    
    //! protected:
    
    void ScheduledContents::_update(){
        if( _currentContent ){
            _currentContent->setSize(width, height);
        }
    }
    
    void ScheduledContents::_draw(){}
    
    void ScheduledContents::_onTimer( TimeUtilEvent * event ){
        
        cout << "SequantialContents::_onTimer("+event->type()+")" << endl;
        
        if( _currentContent ){
            if( MovieTexture * mov = dynamic_cast<MovieTexture*>(_currentContent) ){
                mov->stop();
            }else if( SequentialContents * seq = dynamic_cast<SequentialContents*>(_currentContent) ){
                seq->stop();
            }
            removeChild(_currentContent);
        }
        
        _currentContent = _contentList[event->type()];
        
        if( _currentContent ){
            
            if( MovieTexture * mov = dynamic_cast<MovieTexture*>(_currentContent) ){
                mov->play();
            }else if( SequentialContents * seq = dynamic_cast<SequentialContents*>(_currentContent) ){
                seq->play();
            }
            
            addChild( _currentContent );
            
        }
        
    }
    
}}}