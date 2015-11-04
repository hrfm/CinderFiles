#include "SiFboFactory.h"

namespace hrfm{ namespace gl{
    
    ci::gl::FboRef SiFboFactory::create( int width, int height, bool cache, ci::gl::Fbo::Format format ){
        
        if( cache ){
            
            string key = to_string(width) + "_" + to_string(height);
            
            if( this->find(key) == this->end() ){
                //cout << "SiFboFactory::create( " << width << ", " << height << " ) => Set fbo to Cache" << endl;
                ci::gl::FboRef fbo = ci::gl::Fbo::create( width, height, format );
                this->insert( map<string,ci::gl::FboRef>::value_type( key, fbo ) );
            }else{
                //cout << "SiFboFactory::create( " << width << ", " << height << " ) => Get fbo from cache." << endl;
            }
            
            return this->at(key);
            
        }else{
            
            //cout << "SiFboFactory::create( " << width << ", " << height << " )" << endl;
            return ci::gl::Fbo::create( width, height, format );
            
        }
        
    }
    
    ci::gl::FboRef SiFboFactory::beginTmpFbo( int width, int height, ci::CameraPersp * camera, ci::gl::Fbo::Format format ){
        
        if( _tmpFbo ){
            throw "Temporary FBO is already exists. You should call endTmpFBO before call this method.";
        }
        
        _tmpFbo = create( width, height, true, format );
        
        _tmpViewport = ci::gl::getViewport();
        
        _tmpFbo->bindFramebuffer();
        ci::gl::pushMatrices();
        
        if( camera != NULL ){
            ci::gl::setMatrices( *camera );
        }else{
            ci::gl::setViewport( ci::Area( 0, 0, width, height ) );
            ci::gl::setMatricesWindow( width, height, false );
        }
        
        return _tmpFbo;
        
    }
    
    void SiFboFactory::endTmpFbo(){
        
        if( _tmpFbo == NULL ){
            throw "No temporary FBO.";
        }
        
        ci::gl::popMatrices();
        
        _tmpFbo->unbindFramebuffer();
        
        ci::gl::setViewport( _tmpViewport );
        
        _tmpFbo      = NULL;
        
    }
    
}}