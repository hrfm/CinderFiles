#include "SiAudioInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace sgmnt{ namespace audio{
    
    void SiAudioInput::setup( uint16_t count ){
        
        bandCount = count;
        
        value = 0.0f;
        fft   = new float[bandCount];
        fft2  = new float[bandCount];
        for( int i = 0; i < bandCount; i++ ){
            fft[i]  = 0;
            fft2[i] = 0;
        }
        
        // --- Create Sound Context.
        
        /*
         auto ctx = audio::Context::master();
         
         // The InputDeviceNode is platform-specific, so you create it using a special method on the Context:
         mInputDeviceNode = ctx->createInputDeviceNode();
         
         // By providing an FFT size double that of the window size, we 'zero-pad' the analysis data, which gives
         // an increase in resolution of the resulting spectrum data.
         auto monitorFormat = audio::MonitorSpectralNode::Format().fftSize( bandCount ).windowSize( bandCount / 2.0 );
         mMonitorSpectralNode = ctx->makeNode( new audio::MonitorSpectralNode( monitorFormat ) );
         
         mInputDeviceNode >> mMonitorSpectralNode;
         
         // InputDeviceNode (and all InputNode subclasses) need to be enabled()'s to process audio. So does the Context:
         mInputDeviceNode->enable();
         ctx->enable();
         
         //*/
        
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
    
    void SiAudioInput::update(){
        
        // --- Audio
        
        /*
         // We copy the magnitude spectrum out from the Node on the main thread, once per update:
         mMagSpectrum = mMonitorSpectralNode->getMagSpectrum();
         
         for( int i = 0; i < ( bandCount ); i++ ) {
         fft2[i] *= 0.68;
         }
         
         float maxValue = 0.0f;
         
         for( auto spectrum = mMagSpectrum.begin(); spectrum != mMagSpectrum.end(); ++spectrum ) {
         maxValue = max( maxValue, *spectrum );
         }
         
         if( 1.0f < maxValue ){
         value = 1.0f;
         int i = 0;
         for( auto spectrum = mMagSpectrum.begin(); spectrum != mMagSpectrum.end(); ++spectrum ) {
         fft[i] = ( *spectrum / maxValue );
         if( fft2[i] < fft[i] ){
         fft2[i] = fft[i];
         }
         if( bandCount <= ++i ){
         break;
         }
         }
         }else{
         value = 0.0f;
         int i = 0;
         for( auto spectrum = mMagSpectrum.begin(); spectrum != mMagSpectrum.end(); ++spectrum ) {
         fft[i] = ( *spectrum / 1.0f );
         value = max(value,fft[i]);
         if( bandCount <= ++i ){
         break;
         }
         }
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
         */
        
    }
    
    void SiAudioInput::useAudioManager(){
        sgmnt::osc::SiOscInput::getInstance().addEventListener( "/audio/gain", this, &SiAudioInput::onAudioGain );
        sgmnt::osc::SiOscInput::getInstance().addEventListener( "/audio/fft/average", this, &SiAudioInput::onFFTAverage );
    }
    
    gl::Texture SiAudioInput::getAudioTexture(){
        return soundTexture.getTexture();
    }
    
    float SiAudioInput::getAudioManagerGain(){
        return audioGain;
    }
    
    float SiAudioInput::getAudioManagerFFTAverage(){
        return fftAverage;
    }
    
    void SiAudioInput::drawFFT( Rectf bounds ){
        drawFFT(bounds, ColorA(1.0,1.0,1.0,1.0), ColorA(1.0,1.0,1.0,1.0), bandCount);
    }
    
    void SiAudioInput::drawFFT( Rectf bounds, ColorA color0, ColorA color1 ){
        drawFFT( bounds, color0, color1, bandCount );
    }
    
    void SiAudioInput::drawFFT( Rectf bounds, ColorA color0, ColorA color1, int length ){
        
        glPushMatrix();
        glTranslatef( bounds.x1, bounds.y1, 0.0f );
        
        float width  = bounds.getWidth() / length;
        float height = bounds.getHeight();
        
        for( int i = 1; i < (length); i++ ) {
            
            float barY = fft2[i] * height;
            
            color1.a = barY / height;
            
            //cout << barY << endl;
            
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
        
    }
    
    void SiAudioInput::drawWave( Rectf bounds ){
        drawWave(bounds, Color(1.0,1.0,1.0) );
    }
    
    void SiAudioInput::drawWave( Rectf bounds, Color color ){
        
        /*
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
         */
        
    };
    
    
    void SiAudioInput::onFFTAverage( OscInputEvent * event){
        fftAverage = event->message.getArgAsFloat(0);
    }
    
    void SiAudioInput::onAudioGain( OscInputEvent * event){
        audioGain  = event->message.getArgAsFloat(0);
    }
    
}}