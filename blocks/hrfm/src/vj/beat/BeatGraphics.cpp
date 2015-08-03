#include "BeatGraphics.h"

namespace hrfm{ namespace vj{
    
    // public
    
    BeatGraphics::BeatGraphics(){
        
        hrfm::display::FboDisplayNode();
        
        _fbo = new hrfm::gl::ExFbo(128,128);
        _bpm = &hrfm::vj::SiBPM::getInstance();
        _bpm->addEventListener("bpm",this,&BeatGraphics::_onBPM);
        
        // ==============================
        
        hrfm::vj::BeatFilterBase * blank = new hrfm::vj::BeatFilterBase();
        blank->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterSplit * split = new hrfm::vj::BeatFilterSplit();
        split->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterTransTunnel * tunnel = new hrfm::vj::BeatFilterTransTunnel(3.0);
        tunnel->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterTransField * field = new hrfm::vj::BeatFilterTransField();
        field->setup( Vec2i(1024,1024) );
        
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
        
        hrfm::vj::BeatFilterDot * dot = new hrfm::vj::BeatFilterDot();
        dot->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * kaleido = new hrfm::vj::BeatFilterBase("BeatFilterKaleidoscope.glsl");
        kaleido->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterColor * color = new hrfm::vj::BeatFilterColor();
        color->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * test = new hrfm::vj::BeatFilterBase("BeatFilterTest.glsl");
        test->setup( Vec2i(1024,1024) );
        
        hrfm::vj::BeatFilterBase * expand = new hrfm::vj::BeatFilterBase("BeatFilterExpand.glsl");
        expand->setup( Vec2i(1024,1024) );
        
        // --- -------------------------------------------------
        // --- Post Filter
        
        //addPostFilter( expand );
        addPostFilter( color );
        //addPostFilter( kaleido );
        
        // --- -------------------------------------------------
        // --- TUNNEL
        
        
        {
            BeatPresetBase * preset = new BeatPresetTunnel();
            preset
                ->addContent( new BeatSquare() )
            ;
            addPreset( preset );
        }
        {
            BeatPresetBase * preset = new BeatPresetTunnel();
            preset
                ->addContent( new BeatLine() )
            ;
            addPreset( preset );
        }
        
        // --- -------------------------------------------------
        // --- POLAR
        
        {
            BeatPresetBase * preset = new BeatPresetPolar();
            preset
                ->addContent( new BeatSquare() )
            ;
            addPreset( preset );
        }
        {
            BeatPresetBase * preset = new BeatPresetPolar();
            preset
                ->addContent( new BeatHoge() )
            ;
            addPreset( preset );
        }
        
        // --- -------------------------------------------------
        // --- FIELD V
        
        {
            BeatPresetBase * preset = new BeatPresetFieldV();
            preset
                ->addContent( new BeatSquare() )
            ;
            addPreset( preset );
        }
        {
            BeatPresetBase * preset = new BeatPresetFieldV();
            preset
                ->addContent( new BeatLine() )
            ;
            addPreset( preset );
        }
        {
            BeatPresetBase * preset = new BeatPresetFieldV();
            preset
                ->addContent( new BeatHoge() )
            ;
            addPreset( preset );
        }
        
        // --- -------------------------------------------------
        
        {
            BeatPresetBase * preset = new BeatPresetBase();
            preset
            ->addContent( new BeatSquare() )
            ->addContent( new BeatLine() )
            ->addFilter(split,0)
            ->addFilter(split,1)
            ->addFilter(split,2)
            ->addFilter(tunnel,2)
            ->addFilter(field,2)
            ->addFilter(dot);
            addPreset( preset );
        }
        
        // --- -------------------------------------------------
        // --- SPLIT
        
        {
            BeatPresetBase * preset = new BeatPresetSplit();
            preset
                ->addContent( new BeatSquare() )
            ;
            addPreset( preset );
        }
        {
            BeatPresetBase * preset = new BeatPresetSplit();
            preset
                ->addContent( new BeatLine() )
            ;
            addPreset( preset );
        }
        
        // --- -------------------------------------------------
        
        {
            
            hrfm::vj::BeatFilterSplit * vMirror = new hrfm::vj::BeatFilterSplit();
            vMirror->setup( Vec2i(1024,1024) );
            {
                vector<SplitSetting*> * seq = new vector<SplitSetting*>();
                for( int i=0;i<1;i++ ){
                    SplitSetting * setting = new SplitSetting();
                    setting->segments = Vec2f( 2, 1 );
                    setting->rotate   = 2;
                    seq->push_back(setting);
                }
                vMirror->setSequence(seq);
            }
            
            hrfm::vj::BeatFilterTransPolar * polar = new hrfm::vj::BeatFilterTransPolar(1.0,2.0);
            polar->setup( Vec2i(1024,1024) );
            
            BeatPresetBase * preset = new BeatPresetBase();
            preset
            ->addContent( new BeatAudioWave(true) )
            ->addFilter(vMirror)
            ->addFilter(polar);
            addPreset( preset );
            
        }
        
        {
            
            hrfm::vj::BeatFilterSplit * vMirror = new hrfm::vj::BeatFilterSplit();
            vMirror->setup( Vec2i(1024,1024) );
            {
                vector<SplitSetting*> * seq = new vector<SplitSetting*>();
                for( int i=0;i<1;i++ ){
                    SplitSetting * setting = new SplitSetting();
                    setting->segments = Vec2f( 2, 1 );
                    setting->rotate   = 2;
                    seq->push_back(setting);
                }
                vMirror->setSequence(seq);
            }
            
            hrfm::vj::BeatFilterScroll * scroll = new hrfm::vj::BeatFilterScroll(0.0,-0.06);
            scroll->setup( Vec2i( 1024, 1024 ) );
            
            BeatPresetBase * preset = new BeatPresetBase();
            preset
                ->addContent( new BeatEqualizer() )
                ->addFilter(vMirror)
                ->addFilter(scroll)
            ;
            addPreset( preset );
            
        }
        
        // ---
        
        setPresetByIndex(0);
        
    }
    
    void BeatGraphics::setSize( int w, int h ){
        if( width != w || height != h ){
            DisplayNode::setSize(w, h);
            _fbo = new hrfm::gl::ExFbo(w,h);
        }
    }
    
    // 後で消す.
    void BeatGraphics::setPresetByIndex( int index ){
        
        if( index < 0 ){
            index = 0;
        }
        if( _presetList.size() <= index ){
            index = _presetList.size() - 1;
        }
        
        if( _presetOrder.size() == 0 ){
            _presetOrderPointer = 0;
            _presetOrder.push_back(index);
        }else if( _presetOrder.at( _presetOrder.size()-1 ) == index ){
            _presetOrder.clear();
            _presetOrderPointer = 0;
            _presetOrder.push_back(index);
        }else{
            _presetOrder.push_back(index);
            _presetOrderPointer = _presetOrder.size() - 1;
        }
        
        _onBPM();
        
    }
    
    void BeatGraphics::addPreset( BeatPresetBase * preset ){
        _presetList.push_back(preset);
    }
    
    void BeatGraphics::addPreFilter( hrfm::gl::FilterBase * filter ){
        _preFilter.push_back( filter );
    }
    
    void BeatGraphics::addPostFilter( hrfm::gl::FilterBase * filter ){
        _postFilter.push_back( filter );
    }
    
    // protected
    
    void BeatGraphics::_update(){
        
        _fbo->beginOffscreen();
        {
            ci::gl::clear();
            _preset->getContent()->draw(_bpm->position, width, height);
            ci::gl::color(1.0,1.0,1.0);
            // --- Pre Filter.
            {
                vector<hrfm::gl::FilterBase*>::iterator it;
                vector<hrfm::gl::FilterBase*>::iterator end = _preFilter.end();
                for( it=_preFilter.begin(); it!=end; ++it ){
                    _fbo->applyFilter( *it );
                }
            }
            // --- Apply active filters.
            {
                vector<hrfm::vj::BeatFilterBase*>::iterator it;
                vector<hrfm::vj::BeatFilterBase*>::iterator end = _preset->getActiveFilters()->end();
                for( it=_preset->getActiveFilters()->begin(); it!=end; ++it ){
                    _fbo->applyFilter( *it );
                    (*it)->updated = false;
                }
            }
            // --- Post Filter.
            {
                vector<hrfm::gl::FilterBase*>::iterator it;
                vector<hrfm::gl::FilterBase*>::iterator end = _postFilter.end();
                for( it=_postFilter.begin(); it!=end; ++it ){
                    _fbo->applyFilter( *it );
                }
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
        
        _preset = _presetList.at( _presetOrder.at( _presetOrderPointer++ ) );
        
        if( _presetOrder.size() <= _presetOrderPointer ){
            _presetOrderPointer = 0;
        }
        
        _preset->change(width,height);
        
    }
    
}}