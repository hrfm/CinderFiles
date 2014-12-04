#pragma once

#include "Singleton.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/app/AppBasic.h"
#include "SiOscInput.h"

#include "cinder/audio/Context.h"
#include "cinder/audio/MonitorNode.h"
#include "cinder/audio/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace io{
    
    class SiAudioInput : public hrfm::utils::Singleton<SiAudioInput>{
    public:
        
        void setup( uint16_t count );
        
        void update( float decline = 0.68 );
        
        void  useAudioManager();
        
        ci::gl::Texture getAudioTexture();
        
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
        
        float * fft;
        map<size_t,const float*> channels;
        
    private:
        
        friend Singleton<SiAudioInput>;
        
        SiAudioInput(){
            fftAverage = 0.0f;
            audioGain  = 0.0f;
        }
        
        void onFFTAverage( OscInputEvent * event);
        void onAudioGain( OscInputEvent * event);
        
        cinder::audio::InputDeviceNodeRef     mInputDeviceNode;
        cinder::audio::MonitorSpectralNodeRef mMonitorSpectralNode;
        cinder::audio::MonitorNodeRef         mMonitorNode;
        
        vector<float>					      mMagSpectrum;
        
    };
    
}}