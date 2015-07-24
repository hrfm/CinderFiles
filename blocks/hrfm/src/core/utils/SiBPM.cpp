#include "SiBPM.h"

using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace utils{
    
    void SiBPM::start(){
        this->startTime = ci::app::getElapsedSeconds();
    }
    
    void SiBPM::start( double bpm ){
        this->bpm = bpm;
        this->millisecPerBeat = 60.00000 / bpm;
        start();
    }
    
    void SiBPM::stop(){
        this->startTime = 0;
    }
    
    void SiBPM::update(){
        if( this->startTime == 0 ){
            return;
        }
        double now = ci::app::getElapsedSeconds();
        while( this->millisecPerBeat <= now - this->startTime ){
            this->startTime += this->millisecPerBeat;
            this->dispatchEvent( new hrfm::events::Event("bpm") );
        }
        this->position = ( now - this->startTime ) / this->millisecPerBeat;
    }
    
}}