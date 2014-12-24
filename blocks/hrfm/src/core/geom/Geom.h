#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"

namespace hrfm{ namespace geom{
    
    inline static ci::Vec3f bend( ci::Vec3f context, float pitch, float yaw ){
        
        float     c, s;
        ci::Quatf rotate;
        
        ci::Vec3f norm     = context.normalized();
        ci::Quatf ctxQuatf = Quatf( 0.0, norm.x, norm.y, norm.z );
        float     length   = context.length();
        
        // 回転その1
        c        = cos( pitch / 2.0 );
        s        = sin( pitch / 2.0 );
        rotate   = Quatf( c, norm.y * s, norm.x * s, norm.z * s );
        ctxQuatf = rotate.inverted() * ctxQuatf * rotate;
        
        // 回転その2
        c        = cos( yaw / 2.0 );
        s        = sin( yaw / 2.0 );
        rotate   = Quatf( c, norm.x * s, norm.y * s, norm.z * s );
        ctxQuatf = rotate.inverted() * ctxQuatf * rotate;
        
        return ctxQuatf.getAxis().normalized() * length;
        
    }
    
    inline static ci::Vec3f bendRandomly( ci::Vec3f context, float range ){
        return hrfm::geom::bend( context, ( -range + ( range * 2.0 * randFloat() ) ), ( -range + ( range * 2.0 * randFloat() ) ) );
    }
    
}}