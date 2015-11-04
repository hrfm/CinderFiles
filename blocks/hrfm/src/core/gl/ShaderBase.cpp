#include "ShaderBase.h"

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    ShaderBase::ShaderBase(){
        initShader();
        mShader = ShaderFactory::create( getVertexShader(), getFragmentShader() );
    }
    ShaderBase::ShaderBase( string fragmentShader ){
        initShader( fragmentShader );
        mShader = ShaderFactory::create( getVertexShader(), getFragmentShader() );
    };
    ShaderBase::ShaderBase( string fragmentShader, string vertexShader ){
        initShader( fragmentShader, vertexShader );
        mShader = ShaderFactory::create( getVertexShader(), getFragmentShader() );
    };
    
    void ShaderBase::begin(){
        if( isEnabled() ){
            mShader->bind();
            prepare();
        }
    }
    
    void ShaderBase::end(){
        if( isEnabled() ){
            clear();
            //!!!!!!!! mShader->unbind();
        }
    }
    
    ci::gl::GlslProgRef ShaderBase::getGlslProgRef(){
        return mShader;
    }
    
    void ShaderBase::setStrength( float strength ){
        _strength = strength;
    }
    float ShaderBase::getStrength(){
        return _strength;
    }

    bool ShaderBase::isEnabled(){
        return _enabled && 0.0 < _strength;
    }
    
    void ShaderBase::setEnabled(bool enabled){
        _enabled = enabled;
    }
    
    // protected
    
    void ShaderBase::initShader(string fragmentShader, string vertexShader ){
        mFragmentShader = fragmentShader;
        mVertexShader   = vertexShader;
        cout << "initShader > " << mFragmentShader << ":" << mVertexShader << endl;
    }
    
    DataSourceRef ShaderBase::getVertexShader(){
        return DataLoader::load(mVertexShader);
        //return ci::app::loadResource(this->_vertexShader);
    }
    
    DataSourceRef ShaderBase::getFragmentShader(){
        //cout << "getFragmentShader : " << mFragmentShader << endl;
        return DataLoader::load(mFragmentShader);
        //return ci::app::loadResource(this->_fragmentShader);
    }
        
    // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
    void ShaderBase::prepare(){
        mShader->uniform("time",(float)ci::app::getElapsedSeconds());
        mShader->uniform("strength",_strength);
    }
    
    // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
    void ShaderBase::clear(){}
    
}}