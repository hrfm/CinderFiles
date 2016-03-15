#pragma once

#include "hrfm.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/GeomIo.h"
#include "cinder/Rand.h"
#include "cinder/CameraUi.h"
#include "cinder/Log.h"
#include "cinder/Color.h"
#include "glm/gtx/euler_angles.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

/*
 
 シャドウマッピングを実現するために作った Node.
 
 
 
 */

namespace hrfm { namespace display{
    
    namespace shadowmap{
        
        /*
         シャドウマップを描画するクラス.
         Texture2d と FBO を用いてデプスを描画する.
         */
        typedef std::shared_ptr<class ShadowMap> ShadowMapRef;
        class ShadowMap {
        public:
            
            static ShadowMapRef create( int size ) { return ShadowMapRef( new ShadowMap{ size } ); }
            
            ShadowMap( int size ){
                reset( size );
            }
            void reset( int size ){
                
                // -----------------------------------------------------------------
                // Create Texture2d for Depth.
                
                ci::gl::Texture2d::Format depthFormat;
                
                depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
                depthFormat.setMagFilter( GL_LINEAR );
                depthFormat.setMinFilter( GL_LINEAR );
                depthFormat.setWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
                depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
                depthFormat.setCompareFunc( GL_LEQUAL );
                mTextureShadowMap = ci::gl::Texture2d::create( size, size, depthFormat );
                
                // -----------------------------------------------------------------
                // Create FBO for Depth.
                
                ci::gl::Fbo::Format fboFormat;
                fboFormat.attachment( GL_DEPTH_ATTACHMENT, mTextureShadowMap );
                mShadowMap = ci::gl::Fbo::create( size, size, fboFormat );
                
            }
            
            const ci::gl::FboRef&		getFbo() const { return mShadowMap; }
            const ci::gl::Texture2dRef&	getTexture() const { return mTextureShadowMap; }
            
            float getAspectRatio() const { return mShadowMap->getAspectRatio(); }
            ivec2 getSize() const { return mShadowMap->getSize(); }
            
        private:
            
            ci::gl::FboRef				mShadowMap;
            ci::gl::Texture2dRef		mTextureShadowMap;
        };
        
        /*
         シャドウマップで用いる光源処理のために必要な値を保持する構造体.
         */
        struct LightData {
            bool        toggleViewpoint;
            float       distanceRadius;
            float       fov;
            CameraPersp camera;
            vec3        viewpoint;
            vec3        target;
        };
        
    }
    
    // =======================================================================================
    // ---
    
    /*
     ShadowMappingNode 内で使用する.
     カメラからと光源からを別々に描画してくれるオブジェクト.
     */
    class ShadowMappingObject{
        
    public:
        
        ShadowMappingObject( ci::gl::BatchRef batch, ci::gl::BatchRef shadowedBatch, std::vector<std::pair<mat4, vec3>> transforms ){
            mBatch         = batch;
            mShadowedBatch = shadowedBatch;
            mTransforms    = transforms;
            colorA = ColorA(1.0,1.0,1.0,1.0);
        }
        
        virtual void update(){}
        
        // 通常の描画を行うための処理.特に特殊なことはしない.
        virtual void draw(){
            _draw( mBatch );
        }
        
        // 影を書き込む処理. shadowGlsl は既に bind された状態で実行される.
        // 自分用に uniform 等を指定したい場合に参照を利用する.
        virtual void drawShadowed( const ci::gl::GlslProgRef& shadowGlsl ){
            _draw( mShadowedBatch, shadowGlsl );
        }
        
        ColorA colorA;
        float spinAngle = 0.0;
        
    protected:
        
        virtual void _draw( ci::gl::BatchRef batch, const ci::gl::GlslProgRef& shadowGlsl = nullptr ){
            ci::gl::ScopedColor scpColor( colorA );
            for ( const auto& transform : mTransforms ) {
                ci::gl::ScopedModelMatrix push;
                ci::gl::multModelMatrix( ci::rotate( spinAngle, transform.second ) * transform.first );
                batch->draw();
            }
        }
        
        ci::gl::BatchRef mBatch;
        ci::gl::BatchRef mShadowedBatch;
        std::vector<std::pair<mat4, vec3>>	mTransforms;
        
    };
    
    // =======================================================================================
    // ---
    
    class ShadowMappingNode:public hrfm::display::DisplayNode{
        
    public:
        
        ShadowMappingNode( float shadowMapSize = 2048 ):hrfm::display::DisplayNode(){
            setup( shadowMapSize );
        }
        
        virtual void setup( float shadowMapSize );
        
    protected:
        
        float lightStrength = 0.0f;
        
        // -----
        
        virtual void _draw();
        
        // 引数に深度描画用のシェーダが渡されたかどうかによって
        // 通常の描画とデプスの描画を切り分けて描画する関数.
        virtual void _drawScene( const ci::gl::GlslProgRef& shadowGlsl = nullptr );
        
        CameraPersp  mCamera;
        ci::CameraUi mCamUi;
        
        int                     mShadowMapSize;
        ci::gl::GlslProgRef	    mShadowShader;
        shadowmap::ShadowMapRef mShadowMap;
        shadowmap::LightData    mLight;
        
        bool  mOnlyShadowmap;
        int	  mShadowTechnique;
        float mDepthBias;
        bool  mEnableNormSlopeOffset;
        float mRandomOffset;
        int   mNumRandomSamples;
        float mPolygonOffsetFactor, mPolygonOffsetUnits;
        
        std::vector<ShadowMappingObject*> mObjectList;
        
    };
    
}}