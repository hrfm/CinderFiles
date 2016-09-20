#include "GridInstanced.h"

namespace hrfm{ namespace display{
    
    void GridInstanced::setTexture( ci::gl::TextureRef tex ){
        _useTexture = true;
        _tex = tex;
    }
    
    void GridInstanced::_update(){
        
        auto sec       = hrfm::app::SiAppInfo::getInstance().getElapsedSeconds();
        _shader->uniform( "uTime", float(sec) );
        
        mat4 scaleMtx = ci::scale(_scale);
        int idx = 0;
        mat4 *t = (mat4*)_mInstanceDataVbo->mapReplace();
        for( int x=0; x<_numSegments;x++ ){
            for( int y=0; y<_numSegments; y++ ){
                for( int z=0; z<_numSegments;z++ ){
                    
                    vec3 v  = points.at(idx);
                    vec3 * dV = &(offsets.at(idx));
                    
                    dV->y = tan( (sec+length(v)*0.03) * 3.0 ) / 60.0;
                    
                    //dV->y = tan( (sec+float(z)*0.03) * 3.0 ) / 60.0;
                    if( 10.0 < dV->y ){
                        dV->y = 10.0;
                    }else if( dV->y < -10.0 ){
                        dV->y = -10.0;
                    }
                    
                    mat4 mtx = {};
                    mtx *= ci::translate(v+*dV) * scaleMtx;
                    *t++ = mtx;
                    
                    idx++;
                    
                }
            }
        }
        _mInstanceDataVbo->unmap();
        
        this->transform = ci::rotate( float(sec*0.2), vec3(1.0,1.0,1.0) ) * ci::translate( vec3(0.0,0.0,0.0) );
        
    };
    
    void GridInstanced::_draw(){
        
        ci::gl::ScopedDepth scp(true);
        ci::gl::disableAlphaBlending();
        
        if( _useTexture ){
            _shader->uniform( "uUseTexture", 1 );
            _shader->uniform( "tex", 0 );
            ci::gl::ScopedTextureBind tex(_tex);
            InstancedVboMesh::_draw();
        }else{
            _shader->uniform( "uUseTexture", 0 );
            InstancedVboMesh::_draw();
        }
        
    }
    
}}