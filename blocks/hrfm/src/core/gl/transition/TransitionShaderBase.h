#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Cinder/gl/GlslProg.h"
#include "cinder/Xml.h"
#include "SiFboFactory.h"
#include "ShaderFactory.h"
#include "Utils.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace hrfm::utils;

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    class TransitionShaderBase{
    
    public:
        
        TransitionShaderBase(){
            setSize( 640, 480 );
        };
        
        TransitionShaderBase( DataSourceRef fragment, DataSourceRef vertex = DataLoader::load("simple_vert.glsl") ){
            TransitionShaderBase();
            init( fragment, vertex );
        };
        
        // 最も基本的な初期化を行います.
        virtual void init( DataSourceRef fragment, DataSourceRef vertex = DataLoader::load("simple_vert.glsl") ){
            mShader = ShaderFactory::create( vertex, fragment );
        }
        
        virtual void setSize( int width, int height ){
            setSize( Vec2f( width, height ) );
        }
        
        virtual void setSize( Vec2f size ){
            mSize   = size;
            mAspect = getAspectRatio( size );
        }
        
        virtual void bindShader(){
            mShader.bind();
        }
        
        virtual void bindTexture( const ci::gl::Texture & current, const ci::gl::Texture & next ){
            
            current.bind(0);
            _bindedTex.push_back(current);
            mShader.uniform( "currentTex", 0 );
            
            next.bind(1);
            _bindedTex.push_back( next );
            mShader.uniform( "nextTex", 1 );
            
        }
        
        virtual void addBindingTexture( const string uniform_key, const ci::gl::Texture & tex ){
            
            int index = _bindedTex.size();
            if( index < 2 ){
                index = 2;
            }
            
            tex.bind( index );
            _bindedTex.push_back( tex );
            mShader.uniform( uniform_key, index );
            
        }
        
        virtual void affect( const ci::gl::Texture & current, const ci::gl::Texture & next, float progress ){
            bindTexture( current, next );
            affect( progress );
        };
        
        virtual void affect( float progress ){
            if( _bindedTex.size() < 2 ){
                throw;
            }
            mShader.uniform( "resolution", mSize );
            mShader.uniform( "progress"  , progress );
            ci::gl::drawSolidRect( Rectf( 0, 0, mSize.x, mSize.y ) );
        };
        
        virtual void unbindShader(){
            
            if( 0 < _bindedTex.size() ){
                std::vector<ci::gl::Texture>::iterator it, end;
                for( it = _bindedTex.begin(), end = _bindedTex.end(); it != end; ++it ){
                    (*it).unbind();
                };
            }
            _bindedTex.clear();
            
            mShader.unbind();
            
        }
        
    protected:
        
        ci::gl::GlslProg mShader;
        Vec2f mSize;
        Vec2i mAspect;
        std::vector<ci::gl::Texture> _bindedTex;
        
    private:
        
    };
    
}}