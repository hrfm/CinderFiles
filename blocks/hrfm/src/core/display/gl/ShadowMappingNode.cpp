#include "ShadowMappingNode.h"

namespace hrfm { namespace display{
    
    void ShadowMappingNode::setup( float shadowMapSize ){
        
        try {
            mShadowShader	= ci::gl::GlslProg::create( loadResource( "shadow_mapping.vert"), loadResource("shadow_mapping.frag") );
        } catch ( const ci::gl::GlslProgCompileExc& exc ) {
            CI_LOG_E( "Shader failed to load: " << exc.what() );
            //quit();
        }
        
        mShadowMapSize = shadowMapSize;
        mShadowMap     = shadowmap::ShadowMap::create( mShadowMapSize );
        
        mLight.camera.setPerspective( mLight.fov, mShadowMap->getAspectRatio(), 0.5, 500.0 );
        mLight.distanceRadius	= 100.0f;
        mLight.viewpoint		= vec3( mLight.distanceRadius );
        mLight.fov				= 10.0f;
        mLight.target			= vec3( 0.0 );
        mLight.toggleViewpoint	= false;
        
        mShadowTechnique		= 3;
        mDepthBias				= -0.0005f;
        mRandomOffset			= 1.2f;
        mNumRandomSamples		= 32;
        mEnableNormSlopeOffset	= false;
        mOnlyShadowmap			= false;
        mPolygonOffsetFactor	= mPolygonOffsetUnits = 3.0f;
        
        // ----------
        // カメラ周りは後で要確認
        
        mCamera.setFov( 30.0f );
        mCamera.setAspectRatio( getWindowAspectRatio() );
        //mCamUi = CameraUi( &mCamera, getWindow() );
        
    }
    
    void ShadowMappingNode::_draw(){
        
        ci::gl::enableDepthRead();
        ci::gl::enableDepthWrite();
        
        // Offset to help combat surface acne (self-shadowing)
        ci::gl::enable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( mPolygonOffsetFactor, mPolygonOffsetUnits );
        
        // ------------------------------------------------------------------
        // Render scene into shadow map
        ci::gl::pushMatrices();
        {
            ci::gl::setMatrices( mLight.camera );
            ci::gl::ScopedViewport viewport( mShadowMap->getSize() );
            ci::gl::ScopedFramebuffer bindFbo( mShadowMap->getFbo() );
            ci::gl::clear();
            _drawScene();
        }
        ci::gl::popMatrices();
        
        // ------------------------------------------------------------------
        // Render shadowed scene
        ci::gl::pushMatrices();
        {
            
            ci::gl::setMatrices( mLight.toggleViewpoint ? mLight.camera : mCamera );
            ci::gl::ScopedViewport viewport( toPixels( this->size() ) );
            
            ci::gl::ScopedGlslProg bind( mShadowShader );
            ci::gl::ScopedTextureBind texture( mShadowMap->getTexture() );
            
            // ShadowMap Texture.
            mShadowShader->uniform( "uShadowMap", 0 );
            
            // 光源の強さ.
            mShadowShader->uniform( "uLightStrength", lightStrength );
            mShadowShader->uniform( "uLightPos", vec3( ci::gl::getModelView() * vec4( mLight.viewpoint, 1.0 ) ) );
            
            mShadowShader->uniform( "uShadowMatrix", mLight.camera.getProjectionMatrix() * mLight.camera.getViewMatrix() );
            mShadowShader->uniform( "uDepthBias", mDepthBias );
            
            mShadowShader->uniform( "uShadowTechnique", mShadowTechnique );
            
            mShadowShader->uniform( "uOnlyShadowmap", mOnlyShadowmap );
            mShadowShader->uniform( "uRandomOffset", mRandomOffset );
            mShadowShader->uniform( "uNumRandomSamples", mNumRandomSamples );
            mShadowShader->uniform( "uEnableNormSlopeOffset", mEnableNormSlopeOffset );
            
            _drawScene( mShadowShader );
            
        }
        ci::gl::popMatrices();
        
        ci::gl::disableDepthRead();
        ci::gl::disableDepthWrite();
        
        ci::gl::disable( GL_POLYGON_OFFSET_FILL );
        
    }
    
    void ShadowMappingNode::_drawScene( const ci::gl::GlslProgRef& shadowGlsl ){
        if( shadowGlsl ){
            for( auto it = std::begin(mObjectList); it != std::end(mObjectList); it++ ){
                (*it)->update();
                (*it)->drawShadowed( shadowGlsl );
            }
        }else{
            for( auto it = std::begin(mObjectList); it != std::end(mObjectList); it++ ){
                (*it)->update();
                (*it)->draw();
            }
        }
    }
    
}}