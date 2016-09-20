#pragma once

#include "hrfm.h"
#include "SiAppInfo.h"
#include "cinder/gl/gl.h"

class GeoEqualizer : public hrfm::display::BatchNode{
    
public:
    
    GeoEqualizer():hrfm::display::BatchNode(){
        setup( ci::gl::VboMesh::create( ci::geom::Sphere().subdivisions(80) ) );
    }
    
    GeoEqualizer( TriMesh mesh ):hrfm::display::BatchNode(){
        setup( ci::gl::VboMesh::create(mesh) );
    }
    
    GeoEqualizer( ci::gl::VboMeshRef vboMesh ):hrfm::display::BatchNode(){
        setup( vboMesh );
    }
    
    void setup( ci::gl::VboMeshRef vboMesh ){
        _shader = ci::gl::GlslProg::create( ci::gl::GlslProg::Format()
                                           .vertex  ( DataLoader::load( "geoeq_vert.glsl" ) )
                                           .geometry( DataLoader::load( "geoeq_geo.glsl" ) )
                                           .fragment( DataLoader::load( "geoeq_frag.glsl" ) )
                                           );
        this->setBatchRef( ci::gl::Batch::create( vboMesh, _shader ) );
        
        _audio = &hrfm::io::SiAudioInput::getInstance();
        
        this->setEnableDepthCheck(true);
        //this->setEnableWireframe(true);
        
    }
    
    //void addTexture( ci::fs::path path );
    //void addTexture( ci::gl::TextureRef tex );
    
protected:
    
    hrfm::io::SiAudioInput * _audio;
    ci::gl::GlslProgRef _shader;
    //vector<ci::gl::TextureRef> _textures;
    
    void _update(){
        
        auto sec = hrfm::app::SiAppInfo::getInstance().getElapsedSeconds();
        
        hrfm::io::SiAudioInput * audio = &hrfm::io::SiAudioInput::getInstance();
        _shader->uniform( "uWave"  , audio->getChannelAt(0), 64 );
        _shader->uniform( "uFFT"  , audio->getFFTNormalized(), 16 );
        _shader->uniform( "uVolume", audio->getVolume() );
        _shader->uniform( "uTime"  , float(sec) );
        
        this->transform = ci::translate(vec3(0.0,0.0,2.0))
                            * ci::rotate( -3.41592f/2.0f, vec3(1.0,0.3,0.8) )
                            * ci::rotate( float(sec), vec3(0.0,1.0,0.0) )
        ;
        
    }
    
    void _draw(){
        ci::gl::enable(GL_CULL_FACE,GL_FRONT);
        BatchNode::_draw();
        ci::gl::disable(GL_CULL_FACE);
    }
    
};