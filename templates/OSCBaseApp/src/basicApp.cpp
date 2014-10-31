#include "cinder/app/AppNative.h"
#include <list>

#include "OscInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace sgmnt;
using namespace sgmnt::io;

// We'll create a new Cinder Application by deriving from the AppBasic class
class OSCBaseApp : public AppNative {
  public:
    void setup();
    void update();
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec2f>		mPoints;
    
	OscInput mOscInput;
    
};

void onHogeMessage( const sgmnt::Event &event){
    cout << event.message.getNumArgs() << endl;
}

void OSCBaseApp::setup(){
    mOscInput.setup(10001);
    mOscInput.addEventListener( "/hoge", &onHogeMessage );
}

void OSCBaseApp::mouseDrag( MouseEvent event )
{
	mPoints.push_back( event.getPos() );
}

void OSCBaseApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void OSCBaseApp::update(){
    mOscInput.update();
}

void OSCBaseApp::draw()
{
	gl::clear( Color( 0.1f, 0.1f, 0.15f ) );

	gl::color( 1.0f, 0.5f, 0.25f );	
	gl::begin( GL_LINE_STRIP );
	for( auto pointIter = mPoints.begin(); pointIter != mPoints.end(); ++pointIter ) {
		gl::vertex( *pointIter );
	}
	gl::end();
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( OSCBaseApp, RendererGl )