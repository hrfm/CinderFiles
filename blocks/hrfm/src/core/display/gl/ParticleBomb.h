#pragma once

#include "hrfm.h"
#include "SiAppInfo.h"
#include "cinder/gl/gl.h"

namespace hrfm { namespace display{

    class ParticleBomb : public hrfm::display::BatchNode{
        
    public:
        
        ParticleBomb():hrfm::display::BatchNode(){}
        
        ParticleBomb( ci::gl::VboMeshRef vboMesh, vec3 bias = vec3(0.0), float fadeOut = 3.0 ):hrfm::display::BatchNode(){
            setup( vboMesh, bias, fadeOut );
        }
        
        void setup( ci::gl::VboMeshRef vboMesh, vec3 bias = vec3(0.0), float fadeOut = 3.0 );
        
        void boom();
        
        void addTexture( ci::fs::path path );
        void addTexture( ci::gl::TextureRef tex );
        
        void setBias( vec3 bias );
        void setParticleScale( vec3 scale );
        void setNormalMoveScale( float scale );
        void setRadialMoveScale( float scale, vec3 center );
        
    protected:
        
        double _startSec;
        float  _fadeOut;
        ci::gl::GlslProgRef _shader;
        vector<ci::gl::TextureRef> _textures;
        
        void _update();
        void _draw();
        
    };

}}