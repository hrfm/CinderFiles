#include "MatrixTransition.h"

namespace hrfm { namespace gl{
    
    MatrixTransition::MatrixTransition(){
        
        TransitionShaderBase();
        
        setInterval( 3.0f );
        
        init( DataLoader::load("MatrixTransition.glsl") );
        
        _matrix.setup( Vec2i( 3840, 720 ), 20, "OCRAStd" );
        
    }
    
    void MatrixTransition::prepare(){
        _matrix.shuffle();
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