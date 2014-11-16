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
            alpha = 1.0f;
            x = 0.0f;
            y = 0.0f;
            width  = 0.0f;
            height = 0.0f;
        };
        ~DisplayNode(){};
        
        virtual void setup();
        
        virtual ci::Vec2i getSize();
        virtual void setSize( int w, int h );
        virtual void setSize( ci::Vec2i size );
        
        virtual ci::Rectf getBounds();
        virtual ci::Rectf getDrawBounds();
        
        virtual int numChildren();
        
        virtual DisplayNode * addChild( DisplayNode * child );
        virtual DisplayNode * removeChild( DisplayNode * child );
        
        virtual DisplayNode * addChildAt( DisplayNode * child, int index );
        virtual DisplayNode * removeChildAt( int index );
        virtual DisplayNode * removeOwn();
        
        virtual void update();
        virtual void draw();
        
        virtual bool hasParent();
        virtual DisplayNode * getParent();
        
        virtual bool hasStage();
        virtual Stage * getStage();

        std::vector<DisplayNode*> children;
        
        float alpha;
        float x, y;
        int width, height;
        
    protected:
        
        virtual void _draw();
        virtual void _update();
        
        //! 指定した要素を children から削除します. 削除された場合 true 存在しない場合は false を返します.
        bool eraseFromChildren( DisplayNode * child );
        
        //! addChild で追加された子要素を更新します.
        void _updateChildren();
        
        //! addChild で追加された子要素を描画します.
        void _drawChildren();
        
    protected:
        
        friend class Stage;
        
        Stage * _stage;
        virtual void _setStage( Stage * node );
        virtual void _unsetStage();
        
        DisplayNode * _parent;
        virtual void _setParent( DisplayNode * node );
        virtual void _unsetParent();
        
    };
    
}}