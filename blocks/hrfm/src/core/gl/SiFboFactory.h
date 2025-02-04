#pragma once

#include <map>
#include <sstream>
#include "Singleton.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

using namespace std;
using namespace hrfm::utils;

namespace hrfm{ namespace gl{
    
    /*
     
     std::map を拡張し Singleton で何処からでも特定の Texture にアクセス出来るようにするクラスです.
     
     */
    class SiFboFactory : public Singleton<SiFboFactory>, public std::map<string, ci::gl::Fbo*>{
        
    public:
        
        virtual ci::gl::Fbo * create( int width, int height, bool cache = true, ci::gl::Fbo::Format format = ci::gl::Fbo::Format() );
        
        virtual ci::gl::Fbo * beginTmpFbo( int width, int height, ci::CameraPersp * camera = NULL, ci::gl::Fbo::Format format = ci::gl::Fbo::Format() );
        virtual void endTmpFbo();
        
    private:
        
        friend class Singleton<SiFboFactory>;
        
        SiFboFactory(){
            std::map<string,ci::gl::Fbo*>();
        }
        
        ci::gl::Fbo * _tmpFbo = NULL;
        ci::Area      _tmpViewport;
        
    };
    
}}