#include "AudioInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float ___fftAverage___ = 0.0f;
float ___audioGain___  = 0.0f;

void ___onFFTAverage___( const sgmnt::Event &event){
    ___fftAverage___ = event.message.getArgAsFloat(0);
}

void ___onAudioGain___( const sgmnt::Event &event){
    ___audioGain___  = event.message.getArgAsFloat(0);
}

namespace sgmnt{

    namespace io{
        
        AudioInput::AudioInput(){}
        
        void AudioInput::setup( uint16_t count ){
            
            bandCount = count;
            
            value = 0.0f;
            fft   = new float[bandCount];
            fft2  = new float[bandCount];
            for( int i = 0; i < bandCount; i++ ){
                fft[i]  = 0;
                fft2[i] = 0;
            }
            
            //iterate input devices and print their names to the console
            const vector<audio::InputDeviceRef>& devices = audio::Input::getDevices();
            for( vector<audio::InputDeviceRef>::const_iterator iter = devices.begin(); iter != devices.end(); ++iter ) {
                cout << (*iter)->getName() << endl;
            }
            
            //initialize the audio Input, using the default input device
            _mInput = audio::Input();
            
            //tell the input to start capturing audio
            _mInput.start();
            
            // ----------
            
            gl::Fbo::Format format;
            soundTexture = gl::Fbo( count * 10, count * 10, format );
            
            try {
                soundTexShader = gl::GlslProg(
                    loadResource("../resources/simple_vert.glsl"),
                    loadResource("../resources/audio_frag.glsl")
                );
            }catch( ci::gl::GlslProgCompileExc &exc ) {
                cout << "Shader compile error: " << endl;
                cout << exc.what();
            }catch( ... ) {
                cout << "Unable to load shader" << endl;
            }
            
        }
        
        void AudioInput::update(){
            
            // --- Audio
            
            _mPcmBuffer = _mInput.getPcmBuffer();
            if( ! _mPcmBuffer ) {
                bufferAvailable = false;
                return;
            }
            
            bufferAvailable = true;
            
    #if defined( CINDER_MAC )
            
            //presently FFT only works on OS X, not iOS
            _mFftDataRef = audio::calculateFft( _mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT ), bandCount );
            
            fftBuffer = _mFftDataRef.get();
            if( !fftBuffer ){
                return;
            }
            
            for( int i = 0; i < ( bandCount ); i++ ) {
                fft2[i] *= 0.68;
            }
            
            float maxValue = 0.0f;
            for( int i = 0; i < ( bandCount ); i++ ) {
                maxValue = max( maxValue, fftBuffer[i] );
            }
            
            if( 1.0f < maxValue ){
                value = 1.0f;
                for( int i = 0; i < ( bandCount ); i++ ) {
                    fft[i] = ( fftBuffer[i] / maxValue );
                    if( fft2[i] < fft[i] ){
                        fft2[i] = fft[i];
                    }
                }
            }else{
                value = 0.0f;
                for( int i = 0; i < ( bandCount ); i++ ) {
                    fft[i] = fftBuffer[i] / 1.0f;
                    value = max(value,fft[i]);
                }
            }
            
    #endif
            
            if( _mPcmBuffer ){
                uint32_t bufferLength = _mPcmBuffer->getSampleCount();
                mLeftBuffer = _mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT );
                mRightBuffer = _mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_RIGHT );
            }
            
            soundTexture.bindFramebuffer();
                gl::pushMatrices();
                    gl::setMatricesWindow( soundTexture.getSize(), false );
                    gl::clear();
                    gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
                    soundTexShader.bind();
                        soundTexShader.uniform( "iResolution", Vec2f( soundTexture.getWidth(), soundTexture.getHeight() ) );
                        soundTexShader.uniform( "iFFT", fft2, 64 );
                        gl::drawSolidRect( soundTexture.getBounds() );
                    soundTexShader.unbind();
                gl::popMatrices();
            soundTexture.unbindFramebuffer();
            
        }
        
        gl::Texture AudioInput::getAudioTexture(){
            return soundTexture.getTexture();
        }
        
        void AudioInput::useAudioManager( OscInput &oscInput ){
            oscInput.addEventListener( "/audio/gain", &___onAudioGain___ );
            oscInput.addEventListener( "/audio/fft/average", &___onFFTAverage___ );
        }
        
        float AudioInput::getLeftBufferAt(int index){
            return mLeftBuffer->mData[index];
        }
        
        float AudioInput::getRightBufferAt(int index){
            return mRightBuffer->mData[index];
        }
        
        float AudioInput::getAudioManagerGain(){
            return ___audioGain___;
        }
        
        float AudioInput::getAudioManagerFFTAverage(){
            return ___fftAverage___;
        }
        
        void AudioInput::drawFFT( Rectf bounds ){
    #if defined( CINDER_MAC )
            drawFFT(bounds, ColorA(1.0,1.0,1.0,1.0), ColorA(1.0,1.0,1.0,1.0), bandCount);
    #endif
        }
        
        
        void AudioInput::drawFFT( Rectf bounds, ColorA color0, ColorA color1 ){
            drawFFT( bounds, color0, color1, bandCount );
        }
        
        void AudioInput::drawFFT( Rectf bounds, ColorA color0, ColorA color1, int length ){
            
#if defined( CINDER_MAC )
            
            if( ! _mFftDataRef ) {
                return;
            }
            
            glPushMatrix();
            glTranslatef( bounds.x1, bounds.y1, 0.0f );
            
            float width  = bounds.getWidth() / length;
            float height = bounds.getHeight();
            
            for( int i = 1; i < (length); i++ ) {
                
                float barY = fft2[i] * height;
                
                color1.a = barY / height;
                
                //*
                glBegin( GL_QUADS );
                    gl::color( color0 );
                    glVertex2f( i * width, height );
                    glVertex2f( i * width + width, height );
                    gl::color( color1 );
                    glVertex2f( i * width + width, height - barY );
                    glVertex2f( i * width, height - barY );
                glEnd();
                /*/
                    gl::color( color1 );
                    gl::drawLine(
                        Vec2f( i * width + width, height - barY ),
                        Vec2f( (i-1) * width + width, height - barY )
                    );
                //*/
                
            }
            
            glPopMatrix();
            
#endif
            
        }
        
        void AudioInput::drawWave( Rectf bounds ){
#if defined( CINDER_MAC )
            drawWave(bounds, Color(1.0,1.0,1.0) );
#endif
        }
        
        
        void AudioInput::drawWave( Rectf bounds, Color color ){
            
#if defined( CINDER_MAC )
            
            if( !_mPcmBuffer ){
                return;
            }
            
            uint32_t bufferLength = _mPcmBuffer->getSampleCount();
            audio::Buffer32fRef leftBuffer = _mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT );
            audio::Buffer32fRef rightBuffer = _mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_RIGHT );
            
            float width  = bounds.getWidth() / (float)bufferLength;
            float height = bounds.getHeight();
            
            PolyLine<Vec2f>	leftBufferLine;
            PolyLine<Vec2f>	rightBufferLine;
            
            glPushMatrix();
            glTranslatef( bounds.x1, bounds.y1, 0.0f );
            
            for( int i = 0; i < bufferLength; i++ ) {
                float x = ( i * width );
                //get the PCM value from the left channel buffer
                float y = ( ( leftBuffer->mData[i] - 1 ) * - height / 3 );
                leftBufferLine.push_back( Vec2f( x , y) );
                y = ( ( rightBuffer->mData[i] - 1 ) * - height / 3 );
                rightBufferLine.push_back( Vec2f( x , y) );
            }
            
            gl::color( color );
            gl::draw( leftBufferLine );
            gl::draw( rightBufferLine );
            
            glPopMatrix();
            
#endif
            
        }
        
    }
    
}