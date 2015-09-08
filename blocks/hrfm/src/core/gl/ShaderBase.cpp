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
        mShader.bind();
        prepare();
    }
    
    void ShaderBase::end(){
        clear();
        mShader.unbind();
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
        mShader.uniform("time",(float)ci::app::getElapsedSeconds());
    }
    
    // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
    void ShaderBase::clear(){}
    
}}