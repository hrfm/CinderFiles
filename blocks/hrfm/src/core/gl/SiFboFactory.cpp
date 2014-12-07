#include "SiFboFactory.h"

namespace hrfm{ namespace gl{
    
    ci::gl::Fbo * SiFboFactory::create( int width, int height, bool cache, ci::gl::Fbo::Format format ){
        
        if( cache ){
            
            string key = to_string(width) + "_" + to_string(height);
            
            if( this->find(key) == this->end() ){
                //cout << "SiFboFactory::create( " << width << ", " << height << " ) => Set fbo to Cache" << endl;
                ci::gl::Fbo * fbo;
                fbo = new ci::gl::Fbo( width, height, format );
                this->insert( map<string,ci::gl::Fbo*>::value_type( key, fbo ) );
            }else{
                //cout << "SiFboFactory::create( " << width << ", " << height << " ) => Get fbo from cache." << endl;
            }
            
            return this->at(key);
            
        }else{
            
            //cout << "SiFboFactory::create( " << width << ", " << height << " )" << endl;
            return new ci::gl::Fbo( width, height, format );
            
        }
        
    }
    
}}