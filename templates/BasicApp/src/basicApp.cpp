#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Shader.h"
#include <list>

#include "hrfm.h"
#include "hrfm.gl.h"
#include "VJBase.h"

#include "OscSender.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// We'll create a new Cinder Application by deriving from the AppBasic class
class BasicApp : public hrfm::vj::app::VJBase {
public:
    void setup();
    void update();
    void draw();
    void resize(){}
    vector<cinder::osc::Sender> sender;
    vector<string> addresses;
};

void BasicApp::setup(){
    hrfm::vj::app::VJBase::setup();
    for( int i=0; i<10; i++ ){
        cinder::osc::Sender s;
        s.setup("localhost", 10000);
        sender.push_back(s);
    }
    
    addresses.push_back("test");
    addresses.push_back("hoge");
    addresses.push_back("fuga");
    addresses.push_back("piyo");
    addresses.push_back("aaaa");
    addresses.push_back("bbbb");
    addresses.push_back("cccc");
    addresses.push_back("dddd");
    
}

void BasicApp::update(){
    
    int index = randInt( sender.size() );
    
    string address = "/" + addresses[randInt(addresses.size())];
    
    hrfm::vj::app::VJBase::update();
    ci::osc::Message msg;
    msg.setAddress(address);
    msg.addFloatArg(randFloat());
    
    sender.at(index).sendMessage(msg);
    
}

void BasicApp::draw(){
    hrfm::vj::app::VJBase::draw();
}

CINDER_APP( BasicApp, RendererGl( RendererGl::Options().msaa( 16 ) ), []( App::Settings *settings ) {
    //	settings->enableHighDensityDisplay();
    settings->setWindowSize( 640, 480 );
} )