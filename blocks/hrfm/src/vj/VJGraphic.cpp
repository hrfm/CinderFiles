#include "VJGraphic.h"

namespace hrfm{ namespace vj{
    
    // public
    
    VJGraphic::VJGraphic(){
        hrfm::display::DisplayNode();
        _fbo = new hrfm::gl::ExFbo(128,128);
        _bpm = &hrfm::vj::SiBPM::getInstance();
    }
    
    void VJGraphic::start(){
        _bpm->addEventListener("bpm",this,&VJGraphic::_onBPM);
    }
    void VJGraphic::stop(){
        _bpm->removeEventListener("bpm",this,&VJGraphic::_onBPM);
    }
    
    void VJGraphic::setSize( int w, int h ){
        if( width != w || height != h ){
            hrfm::display::DisplayNode::setSize(w, h);
            _fbo = new hrfm::gl::ExFbo(w,h);
        }
    }
    
    void VJGraphic::addPreFilter( hrfm::gl::FilterBase * filter ){
        _preFilter.push_back( filter );
    }
    
    void VJGraphic::addPostFilter( hrfm::gl::FilterBase * filter ){
        _postFilter.push_back( filter );
    }
    
    void VJGraphic::update(){
        
        DisplayNode::update();
        
        vector<hrfm::gl::FilterBase*>::iterator it;
        vector<hrfm::gl::FilterBase*>::iterator end;
        
        _fbo->beginOffscreen(true);
        {
            ci::gl::color(1.0,1.0,1.0);
            _draw();
            _drawChildren();
            ci::gl::color(1.0,1.0,1.0);
            // --- Pre Filter.
            {
                end = _preFilter.end();
                for( it=_preFilter.begin(); it!=end; ++it ){
                    _fbo->applyFilter( *it );
                }
            }
            // --- Post Filter.
            {
                end = _postFilter.end();
                for( it=_postFilter.begin(); it!=end; ++it ){
                    _fbo->applyFilter( *it );
                }
            }
        }
        _fbo->endOffscreen();
        
    }
    
    void VJGraphic::draw( ci::ColorA * drawColor ){
        ci::gl::pushMatrices();
        ci::gl::translate( x, y );
        ci::gl::enableAdditiveBlending();
        {
            ci::gl::draw( _fbo->getTexture() );
        }
        ci::gl::disableAlphaBlending();
        ci::gl::popMatrices();
    }
    
    // protected
    
    void VJGraphic::_onBPM( hrfm::events::Event * event ){}
    
}}