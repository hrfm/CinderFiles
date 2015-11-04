#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <list>

#include "hrfm.h"
#include "hrfm.gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// We'll create a new Cinder Application by deriving from the AppBasic class
class BasicApp : public hrfm::app::AppBase {
  public:
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<vec2>		mPoints;
};

void BasicApp::mouseDrag( MouseEvent event )
{
	mPoints.push_back( event.getPos() );
}

void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void BasicApp::draw()
{
    ci::gl::clear( Color( 0.1f, 0.1f, 0.15f ) );

	ci::gl::color( 1.0f, 0.5f, 0.25f );
	ci::gl::begin( GL_LINE_STRIP );
	for( auto pointIter = mPoints.begin(); pointIter != mPoints.end(); ++pointIter ) {
		ci::gl::vertex( *pointIter );
	}
	ci::gl::end();
}

CINDER_APP( BasicApp, RendererGl( RendererGl::Options().msaa( 16 ) ), []( App::Settings *settings ) {
    //	settings->enableHighDensityDisplay();
    settings->setWindowSize( 640, 480 );
} )