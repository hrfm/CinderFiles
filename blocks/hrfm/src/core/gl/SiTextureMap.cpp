#include "SiTextureMap.h"

namespace hrfm{ namespace gl{
    
    ci::gl::TextureRef SiTextureMap::getTexture( string key ){
        return this->at(key);
    }
    
    void SiTextureMap::addTexture( string key, ci::gl::TextureRef texture ){
        cout << "SiTextureMap::addTexture[" << key << "]" << endl;
        if( this->find(key) != this->end() ){
            cout << "Key " + key + " is already exists" << endl;
            return;
        }
        this->insert( map<string,ci::gl::TextureRef>::value_type( key, texture ) );
    }
    
    void SiTextureMap::removeTexture( string key ){
        this->erase( key );
    }
    
}}