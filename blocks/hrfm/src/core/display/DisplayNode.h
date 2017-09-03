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
            _bounds = Rectf(0,0,0,0);
            _drawBounds = Rectf(0,0,0,0);
            visible(true);
            position(vec3(0.0f));
            size(vec2(0.0f));
            _beforeSize = vec2(0.0f);
            scale       = ci::vec3(1.0f);
            rotation    = ci::vec4(0.0f);
            transform   = ci::mat4{};
            colorA      = ci::ColorA(1.0,1.0,1.0,1.0);
        };
        
        ~DisplayNode(){};
        
        virtual void setup();
        virtual void clear();
        
        virtual ci::Rectf getBounds();
        virtual ci::Rectf getDrawBounds();
        virtual ci::vec3  getAbsolutePosition();
        virtual ci::vec3  getGlobalPosition();
        
        virtual bool isResized();
        
        virtual void enableAlphaBlending();
        virtual void enableAdditiveBlending();
        virtual void disableAlphaBlending();
        
        // ---------------------------------------------------------------
        // --- Manage Child Nodes.
        
        std::vector<DisplayNode*> children;
        
        virtual int  numChildren();
        
        virtual bool hasChildOf( DisplayNode * child );
        
        virtual bool hasStage();
        virtual IStage * getStage();
        
        virtual bool hasParent();
        virtual DisplayNode * getParent();
        
        virtual bool isRootNode();
        virtual DisplayNode * getRootNode();
        
        virtual DisplayNode * addChild( DisplayNode * child );
        virtual DisplayNode * addChildAt( DisplayNode * child, int index );
        
        virtual DisplayNode * removeChild( DisplayNode * child );
        virtual DisplayNode * removeChildAt( int index );
        virtual DisplayNode * removeOwn();
        
        // ---------------------------------------------------------------
        // --- render functions.
        
        bool _visible;
        void visible( bool value ){
            this->_visible = value;
        };
        bool visible(){ return this->_visible; };
        
        // --- Position.
        
        ci::vec3 _position;
        void position( vec2 v ){
            if( this->_position.x != v.x || this->_position.y != v.y ){
                this->_position.x = v.x;
                this->_position.y = v.y;
                this->_bounds.x1 = v.x;
                this->_bounds.x2 = v.x + this->_size.x;
                this->_bounds.y1 = v.y;
                this->_bounds.y2 = v.y + this->_size.y;
                //this->_bounds.offset( vec2( v.x - this->_bounds.x1, v.y - this->_bounds.y1 ) );
            }
        }
        void position( vec3 v ){
            if( this->_position != v ){
                this->_position = v;
                //this->_bounds.offset( vec2( v.x - this->_bounds.x1, v.y - this->_bounds.y1 ) );
                this->_bounds.x1 = v.x;
                this->_bounds.x2 = v.x + this->_size.x;
                this->_bounds.y1 = v.y;
                this->_bounds.y2 = v.y + this->_size.y;
            }
        }
        ci::vec3 position(){ return this->_position; }
        
        void x( float x ){
            if( this->_position.x != x ){
                this->_position.x = x;
                this->_bounds.offset(vec2(x-this->_bounds.x1,0));
            }
        };
        float x(){ return this->_position.x; };
        
        void y( float y ){
            if( this->_position.y != y ){
                this->_position.y = y;
                this->_bounds.offset(vec2(0,y-this->_bounds.y1));
            }
        };
        float y(){ return this->_position.y; };
        
        void z( float z ){
            if( this->_position.z != z ){
                this->_position.z = z;
            }
        };
        float z(){ return this->_position.z; };
        
        // --- Size.
        
        ci::vec2 _size;
        void size( vec2 v ){
            if( this->_size != v ){
                this->_size = v;
                this->_bounds.x2 = this->_bounds.x1 + v.x;
                this->_bounds.y2 = this->_bounds.x2 + v.y;
                this->_drawBounds.x2 = v.x;
                this->_drawBounds.y2 = v.y;
            }
        }
        ci::vec2 size(){ return this->_size; }
        
        void width( float value ){
            if( this->_size.x != value ){
                this->_size.x = value;
                this->_bounds.x2 = this->_bounds.x1 + value;
                this->_drawBounds.x2 = value;
            }
        }
        float width(){ return this->_size.x; }
        
        void height( float value ){
            if( this->_size.y != value ){
                this->_size.y = value;
                this->_bounds.y2 = this->_bounds.y1 + value;
                this->_drawBounds.y2 = value;
            }
        }
        float height(){ return this->_size.y; }
        
        /*
        bool visible;
        ci::vec3 position;
         ci::vec2 size;
         //*/
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
        
        ci::Rectf _bounds;
        ci::Rectf _drawBounds;
        
        // リサイズが行われたかを保持するための内部変数
        vec2 _beforeSize;
        bool _resized;
        
        // 描画処理用のフラグ.
        int _blendMode = 0;
        
        // 更新頻度を変更する用.
        unsigned int _updateFrequency   = 1;
        unsigned int _updateCount       = 0;
        uint32_t     _beforeUpdateFrame = 0;
        
        // 更新処理.
        virtual void _update();
        virtual void _updateChildren();
        
        // 描画処理.
        virtual void _draw();
        virtual void _drawChildren();
        
        // Display Tree Control.
        
        //! 指定した要素を children から削除します. 削除された場合 true 存在しない場合は false を返します.
        bool eraseFromChildren( DisplayNode * child );
        
        friend class IStage;
        IStage * _stage = NULL;
        virtual void _setStage( IStage * node );
        virtual void _unsetStage();
        
        DisplayNode * _parent = NULL;
        virtual void _setParent( DisplayNode * node );
        virtual void _unsetParent();
        
    };
    
}}
