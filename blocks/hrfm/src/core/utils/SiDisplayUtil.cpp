#include "SiDisplayUtil.h"

using namespace std;

namespace hrfm{ namespace utils{
    
    // ======================================================================
    // TimingData class.
    
    ci::Rectf SiDisplayUtil::letterBox( ci::Rectf content, ci::Rectf box ){
        
        ci::Rectf bounds;
        
        int w = content.getWidth();
        int h = content.getHeight();
        
        float wScale = (float)box.getWidth() / (float)w;
        float hScale = (float)box.getHeight() / (float)h;
        
        if ( wScale < hScale ) {
            w = w * wScale;
            h = h * wScale;
        }else {
            w = w * hScale;
            h = h * hScale;
        }
        
        bounds.x1 = box.x1 + ( box.getWidth() - w ) / 2.0f;
        bounds.x2 = bounds.x1 + w;
        bounds.y1 = box.y1 + ( box.getHeight() - h ) / 2.0f;
        bounds.y2 = bounds.y1 + h;
        
        return bounds;
        
    }
    
    ci::Rectf SiDisplayUtil::letterBox( ci::Rectf content, ci::Vec2f size ){
        
        ci::Rectf bounds;
        
        int w = content.getWidth();
        int h = content.getHeight();
        
        float wScale = size.x / (float)w;
        float hScale = size.y / (float)h;
        
        if ( wScale < hScale ) {
            w = w * wScale;
            h = h * wScale;
        }else {
            w = w * hScale;
            h = h * hScale;
        }
        
        bounds.x1 = content.x1 + ( size.x - w ) / 2.0f;
        bounds.x2 = bounds.x1 + w;
        bounds.y1 = content.y1 + ( size.y - h ) / 2.0f;
        bounds.y2 = bounds.y1 + h;
        
        return bounds;
        
    }
    
    ci::Rectf SiDisplayUtil::fullOf( ci::Rectf content, ci::Rectf box ){
        
        ci::Rectf bounds;
        
        int w = content.getWidth();
        int h = content.getHeight();
        
        float wScale = (float)box.getWidth() / (float)w;
        float hScale = (float)box.getHeight() / (float)h;
        
        if ( wScale > hScale ) {
            w = w * wScale;
            h = h * wScale;
        }else {
            w = w * hScale;
            h = h * hScale;
        }
        
        bounds.x1 = box.x1 + ( box.getWidth() - w ) / 2.0f;
        bounds.x2 = bounds.x1 + w;
        bounds.y1 = box.y1 + ( box.getHeight() - h ) / 2.0f;
        bounds.y2 = bounds.y1 + h;
        
        return bounds;
        
    }
    
}}