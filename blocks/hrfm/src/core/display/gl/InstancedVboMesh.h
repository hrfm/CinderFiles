#pragma once

#include "hrfm.h"
#include "cinder/gl/gl.h"

namespace hrfm{ namespace display{

    class InstancedVboMesh : public hrfm::display::DisplayNode{
        
    public:
        
        InstancedVboMesh():hrfm::display::DisplayNode(){}
        
        InstancedVboMesh( ci::gl::VboMeshRef vboMesh, ci::gl::GlslProgRef shader = nullptr, int numInstances = 10000 ):hrfm::display::DisplayNode(){
            this->init( vboMesh, shader, numInstances );
        };
        
        virtual void init( ci::gl::VboMeshRef vboMesh, ci::gl::GlslProgRef shader = nullptr, int numInstances = 10000 );
        
        vector<mat4> transforms;
        
    protected:
        
        int                    _numInstances;
        ci::gl::VboMeshRef     _vboMesh;
        ci::gl::GlslProgRef    _shader;
        ci::gl::BatchRef       _mBatch;
        ci::geom::BufferLayout _instanceDataLayout;
        ci::gl::VboRef         _mInstanceDataVbo;
        
        virtual void _update();
        virtual void _draw();
        
    };

}}