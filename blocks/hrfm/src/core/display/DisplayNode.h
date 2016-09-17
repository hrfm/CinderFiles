#pragma once

#include <list>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "EventDispatcher.h"
#include "DynamicMap.h"
#include "SiAppInfo.h"

namespace hrfm{ namespace display{
    
    class Stage;
    
    /**
     DisplayNode is base of managed display objects likes Flash.
     */
    class DisplayNode : public hrfm::events::EventDispatcher{
    
    public:
        
        DisplayNode():hrfm::events::EventDispatcher(){
            
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            
            width  = 1.0f;
            height = 1.0f;
            _beforeWidth  = 0.0f;
            _beforeHeight = 0.0f;
            
            transform = ci::mat4{};
            
            scale   = ci::vec3(1.0f,1.0f,1.0f);
            
            colorA  = ci::ColorA(1.0,1.0,1.0,1.0);
            
            visible = true;
            
        };
        
        ~DisplayNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual void enableAdditiveBlending( bool flag = true );
        
        virtual ci::vec3 getPosition();
        virtual void setPosition( float x, float y );
        virtual void setPosition( float x, float y, float z );
        virtual void setPosition( ci::vec2 position );
        virtual void setPosition( ci::vec3 position );
        
        virtual ci::ivec2 getSize();
        virtual void setSize( int w, int h );
        virtual void setSize( ci::ivec2 size );
        
        virtual ci::vec3 getScale();
        virtual void setScale( float scale );
        virtual void setScale( ci::vec3 scale );
        
        virtual ci::Rectf getBounds();
        virtual ci::Rectf getDrawBounds();
        virtual ci::vec2  getAbsolutePosition();
        virtual ci::vec2  getGlobalPosition();
        
        virtual int  numChildren();
        virtual bool hasChildOf( DisplayNode * child );
        
        virtual ci::Camera * getCamera();
        
        virtual DisplayNode * addChild( DisplayNode * child );
        virtual DisplayNode * addChildAt( DisplayNode * child, int index );
        virtual DisplayNode * removeChild( DisplayNode * child );
        virtual DisplayNode * removeChildAt( int index );
        virtual DisplayNode * removeOwn();
        
        virtual void update();
        virtual void draw( ci::ColorA * drawColor = NULL );
        
        virtual bool isResized();
        
        virtual bool hasParent();
        virtual DisplayNode * getParent();
        
        virtual bool hasStage();
        virtual Stage * getStage();
        
        // --- PROPERTY -------------
        hrfm::utils::DynamicMap values;
        std::vector<DisplayNode*> children;
        ci::Camera * camera = NULL;
        bool visible;
        ci::ColorA  colorA;
        int width, height;
        ci::mat4 transform;
        float    x, y, z;
        ci::vec3 scale;
        float    rotate = 0.0;
        // --------------------------
        
        void setUpdateFrequency( unsigned int freq );
        
    protected:
        
        unsigned int _updateFequency    = 1;
        unsigned int _updateCount       = 0;
        uint32_t     _beforeUpdateFrame = 0;
        
        //! 指定した要素を children から削除します. 削除された場合 true 存在しない場合は false を返します.
        bool eraseFromChildren( DisplayNode * child );
        
        virtual void _updatePosition( float x, float y, float z );
        
        virtual void _update();
        virtual void _updateChildren();
        
        virtual void _draw();
        virtual void _drawChildren( ci::ColorA * drawColor = NULL );
        
        bool _enableAdditiveBlending;
        
        int _beforeWidth;
        int _beforeHeight;
        bool _resized;
        
        // Stage control.
        
        friend class Stage;
        
        Stage * _stage = NULL;
        
        std::vector<Stage*> _stages;
        
        virtual void _setStage( Stage * node );
        virtual void _unsetStage();
        
        DisplayNode * _parent = NULL;
        virtual void _setParent( DisplayNode * node );
        virtual void _unsetParent();
        
    };
    
}}