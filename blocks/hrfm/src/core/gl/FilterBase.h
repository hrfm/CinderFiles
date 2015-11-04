#pragma once

#include "cinder/app/App.h"
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
        
        FilterBase( ivec2 size = ivec2(256,256) ):ShaderBase(){
            setup( size );
        };
        FilterBase( string fragmentShader, ivec2 size = ivec2(256,256) ):ShaderBase(fragmentShader){
            setup( size );
        };
        FilterBase( string fragmentShader, string vertexShader, ivec2 size = ivec2(256,256) ):ShaderBase(fragmentShader,vertexShader){
            setup( size );
        };
        
        virtual void setup( ivec2 size );
        
        virtual void setSize( int w, int h );
        virtual void setSize( ivec2 size );
        
        virtual void affect( ci::gl::TextureRef tex, vec2 windowSize, vec2 resolution, Rectf drawRect );
        virtual ci::gl::TextureRef affect( ci::gl::TextureRef tex );
        
        void draw( Rectf bounds );
        
        ci::gl::TextureRef getTexture();
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
        ivec2 mAspect;
        ci::gl::FboRef mFbo;
        
    private:
        
    };
    
}
    
}