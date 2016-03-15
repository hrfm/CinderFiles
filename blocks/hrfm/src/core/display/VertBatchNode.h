#pragma once

#include <list>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Batch.h"

#include "DisplayNode.h"

// 基本 Batch 化する。
// drawChildren 等は指定がない限り 親の shader を継承する.
// mesh を指定しない場合は draw 内で描画はせず、子に shader を渡す。

namespace hrfm{ namespace display{
    
    class VertBatchNode : public hrfm::display::DisplayNode{
        
    public:
        
        VertBatchNode():hrfm::display::DisplayNode(){
            this->rotation = ci::mat4();
        }
        
        VertBatchNode( ci::gl::VertBatchRef batchRef, ci::gl::GlslProgRef shader = nullptr ):hrfm::display::DisplayNode(){
            this->rotation = ci::mat4();
            this->setBatchRef( batchRef, shader );
        };
        
        ~VertBatchNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual void setBatchRef( ci::gl::VertBatchRef batchRef, ci::gl::GlslProgRef shader = nullptr );
        virtual ci::gl::VertBatchRef getBatchRef();
        
        virtual void setEnableWireframe( bool flag );
        
        virtual ci::mat4 getRotation();
        virtual void setRotation( ci::mat4 rotation );
        
        virtual void addTexture( ci::gl::TextureRef tex );
        virtual void removeTexture( ci::gl::TextureRef tex );
        
        virtual void update();
        virtual void draw( ci::ColorA * drawColor = NULL );
        
        // --- PROPERTY -------------
        ci::mat4 rotation;
        // --------------------------
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        bool eraseTextureFromList( ci::gl::TextureRef tex );
        
        // --- PROPERTY -------------
        bool _enableWireframe = false;
        ci::gl::VertBatchRef _batchRef = NULL;
        ci::gl::GlslProgRef  _shader;
        vector<ci::gl::TextureRef> _textures;
        // --------------------------
        
    };
    
}}