#include "ShaderFactory.h"

using namespace ci;
using namespace hrfm::utils;

namespace hrfm{ namespace gl{
    
    ci::gl::GlslProg ShaderFactory::create( DataSourceRef vertex, DataSourceRef fragment,
                                            DataSourceRef geometory, GLint geometryInputType, GLint geometryOutputType, GLint geometryOutputVertices ){
        try {
            return ci::gl::GlslProg( vertex, fragment, geometory, geometryInputType, geometryOutputType, geometryOutputVertices );
        }catch( ci::gl::GlslProgCompileExc &exc ) {
            cout << "Shader compile error: " << endl;
            cout << exc.what();
        }catch( ... ) {
            cout << "Unable to load shader" << endl;
        }
        return NULL;
    };
    
    ci::gl::GlslProg ShaderFactory::create( fs::path vertex ){
        return ShaderFactory::create( DataLoader::load(vertex) );
    }
    
    ci::gl::GlslProg ShaderFactory::create( fs::path vertex, fs::path fragment ){
        return ShaderFactory::create( DataLoader::load(vertex), DataLoader::load(fragment) );
    }
    
    ci::gl::GlslProg ShaderFactory::create( fs::path vertex, fs::path fragment,
                                            fs::path geometry, GLint geometryInputType, GLint geometryOutputType, GLint geometryOutputVertices ){
        return ShaderFactory::create( DataLoader::load(vertex), DataLoader::load(fragment),
                                      DataLoader::load(geometry), geometryInputType, geometryOutputType, geometryOutputVertices );
    }
    
}}