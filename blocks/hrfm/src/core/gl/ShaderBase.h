#pragma once

#include "cinder/app/App.h"
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
        
        // --------------------------------------------------------------------------------------------
        
        ShaderBase();
        ShaderBase( ci::gl::GlslProgRef shader );
        ShaderBase( fs::path fragment, fs::path vertex="simple_vert.glsl", fs::path geometory="", fs::path tessEval="", fs::path tessCtrl="" );
        
        ci::gl::GlslProgRef getGlslProg();
        
        virtual void  setEnabled(bool enabled);
        virtual bool  isEnabled();
        
        virtual void  setStrength( float strength );
        virtual float getStrength();
        
        // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
        virtual void prepare();
        // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
        virtual void clear();
        
    protected:
        
        // --------------------------------------------------------------------------------------------
        
        ci::gl::GlslProgRef mShader;
        float _strength = 1.0;
        bool  _enabled   = true;
        
        // --------------------------------------------------------------------------------------------
        
        fs::path mFragmentPath;
        fs::path mVertexPath;
        fs::path mGeometoryPath;
        fs::path mTessEvalPath;
        fs::path mTessCtrlPath;
        
        virtual void initShader( fs::path fragment, fs::path vertex, fs::path geometory, fs::path tessEval, fs::path tessCtrl );
        virtual DataSourceRef loadShader( fs::path );
        virtual DataSourceRef getVertexShader();
        virtual DataSourceRef getFragmentShader();
        virtual DataSourceRef getGeometoryShader();
        virtual DataSourceRef getTessEvalShader();
        virtual DataSourceRef getTessCtrlShader();
        
    private:
        
    };
    
}
    
}