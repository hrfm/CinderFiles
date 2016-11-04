#pragma once

#include <list>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "EventDispatcher.h"
#include "DynamicMap.h"
#include "SiAppInfo.h"

namespace hrfm{ namespace display{
    
    class IStage;
    
    /**
     DisplayNode is base of managed display objects likes Flash.
     */
    class DisplayNode : public hrfm::events::EventDispatcher{
    
    public:
        
        DisplayNode():hrfm::events::EventDispatcher(){
            position    = vec3(0.0f);
            size        = vec2(0.0f);
            _beforeSize = vec2(0.0f);
            scale       = ci::vec3(1.0f);
            rotation    = ci::vec4(0.0f);
            transform   = ci::mat4{};
            colorA      = ci::ColorA(1.0,1.0,1.0,1.0);
            visible     = true;
        };
        
        ~DisplayNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual ci::Rectf getBounds();
        virtual ci::Rectf getDrawBounds();
        virtual ci::vec3  getAbsolutePosition();
        virtual ci::vec3  getGlobalPosition();
        
        virtual bool isResized();
        
        // ---------------------------------------------------------------
        // --- Manage Child Nodes.
        
        std::vector<DisplayNode*> children;
        
        virtual int  numChildren();
        
        virtual bool hasChildOf( DisplayNode * child );
        
        virtual DisplayNode * addChild( DisplayNode * child );
        virtual DisplayNode * addChildAt( DisplayNode * child, int index );
        
        virtual DisplayNode * removeChild( DisplayNode * child );
        virtual DisplayNode * removeChildAt( int index );
        virtual DisplayNode * removeOwn();
        
        virtual bool hasParent();
        virtual DisplayNode * getParent();
        
        virtual bool isRootNode();
        virtual DisplayNode * getRootNode();
        
        virtual bool hasStage();
        virtual IStage * getStage();
        
        // ---------------------------------------------------------------
        // --- render functions.
        
        bool visible;
        ci::vec3 position;
        ci::vec2 size;
        ci::vec3 scale;
        ci::vec4 rotation;
        ci::ColorA colorA;
        ci::mat4 transform;
        
        void setUpdateFrequency( unsigned int freq );
        
        virtual void update();
        virtual void draw();
        
        // ---------------------------------------------------------------
        // --- etc.
        
        hrfm::utils::DynamicMap values;
        
    protected:
        
        unsigned int _updateFrequency   = 1;
        unsigned int _updateCount       = 0;
        uint32_t     _beforeUpdateFrame = 0;
        
        virtual void _update();
        virtual void _updateChildren();
        
        virtual void _draw();
        virtual void _drawChildren();
        
        //! 指定した要素を children から削除します. 削除された場合 true 存在しない場合は false を返します.
        bool eraseFromChildren( DisplayNode * child );
        
        vec2 _beforeSize;
        bool _resized;
        
        // Stage control.
        
        friend class IStage;
        
        IStage * _stage = NULL;
        virtual void _setStage( IStage * node );
        virtual void _unsetStage();
        
        DisplayNode * _parent = NULL;
        virtual void _setParent( DisplayNode * node );
        virtual void _unsetParent();
        
    };
    
}}