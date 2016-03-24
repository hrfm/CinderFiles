#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "Cinder/gl/GlslProg.h"
#include "cinder/Xml.h"
#include "SiFboFactory.h"
#include "ShaderFactory.h"
#include "ShaderBase.h"

using namespace ci;
using namespace std;
using namespace hrfm::utils;

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    class FilterBase:public ShaderBase{
        
    public:
        
        FilterBase( ivec2 size = ivec2(256,256) ):ShaderBase(ci::fs::path("simple_frag.glsl"),ci::fs::path("filter_vert.glsl")){
            setup( size );
        };
        FilterBase( ci::fs::path fragment, ivec2 size = ivec2(256,256) ):ShaderBase(fragment,ci::fs::path("filter_vert.glsl")){
            setup( size );
        };
        FilterBase( ci::fs::path fragment, ci::fs::path vertex, ivec2 size = ivec2(256,256) ):ShaderBase(fragment,vertex){
            setup( size );
        };
        
        virtual void updateShader(ci::fs::path fragment = ci::fs::path("simple_frag.glsl"), ci::fs::path vertex = ci::fs::path("filter_vert.glsl") );
        
        virtual void setup( ivec2 size );
        
        virtual void setSize( int w, int h );
        virtual void setSize( ivec2 size );
        
        virtual void affect( ci::gl::TextureRef tex, vec2 resolution );
        virtual void affect( ci::gl::TextureRef tex, vec2 resolution, Rectf drawArea );
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
        
        bool _needUpdateShader = false;
        ci::fs::path _updateFragment = "";
        ci::fs::path _updateVertex   = "";
        
        float mCycle;
        ivec2 mAspect;
        ci::gl::FboRef mFbo;
        
    private:
        
    };
    
}
    
}