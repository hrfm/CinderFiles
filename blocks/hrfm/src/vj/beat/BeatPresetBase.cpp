#include "BeatPresetBase.h"

namespace hrfm{ namespace vj{
    
    BeatPresetBase::BeatPresetBase(){
        _activeFilters = new vector<BeatFilterBase*>();
    }
    
    BeatPresetBase * BeatPresetBase::addContent( BeatContentBase * content ){
        _contentVector.push_back( content );
        return this;
    }
    
    BeatPresetBase * BeatPresetBase::addFilter( BeatFilterBase * filter ){
        addFilter( filter, _filterStock.size() );
        return this;
    }
    BeatPresetBase * BeatPresetBase::addFilter( BeatFilterBase * filter, int index ){
        while( _filterStock.size() <= index ){
            _filterStock.push_back( new vector<BeatFilterBase*>() );
        }
        _filterStock.at(index)->push_back(filter);
        return this;
    }
    
    BeatContentBase * BeatPresetBase::getContent(){
        return _content;
    }
    
    vector<BeatFilterBase*> * BeatPresetBase::getActiveFilters(){
        return _activeFilters;
    }
    
    void BeatPresetBase::change( int width, int height ){
        
        // --- Pick the content. ---
        
        _content = _contentVector.at( randInt( _contentVector.size() ) );
        _content->setup();
        
        // --- Create active filter list. ---
        
        {
            _activeFilters->clear();
            vector<vector<hrfm::vj::BeatFilterBase*>*>::iterator it;
            vector<vector<hrfm::vj::BeatFilterBase*>*>::iterator end = _filterStock.end();
            for( it=_filterStock.begin(); it!=end; ++it ){
                if( 0 < (*it)->size() ){
                    BeatFilterBase * filter = (*it)->at( randInt( (*it)->size() ) );
                    if( 0 < width * height && ( filter->getWidth() != width || filter->getHeight() != height ) ){
                        filter->setSize( width, height );
                    }
                    filter->update( _content );
                    _activeFilters->push_back( filter );
                }
            }
        }
        
    }
    
}}