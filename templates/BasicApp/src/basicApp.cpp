#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Shader.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include <list>

#include "hrfm.h"
#include "hrfm.gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// We'll create a new Cinder Application by deriving from the AppBasic class
class BasicApp : public ci::app::App {
public:
    void setup();
    void update();
    void draw();
    void resize(){}
    
    hrfm::gl::ExFboRef _fbo;
    hrfm::gl::FilterBase * _filter;
    ci::gl::TextureRef _tex;
    
};

void BasicApp::setup(){
    _fbo = hrfm::gl::ExFbo::create(8000, 4000);
    _filter = new hrfm::gl::FilterBase("pano.frag");
    //_tex = ci::gl::Texture::create(ci::loadImage( hrfm::io::DataLoader::load("minnanokaigo-logo-tex.png") ));
    _tex = ci::gl::Texture::create(ci::loadImage( hrfm::io::DataLoader::load("minnanokaigo-logo-tex.png") ));
}

void BasicApp::update(){
    _fbo->beginOffscreen(true);
    {
        ci::gl::enableAlphaBlending();
        ci::gl::draw(_tex,_fbo->getBounds());
    }
    _fbo->endOffscreen();
    _fbo->applyFilter(_filter);
}

int ii = 0;

void BasicApp::draw(){
    ci::gl::clear( ci::ColorA(0.0f,0.0f,0.0f,0.0f) );
    ci::gl::draw(_fbo->getTexture(),getWindowBounds());
    if( ii == 0 ){
        writeImage( getHomeDirectory()/"Desktop"/"logo-layer.png", _fbo->getTexture()->createSource() );
        ii = 1;
    }
}

CINDER_APP( BasicApp, RendererGl( RendererGl::Options().msaa( 16 ) ), []( App::Settings *settings ) {
    //	settings->enableHighDensityDisplay();
    settings->setWindowSize( 640, 480 );
} )