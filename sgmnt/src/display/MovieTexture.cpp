#include "MovieTexture.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    // public:
    
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
    
}}