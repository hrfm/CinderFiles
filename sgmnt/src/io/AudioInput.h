#pragma once

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/app/AppBasic.h"
#include "OscInput.h"

#include "cinder/audio/Context.h"
#include "cinder/audio/MonitorNode.h"
#include "cinder/audio/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace sgmnt{ namespace io{
    
    class AudioInput {
    public:
        
        AudioInput();
        
        void setup( uint16_t count );
        
        void update();
        
        void  useAudioManager( sgmnt::osc::OscInput &oscInput );
        
        gl::Texture getAudioTexture();
        
        float getAudioManagerGain();
        float getAudioManagerFFTAverage();
        
        void drawFFT( Rectf bounds );
        void drawFFT( Rectf bounds, ColorA color0, ColorA color1 );
        void drawFFT( Rectf bounds, ColorA color0, ColorA color1, int length );
        
        void drawWave( Rectf bounds );
        void drawWave( Rectf bounds, Color color );
        
        float fftAverage;
        float audioGain;
        
        uint16_t bandCount;
        bool bufferAvailable;
        
        float value;
        float * fftBuffer;
        float * fft;
        float * fft2;
        
    private:
        
        void onFFTAverage( sgmnt::osc::OscInputEvent & event);
        
        void onAudioGain( sgmnt::osc::OscInputEvent & event);
        
        gl::Fbo      soundTexture;
        gl::GlslProg soundTexShader;
        
        audio::InputDeviceNodeRef		mInputDeviceNode;
        audio::MonitorSpectralNodeRef	mMonitorSpectralNode;
        vector<float>					mMagSpectrum;
        
    };
    
}}