#include "BeatGraphics.h"

namespace hrfm{ namespace vj{
    
    // public
    
    BeatGraphics::BeatGraphics(){
        
        hrfm::display::FboDisplayNode();
        
        _fbo = new hrfm::gl::ExFbo(128,128);
        _bpm = &hrfm::vj::SiBPM::getInstance();
        _bpm->addEventListener("bpm",this,&BeatGraphics::_onBPM);
        
        // --- Content. あとで外部から指定するようにする.
        
        addContent( new BeatSquare() );
        addContent( new BeatLine() );
        addContent( new BeatCircle() );
        
        //_contentVector.push_back( new BeatSquare4() );
        
        // --- Filter. あとで外部から指定するようにする.
        
        hrfm::vj::BeatFilterBase * polar = new hrfm::vj::BeatFilterBase("BeatFilterPolar.glsl");
        polar->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * split = new hrfm::vj::BeatFilterSplit();
        split->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * color = new hrfm::vj::BeatFilterBase("BeatFilterColor.glsl");
        color->setup( Vec2i(1024,1024) );
        
        addFilter(0,polar);
        addFilter(1,split);
        addFilter(2,split);
        addFilter(3,split);
        addFilter(4,color);
        
        // ---
        
        _onBPM();
        
    }
    
    void BeatGraphics::setSize( int w, int h ){
        if( width != w || height != h ){
            DisplayNode::setSize(w, h);
            _fbo = new hrfm::gl::ExFbo(w,h);
        }
    }
    
    void BeatGraphics::addContent( BeatContentBase * content ){
        _contentVector.push_back( content );
    }
    
    void BeatGraphics::addFilter( int index, BeatFilterBase * filter ){
        while( _filterStock.size() <= index ){
            _filterStock.push_back( new vector<hrfm::vj::BeatFilterBase*>() );
        }
        _filterStock.at(index)->push_back(filter);
    }
    
    // protected
    
    void BeatGraphics::_update(){
        
        _fbo->beginOffscreen();
        {
            ci::gl::clear();
            _content->draw(_bpm->position, width, height);
            ci::gl::color(1.0,1.0,1.0);
            vector<hrfm::vj::BeatFilterBase*>::iterator it;
            vector<hrfm::vj::BeatFilterBase*>::iterator end = _activeFilters.end();
            for( it=_activeFilters.begin(); it!=end; ++it ){
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
    
    // private
    
    void BeatGraphics::_onBPM( hrfm::events::Event * event ){
        
        // --- Pick the content. ---
        
        _content = _contentVector.at( randInt( _contentVector.size() ) );
        
        // --- Create active filter list. ---
        
        _activeFilters.clear();
        vector<vector<hrfm::vj::BeatFilterBase*>*>::iterator it;
        vector<vector<hrfm::vj::BeatFilterBase*>*>::iterator end = _filterStock.end();
        for( it=_filterStock.begin(); it!=end; ++it ){
            if( 0 < (*it)->size() ){
                BeatFilterBase * filter = (*it)->at( randInt( (*it)->size() ) );
                if( 0 < width * height && ( filter->getWidth() != width || filter->getHeight() != height ) ){
                    filter->setSize( width, height );
                }
                filter->update();
                _activeFilters.push_back( filter );
            }
        }
        
    }
    
}}