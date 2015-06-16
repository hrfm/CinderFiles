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
    
    void TextureNode::setLetterbox(bool flag){
        _letterbox = flag;
    }
    
    gl::Texture TextureNode::getTexture(){
        return _texture;
    }
    
    // protected:
    
    void TextureNode::_draw(){
        if( isDrawable() ){
            if( _letterbox ){
                Rectf rect = Rectf( 0, 0, getTexture().getWidth(), getTexture().getHeight() );
                rect = hrfm::utils::DisplayUtil::letterBox( rect, getDrawBounds() );
                gl::draw( getTexture(), rect );
            }else{ 
                gl::draw( getTexture(), getDrawBounds() );
            }
        }
    }
    
}}