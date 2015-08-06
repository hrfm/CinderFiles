#include "BeatGraphics.h"

namespace hrfm{ namespace vj{
    
    // public
    
    BeatGraphics::BeatGraphics(){
        VJGraphic();
        start();
    }
    
    void BeatGraphics::addPreset( BeatPresetBase * preset ){
        _presetList.push_back(preset);
    }
    
    void BeatGraphics::displayPresetAt( int index ){
        
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
    
    void BeatGraphics::update(){
        
        DisplayNode::update();
        
        vector<hrfm::gl::FilterBase*>::iterator it;
        vector<hrfm::gl::FilterBase*>::iterator end;
        
        _fbo->beginOffscreen(true);
        {
            ci::gl::color(1.0,1.0,1.0);
            _preset->getContent()->draw(_bpm->position, width, height);
            ci::gl::color(1.0,1.0,1.0);
            // --- Pre Filter.
            {
                end = _preFilter.end();
                for( it=_preFilter.begin(); it!=end; ++it ){
                    _fbo->applyFilter( *it );
                }
            }
            // ---
            {
                end = _preset->getActiveFilters()->end();
                for( it=_preset->getActiveFilters()->begin(); it!=end; ++it ){
                    _fbo->applyFilter( *it );
                    if( BeatFilterBase * f = dynamic_cast<BeatFilterBase*>( *it ) ){
                        f->updated = false;
                    }
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
    
    // protected
    
    void BeatGraphics::_onBPM( hrfm::events::Event * event ){
        VJGraphic::_onBPM(event);
        _preset = _presetList.at( _presetOrder.at( _presetOrderPointer++ ) );
        if( _presetOrder.size() <= _presetOrderPointer ){
            _presetOrderPointer = 0;
        }
        _preset->change(width,height);
    }
    
}}