#include "BeatGraphics.h"

namespace hrfm{ namespace vj{
    
    void BeatGraphics::_onBPM( hrfm::events::Event * event ){
        cout << "bpm" << endl;
    }
    
    void BeatGraphics::setSize( int w, int h ){
        if( width != w || height != h ){
            DisplayNode::setSize(w, h);
            _fbo = new hrfm::gl::ExFbo(w,h);
            vector<hrfm::gl::FilterBase*>::iterator it;
            vector<hrfm::gl::FilterBase*>::iterator end = _filters.end();
            for( it=_filters.begin(); it!=end; ++it ){
                (*it)->setSize( w, h );
            }
        }
    }
    
    void BeatGraphics::_update(){
        
        _fbo->beginOffscreen();
        {
            ci::gl::clear();
            
            ci::gl::color(1.0,1.0,1.0);
            Rectf rect = Rectf(0,0,width,height);
            rect.scaleCentered( min( 1.0, _bpm->position / 0.5 ) );
            ci::gl::drawSolidRect( rect );
            
            ci::gl::color(0.0,0.0,0.0);
            rect = Rectf(0,0,width,height);
            rect.scaleCentered( max( 0.0, ( _bpm->position - 0.5 ) / 0.5 ) );
            ci::gl::drawSolidRect( rect );
            
            ci::gl::color(1.0,1.0,1.0);
            
            vector<hrfm::gl::FilterBase*>::iterator it;
            vector<hrfm::gl::FilterBase*>::iterator end = _filters.end();
            for( it=_filters.begin(); it!=end; ++it ){
                _fbo->applyFilter( *it );
            }
            
        }
        _fbo->endOffscreen();
        
    }
    
    void BeatGraphics::_draw(){
        ci::gl::enableAdditiveBlending();
        {
            ci::gl::draw( _fbo->getTexture() );
        }
        ci::gl::disableAlphaBlending();
    }
    
}}