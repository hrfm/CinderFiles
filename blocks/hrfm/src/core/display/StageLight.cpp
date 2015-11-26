#include "StageLight.h"

namespace hrfm{ namespace display{
    
    void StageLight::init( ci::ivec2 size, ci::vec3 pos, ci::vec3 lookAt ){
        
        ci::gl::Texture2d::Format depthFormat;
#if defined( CINDER_GL_ES )
        depthFormat.setInternalFormat( GL_DEPTH_COMPONENT16 );
        depthFormat.setDataType( GL_UNSIGNED_INT );
        depthFormat.setMagFilter( GL_NEAREST );
        depthFormat.setMinFilter( GL_NEAREST );
#else
        depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
        depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
        depthFormat.setMagFilter( GL_LINEAR );
        depthFormat.setMinFilter( GL_LINEAR );
        depthFormat.setWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
#endif
        depthFormat.setCompareFunc( GL_LEQUAL );
        
        _shadowMapTex = ci::gl::Texture2d::create( size.x, size.y, depthFormat );
        
        ci::gl::Fbo::Format fboFormat;
        fboFormat.attachment( GL_DEPTH_ATTACHMENT, _shadowMapTex );
        _fbo = ci::gl::Fbo::create( size.x, size.y, fboFormat );
        
        // Set up camera from the light's viewpoint
        _cam.setPerspective( 40.0f, _fbo->getAspectRatio(), 0.1f, 100.0f );
        setPosition( pos, lookAt );
        
    }
    
    void StageLight::setPosition( ci::vec3 pos ){ setPosition(pos,_lookAt); }
    void StageLight::setPosition( ci::vec3 pos, ci::vec3 lookAt ){
        _position = pos;
        _lookAt   = lookAt;
        _cam.lookAt( _position, _lookAt );
    }
    ci::vec3 StageLight::getPosition(){ return _position; }
    
    void StageLight::setLookAt( ci::vec3 lookAt ){
        _lookAt = lookAt;
        _cam.lookAt( _position, _lookAt );
    }
    ci::vec3 StageLight::getLookAt(){ return _lookAt; }
    
    void StageLight::renderDepthMap( Stage * stage ){
        
        // Set polygon offset to battle shadow acne
        ci::gl::enable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 2.0f, 2.0f );
        
        ci::gl::enableDepthWrite();
        ci::gl::enableDepthRead();
        
        // ---
        
        // Render scene to fbo from the view of the light
        ci::gl::ScopedFramebuffer fbo( _fbo );
        
        ci::gl::clear( ci::Color::black() );
        ci::gl::color( ci::Color::white() );

        ci::gl::pushMatrices();
            ci::gl::ScopedViewport viewport( ci::ivec2(0), _fbo->getSize() );
            ci::gl::setMatrices( _cam );
            stage->drawForLights();
        ci::gl::popMatrices();
        
        // ---
        
        ci::gl::disableDepthWrite();
        ci::gl::disableDepthRead();
        
        // Disable polygon offset for final render
        ci::gl::disable( GL_POLYGON_OFFSET_FILL );
        
    }
    
    ci::gl::Texture2dRef StageLight::getDepthTexture(){
        /*
        return _shadowMapTex;
        /*/
        return _fbo->getColorTexture();
        //*/
    }
    
    ci::vec3 StageLight::getMVLightPos( ci::mat4 modelViewMatrix ){
        return ci::vec3( modelViewMatrix * ci::vec4( _position, 1.0f ) );
    }
    
    ci::mat4 StageLight::getShadowMatrix(){
        return _cam.getProjectionMatrix() * _cam.getViewMatrix();
    }

    
}}