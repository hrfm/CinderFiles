#include "BeatGraphics.h"

namespace hrfm{ namespace vj{
    
    // public
    
    BeatGraphics::BeatGraphics(){
        
        hrfm::display::FboDisplayNode();
        
        _fbo = new hrfm::gl::ExFbo(128,128);
        _bpm = &hrfm::vj::SiBPM::getInstance();
        _bpm->addEventListener("bpm",this,&BeatGraphics::_onBPM);
        
        // --- Content. あとで外部から指定するようにする.
        
        // --- Filter. あとで外部から指定するようにする.
        
        hrfm::vj::BeatFilterBase * blank = new hrfm::vj::BeatFilterBase();
        blank->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * polar = new hrfm::vj::BeatFilterBase("BeatFilterTransPolar.glsl");
        polar->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * tunnel = new hrfm::vj::BeatFilterBase("BeatFilterTransTunnel.glsl");
        tunnel->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * sphere = new hrfm::vj::BeatFilterBase("BeatFilterTransSphere.glsl");
        sphere->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * flr = new hrfm::vj::BeatFilterBase("BeatFilterTransFloor.glsl");
        flr->setup( Vec2i(1024,1024) );
        
        // ---
        
        hrfm::vj::BeatFilterSplit * split2 = new hrfm::vj::BeatFilterSplit();
        split2->setup( Vec2i(1024,1024) );
        {
            vector<SplitSetting*> * seq = new vector<SplitSetting*>();
            for( int i=0;i<1;i++ ){
                SplitSetting * setting = new SplitSetting();
                setting->segments = Vec2f( 2, 2 );
                setting->rotate   = 100;
                seq->push_back(setting);
            }
            split2->setSequence(seq);
        }
        
        hrfm::vj::BeatFilterSplit * vMirror = new hrfm::vj::BeatFilterSplit();
        vMirror->setup( Vec2i(1024,1024) );
        {
            vector<SplitSetting*> * seq = new vector<SplitSetting*>();
            for( int i=0;i<1;i++ ){
                SplitSetting * setting = new SplitSetting();
                setting->segments = Vec2f( 2, 1 );
                setting->rotate   = 0;
                seq->push_back(setting);
            }
            vMirror->setSequence(seq);
        }
        
        hrfm::vj::BeatFilterSplit * split = new hrfm::vj::BeatFilterSplit();
        split->setup( Vec2i(1024,1024) );
        /*
         {
         vector<SplitSetting*> * seq = new vector<SplitSetting*>();
         for( int i=0;i<1;i++ ){
         SplitSetting * setting = new SplitSetting();
         setting->segments = Vec2f( 2, 2 );
         setting->rotate   = 0;
         seq->push_back(setting);
         }
         split->setSequence(seq);
         }
         //*/
        
        // ---
        
        hrfm::vj::BeatFilterBase * color = new hrfm::vj::BeatFilterBase("BeatFilterColor.glsl");
        color->setup( Vec2i(1024,1024) );
        
        
        /*
        // FIX!
        addContent( new BeatSquare() );
        addContent( new BeatLine() );
        addFilter(split,0);
        addFilter(split,1);
        addFilter(tunnel,2);
        //*/
        
        /*
        // FIX!
        addContent( new BeatSquare() );
        addContent( new BeatLine() );
        addFilter(split);
        addFilter(blank,1);
        addFilter(split,1);
        addFilter(blank,2);
        addFilter(split,2);
        addFilter(blank,3);
        addFilter(split,3);
        addFilter(flr,4);
        //*/
        
        /*
        // FIX!
        addContent( new BeatSquare() );
        addContent( new BeatHoge() );
        addFilter(split2);
        addFilter(blank,1);
        addFilter(split2,1);
        addFilter(polar);
        //*/
        
        //*
        //addContent( new BeatSquare() );
        //addContent( new BeatLine() );
        //addContent( new BeatCircle() );
        addContent( new BeatEqualizer() );
        //addContent( new BeatAudioWave() );
        addFilter(vMirror,0);
        addFilter(flr,4);
        //addFilter(split,1);
        //addFilter(tunnel,2);
        //*/
        
        
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
    
    void BeatGraphics::addFilter( BeatFilterBase * filter ){
        addFilter( filter, _filterStock.size() );
    }
    void BeatGraphics::addFilter( BeatFilterBase * filter, int index ){
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
                (*it)->updated = false;
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
        _content->setup();
        
        // --- Create active filter list. ---
        
        {
            _activeFilters.clear();
            vector<vector<hrfm::vj::BeatFilterBase*>*>::iterator it;
            vector<vector<hrfm::vj::BeatFilterBase*>*>::iterator end = _filterStock.end();
            for( it=_filterStock.begin(); it!=end; ++it ){
                if( 0 < (*it)->size() ){
                    BeatFilterBase * filter = (*it)->at( randInt( (*it)->size() ) );
                    if( 0 < width * height && ( filter->getWidth() != width || filter->getHeight() != height ) ){
                        filter->setSize( width, height );
                    }
                    filter->update( _content );
                    _activeFilters.push_back( filter );
                }
            }
        }
        
    }
    
}}