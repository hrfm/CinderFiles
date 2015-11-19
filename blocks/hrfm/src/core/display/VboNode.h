#pragma once

#include <list>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
//#include "cinder/gl/Light.h"
//#include "cinder/gl/Material.h"
#include "cinder/gl/Vbo.h"

#include "EventDispatcher.h"
#include "ShaderBase.h"

namespace hrfm{ namespace display{
    
    class VboStage;
    
    class VboNode : public hrfm::events::EventDispatcher{
    
    public:
        
        VboNode( ci::gl::VboMeshRef mesh = NULL ):hrfm::events::EventDispatcher(){
            position = ci::vec3(0.0f,0.0f,0.0f);
            scale    = ci::vec3(1.0f,1.0f,1.0f);
            rotation = mat4();
            colorA  = ci::ColorA(1.0,1.0,1.0,1.0);
            visible = true;
            this->mesh = mesh;
        };
        ~VboNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual ci::vec3 getAbsolutePosition();
        
        virtual ci::vec3 getPosition();
        virtual void setPosition( ci::vec3 position );
        
        virtual ci::vec3 getScale();
        virtual void setScale( float scale );
        virtual void setScale( ci::vec3 scale );
        
        virtual ci::mat4 getRotation();
        virtual void setRotation( ci::mat4 rotation );
        
        virtual void setEnableWireframe( bool flag );
        virtual void setShader( hrfm::gl::ShaderBase * shader );
        //virtual void setMaterial( ci::gl::Material * material );
        //virtual ci::gl::Material * getMaterial();
        
        virtual void addTexture( ci::gl::TextureRef tex );
        virtual void removeTexture( ci::gl::TextureRef tex );
        virtual void setTextureAt( int index, ci::gl::TextureRef tex );
        
        virtual int numChildren();
        
        virtual VboNode * addChild( VboNode * child );
        virtual VboNode * removeChild( VboNode * child );
        
        virtual VboNode * addChildAt( VboNode * child, int index );
        virtual VboNode * removeChildAt( int index );
        virtual VboNode * removeOwn();
        
        virtual bool hasChildOf( VboNode * child );
        
        virtual void update( ci::CameraPersp * camera );
        virtual void draw( ci::CameraPersp * camera, ci::ColorA * drawColor = NULL );
        
        virtual bool hasParent();
        virtual VboNode * getParent();
        
        virtual bool hasStage();
        virtual VboStage * getStage();
        
        std::vector<VboNode*> children;
        
        // --- PROPERTY -------------
        ci::vec3 position, scale;
        ci::mat4 rotation;
        bool  visible;
        ci::ColorA colorA;
        ci::gl::VboMeshRef mesh;
        // --------------------------
        
    protected:
        
        virtual void _appendVertex( ci::TriMeshRef m, vec3 v0, vec3 v1, vec3 v2 );
        
        virtual void _update( ci::CameraPersp * camera );
        virtual void _draw( ci::CameraPersp * camera );
        
        //! 指定した要素を children から削除します. 削除された場合 true 存在しない場合は false を返します.
        bool eraseFromChildren( VboNode * child );
        
        //! addChild で追加された子要素を更新します.
        virtual void _updateChildren( ci::CameraPersp * camera );
        
        //! addChild で追加された子要素を描画します.
        virtual void _drawChildren( ci::CameraPersp * camera, ci::ColorA * drawColor = NULL );
        
    protected:
        
        friend class VboStage;
        
        bool _enableWireframe = false;
        hrfm::gl::ShaderBase * _shader = NULL;
        //ci::gl::Material * _material = NULL;
        
        vector<ci::gl::TextureRef> _textures;
        bool eraseTextureFromList( ci::gl::TextureRef tex );
        
        VboStage * _stage = NULL;
        virtual void _setStage( VboStage * node );
        virtual void _unsetStage();
        
        VboNode * _parent = NULL;
        virtual void _setParent( VboNode * node );
        virtual void _unsetParent();
        
    };
    
}}