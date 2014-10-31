#pragma once

// ==================================================================
//
// 【注意】
// この header を扱うには別途
// sgmnt/resources/effect_shader を Resources に入れる必要があります.
//
// ==================================================================

#include "Base2DEffect.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace sgmnt::visual;

class SciFi2DEffect : public Base2DEffect{
    public :
    SciFi2DEffect(){ Base2DEffect(); }
    protected :
    DataSourceRef getFragmentShader(){ return loadResource("../resources/SciFi2DEffect.glsl"); }
    void prepare(){}
    void clear(){}
};

/**
 * ブラウン管のような走査線表現.
 */
class Line2DEffect : public Base2DEffect{
    public :
    Line2DEffect(){ Base2DEffect(); }
    protected :
    DataSourceRef getFragmentShader(){ return loadResource("../resources/Line2DEffect.glsl"); }
    void prepare(){}
    void clear(){}
};

/**
 * デジタルノイズ / アナログノイズ 両方を表現する.
 */
class Noise2DEffect : public Base2DEffect{
    public :
    Noise2DEffect(){
        Base2DEffect();
        value      = 0;
        isVertical = 0;
    }
    void setup( Vec2i size, int analog = 0 ){
        this->Base2DEffect::setup(size);
        isAnalog = analog;
    }
    float value;
    protected :
    DataSourceRef getFragmentShader(){ return loadResource("../resources/Noise2DEffect.glsl"); }
    void prepare(){
        mShader.uniform( "value", value );
        mShader.uniform( "vertical", isVertical );
        mShader.uniform( "analog", isAnalog );
    }
    void clear(){}
    int isVertical;
    int isAnalog;
};

/**
 * 色が分離するようなノイズ.
 */
class ColorNoise2DEffect : public Base2DEffect{
    public :
    ColorNoise2DEffect(){
        Base2DEffect();
        value = 0;
    }
    float value;
    protected :
    DataSourceRef getFragmentShader(){ return loadResource("../resources/ColorNoise2DEffect.glsl"); }
    void prepare(){
        mShader.uniform( "value", value );
    }
    void clear(){}
};

/**
 *　表示が上下左右にスライドしてずれてループする表現.
 */
class Slide2DEffect : public Base2DEffect{
    public :
    Slide2DEffect(){
        Base2DEffect();
    }
    void setUniform( float slideX, float slideY ){
        mSlideX = slideX;
        mSlideY = slideY;
    }
    protected :
    DataSourceRef getFragmentShader(){ return loadResource("../resources/Slide2DEffect.glsl"); }
    void prepare(){
        mShader.uniform( "slideX", mSlideX );
        mShader.uniform( "slideY", mSlideY );
    }
    void clear(){}
    float mSlideX;
    float mSlideY;
};

class Blur2DEffect : public Base2DEffect{
    public :
    Blur2DEffect(){ Base2DEffect(); }
    void setUniform( Vec2f sample_offset, float attenuation ){
        mSampleOffset = sample_offset;
        mAttenuation  = attenuation;
    }
    protected :
    DataSourceRef getFragmentShader(){ return loadResource("../resources/Blur2DEffect.glsl"); }
    void prepare(){
        mShader.uniform( "sample_offset", mSampleOffset );
        mShader.uniform( "attenuation"  , mAttenuation );
    }
    void clear(){}
    Vec2f mSampleOffset;
    float mAttenuation;
};

class ScanLine2DEffect : public Base2DEffect{
    public :
    ScanLine2DEffect(){ Base2DEffect(); }
    protected :
    DataSourceRef getFragmentShader(){ return loadResource("../resources/ScanLine2DEffect.glsl"); }
    void prepare(){}
    void clear(){}
};
