#include "BeatPresetBase.h"

namespace hrfm{ namespace vj{
    
    BeatPresetBase::BeatPresetBase(){
        _activeFilters = new vector<hrfm::gl::FilterBase*>();
    }
    
    BeatPresetBase * BeatPresetBase::addContent( BeatContentBase * content ){
        _contentVector.push_back( content );
        return this;
    }
    
    BeatPresetBase * BeatPresetBase::addFilter( hrfm::gl::FilterBase * filter ){
        addFilter( filter, _filterStock.size() );
        return this;
    }
    BeatPresetBase * BeatPresetBase::addFilter( hrfm::gl::FilterBase * filter, int index ){
        while( _filterStock.size() <= index ){
            _filterStock.push_back( new vector<hrfm::gl::FilterBase*>() );
        }
        _filterStock.at(index)->push_back(filter);
        return this;
    }
    
    BeatContentBase * BeatPresetBase::getContent(){
        return _content;
    }
    
    vector<hrfm::gl::FilterBase*> * BeatPresetBase::getActiveFilters(){
        return _activeFilters;
    }
    
    void BeatPresetBase::change( int width, int height ){
        
        // --- Pick the content. ---
        
        _content = _contentVector.at( randInt( _contentVector.size() ) );
        _content->setup();
        
        // --- Create active filter list. ---
        
        {
            _activeFilters->clear();
            vector<vector<hrfm::gl::FilterBase*>*>::iterator it;
            vector<vector<hrfm::gl::FilterBase*>*>::iterator end = _filterStock.end();
            for( it=_filterStock.begin(); it!=end; ++it ){
                if( 0 < (*it)->size() ){
                    hrfm::gl::FilterBase * filter = (*it)->at( randInt( (*it)->size() ) );
                    if( 0 < width * height && ( filter->getWidth() != width || filter->getHeight() != height ) ){
                        filter->setSize( width, height );
                    }
                    if( BeatFilterBase * f = dynamic_cast<BeatFilterBase*>( filter ) ){
                        f->update( _content );
                    }
                    _activeFilters->push_back( filter );
                }
            }
        }
        
    }
    
}}