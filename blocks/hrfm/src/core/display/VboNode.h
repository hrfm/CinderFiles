#pragma once

#include <list>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
//#include "cinder/gl/Light.h"
//#include "cinder/gl/Material.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Batch.h"

#include "DisplayNode.h"
#include "ShaderBase.h"

// 基本 Batch 化する。
// drawChildren 等は指定がない限り 親の shader を継承する.
// mesh を指定しない場合は draw 内で描画はせず、子に shader を渡す。



namespace hrfm{ namespace display{
    
    class VboNode : public hrfm::display::DisplayNode{
    
    public:
        
        VboNode():hrfm::display::DisplayNode(){
            this->rotation = mat4();
        }
        
        VboNode( ci::gl::VboMeshRef meshRef, ci::gl::GlslProgRef shader = NULL ):hrfm::display::DisplayNode(){
            this->rotation = mat4();
            this->setVboMeshRef( meshRef );
            if( shader != NULL ){
                this->setShader( shader );
            }
        };
        
        ~VboNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual ci::mat4 getRotation();
        virtual void setRotation( ci::mat4 rotation );
        
        virtual void setEnableWireframe( bool flag );
        
        virtual void setVboMeshRef( ci::gl::VboMeshRef vboMeshRef );
        virtual ci::gl::VboMeshRef getVboMeshRef();
        
        virtual void setShader( ci::gl::GlslProgRef shader );
        virtual ci::gl::GlslProgRef getShader();
        
        virtual void addTexture( ci::gl::TextureRef tex );
        virtual void removeTexture( ci::gl::TextureRef tex );
        
        virtual void update();
        virtual void draw( ci::ColorA * drawColor = NULL );
        
        // --- PROPERTY -------------
        ci::mat4 rotation;
        // --------------------------
        
        //virtual void setMaterial( ci::gl::Material * material );
        //virtual ci::gl::Material * getMaterial();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        virtual void _drawForLights();
        virtual void _drawChildren( ci::ColorA * drawColor = NULL );
        
        bool eraseTextureFromList( ci::gl::TextureRef tex );
        
        // --- PROPERTY -------------
        bool _enableWireframe = false;
        ci::gl::VboMeshRef  _vboMeshRef  = NULL;
        ci::gl::GlslProgRef _glslProgRef = NULL;
        ci::gl::BatchRef    _defBatchRef = NULL;
        ci::gl::BatchRef    _batchRef    = NULL;
        vector<ci::gl::TextureRef> _textures;
        //ci::gl::Material * _material = NULL;
        // --------------------------
        
        // Util に移動したい.
        virtual void _appendVertex( ci::TriMeshRef m, vec3 v0, vec3 v1, vec3 v2 );
        
    };
    
}}