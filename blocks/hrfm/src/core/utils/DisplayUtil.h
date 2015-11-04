#pragma once

#include <map>
#include "Singleton.h"
#include "cinder/gl/Texture.h"

using namespace std;

namespace hrfm{ namespace utils{
    
    /*
     
     表示に関する便利関数を提供する Utility クラス.
     
     */
    class DisplayUtil{
    public:
        static ci::Rectf letterBox( ci::Rectf content, ci::Rectf box );
        static ci::Rectf letterBox( ci::Rectf content, ci::vec2 size );
        static ci::Rectf fullOf( ci::Rectf content, ci::Rectf box );
    private:
    };
    
}}