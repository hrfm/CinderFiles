#include "TextureNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    // public:
    
    bool TextureNode::isDrawable(){
        return ( getTexture() != nullptr && getTexture()->getWidth() != 0 && getTexture()->getHeight() != 0 );
    }
    
    void TextureNode::setLetterbox(bool flag){
        _letterbox = flag;
    }
    
    void TextureNode::setTexture( ci::fs::path path ){
        setValue("srcPath", path);
        _init( ci::gl::Texture::create( ci::loadImage( hrfm::utils::DataLoader::load(path) ) ) );
    }
    void TextureNode::setTexture( ci::ImageSourceRef src ){
        setValue("srcPath", "__from_source__");
        _init( ci::gl::Texture::create( src ) );
    }
    void TextureNode::setTexture( ci::gl::TextureRef tex ){
        setValue("srcPath", "__from_texture__");
        _init( tex );
    }
    void TextureNode::setTexture( ci::gl::FboRef fbo ){
        setValue("srcPath", "__from_fbo__");
        _init( fbo->getColorTexture() );
    }
    
    gl::TextureRef TextureNode::getTexture(){
        return _texture;
    }
    
    // protected:
    
    void TextureNode::_init( ci::gl::TextureRef texRef ){
        _texture = texRef;
        if( width == 0 ){
            width = _texture->getWidth();
        }
        if( height == 0 ){
            height = _texture->getHeight();
        }
    }
    
    void TextureNode::_draw(){
        if( isDrawable() ){
            if( _letterbox ){
                Rectf rect = Rectf( 0, 0, getTexture()->getWidth(), getTexture()->getHeight() );
                rect = hrfm::utils::DisplayUtil::letterBox( rect, getDrawBounds() );
                gl::draw( getTexture(), rect );
            }else{ 
                gl::draw( getTexture(), getDrawBounds() );
            }
        }
    }
    
}}