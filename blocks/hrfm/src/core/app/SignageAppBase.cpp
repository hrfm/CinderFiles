#include "SignageAppBase.h"

namespace hrfm{ namespace app{
    
    void SignageAppBase::setup(){
        
        hrfm::app::AppBase::setup();
        
        cout << "--- SignageAppBase::setup() -------------------------------------------------" << endl << endl;
        
        XmlTree setting = hrfm::app::SiSetting::getInstance().getXmlTree();
        
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
    
    void SignageAppBase::_initScheduled( XmlTree settings ){
        scheduledContents = new hrfm::signage::display::ScheduledContents( settings );
        scheduledContents->addEventListener("moviecomplete", this, &SignageAppBase::onScheduleComplete );
        stage.addChild( scheduledContents );
    }
    
    void SignageAppBase::_initSequential( XmlTree settings ){
        sequentialContents = new hrfm::signage::display::SequentialContents( settings );
        /*
        sequentialContents->setTransition(
            new hrfm::signage::display::Transition(
                mWindowSize,
                new hrfm::gl::TransitionShaderBase( DataLoader::load("DissolvTransition.glsl") )
            )
        );
        //*/
        stage.addChild( sequentialContents );
        sequentialContents->play(0);
    }
    
    void SignageAppBase::onTrigger( hrfm::events::TriggerEvent * event ){
        cout << event->triggerName() << endl;
    }
    
    void SignageAppBase::onScheduleComplete( hrfm::events::Event * event ){}
    
}}