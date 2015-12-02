#pragma once

#include "DisplayNode.h"
#include "DisplayUtil.h"
#include "DataLoader.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

namespace hrfm{ namespace display{
    
    class TextureNode : public DisplayNode{
    
    public:
        
        TextureNode():DisplayNode(){};
        TextureNode( ci::fs::path path ):DisplayNode(){
            setTexture(path);
        };
        TextureNode( ci::ImageSourceRef src ):DisplayNode(){
            setTexture(src);
        };
        TextureNode( ci::gl::TextureRef tex ):DisplayNode(){
            setTexture(tex);
        };
        TextureNode( ci::gl::FboRef fbo ):DisplayNode(){
            setTexture(fbo);
        };
        ~TextureNode(){};
        
        virtual bool isDrawable();
        
        void setLetterbox( bool flag );
        
        void setTexture( ci::fs::path path );
        void setTexture( ci::ImageSourceRef src );
        void setTexture( ci::gl::TextureRef tex );
        void setTexture( ci::gl::FboRef fbo );
        
        virtual ci::gl::TextureRef getTexture();
        
    protected:
        
        virtual void _init( ci::gl::TextureRef tex );
        virtual void _draw();
        
        bool _letterbox;
        ci::gl::TextureRef _texture;
        
    private:
        
    };
    
}}