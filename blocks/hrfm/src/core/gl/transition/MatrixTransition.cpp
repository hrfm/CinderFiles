#include "MatrixTransition.h"

namespace hrfm { namespace gl{
    
    MatrixTransition::MatrixTransition(){
        
        TransitionShaderBase();
        
        setInterval( 2.0f );
        
        init( DataLoader::load("MatrixTransition.glsl") );
        
        _beforeTime = 0.0;
        
        _matrix.setup( Vec2i( 3840, 720 ), 20, "OCRAStd" );
        _matrix.shuffle();
        
    }
    
    void MatrixTransition::prepare(){
        double elapsed = ci::app::getElapsedSeconds();
        if( 0.05 < elapsed - _beforeTime ){
            _matrix.shuffle();
            _beforeTime = elapsed;
        }
    }
    
    void MatrixTransition::bindTexture( const ci::gl::Texture & current, const ci::gl::Texture & next ){
        TransitionShaderBase::bindTexture( current, next );
        addBindingTexture( "matrixTex", _matrix.getTexture() );
    }
    
    void MatrixTransition::affect( float progress ){
        mShader.uniform("segments", Vec2f( _matrix.getMatrixSize() ) );
        TransitionShaderBase::affect(progress);
    }
    
}}