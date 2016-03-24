#include "InstancedVboMesh.h"

namespace hrfm{ namespace display{
    
    
    void InstancedVboMesh::init( ci::gl::VboMeshRef vboMesh, ci::gl::GlslProgRef shader, int numInstances ){
        
        _vboMesh      = vboMesh;
        _numInstances = numInstances;
        
        if( shader == nullptr ){
            _shader = ci::gl::GlslProg::create( ci::gl::GlslProg::Format()
                                               .vertex  ( DataLoader::load( "InstancedVboMesh_vert.glsl" ) )
                                               .fragment( DataLoader::load( "InstancedVboMesh_frag.glsl" ) )
                                               );
        }else{
            _shader = shader;
        }
        
        for( int i=0; i<numInstances; ++i ) {
            transforms.push_back(mat4{});
        }
        
        // create the VBO which will contain per-instance (rather than per-vertex) data
        this->_mInstanceDataVbo = ci::gl::Vbo::create( GL_ARRAY_BUFFER, transforms.size() * sizeof(mat4), transforms.data(), GL_DYNAMIC_DRAW );
        
        // we need a geom::BufferLayout to describe this data as mapping to the CUSTOM_0 semantic, and the 1 (rather than 0) as the last param indicates per-instance (rather than per-vertex)
        this->_instanceDataLayout.append( ci::geom::Attrib::CUSTOM_0, 16, sizeof(mat4), 0, 1 /* per instance */ );
        
        // now add it to the VboMesh we already made of the Teapot
        this->_vboMesh->appendVbo( this->_instanceDataLayout, this->_mInstanceDataVbo );
        
        // and finally, build our batch, mapping our CUSTOM_0 attribute to the "vInstancePosition" GLSL vertex attribute
        this->_mBatch = ci::gl::Batch::create( this->_vboMesh, this->_shader, {
            { ci::geom::Attrib::CUSTOM_0, "uInstanceTransform"},
        } );
        
        cout << "inittttt" << endl;
        
    }
    
    
    void InstancedVboMesh::_update(){
        
        mat4 *t = (mat4*)_mInstanceDataVbo->mapReplace();
        
        auto sec = hrfm::app::SiAppInfo::getInstance().getElapsedSeconds();
        
        for( int i = 0; i < _numInstances; ++i ) {
            mat4 mtx = {};
            mtx *= ci::translate( vec3(150.0*sin(float(i)+sec),0.0,150.0*cos(float(i)+sec)) );
            *t++ = mtx;
        }
        _mInstanceDataVbo->unmap();
        
    };
    
    void InstancedVboMesh::_draw(){
        //ci::gl::ScopedDepth depth( false, true );
        ci::gl::ScopedDepthTest depth( true, GL_LEQUAL );
        _mBatch->drawInstanced( _numInstances );
    };
    
    
}}