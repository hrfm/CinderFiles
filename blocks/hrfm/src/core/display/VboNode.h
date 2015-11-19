#pragma once

#include <list>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
//#include "cinder/gl/Light.h"
//#include "cinder/gl/Material.h"
#include "cinder/gl/Vbo.h"

#include "DisplayNode.h"
#include "ShaderBase.h"

namespace hrfm{ namespace display{
    
    class VboStage;
    
    class VboNode : public hrfm::display::DisplayNode{
    
    public:
        
        VboNode( ci::gl::VboMeshRef mesh = NULL ):hrfm::display::DisplayNode(){
            rotation = mat4();
            this->mesh = mesh;
        };
        ~VboNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual ci::mat4 getRotation();
        virtual void setRotation( ci::mat4 rotation );
        
        virtual void setEnableWireframe( bool flag );
        virtual void setShader( hrfm::gl::ShaderBase * shader );
        //virtual void setMaterial( ci::gl::Material * material );
        //virtual ci::gl::Material * getMaterial();
        
        virtual void addTexture( ci::gl::TextureRef tex );
        virtual void removeTexture( ci::gl::TextureRef tex );
        //virtual void setTextureAt( ci::gl::TextureRef tex, int index );
        
        virtual void update();
        virtual void draw( ci::ColorA * drawColor = NULL );
        
        // --- PROPERTY -------------
        ci::mat4 rotation;
        ci::gl::VboMeshRef mesh;
        // --------------------------
        
    protected:
        
        virtual void _appendVertex( ci::TriMeshRef m, vec3 v0, vec3 v1, vec3 v2 );
        
        virtual void _update();
        virtual void _draw();
        
    protected:
        
        bool _enableWireframe = false;
        hrfm::gl::ShaderBase * _shader = NULL;
        //ci::gl::Material * _material = NULL;
        
        vector<ci::gl::TextureRef> _textures;
        bool eraseTextureFromList( ci::gl::TextureRef tex );
        
    };
    
}}