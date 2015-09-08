#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "Cinder/gl/GlslProg.h"
#include "cinder/Xml.h"
#include "SiFboFactory.h"
#include "ShaderFactory.h"
#include "Utils.h"
#include "ShaderBase.h"

using namespace ci;
using namespace std;
using namespace hrfm::utils;

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    class FilterBase:public ShaderBase{
        
    public:
        
        FilterBase( Vec2i size = Vec2i(256,256) ):ShaderBase(){
            setup( size );
        };
        FilterBase( string fragmentShader, Vec2i size = Vec2i(256,256) ):ShaderBase(fragmentShader){
            setup( size );
        };
        FilterBase( string fragmentShader, string vertexShader, Vec2i size = Vec2i(256,256) ):ShaderBase(fragmentShader,vertexShader){
            setup( size );
        };
        
        virtual void setup( Vec2i size );
        
        virtual void setSize( int w, int h );
        virtual void setSize( Vec2i size );
        
        virtual void affect( ci::gl::Texture * tex, Vec2f windowSize, Vec2f resolution, Rectf drawRect );
        virtual ci::gl::Texture affect( ci::gl::Texture tex );
        
        void draw( Rectf bounds );
        
        ci::gl::Texture getTexture();
        void bindTexture( int index );
        void unbindTexture();
        
        int getWidth();
        int getHeight();
        Rectf getBounds();
        
    protected:
        
        // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
        virtual void prepare();
        
        // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
        virtual void clear();
        
        float mCycle;
        Vec2i mAspect;
        ci::gl::Fbo mFbo;
        
    private:
        
    };
    
}
    
}