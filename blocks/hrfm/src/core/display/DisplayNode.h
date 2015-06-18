#pragma once

#include <list>
#include "cinder/app/AppNative.h"
#include "EventDispatcher.h"

namespace hrfm{ namespace display{
    
    class Stage;
    
    class DisplayNode : public hrfm::events::EventDispatcher{
    
    public:
        
        DisplayNode(){
            hrfm::events::EventDispatcher();
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
        
        virtual ci::Vec2i getSize();
        virtual void setSize( int w, int h );
        virtual void setSize( ci::Vec2i size );
        
        virtual ci::fs::path * getSrcPath();
        
        virtual ci::Rectf getBounds();
        virtual ci::Rectf getDrawBounds();
        virtual ci::Vec2f getAbsolutePosition();
        
        virtual int numChildren();
        
        virtual DisplayNode * addChild( DisplayNode * child );
        virtual DisplayNode * removeChild( DisplayNode * child );
        
        virtual DisplayNode * addChildAt( DisplayNode * child, int index );
        virtual DisplayNode * removeChildAt( int index );
        virtual DisplayNode * removeOwn();
        
        virtual bool hasChildOf( DisplayNode * child );
        
        virtual void update();
        virtual void draw( ci::ColorA * drawColor = NULL );
        
        virtual bool hasParent();
        virtual DisplayNode * getParent();
        
        virtual bool hasStage();
        virtual Stage * getStage();

        std::vector<DisplayNode*> children;
        
        virtual ci::Vec2f getGlobalPosition();
        
        // --- PROPERTY -------------
        float       x, y;
        int         width, height;
        bool        visible;
        ci::ColorA  colorA;
        // --------------------------
        
    protected:
        
        virtual void _draw();
        virtual void _update();
        
        //! 指定した要素を children から削除します. 削除された場合 true 存在しない場合は false を返します.
        bool eraseFromChildren( DisplayNode * child );
        
        //! addChild で追加された子要素を更新します.
        virtual void _updateChildren();
        
        //! addChild で追加された子要素を描画します.
        virtual void _drawChildren( ci::ColorA * drawColor = NULL );
        
        int _beforeWidth;
        int _beforeHeight;
        
    protected:
        
        friend class Stage;
        
        Stage * _stage = NULL;
        virtual void _setStage( Stage * node );
        virtual void _unsetStage();
        
        DisplayNode * _parent = NULL;
        virtual void _setParent( DisplayNode * node );
        virtual void _unsetParent();
        
        ci::fs::path * _srcPath = NULL;
        
    };
    
}}