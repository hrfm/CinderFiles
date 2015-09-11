#pragma once

#include <list>

#include "cinder/Camera.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Vbo.h"

#include "EventDispatcher.h"
#include "ShaderBase.h"

namespace hrfm{ namespace gl{
    
    class VboStage;
    
    class VboNode : public hrfm::events::EventDispatcher{
    
    public:
        
        VboNode( ci::gl::VboMesh * mesh = NULL ):hrfm::events::EventDispatcher(){
            position = ci::Vec3f(0.0f,0.0f,0.0f);
            scale    = ci::Vec3f(1.0f,1.0f,1.0f);
            rotation.setToIdentity();
            colorA  = ci::ColorA(1.0,1.0,1.0,1.0);
            visible = true;
            this->mesh = mesh;
        };
        ~VboNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual ci::Vec3f getAbsolutePosition();
        
        virtual ci::Vec3f getPosition();
        virtual void setPosition( ci::Vec3f position );
        
        virtual ci::Vec3f getScale();
        virtual void setScale( float scale );
        virtual void setScale( ci::Vec3f scale );
        
        virtual ci::Matrix44f getRotation();
        virtual void setRotation( ci::Matrix44f rotation );
        
        virtual void setEnableWireframe( bool flag );
        virtual void setShader( hrfm::gl::ShaderBase * shader );
        virtual void setMaterial( ci::gl::Material * material );
        
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
        ci::Vec3f position, scale;
        ci::Matrix44f rotation;
        bool  visible;
        ci::ColorA colorA;
        ci::gl::VboMesh * mesh;
        // --------------------------
        
    protected:
        
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
        ci::gl::Material * _material = NULL;
        
        VboStage * _stage = NULL;
        virtual void _setStage( VboStage * node );
        virtual void _unsetStage();
        
        VboNode * _parent = NULL;
        virtual void _setParent( VboNode * node );
        virtual void _unsetParent();
        
    };
    
}}