#include "SiFboFactory.h"

namespace hrfm{ namespace gl{
    
    ci::gl::Fbo * SiFboFactory::create( int width, int height, bool cache, ci::gl::Fbo::Format format ){
        
        if( cache ){
            
            string key = to_string(width) + "_" + to_string(height);
            
            if( this->find(key) == this->end() ){
                cout << "[SiFboFactory] Create new FBO ( " << width << ", " << height << " ) on cache." << endl;
                ci::gl::Fbo * fbo;
                fbo = new ci::gl::Fbo( width, height, format );
                this->insert( map<string,ci::gl::Fbo*>::value_type( key, fbo ) );
            }else{
                cout << "[SiFboFactory] Get FBO from cache." << endl;
            }
            
            return this->at(key);
            
        }else{
            
            cout << "[SiFboFactory] Create new FBO ( " << width << ", " << height << " )." << endl;
            return new ci::gl::Fbo( width, height, format );
            
        }
        
    }
    
}}