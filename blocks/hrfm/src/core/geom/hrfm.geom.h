#pragma once

#include "cinder/app/App.h"
#include "cinder/Rand.h"

namespace hrfm{ namespace geom{
    
    inline static ci::vec3 bend( ci::vec3 context, float pitch, float yaw ){
        
        float    c, s;
        ci::vec3 norm     = ci::normalize( context );
        ci::quat ctxQuatf = quat( 0.0, norm.x, norm.y, norm.z );
        float    length   = ci::length(context);
        
        ci::quat rotate;
        ci::quat reset( norm, ci::normalize(vec3( 0.0, 1.0, 0.0 )) );
        
        if( 0 < ci::angle(reset) ){ //!!!!!!! getAngle から変更
            ctxQuatf = ci::inverse(reset) * ctxQuatf * reset;
        }
        
        // 回転その1
        c = cos( pitch / 2.0 );
        s = sin( pitch / 2.0 );
        rotate   = ci::quat( c, 1.0 * s, 0.0 * s, 0.0 * s );
        ctxQuatf = ci::inverse(rotate) * ctxQuatf * rotate;
        
        // 回転その2
        c = cos( yaw / 2.0 );
        s = sin( yaw / 2.0 );
        rotate   = ci::quat( c, 0.0 * s, 1.0 * s, 0.0 * s );
        ctxQuatf = ci::inverse(rotate) * ctxQuatf * rotate;
        
        if( 0 < ci::angle(reset) ){ //!!!!!!! getAngle から変更
            ctxQuatf = reset * ctxQuatf * ci::inverse(reset);
        }
        
        //return ctxQuatf.v * length; //!!!!! 直し方がわからなかったのでペンド
        return vec3();
        
    }
    
    inline static ci::vec3 bendRandomly( ci::vec3 context, float range ){
        return hrfm::geom::bend( context, ( -range + ( range * 2.0 * randFloat() ) ), ( -range + ( range * 2.0 * randFloat() ) ) );
    }
    
}}