#pragma once

#include "InstancedVboMesh.h"

namespace hrfm{ namespace display{
    
    class GridInstanced : public InstancedVboMesh{
        
    public:
        
        GridInstanced():InstancedVboMesh(){};
        
        GridInstanced( ci::gl::VboMeshRef mesh, int numSegments = 30, vec3 scale = vec3(0.06) ):InstancedVboMesh(){
            
            _scale = scale;
            _numSegments = numSegments;
            
            auto shader = ci::gl::GlslProg::create( ci::gl::GlslProg::Format()
                                                   .vertex  ( hrfm::io::DataLoader::load("InstancedVboMesh_vert.glsl") )
                                                   //.geometry(DataLoader::load("geo.glsl"))
                                                   .fragment( hrfm::io::DataLoader::load( "grid_frag.glsl" ) )
                                                   );
            shader->uniform("shadowDistanceFrom",1.0f);
            shader->uniform("shadowDistance",4.0f);
            
            init( mesh, shader, numSegments * numSegments * numSegments );
            
            float step = 1.0 / float(numSegments);
            for( int x=0; x<numSegments;x++ ){
                for( int y=0; y<numSegments; y++ ){
                    for( int z=0; z<numSegments;z++ ){
                        vec3 v = vec3( -0.5 + step*float(x), -0.5 + step*float(y), -0.5 + step*float(z) );
                        if( v.x == 0 && v.z == 0 ){
                            points.push_back( vec3(-10000.0,0,0) );
                            offsets.push_back(vec3(0.0));
                        }else{
                            v *= vec3( 10.0 );
                            points.push_back(v);
                            offsets.push_back(vec3(0.0));
                        }
                    }
                }
            }
            
            this->transform = ci::translate( vec3(0.0,0.0,0.0) );
            
        };
        
        void setTexture( ci::gl::TextureRef tex );
        
    protected:
        
        int _numSegments;
        
        bool _useTexture = false;
        ci::gl::TextureRef _tex;
        
        vec3 _scale;
        vector<vec3> offsets;
        vector<vec3> points;
        
        virtual void _update();
        virtual void _draw();
        
    };
    
}}