#include "ShaderBase.h"

namespace hrfm { namespace gl{
    
    // ----------------------------------------------------------------------------------------------
    // public
    
    ShaderBase::ShaderBase(){
        mShader = ci::gl::getStockShader( ci::gl::ShaderDef().color() );
    }
    ShaderBase::ShaderBase( ci::gl::GlslProgRef shader ){
        mShader = shader;
    };
    ShaderBase::ShaderBase( fs::path fragment, fs::path vertex, fs::path geometory, fs::path tessEval, fs::path tessCtrl ){
        try{
            mShader = initShader( fragment, vertex, geometory, tessEval, tessCtrl );
        }catch(...){
            mShader = ci::gl::getStockShader( ci::gl::ShaderDef().color() );
        }
    };
    
    ci::gl::GlslProgRef ShaderBase::getGlslProg(){
        return mShader;
    }
    
    void ShaderBase::setEnabled(bool enabled){
        _enabled = enabled;
    }
    bool ShaderBase::isEnabled(){
        return _enabled && 0.0 < _strength;
    }
    
    void  ShaderBase::setStrength( float strength ){
        _strength = strength;
    }
    
    void  ShaderBase::setStrength( float * strength ){
        _strengthPtr = strength;
    }
    
    float ShaderBase::getStrength(){
        float s = _strength;
        hrfm::io::SiAudioInput * audio = &hrfm::io::SiAudioInput::getInstance();
        if( _biasMode == BIAS_MODE_FFT ){
            s *= audio->getFFTRangedAt(_biasFFTIndex) * _biasTimes;
        }else if( _biasMode == BIAS_MODE_VOLUME ){
            s *= audio->getVolume() * _biasTimes;
        }
        return s;
    }
    
    void ShaderBase::clearAudioBias(){
        _biasMode = BIAS_MODE_NONE;
    }
    
    void ShaderBase::setAudioBiasByFFT( int index, float times ){
        _biasMode = BIAS_MODE_FFT;
        _biasTimes = times;
        _biasFFTIndex = index;
    }
    
    void ShaderBase::setAudioBiasByVolume( float times ){
        _biasTimes = times;
        _biasMode = BIAS_MODE_VOLUME;
    }
    
    // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
    void ShaderBase::prepare(){
        mShader->uniform("time",(float)ci::app::getElapsedSeconds());
        mShader->uniform("strength",getStrength());
    }
    
    // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
    void ShaderBase::clear(){}
    
    // ----------------------------------------------------------------------------------------------
    // protected
    
    ci::gl::GlslProgRef ShaderBase::initShader( fs::path fragment, fs::path vertex, fs::path geometory, fs::path tessEval, fs::path tessCtrl ){
        mVertexPath    = vertex;
        mFragmentPath  = fragment;
        mGeometoryPath = geometory;
        mTessEvalPath  = tessEval;
        mTessCtrlPath  = tessCtrl;
        try{
            return ShaderFactory::create( getVertexShader(), getFragmentShader(), getGeometoryShader(), getTessEvalShader(), getTessCtrlShader() );
        }catch(...){
            throw;
        }
    }
    std::string ShaderBase::loadShader( fs::path srcPath ){
        if( srcPath != "" ){
            return ci::loadString( hrfm::io::DataLoader::load(srcPath) );
        }else{
            return std::string();
        }
    }
    std::string ShaderBase::getVertexShader()   { return this->loadShader( mVertexPath    ); }
    std::string ShaderBase::getFragmentShader() { return this->loadShader( mFragmentPath  ); }
    std::string ShaderBase::getGeometoryShader(){ return this->loadShader( mGeometoryPath ); }
    std::string ShaderBase::getTessEvalShader() { return this->loadShader( mTessEvalPath  ); }
    std::string ShaderBase::getTessCtrlShader() { return this->loadShader( mTessCtrlPath  ); }
    
}}