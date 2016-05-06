#include "TextureLoader.h"

using namespace ci;
using namespace std;

namespace hrfm{ namespace io{
    
    map<const string,ci::gl::TextureRef> TextureLoader::_cache;
    
    ci::gl::TextureRef TextureLoader::load( ci::fs::path filePath, bool useCache ){
        return TextureLoader::load( filePath, ci::UrlOptions::UrlOptions( true, 30.0f ), useCache );
    }
    
    ci::gl::TextureRef TextureLoader::load( ci::fs::path filePath, ci::UrlOptions::UrlOptions urlOptions, bool useCache ){
        
        if( useCache && TextureLoader::_cache.find( filePath.string() ) != TextureLoader::_cache.end() ){
            return TextureLoader::_cache.at(filePath.string());
        }
        try{
            auto tex = ci::gl::Texture::create( loadImage( DataLoader::load( filePath, urlOptions ) ) );
            TextureLoader::_cache[filePath.string()] = tex;
            return tex;
        }catch(...){
            return NULL;
        }
        
    }
    
}}