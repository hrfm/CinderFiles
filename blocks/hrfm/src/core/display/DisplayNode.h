#pragma once

#include "IDrawable.h"
#include <list>

namespace hrfm{ namespace display{
    
    class DisplayNode : public IDrawable{
    
    public:
        
        DisplayNode(){ IDrawable(); };
        ~DisplayNode(){};
        
        virtual int numChildren();
        
        virtual IDrawable * addChild( IDrawable * child );
        virtual IDrawable * removeChild( IDrawable * child );
        
        virtual IDrawable * addChildAt( IDrawable * child, int index );
        virtual IDrawable * removeChildAt( int index );
        
        virtual void update();
        virtual void draw();
        
        virtual DisplayNode * removeOwn();
        
        std::vector<IDrawable*> children;
        
    protected:
        
        //! 指定した要素を children から削除します. 削除された場合 true 存在しない場合は false を返します.
        bool eraseFromChildren( IDrawable * child );
        
        //! addChild で追加された子要素を更新します.
        void _updateChildren();
        
        //! addChild で追加された子要素を描画します.
        void _drawChildren();
        
        virtual void _unsetStage();
        
    };
    
}}