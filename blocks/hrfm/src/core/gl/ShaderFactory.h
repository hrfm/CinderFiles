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
        
        static ci::gl::GlslProgRef create(
                                          DataSourceRef vertex,
                                          DataSourceRef fragment=DataSourceRef(),
                                          DataSourceRef geometry=DataSourceRef(),
                                          DataSourceRef tessEval=DataSourceRef(),
                                          DataSourceRef tessCtrl=DataSourceRef()
                                          );
        
        // --- From const string.
        
        static ci::gl::GlslProgRef create(
                                          const std::string &vertex,
                                          const std::string &fragment=std::string(),
                                          const std::string &geometry=std::string(),
                                          const std::string &tessEval=std::string(),
                                          const std::string &tessCtrl=std::string()
                                          );
        
        // --- From fs::path.
        
        static ci::gl::GlslProgRef create( ci::fs::path vertex );
        static ci::gl::GlslProgRef create( ci::fs::path vertex, ci::fs::path fragment );
        static ci::gl::GlslProgRef create( ci::fs::path vertex, ci::fs::path fragment, ci::fs::path geometry );
        static ci::gl::GlslProgRef create( ci::fs::path vertex, ci::fs::path fragment, ci::fs::path geometry, ci::fs::path tessEval );
        static ci::gl::GlslProgRef create( ci::fs::path vertex, ci::fs::path fragment, ci::fs::path geometry, ci::fs::path tessEval, ci::fs::path tessCtrl );
        
        
    private:
        
    };
    
}}