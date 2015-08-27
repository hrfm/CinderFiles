#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "Cinder/gl/GlslProg.h"
#include "cinder/Xml.h"
#include "SiFboFactory.h"
#include "ShaderFactory.h"
#include "Utils.h"

using namespace ci;
using namespace std;
using namespace hrfm::utils;

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    class FilterBase{
        
    public:
        
        FilterBase( Vec2i size = Vec2i(256,256) );
        FilterBase( string fragmentShader, Vec2i size = Vec2i(256,256) );
        FilterBase( string fragmentShader, string vertexShader, Vec2i size = Vec2i(256,256) );
        
        // 最も基本的な初期化を行います.
        // どのシェーダを使うかはクラスに任せます.
        virtual void setup( Vec2i size );
        virtual void setup( Vec2i size, string fragmentShader );
        virtual void setup( Vec2i size, string fragmentShader, string vertexShader );
        
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
        
        void init( string fragmentShader = "simple_frag.glsl", string vertexShader = "simple_vert.glsl");
        
        virtual DataSourceRef getVertexShader();
        
        virtual DataSourceRef getFragmentShader();
        
        // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
        virtual void prepare();
        
        // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
        virtual void clear();
        
        string mFragmentShader;
        string mVertexShader;
        
        ci::gl::GlslProg mShader;
        float mCycle;
        Vec2i mAspect;
        
        ci::gl::Fbo mFbo;
        
    private:
        
    };
    
}
    
}