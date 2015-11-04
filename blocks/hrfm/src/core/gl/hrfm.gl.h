#pragma once

#include "cinder/gl/Texture.h"
#include "SiFboFactory.h"
#include "ShaderFactory.h"
#include "VboStage.h"

using namespace std;
using namespace hrfm::utils;

namespace hrfm{ namespace gl{
    
    static ci::gl::Texture2dRef resizeTexture( ci::gl::Texture2dRef texture, ivec2 size ){
        
        ci::gl::FboRef fbo = hrfm::gl::SiFboFactory::getInstance().create( size.x, size.y );
        
        std::pair<ivec2,ivec2> viewport = ci::gl::getViewport();
        
        fbo->bindFramebuffer();
        {
            ci::gl::pushMatrices();
            //ci::gl::setViewport( fbo->getBounds() );
            ci::gl::setMatricesWindow( fbo->getSize(), false );
            {
                ci::gl::color( Color( 1.0, 1.0, 1.0 ) );
                ci::gl::draw( texture, fbo->getBounds() );
            }
            ci::gl::popMatrices();
        }
        fbo->unbindFramebuffer();
        
        //ci::gl::setViewport(viewport);
        
        return fbo->getColorTexture();
        
    }
    
}}