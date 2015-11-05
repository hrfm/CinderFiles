#include "SiAudioInput.h"

using namespace ci;
using namespace std;

namespace hrfm{ namespace io{
    
    //! public:
    
    void SiAudioInput::showAllDevices(){
        // print the devices
        cout << endl << "SiAudioInput::showAllDevices()" << endl;
        vector<ci::audio::DeviceRef> list = ci::audio::Device::getInputDevices();
        for( auto device = list.begin(); device != list.end(); ++device ) {
            cout << "- NAME:" << (*device)->getName() << " KEY:" << (*device)->getKey() << endl;
        }
        cout << endl;
    }
    
    void SiAudioInput::setup( uint16_t count, uint16_t bufferLength ){
        setup( "", count, bufferLength );
    }
    
    void SiAudioInput::setup( string deviceName, uint16_t count, uint16_t bufferLength ){
        
        cout << "SiAudioInput::setup" << endl;
        
        showAllDevices();
        
        _enabled      = true;
        _bandCount    = count;
        _bufferLength = bufferLength;
        _waveBias     = 1.0f;
        _fftBias      = 1.0f;
        
        // --- Create Sound Context.
        
        cout << "create Context." << endl;
        
        auto ctx = audio::Context::master();
        
        cout << "get InputDevice : ";
        
        ci::audio::DeviceRef dev;
        if( deviceName == "*" ){
            dev = ci::audio::Device::getDefaultInput();
        }else{
            dev = ci::audio::Device::findDeviceByName(deviceName);
        }
        if( !dev ){
            cout << "AudioDevice \"" << deviceName << "\" is not found." << endl;
            throw;
        }
        
        cout << dev->getName() << endl;
        // The InputDeviceNode is platform-specific, so you create it using a special method on the Context:
        mInputDeviceNode = ctx->createInputDeviceNode( dev );
        
        // --- init FFT
        
        cout << "Setup FFT." << endl;
        
        _fftValues     = new float[_bandCount];
        _fftNormalized = new float[_bandCount];
        _fftRanged     = new float[_bandCount/8];
        
        auto monitorSpectralFormat = audio::MonitorSpectralNode::Format().windowSize( _bandCount * 8 );
        mMonitorSpectralNode = ctx->makeNode( new audio::MonitorSpectralNode( monitorSpectralFormat ) );
        try{
            mInputDeviceNode->connect( mMonitorSpectralNode );
        }catch(...){
            cout << "error" << endl;
            _enabled = false;
        }
        
        // --- init Wave
        
        cout << "Setup Audio Channels." << endl;
        
        auto monitorFormat   = audio::MonitorSpectralNode::Format().windowSize( bufferLength );
        mMonitorNode = ctx->makeNode( new audio::MonitorNode(monitorFormat) );
        try{
            mInputDeviceNode->connect(mMonitorNode);
        }catch(...){
            cout << "error" << endl;
            _enabled = false;
        }
        
        // ---
        
        try{
            
            // InputDeviceNode (and all InputNode subclasses) need to be enabled()'s to process audio. So does the Context:
            mInputDeviceNode->enable();
            ctx->enable();
            
            const audio::Buffer &buffer = mMonitorNode->getBuffer();
            _numChannels = buffer.getNumChannels();
            
        }catch(...){
            
            _numChannels = 2;
            _enabled = false;
            
        }
        
        cout << "done" << endl;
        
    }
    
    void SiAudioInput::setWaveBias( float bias ){
        _waveBias = bias;
    }
    
    float SiAudioInput::getWaveBias(){
        return _waveBias;
    }
    
    void SiAudioInput::setFFTBias( float bias ){
        _fftBias = bias;
    }
    
    float SiAudioInput::getFFTBias(){
        return _fftBias;
    }
    
    float SiAudioInput::getVolume(){
        if( _enabled ){
            return mMonitorNode->getVolume() * _waveBias;
        }else{
            return 0.5;
        }
    }
    
    size_t SiAudioInput::numChannels(){
        return _numChannels;
    }
    
    const float * SiAudioInput::getChannelAt( size_t ch ){
        return _channels[ch];
    }
    
    uint16_t SiAudioInput::getBandCount(){
        return _bandCount;
    }
    
    uint16_t SiAudioInput::getBufferLength(){
        return _bufferLength;
    }
    
    float * SiAudioInput::getFFT(){
        return _fftValues;
    }
    
    float * SiAudioInput::getFFTNormalized(){
        return _fftNormalized;
    }
    
    int SiAudioInput::getFFTRangedLength(){
        return _bandCount / 8;
    }
    
    float * SiAudioInput::getFFTRanged(){
        return _fftRanged;
    }
    
    float SiAudioInput::getFFTRangedAt( int index ){
        if( 0 <= index && index < getFFTRangedLength() ){
            return _fftRanged[index];
        }else{
            return 0.0f;
        }
    }
    
    void SiAudioInput::update( float decline ){
        
        float scale = 10.0 * _fftBias;
        
        // --- Wave
        
        if( _enabled ){
            
            const audio::Buffer &buffer = mMonitorNode->getBuffer();
            for( size_t ch = 0; ch < buffer.getNumChannels(); ch++ ) {
                const float * channel = buffer.getChannel(ch);
                if( _waveBias != 1.0 ){
                    float * list = new float[_bandCount*2];
                    for( int i = 0, len = _bandCount*2; i < len; i++ ){
                        list[i] = channel[i] * _waveBias;
                    }
                    _channels[ch] = list;
                }else{
                    _channels[ch] = channel;
                }
            }
            
        }else{
            
            for( size_t ch = 0; ch < _numChannels; ch++ ) {
                float * list = new float[_bandCount*2];
                //*
                for( int i = 0, len = _bandCount*2; i < len; i++ ){
                    list[i] = -0.5f + randFloat();
                }
                //*/
                _channels[ch] = list;
            }
            
        }
        
        // --- FFT
        
        if( _enabled ){
            
            int i = 0;
            float maxValue = 0.0000001f;
            
            // We copy the magnitude spectrum out from the Node on the main thread, once per update:
            mMagSpectrum = mMonitorSpectralNode->getMagSpectrum();
            for( auto spectrum = mMagSpectrum.begin(); spectrum != mMagSpectrum.end(); ++spectrum ) {
                maxValue = max( maxValue, *spectrum * scale );
            }
            
            if( 1.0 < maxValue ){
                maxValue = 1.0;
            }
            
            float tmp    = 0;
            int idx      = 0;
            int count    = 0;
            int countMax = 8;
            
            for( auto spectrum = mMagSpectrum.begin(); spectrum != mMagSpectrum.end(); ++spectrum ) {
                
                // --- fft の素の値を格納する.
                
                _fftValues[i] = max( _fftValues[i] * decline, *spectrum * scale );
                if( _fftValues[i] != _fftValues[i] ){
                    _fftValues[i] = 0;
                }
                // --- fft の値を maxValue で割った値を格納する.
                
                _fftNormalized[i] = _fftValues[i] / maxValue;
                
                // --- fft の値を _bandCount / 16 ごとに分けて平均を格納する.
                
                if( tmp < _fftValues[i] ){
                    tmp = _fftValues[i];
                }
                
                if( ++count == countMax ){
                   _fftRanged[idx++] = tmp;
                    tmp   = 0.0f;
                    count = 0;
                }
                
                // ---
                
                if( _bandCount <= ++i ){
                    break;
                }
                
            }
            
        }else{
            
            //*
            float bias = 0.98;
            for( int i = 0; i < _bandCount; i++ ){
                _fftValues[i] = max( _fftValues[i] * decline, randFloat() * bias );
                bias *= 0.92;
            }
            //*/
            
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
        drawFFT( bounds, _bandCount );
    }
    
    void SiAudioInput::drawFFT( Rectf bounds, int length ){
        
        float width  = (bounds.x2 - bounds.x1) / ( length - 1 );
        float height = (bounds.y2 - bounds.y1);
        
        ci::gl::pushMatrices();
        {
            ci::gl::translate( vec3(bounds.x1, bounds.y1, 0.0f) );
            for( int i = 0; i < (length-1); i++ ) {
                float barY     = _fftValues[i] * height;
                float nextBarY = _fftValues[i+1] * height;
                ci::gl::drawLine( vec2( i*width, height-barY ), vec2( (i+1)*width, height - nextBarY ) );
            }
        }
        ci::gl::popMatrices();
        
    }
    
    void SiAudioInput::drawFFTRanged( Rectf bounds ){
        
        int   length = _bandCount / 8;
        
        float width  = (bounds.x2 - bounds.x1) / ( length - 1 );
        float height = (bounds.y2 - bounds.y1);
        
        ci::gl::pushMatrices();
        {
            ci::gl::translate( vec3(bounds.x1, bounds.y1, 0.0f) );
            for( int i = 0; i < (length-1); i++ ) {
                float barY = _fftRanged[i] * height;
                ci::gl::drawStrokedRect(
                    Rectf( i*width, height-barY, (i+1)*width, height )
                );
            }
        }
        ci::gl::popMatrices();
        
    }
    
    void SiAudioInput::drawWave( Rectf bounds ){
        
        float width  = bounds.getWidth()  / (float)_bufferLength;
        float height = bounds.getHeight() / 2.0;
        
        
        PolyLineT<vec2>	leftBufferLine;
        
        ci::gl::pushMatrices();
        {
            ci::gl::translate( vec3( bounds.x1, bounds.y1 + height, 0.0f ) );
            for( int i = 0; i < _bufferLength; i++ ) {
                float x = i * width;
                float y = _channels[0][i] * height;
                leftBufferLine.push_back( vec2( x , y ) );
            }
            ci::gl::draw( leftBufferLine );
        }
        ci::gl::popMatrices();
        
    };
    
    //! protected:
    
    void SiAudioInput::onFFTAverage( OscInputEvent * event){
        _fftAverage = event->message.getArgAsFloat(0);
    }
    
    void SiAudioInput::onAudioGain( OscInputEvent * event){
        _audioGain  = event->message.getArgAsFloat(0);
    }
    
}}