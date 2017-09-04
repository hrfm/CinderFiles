#include "AppSignage.h"

namespace hrfm{ namespace app{
    
    void AppSignage::setup(){
        
        hrfm::app::AppInstallation::setup();
        mWindowSize = ::hrfm::app::SiAppSettings::getInstance().getWindowSize();
        stage = hrfm::display::Stage::create();
        resize();
        
        cout << "--- AppSignage::setup() -------------------------------------------------" << endl << endl;
        
        XmlTree setting = hrfm::app::SiAppSettings::getInstance().getXml();
        
        if( setting.hasChild("setting/xml") ){
            
            XmlTree setting_xml = setting.getChild("setting/xml");
            
            // ------------------------------------------------------------------------
            // スケジュール設定があった場合それを読み込みます.
            if( setting_xml.hasAttribute("schedule") ){
                XmlTree xml = XmlLoader::load( setting_xml.getAttributeValue<string>("schedule") );
                _initScheduled( xml );
            }
            
            // ------------------------------------------------------------------------
            // シーケンス設定があった場合それを読み込みます.
            if( setting_xml.hasAttribute("sequence") ){
                XmlTree xml = XmlLoader::load( setting_xml.getAttributeValue<string>("sequence") );
                _initSequential( xml );
            }
            
        }
        
        //cout << "------------------------------------------------------------------------" << endl << endl;
        
    }
    
    void AppSignage::update(){
        hrfm::app::AppInstallation::update();
        updateStage();
    }
    
    void AppSignage::updateStage(){
        vector<DisplayNode*>::iterator it;
        vector<DisplayNode*>::iterator end = stage->children.end();
        for( it=stage->children.begin(); it!=end; ++it ){
            (*it)->size( getWindowSize() );
        }
        stage->update();
        stage->draw();
    }
    
    void AppSignage::draw(){
        ci::gl::clear();
        ci::gl::draw( stage->getTexture(), getWindowBounds() );
        if( _isDebugMode ){
            //cout << "debug" << endl;
            _drawDebug();
        }
    }
    
    ivec2 AppSignage::getWindowSize(){
        return mWindowSize;
    }
    
    void AppSignage::resize(){
        camera.setPerspective( 30, getWindowAspectRatio(), 0.1, 100 );
        stage->size( getWindowSize() );
    }
    
    void AppSignage::_initScheduled( XmlTree settings ){
        scheduledContents = new hrfm::signage::display::ScheduledContents( settings );
        scheduledContents->addEventListener( hrfm::signage::events::ScheduledContentsEvent::SCHEDULE_START, this, &AppSignage::onScheduleStart );
        scheduledContents->addEventListener( hrfm::signage::events::ScheduledContentsEvent::SCHEDULE_END  , this, &AppSignage::onScheduleComplete );
        stage->addChild( scheduledContents );
    }
    
    void AppSignage::_initSequential( XmlTree settings ){
        sequentialContents = new hrfm::signage::display::SequentialContents( settings );
        /*
        sequentialContents->setTransition(
            new hrfm::signage::display::Transition(
                mWindowSize,
                new hrfm::gl::TransitionShaderBase( DataLoader::load("DissolvTransition.glsl") )
            )
        );
        //*/
        stage->addChild( sequentialContents );
        sequentialContents->play(0);
    }
    
    void AppSignage::onScheduleStart( hrfm::signage::events::ScheduledContentsEvent * event ){}
    void AppSignage::onScheduleComplete( hrfm::signage::events::ScheduledContentsEvent * event ){}
    
}}
