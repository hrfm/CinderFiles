#pragma once

#include <map>
#include "Singleton.h"
#include "cinder/gl/Texture.h"

using namespace std;

namespace hrfm{ namespace gl{
    
    /*
     
     std::map を拡張し Singleton で何処からでも特定の Texture にアクセス出来るようにするクラスです.
     
     */
    class SiTextureMap : public hrfm::utils::Singleton<SiTextureMap>, public std::map<string, ci::gl::TextureRef>{
        
    public:
        
        virtual ci::gl::TextureRef getTexture( string key );
        
        virtual void addTexture( string key, ci::gl::TextureRef tex );
        
        virtual void removeTexture( string key );
        
    private:
        
        friend class hrfm::utils::Singleton<SiTextureMap>;
        
        SiTextureMap(){
            std::map<string,ci::gl::TextureRef>();
        }
        
    };
    
}}