#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "IStage.h"

namespace hrfm{ namespace display{
    
    typedef std::shared_ptr<class Stage> StageRef;
    
    class Stage : public IStage{
    public:
        
        static StageRef create( ivec2 size = ivec2(1024,1024), ci::gl::Fbo::Format format = ci::gl::Fbo::Format() ){ return StageRef( new Stage(size,format) ); }
        
        Stage( ivec2 size = ivec2(1024,1024), ci::gl::Fbo::Format format = ci::gl::Fbo::Format() ):IStage(size,format){};
        ~Stage(){};
        
    protected:
        
    };
    
}}