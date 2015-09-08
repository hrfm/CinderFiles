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
    
    class ShaderBase{
        
    public:
        
        ShaderBase();
        ShaderBase( string fragmentShader );
        ShaderBase( string fragmentShader, string vertexShader );
        
        virtual void begin();
        virtual void end();
        
    protected:
        
        void initShader( string fragmentShader = "simple_frag.glsl", string vertexShader = "simple_vert.glsl");
        virtual DataSourceRef getVertexShader();
        virtual DataSourceRef getFragmentShader();
        
        // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
        virtual void prepare();
        
        // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
        virtual void clear();
        
        string mFragmentShader;
        string mVertexShader;
        ci::gl::GlslProg mShader;
        
    private:
        
    };
    
}
    
}