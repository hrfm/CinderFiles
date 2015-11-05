#pragma once

#include <list>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "EventDispatcher.h"

namespace hrfm{ namespace display{
    
    class Stage;
    
    /**
     DisplayNode is base of managed display objects likes Flash.
     */
    class DisplayNode : public hrfm::events::EventDispatcher{
    
    public:
        
        DisplayNode():hrfm::events::EventDispatcher(){
            x       = 0.0f;
            y       = 0.0f;
            colorA  = ci::ColorA(1.0,1.0,1.0,1.0);
            visible = true;
            width   = 0.0f;
            height  = 0.0f;
            _beforeWidth  = 0.0f;
            _beforeHeight = 0.0f;
        };
        ~DisplayNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual ci::ivec2 getSize();
        virtual void setSize( int w, int h );
        virtual void setSize( ci::ivec2 size );
        
        virtual ci::fs::path getSrcPath();
        
        virtual ci::Rectf getBounds();
        virtual ci::Rectf getDrawBounds();
        virtual ci::vec2  getAbsolutePosition();
        virtual ci::vec2  getGlobalPosition();
        
        virtual int  numChildren();
        virtual bool hasChildOf( DisplayNode * child );
        
        virtual DisplayNode * addChild( DisplayNode * child );
        virtual DisplayNode * addChildAt( DisplayNode * child, int index );
        virtual DisplayNode * removeChild( DisplayNode * child );
        virtual DisplayNode * removeChildAt( int index );
        virtual DisplayNode * removeOwn();
        
        virtual void update();
        virtual void draw( ci::ColorA * drawColor = NULL );
        
        virtual bool hasParent();
        virtual DisplayNode * getParent();
        
        virtual bool hasStage();
        virtual Stage * getStage();
        
        // --- PROPERTY -------------
        std::vector<DisplayNode*> children;
        float x, y;
        int   width, height;
        bool  visible;
        ci::ColorA  colorA;
        // --------------------------
        
    protected:
        
        //! 指定した要素を children から削除します. 削除された場合 true 存在しない場合は false を返します.
        bool eraseFromChildren( DisplayNode * child );
        
        virtual void _update();
        virtual void _updateChildren();
        
        virtual void _draw();
        virtual void _drawChildren( ci::ColorA * drawColor = NULL );
        
        int _beforeWidth;
        int _beforeHeight;
        ci::fs::path _srcPath = "";
        
        // Stage control.
        
        friend class Stage;
        
        Stage * _stage = NULL;
        virtual void _setStage( Stage * node );
        virtual void _unsetStage();
        
        DisplayNode * _parent = NULL;
        virtual void _setParent( DisplayNode * node );
        virtual void _unsetParent();
        
        
    };
    
}}