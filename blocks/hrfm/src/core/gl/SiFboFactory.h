#pragma once

#include <map>
#include <sstream>
#include "Singleton.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

using namespace std;
using namespace hrfm::utils;

namespace hrfm{ namespace gl{
    
    /*
     
     std::map を拡張し Singleton で何処からでも特定の Texture にアクセス出来るようにするクラスです.
     
     */
    class SiFboFactory : public Singleton<SiFboFactory>, public std::map<string, ci::gl::FboRef>{
        
    public:
        
        virtual ci::gl::FboRef create( int width, int height, bool cache = true, ci::gl::Fbo::Format format = ci::gl::Fbo::Format() );
        
        virtual ci::gl::FboRef beginTmpFbo( int width, int height, ci::CameraPersp * camera = NULL, ci::gl::Fbo::Format format = ci::gl::Fbo::Format() );
        virtual void endTmpFbo();
        
    private:
        
        friend class Singleton<SiFboFactory>;
        
        SiFboFactory(){
            std::map<string,ci::gl::Fbo*>();
        }
        
        ci::gl::FboRef _tmpFbo = NULL;
        std::pair<ci::ivec2,ci::ivec2> _tmpViewport;
        
    };
    
}}