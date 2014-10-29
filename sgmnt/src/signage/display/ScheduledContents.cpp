#include "ScheduledContents.h"

using namespace ci;
using namespace std;
using namespace sgmnt::events;
using namespace sgmnt::utils;

namespace sgmnt{ namespace signage{ namespace display{
    
    //! public:
    
    ScheduledContents::ScheduledContents(){
        DisplayNode();
        EventDispatcher();
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
                        
                        // 設定されているものが画像の場合
                        
                        _contentList[time] = new sgmnt::display::Texture( loadImage(path) );
                        
                    }else if( type == "mov" ){
                        
                        // 設定されている者が動画の場合
                        
                        sgmnt::display::MovieTexture * mov = new sgmnt::display::MovieTexture(path);
                        if( item->getAttribute("loop").getValue<string>() == "true" ){
                            mov->getMovieGlRef()->setLoop();
                        }
                        mov->getMovieGlRef()->play();
                        
                        _contentList[time] = mov;
                        
                    }else{
                        
                        // それ以外の場合は無視.
                        
                        continue;
                        
                    };
                    
                    cout << "[ " << hour << ":" << min << " ] " << path << endl;
                    
                    addSchedule( time, hour, min );
                    
                }catch(...){}
                
            }
            
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
    
    void ScheduledContents::_draw(){
    }
    
    void ScheduledContents::_onTimer( TimeUtilEvent * event ){
        
        cout << "SequantialContents::_onTimer("+event->type()+")" << endl;
        
        if( _currentContent ){
            removeChild(_currentContent);
        }
        
        _currentContent = _contentList[event->type()];
        
        if( _currentContent ){
            addChild( _currentContent );
        }
        
    }
    
}}}