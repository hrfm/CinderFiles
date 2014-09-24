#pragma once

#include "cinder/audio/FftProcessor.h"
#include "cinder/audio/Input.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/app/AppBasic.h"
#include "OscInput.h"

using namespace ci;
using namespace std;

namespace sgmnt {
	
    namespace io{
        
        class AudioInput {
        public:
            
            AudioInput();
            
            void setup( uint16_t count );
            
            void update();
            
            gl::Texture getAudioTexture();
            
            void  useAudioManager( OscInput &oscInput );
            float getAudioManagerGain();
            float getAudioManagerFFTAverage();
            
            void drawFFT( Rectf bounds );
            void drawFFT( Rectf bounds, ColorA color0, ColorA color1 );
            void drawFFT( Rectf bounds, ColorA color0, ColorA color1, int length );
            void drawWave( Rectf bounds );
            void drawWave( Rectf bounds, Color color );
            
            float getLeftBufferAt( int index );
            float getRightBufferAt( int index );
            
            uint16_t bandCount;
            bool bufferAvailable;
            
            float value;
            float * fftBuffer;
            float * fft;
            
            float * fft2;
            
        private:
            
            audio::Input           _mInput;
            audio::PcmBuffer32fRef _mPcmBuffer;
            std::shared_ptr<float> _mFftDataRef;
            
            audio::Buffer32fRef mLeftBuffer;
            audio::Buffer32fRef mRightBuffer;
            
            gl::Fbo      soundTexture;
            gl::GlslProg soundTexShader;
            
        };
        
    }

}