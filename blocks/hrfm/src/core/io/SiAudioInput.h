#pragma once

#include "Singleton.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"

#include "cinder/app/AppBasic.h"
#include "SiOscInput.h"
#include "SiKORGMIDIInterface.h"

#include "cinder/audio/Context.h"
#include "cinder/audio/MonitorNode.h"
#include "cinder/audio/Utilities.h"

using namespace ci;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace io{
    
    class SiAudioInput : public hrfm::utils::Singleton<SiAudioInput>{
    public:
        
        void showAllDevices();
        
        void setup( uint16_t count, uint16_t bufferLength = 128 );
        void setup( string deviceName, uint16_t count, uint16_t bufferLength = 128 );
        
        void  useAudioManager();
        
        size_t numChannels();
        const float * getChannelAt( size_t ch );
        
        uint16_t getBandCount();
        uint16_t getBufferLength();
        
        float * getFFT();
        float * getFFTNormalized();
        
        float getFFTRangedAt( int index );
        
        float getAudioManagerGain();
        float getAudioManagerFFTAverage();
        
        void update( float decline = 0.68 );
        
        // --- draw method.
        
        void drawFFT( Rectf bounds );
        void drawFFT( Rectf bounds, int length );
        
        void drawFFTRanged( Rectf bounds );
        
        void drawWave( Rectf bounds );
        
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
        size_t   _numChannels;
        
        float * _fftValues;
        float * _fftNormalized;
        float * _fftRanged;
        
        map<size_t,const float*> _channels;
        
        cinder::audio::InputDeviceNodeRef     mInputDeviceNode;
        cinder::audio::MonitorNodeRef         mMonitorNode;
        cinder::audio::MonitorSpectralNodeRef mMonitorSpectralNode;
        vector<float>					      mMagSpectrum;
        
    };
    
}}