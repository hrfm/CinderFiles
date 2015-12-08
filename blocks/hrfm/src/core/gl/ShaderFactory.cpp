#include "ShaderFactory.h"

using namespace ci;
using namespace hrfm::io;

namespace hrfm{ namespace gl{
    
    // -----------------------------------------------------------------------------------------------------------
    // --- From DataSourceRef.
    
    ci::gl::GlslProgRef ShaderFactory::create(
                                              DataSourceRef vertex,
                                              DataSourceRef fragment,
                                              DataSourceRef geometory,
                                              DataSourceRef tessEval,
                                              DataSourceRef tessCtrl
                                              )
    {
        try {
            return ci::gl::GlslProg::create( vertex, fragment, geometory, tessEval, tessCtrl );
        }catch( ci::gl::GlslProgCompileExc &exc ) {
            cout << "Shader compile error: " << endl;
            cout << exc.what();
        }catch( ... ) {
            cout << "Unable to load shader" << endl;
        }
        return NULL;
    };
    
    // -----------------------------------------------------------------------------------------------------------
    // --- From const string.
    
    ci::gl::GlslProgRef ShaderFactory::create(
                                              const std::string & vertex,
                                              const std::string & fragment,
                                              const std::string & geometory,
                                              const std::string & tessEval,
                                              const std::string & tessCtrl
                                              )
    {
        try {
            return ci::gl::GlslProg::create( vertex, fragment, geometory, tessEval, tessCtrl );
        }catch( ci::gl::GlslProgCompileExc &exc ) {
            cout << "Shader compile error: " << endl;
            cout << exc.what();
        }catch( ... ) {
            cout << "Unable to load shader" << endl;
        }
        return NULL;
    };
    
    // -----------------------------------------------------------------------------------------------------------
    // --- From fs::path.
    
    ci::gl::GlslProgRef ShaderFactory::create( fs::path vertex ){
        return ShaderFactory::create(
                                     DataLoader::load(vertex)
                                     );
    }
    
    ci::gl::GlslProgRef ShaderFactory::create( fs::path vertex, fs::path fragment ){
        return ShaderFactory::create(
                                     DataLoader::load(vertex),
                                     DataLoader::load(fragment)
                                     );
    }
    
    ci::gl::GlslProgRef ShaderFactory::create( fs::path vertex, fs::path fragment, fs::path geometry ){
        return ShaderFactory::create(
                                     DataLoader::load(vertex),
                                     DataLoader::load(fragment),
                                     DataLoader::load(geometry)
                                     );
    }
    
    ci::gl::GlslProgRef ShaderFactory::create( fs::path vertex, fs::path fragment, fs::path geometry, fs::path tessEval ){
        return ShaderFactory::create(
                                     DataLoader::load(vertex),
                                     DataLoader::load(fragment),
                                     DataLoader::load(geometry),
                                     DataLoader::load(tessEval)
                                     );
    }
    
    ci::gl::GlslProgRef ShaderFactory::create( fs::path vertex, fs::path fragment, fs::path geometry, fs::path tessEval, fs::path tessCtrl ){
        return ShaderFactory::create(
                                     DataLoader::load(vertex),
                                     DataLoader::load(fragment),
                                     DataLoader::load(geometry),
                                     DataLoader::load(tessEval),
                                     DataLoader::load(tessCtrl)
                                     );
    }
    
}}