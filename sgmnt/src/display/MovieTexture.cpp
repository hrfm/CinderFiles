#include "MovieTexture.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    MovieTexture::MovieTexture(){
        IDrawable();
        sgmnt::events::EventDispatcher();
    };
    
    MovieTexture::MovieTexture( const string filePath ){
        MovieTexture();
        init(filePath);
    }
    
    MovieTexture::MovieTexture( ci::fs::path filePath ){
        MovieTexture();
        init(filePath);
    }
    
    MovieTexture::~MovieTexture(){
        
    }
    
    void MovieTexture::init( ci::fs::path filePath ){
        init(filePath.native());
    };
    
    void MovieTexture::init( const string filePath ){
        mMovieGlRef = ci::qtime::MovieGl::create( filePath );
    };
    
    ci::qtime::MovieGlRef MovieTexture::getMovieGlRef(){
        return mMovieGlRef;
    }
    
    ci::gl::Texture MovieTexture::getTexture(){
        return mMovieGlRef->getTexture();
    }
    
    void MovieTexture::update(){
        
    }
    
    void MovieTexture::_draw(){
        if( width == 0 && height == 0 ){
            gl::draw( getTexture() );
        }else{
            gl::draw( getTexture(), Rectf( 0, 0, width, height ) );
        }
    }
    
}}