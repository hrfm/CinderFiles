#pragma once

#include <map>
#include "Singleton.h"
#include "cinder/gl/Texture.h"

using namespace std;

namespace hrfm{ namespace utils{
    
    /*
     
     std::map を拡張し Singleton で何処からでも特定の Texture にアクセス出来るようにするクラスです.
     
     */
    class SiDisplayUtil : public Singleton<SiDisplayUtil>{
        
    public:
        
        static ci::Rectf letterBox( ci::Rectf content, ci::Rectf box );
        static ci::Rectf fullOf( ci::Rectf content, ci::Rectf box );
        
    private:
        
        friend class Singleton<SiDisplayUtil>;
        
        SiDisplayUtil(){
            
        }
        
    };
    
}}