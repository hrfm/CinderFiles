#pragma once

#include <map>
#include "Singleton.h"
#include "cinder/gl/Texture.h"

using namespace std;

namespace hrfm{ namespace utils{
    
    /*
     
     std::map を拡張し Singleton で何処からでも特定の Texture にアクセス出来るようにするクラスです.
     
     */
    class SiTextureMap : public Singleton<SiTextureMap>, public std::map<string, ci::gl::Texture>{
        
    public:
        
        virtual ci::gl::Texture getTexture( string key );
        
        virtual void addTexture( string key, ci::gl::Texture tex );
        
        virtual void removeTexture( string key );
        
    private:
        
        friend class Singleton<SiTextureMap>;
        
        SiTextureMap(){
            std::map<string,ci::gl::Texture>();
        }
        
    };
    
}}