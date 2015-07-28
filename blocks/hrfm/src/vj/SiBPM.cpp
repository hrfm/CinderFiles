#include "SiBPM.h"

using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace vj{
    
    void SiBPM::setBPM( double bpm ){
        this->bpm = bpm;
        this->millisecPerBeat = 60.00000 / bpm;
    }
    
    double SiBPM::getBPM(){
        return this->bpm;
    }
    
    void SiBPM::start(){
        this->startTime = ci::app::getElapsedSeconds();
    }
    
    void SiBPM::start( double bpm ){
        this->setBPM(bpm);
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