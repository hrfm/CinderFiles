#include "SiAudioInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace hrfm{ namespace io{
    
    //! public:
    
    void SiAudioInput::setup( uint16_t count ){
        
        _bandCount = count;
        
        // --- Create Sound Context.
        
        auto ctx = audio::Context::master();
        
        // The InputDeviceNode is platform-specific, so you create it using a special method on the Context:
        mInputDeviceNode = ctx->createInputDeviceNode();
        
        // --- init FFT
        
        _fft = new float[_bandCount];
        
        auto monitorSpectralFormat   = audio::MonitorSpectralNode::Format().windowSize( _bandCount * 2 );
        mMonitorSpectralNode = ctx->makeNode( new audio::MonitorSpectralNode( monitorSpectralFormat ) );
        mInputDeviceNode >> mMonitorSpectralNode;
        
        // --- init Wave
        
        auto monitorFormat   = audio::MonitorSpectralNode::Format().windowSize( 512 );
        mMonitorNode = ctx->makeNode( new audio::MonitorNode(monitorFormat) );
        mInputDeviceNode >> mMonitorNode;
        
        // ---
        
        // InputDeviceNode (and all InputNode subclasses) need to be enabled()'s to process audio. So does the Context:
        mInputDeviceNode->enable();
        ctx->enable();
        
    }
    
    size_t SiAudioInput::numChannels(){
        const audio::Buffer &buffer = mMonitorNode->getBuffer();
        return buffer.getNumChannels();
    }
    
    const float * SiAudioInput::getChannelAt( size_t ch ){
        return _channels[ch];
    }
    
    float * SiAudioInput::getFFT(){
        return _fft;
    }
    
    void SiAudioInput::update( float decline ){
        
        // --- Wave
        
        const audio::Buffer &buffer = mMonitorNode->getBuffer();
        for( size_t ch = 0; ch < buffer.getNumChannels(); ch++ ) {
            _channels[ch] = buffer.getChannel(ch);
        }
        
        // --- FFT
        
        int i = 0;
        float maxValue = 0.0f;
        
        for( int i = 0; i < _bandCount; i++ ) {
            _fft[i] *= decline;
        }
        
        // We copy the magnitude spectrum out from the Node on the main thread, once per update:
        mMagSpectrum = mMonitorSpectralNode->getMagSpectrum();
        for( auto spectrum = mMagSpectrum.begin(); spectrum != mMagSpectrum.end(); ++spectrum ) {
            maxValue = max( maxValue, *spectrum );
        }
        for( auto spectrum = mMagSpectrum.begin(); spectrum != mMagSpectrum.end(); ++spectrum ) {
            _fft[i] = ( *spectrum / maxValue );
            if( _bandCount <= ++i ){
                break;
            }
        }
        
    }
    
    void SiAudioInput::useAudioManager(){
        hrfm::io::SiOscInput::getInstance().addEventListener( "/audio/gain", this, &SiAudioInput::onAudioGain );
        hrfm::io::SiOscInput::getInstance().addEventListener( "/audio/fft/average", this, &SiAudioInput::onFFTAverage );
    }
    
    float SiAudioInput::getAudioManagerGain(){
        return _audioGain;
    }
    
    float SiAudioInput::getAudioManagerFFTAverage(){
        return _fftAverage;
    }
    
    // --- Draw method.
    
    void SiAudioInput::drawFFT( Rectf bounds ){
        drawFFT( bounds, ColorA(1.0,1.0,1.0,1.0), ColorA(1.0,1.0,1.0,1.0), _bandCount );
    }
    
    void SiAudioInput::drawFFT( Rectf bounds, ColorA color0, ColorA color1 ){
        drawFFT( bounds, color0, color1, _bandCount );
    }
    
    void SiAudioInput::drawFFT( Rectf bounds, ColorA color0, ColorA color1, int length ){
        
        float width  = bounds.getWidth() / length;
        float height = bounds.getHeight();
        
        glPushMatrix();
        glTranslatef( bounds.x1, bounds.y1, 0.0f );
        {
            for( int i = 1; i < (length); i++ ) {
                float barY = _fft[i] * height;
                glBegin( GL_QUADS );
                {
                    gl::color( color0 );
                    glVertex2f( i * width, height );
                    glVertex2f( i * width + width, height );
                    gl::color( color1 );
                    glVertex2f( i * width + width, height - barY );
                    glVertex2f( i * width, height - barY );
                }
                glEnd();
            }
        }
        glPopMatrix();
        
    }
    
    void SiAudioInput::drawWave( Rectf bounds ){
        drawWave(bounds, Color(1.0,1.0,1.0) );
    }
    
    void SiAudioInput::drawWave( Rectf bounds, Color color ){
        
        float width  = bounds.getWidth() / 512.0f;
        float height = bounds.getHeight();
        
        PolyLine<Vec2f>	leftBufferLine;
        //PolyLine<Vec2f>	rightBufferLine;
        
        glPushMatrix();
        glTranslatef( bounds.x1, bounds.y1, 0.0f );
        {
            for( int i = 0; i < 512; i++ ) {
                float x = ( i * width );
                float y = ( ( _channels[0][i] - 1 ) * - height / 2 );
                leftBufferLine.push_back( Vec2f( x , y) );
                /*
                y = ( ( _channels[1][i] - 1 ) * - height / 2 );
                rightBufferLine.push_back( Vec2f( x , y) );
                */
            }
            gl::color( color );
            gl::draw( leftBufferLine );
            //gl::draw( rightBufferLine );
        }
        glPopMatrix();
        
    };
    
    //! protected:
    
    void SiAudioInput::onFFTAverage( OscInputEvent * event){
        _fftAverage = event->message.getArgAsFloat(0);
    }
    
    void SiAudioInput::onAudioGain( OscInputEvent * event){
        _audioGain  = event->message.getArgAsFloat(0);
    }
    
}}