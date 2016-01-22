#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "Cinder/gl/GlslProg.h"
#include "cinder/Xml.h"
#include "SiFboFactory.h"
#include "ShaderFactory.h"
#include "SiAudioInput.h"

using namespace ci;
using namespace std;
using namespace hrfm::utils;

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    class ShaderBase{
        
    public:
        
        static const int BIAS_MODE_NONE   = 0;
        static const int BIAS_MODE_FFT    = 1;
        static const int BIAS_MODE_VOLUME = 2;
        
        // --------------------------------------------------------------------------------------------
        
        ShaderBase();
        ShaderBase( ci::gl::GlslProgRef shader );
        ShaderBase( ci::fs::path fragment, ci::fs::path vertex="simple_vert.glsl", ci::fs::path geometory="", ci::fs::path tessEval="", ci::fs::path tessCtrl="" );
        
        ci::gl::GlslProgRef getGlslProg();
        
        virtual void  setEnabled(bool enabled);
        virtual bool  isEnabled();
        
        virtual void  setStrength( float strength );
        virtual float getStrength();
        
        virtual void setAudioBiasByFFT( int index, float times = 1.0f );
        virtual void setAudioBiasByVolume( float times = 1.0f );
        
        // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
        virtual void prepare();
        // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
        virtual void clear();
        
    protected:
        
        int   _biasMode     = 0;
        float _biasTimes    = 1.0f;
        int   _biasFFTIndex = 0;
        
        // --------------------------------------------------------------------------------------------
        
        ci::gl::GlslProgRef mShader;
        float _strength = 1.0;
        bool  _enabled   = true;
        
        // --------------------------------------------------------------------------------------------
        
        ci::fs::path mFragmentPath;
        ci::fs::path mVertexPath;
        ci::fs::path mGeometoryPath;
        ci::fs::path mTessEvalPath;
        ci::fs::path mTessCtrlPath;
        
        ci::gl::GlslProgRef initShader( ci::fs::path fragment, ci::fs::path vertex="simple_vert.glsl", ci::fs::path geometory="", ci::fs::path tessEval="", ci::fs::path tessCtrl="" );
        virtual std::string loadShader( ci::fs::path srcPath );
        virtual std::string getVertexShader();
        virtual std::string getFragmentShader();
        virtual std::string getGeometoryShader();
        virtual std::string getTessEvalShader();
        virtual std::string getTessCtrlShader();
        
    private:
        
    };
    
}
    
}