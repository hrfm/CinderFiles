#include "ParticleBomb.h"

namespace hrfm { namespace display{
    
    void ParticleBomb::setup( ci::gl::VboMeshRef vboMesh, vec3 bias, float fadeOut ){
        
        _shader = ci::gl::GlslProg::create( ci::gl::GlslProg::Format()
                                           .vertex  ( DataLoader::load( "bomb_vert.glsl" ) )
                                           .geometry( DataLoader::load( "bomb_geo.glsl" ) )
                                           .fragment( DataLoader::load( "bomb_frag.glsl" ) )
                                           );
        
        _fadeOut = fadeOut;
        
        this->setBatchRef( ci::gl::Batch::create( vboMesh, _shader ) );
        _shader->uniform( "uBias", bias );
        _shader->uniform( "uFadeOut", fadeOut );
        
        _startSec = -1.0;
        
    }
    
    void ParticleBomb::boom(){
        _startSec = hrfm::app::SiAppInfo::getInstance().getElapsedSeconds();
    }
    
    void ParticleBomb::addTexture( ci::fs::path path ){
        addTexture( ci::gl::Texture::create( ci::loadImage( DataLoader::load(path) ) ) );
    }
    
    void ParticleBomb::addTexture( ci::gl::TextureRef tex ){
        _textures.push_back(tex);
    }
    
    void ParticleBomb::setBias( vec3 bias ){
        _shader->uniform( "uBias", bias );
    }
    
    void ParticleBomb::setParticleScale( vec3 scale ){
        _shader->uniform( "uParticleScale", scale );
    }
    
    void ParticleBomb::setNormalMoveScale( float scale ){
        _shader->uniform( "uNormalMoveScale", scale );
    }
    
    void ParticleBomb::setRadialMoveScale( float scale, vec3 center ){
        _shader->uniform( "uRadialMoveScale", scale );
        _shader->uniform( "uRadialCenter", center );
    }
    
    void ParticleBomb::_update(){
        if( 0.0 <= _startSec ){
            double sec = hrfm::app::SiAppInfo::getInstance().getElapsedSeconds();
            _shader->uniform( "uTime", float(sec-_startSec) );
            if( _fadeOut + 1.0 < sec-_startSec ){
                this->removeOwn();
            }
        }else{
            _shader->uniform( "uTime", 0.0f );
        }
    }
    
    void ParticleBomb::_draw(){
        
        int idx = 0;
        std::vector<ci::gl::TextureRef>::iterator it, end;
        for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
            (*it)->bind(idx);
            _shader->uniform( "uTex[" + toString(idx) + "]", idx );
            idx++;
        }
        _shader->uniform( "uNumTexture", float(_textures.size()) );
        
        ci::gl::disableDepthRead();
        ci::gl::enableAlphaBlending();
        BatchNode::_draw();
        
        for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
            (*it)->unbind();
        }
        
    }
    
}}