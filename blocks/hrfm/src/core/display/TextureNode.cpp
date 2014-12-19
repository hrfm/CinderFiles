#include "TextureNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    // public:
    
    TextureNode::TextureNode(){
        DisplayNode();
    }
    
    TextureNode::~TextureNode(){}
    
    bool TextureNode::isDrawable(){
        return ( getTexture() != nullptr && getTexture().getWidth() != 0 && getTexture().getHeight() != 0 );
    }
    
    void TextureNode::setTexture( ci::gl::Texture tex ){
        _texture = tex;
    }
    
    gl::Texture TextureNode::getTexture(){
        return _texture;
    }
    
    // protected:
    
    void TextureNode::_draw(){
        if( isDrawable() ){
            gl::draw( getTexture(), getDrawBounds() );
        }
    }
    
}}