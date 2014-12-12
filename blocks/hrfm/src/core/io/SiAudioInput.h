#pragma once

#include "Singleton.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"

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
        
        void setup( uint16_t count, uint16_t bufferLength = 128 );
        
        void  useAudioManager();
        
        size_t numChannels();
        const float * getChannelAt( size_t ch );
        float * getFFT();
        float getAudioManagerGain();
        float getAudioManagerFFTAverage();
        
        void update( float decline = 0.68 );
        
        // --- draw method.
        
        void drawFFT( Rectf bounds );
        void drawFFT( Rectf bounds, ColorA color0, ColorA color1 );
        void drawFFT( Rectf bounds, ColorA color0, ColorA color1, int length );
        
        void drawWave( Rectf bounds );
        void drawWave( Rectf bounds, Color color );
        
    private:
        
        friend Singleton<SiAudioInput>;
        
        SiAudioInput(){
            _fftAverage = 0.0f;
            _audioGain  = 0.0f;
        }
        
        void onFFTAverage( OscInputEvent * event);
        void onAudioGain( OscInputEvent * event);
        
        bool _enabled;
        
        float _audioGain;
        float _fftAverage;
        
        uint16_t _bandCount;
        uint16_t _bufferLength;
        size_t _numChannels;
        float * _fft;
        map<size_t,const float*> _channels;
        
        cinder::audio::InputDeviceNodeRef     mInputDeviceNode;
        cinder::audio::MonitorNodeRef         mMonitorNode;
        cinder::audio::MonitorSpectralNodeRef mMonitorSpectralNode;
        vector<float>					      mMagSpectrum;
        
    };
    
}}