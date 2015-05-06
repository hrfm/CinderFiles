#include "SiVolumeDetector.h"

using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace utils{
    
    // ======================================================================
    // SiTimeUtil class.
    
    void SiVolumeDetector::check( int throttle ){
        
        unsigned long int now = time(NULL);
        
        // TODO やり方がダサいので直したい
        
        if( throttle < now - _tick ){
            
            try{
                boost::filesystem::path dir("/Volumes");
                if( boost::filesystem::exists(dir) && boost::filesystem::is_directory(dir) && !boost::filesystem::is_empty(dir) ){
                    string volume = "";
                    boost::filesystem::directory_iterator endItr;
                    for (boost::filesystem::directory_iterator p(dir); p != endItr; ++p) {
                        if (boost::filesystem::is_directory(*p)) {  //ディレクトリの時
                            string filename = p->path().filename().string();
                            if( filename != "Macintosh HD" ){
                                volume = p->path().string();
                                break;
                            }
                        }
                    }
                    if( volume == "" && _path != "" ){
                        _path = volume;
                        dispatchEvent(new VolumeDetectorEvent(VolumeDetectorEvent::UNMOUNTED));
                    }else if( _path != volume ){
                        _path = volume;
                        dispatchEvent( new VolumeDetectorEvent( VolumeDetectorEvent::DETECTED ) );
                    }
                }else{
                    cout << "path is not correct" << endl;
                }
            }catch(...){
                cout << "error" << endl;
            }
            
            _tick = now;
            
        }
        
    }
    
    string SiVolumeDetector::getPath(){
        return _path;
    }
    
    void SiVolumeDetector::unmount(){
        try{
            if( _path != "" ){
                dispatchEvent(new VolumeDetectorEvent(VolumeDetectorEvent::BEFORE_UNMOUNT));
                system( ("diskutil unmount \""+_path+"\"").c_str() );
            }
        }catch(...){}
    }
    
}}