#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"

namespace hrfm{ namespace geom{
    
    inline static ci::Vec3f bend( ci::Vec3f context, float pitch, float yaw ){
        
        float     c, s;
        ci::Vec3f norm     = context.normalized();
        ci::Quatf ctxQuatf = Quatf( 0.0, norm.x, norm.y, norm.z );
        float     length   = context.length();
        
        ci::Quatf rotate;
        ci::Quatf reset( norm, Vec3f( 0.0, 1.0, 0.0 ).normalized() );
        
        if( 0 < reset.getAngle() ){
            ctxQuatf = reset.inverse() * ctxQuatf * reset;
        }
        
        // 回転その1
        c = cos( pitch / 2.0 );
        s = sin( pitch / 2.0 );
        rotate   = ci::Quatf( c, 1.0 * s, 0.0 * s, 0.0 * s );
        ctxQuatf = rotate.inverse() * ctxQuatf * rotate;
        
        // 回転その2
        c = cos( yaw / 2.0 );
        s = sin( yaw / 2.0 );
        rotate   = ci::Quatf( c, 0.0 * s, 1.0 * s, 0.0 * s );
        ctxQuatf = rotate.inverse() * ctxQuatf * rotate;
        
        if( 0 < reset.getAngle() ){
            ctxQuatf = reset * ctxQuatf * reset.inverse();
        }
        
        
        return ctxQuatf.v * length;
        
    }
    
    inline static ci::Vec3f bendRandomly( ci::Vec3f context, float range ){
        return hrfm::geom::bend( context, ( -range + ( range * 2.0 * randFloat() ) ), ( -range + ( range * 2.0 * randFloat() ) ) );
    }
    
}}