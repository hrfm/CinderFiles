#pragma once

#include "cinder/gl/GlslProg.h"
#include "DataLoader.h"

using namespace ci;
using namespace std;

namespace hrfm{ namespace gl{
    
    /*
     
     Shader の生成用関数を実装した Static クラス.
     このクラスを経由する利点は、エラー発生時にその理由を出力するなど扱う上であると便利な機能を提供します.
     
     */
    class ShaderFactory{
        
    private:
        
        ShaderFactory(){};
        ~ShaderFactory(){};
        
    public:
        
        // --- From DataSourceRef.
        
        static ci::gl::GlslProg create(
                                       DataSourceRef vertex,
                                       DataSourceRef fragment = DataSourceRef(),
                                       DataSourceRef geometry = DataSourceRef(),
                                       GLint geometryInputType = GL_POINTS,
                                       GLint geometryOutputType = GL_TRIANGLES,
                                       GLint geometryOutputVertices = 0
                                       );
        
        // --- From fs::path.
        
        static ci::gl::GlslProg create( fs::path vertex );
        static ci::gl::GlslProg create( fs::path vertex, fs::path fragment );
        static ci::gl::GlslProg create(
                                       fs::path vertex,
                                       fs::path fragment,
                                       fs::path geometry,
                                       GLint geometryInputType = GL_POINTS,
                                       GLint geometryOutputType = GL_TRIANGLES,
                                       GLint geometryOutputVertices = 0
                                       );
        
        // --- From const char*.
        
    private:
        
    };
    
}}