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
        initShader( fragment, vertex, geometory, tessEval, tessCtrl );
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
    float ShaderBase::getStrength(){
        return _strength;
    }
    
    // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
    void ShaderBase::prepare(){
        mShader->uniform("time",(float)ci::app::getElapsedSeconds());
        mShader->uniform("strength",_strength);
    }
    
    // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
    void ShaderBase::clear(){}
    
    // ----------------------------------------------------------------------------------------------
    // protected
    
    void ShaderBase::initShader( fs::path fragment, fs::path vertex, fs::path geometory, fs::path tessEval, fs::path tessCtrl ){
        mVertexPath    = vertex;
        mFragmentPath  = fragment;
        mGeometoryPath = geometory;
        mTessEvalPath  = tessEval;
        mTessCtrlPath  = tessCtrl;
        mShader = ShaderFactory::create( getVertexShader(), getFragmentShader(), getGeometoryShader(), getTessEvalShader(), getTessCtrlShader() );
    }
    std::string loadShader( fs::path srcPath ){
        if( srcPath != "" ){
            return ci::loadString( DataLoader::load(srcPath) );
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