#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <sys/time.h>

#include "VolumeDetectorEvent.h"
#include "Singleton.h"

using namespace std;

namespace hrfm{ namespace utils{
    
    /*
     
     /Volumes を監視して USB メモリを検知する処理を行います.
     
     */
    class SiVolumeDetector : public Singleton<SiVolumeDetector>, public hrfm::events::EventDispatcher{
        
    public:
        
        void init( vector<string> * ignoreVolumeList = new vector<string>() );
        
        /**
         * /Volumes にディスクが新たにマウントされていないかを調べます.
         */
        void check( int throttle = 3 );
        
        bool isDetected();
        
        string getPath();
        
        void unmount();
        
    private:
        
        friend class Singleton<SiVolumeDetector>;
        
        SiVolumeDetector(){
            hrfm::events::EventDispatcher();
            _path = "";
        }
        
        vector<string> * _ignoreVolumeList = NULL;
        string _path;
        unsigned long int _tick = 0;
        
    };
    
}}