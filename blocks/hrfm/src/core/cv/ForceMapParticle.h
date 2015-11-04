#pragma once

using namespace std;

namespace hrfm{ namespace cv{
    
    // パーティクルクラス
    class Particle {
        
        public :
        
            Particle( double beginX, double beginY ){
                x  = beginX;
                y  = beginY;
                ax = 0;
                ay = 0;
                vx = 0;
                vy = 0;
            }
            
            void update( vec2 force ){
                
                mBeforePos.set( x, y );
                
                ax += force.x;
                ay += force.y;
                
                vx += ax;
                vy += ay;
                
                x += vx;
                y += vy;
                
                if( x < 0.0 || 1.0 < x ){
                    x = randFloat();
                    mBeforePos.x = x;
                    ax = 0.0;
                    vx = 0.0;
                }else{
                    ax *= 0.4;
                    vx += ( 0.0 - vx ) * 0.002;
                }
                
                if( y < 0.0 || 1.0 < y ){
                    y = randFloat();
                    mBeforePos.y = y;
                    ay = 0.0;
                    vy = 0.0;
                }else{
                    ay *= 0.4;
                    vy += ( -0.001 - vy ) * 0.002;
                }
                
                mPos.set( x, y );
                mAccel.set( ax, ay );
                mVec.set( vx, vy );
            }
            
            vec2 getPosition(){
                return mPos;
            }
            
            vec2 getBeforePosition(){
                return mBeforePos;
            }
            
            vec2 getAccel(){
                return mAccel;
            }
        
            vec2 getVec(){
                return mVec;
            }
            
        private :
        
            vec2 mPos;
            vec2 mBeforePos;
            vec2 mAccel;
            vec2 mVec;
        
            // 位置
            double x;
            double y;
            
            // 加速度
            double ax;
            double ay;
            
            // 速度
            double vx;
            double vy;
            
    };
    
    class ParticleController {
        
        public :
        
            ParticleController(){
                setup( 1000 );
            }
            
            void setup( int numParticles ){
                while( particles.size() < numParticles ){
                    particles.push_back( new Particle( randFloat(), randFloat() ) );
                }
            }
            
            void update( ForceMap map ){
                vector<Particle*>::iterator it;
                const std::vector<Particle*>::const_iterator & end = particles.end();
                for (it = particles.begin(); it != end; ++it) {
                    vec2 force = map.getForce((*it)->getPosition()) / 3000.0f;
                    (*it)->update( force );
                }
            }
        
            void draw( Rectf bounds, float particleSize ){
                
                ivec2 size = bounds.getSize();
                
                ci::gl::pushMatrices();
                ci::gl::translate( bounds.x1, bounds.y1, 0 );
                
                vector<Particle*>::iterator it;
                const std::vector<Particle*>::const_iterator & end = particles.end();
                for (it = particles.begin(); it != end; ++it) {
                    float length = (*it)->getAccel().length();
                    if( 0.000001 < length ){
                        float alpha = length/0.00001;
                        if( alpha < 0.001 ){
                            return;
                        }else if( 0.5 < alpha ){
                            alpha = 0.2;
                        }
                        glColor4f(1.0,1.0,1.0,alpha);
                        ci::gl::drawSolidCircle( (*it)->getPosition() * vec2(size), particleSize * alpha );
                    }
                }
                
                ci::gl::popMatrices();
                
            }
            
            void drawLine( Rectf bounds ){
                
                ivec2 size = bounds.getSize();
                
                ci::gl::pushMatrices();
                ci::gl::translate( bounds.x1, bounds.y1, 0 );
                
                ci::gl::disable( GL_TEXTURE_2D );
                ci::gl::begin( GL_LINES );
                    vector<Particle*>::iterator it;
                    const std::vector<Particle*>::const_iterator & end = particles.end();
                    for (it = particles.begin(); it != end; ++it) {
                        float length = (*it)->getAccel().length();
                        if( 0.000001 < length ){
                            float alpha = length/0.00001;
                            if( alpha < 0.001 ){
                                return;
                            }else if( 1.0 < alpha ){
                                alpha = 1.0;
                            }
                            glColor4f(1.0,1.0,1.0,alpha);
                            ci::gl::vertex( (*it)->getBeforePosition()* vec2(size) );
                            ci::gl::vertex( (*it)->getPosition()* vec2(size) );
                        }
                    }
                ci::gl::end();
                ci::gl::enable( GL_TEXTURE_2D );
                
                ci::gl::popMatrices();
                
            }
        
        private :
        
            vector<Particle*> particles;
            
    };
    
    // ---
    
    class ActiveEffect{
        
        public :
        
            ActiveEffect(){}
            
            void setup( ivec2 size ){
                ci::gl::Fbo::Format format;
                motionFbo = ci::gl::Fbo( size.x, size.y, format );
            }
            
            void update( ForceMap map ){
                mParticle.update( map );
                motionFbo.bindFramebuffer();
                    ci::gl::clear();
                motionFbo.unbindFramebuffer();
            }
            
            void drawSpark( float alpha = 1.0 ){
                
                if( alpha <= 0 ){
                    return;
                }
                
                ci::gl::enableAdditiveBlending();
                motionFbo.bindFramebuffer();
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow( ci::app::getWindowSize(), false );
                    glColor4f(1.0,1.0,1.0,alpha);
                    //captureInput.drawForceMap();
                    mParticle.drawLine( ci::app::getWindowBounds() );
                    ci::gl::popMatrices();
                motionFbo.unbindFramebuffer();
                ci::gl::disableAlphaBlending();
                
            }
            
            void drawParticle( float alpha = 1.0, float particleSize = 5.0 ){
                
                if( alpha <= 0 ){
                    return;
                }
                
                ci::gl::enableAdditiveBlending();
                motionFbo.bindFramebuffer();
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow( ci::app::getWindowSize(), false );
                    glColor4f(1.0,1.0,1.0,alpha);
                    //captureInput.drawForceMap();
                    mParticle.draw( ci::app::getWindowBounds(), particleSize );
                    ci::gl::popMatrices();
                motionFbo.unbindFramebuffer();
                ci::gl::disableAlphaBlending();
                
            }
        
            void drawLines( ForceMap map, float audioAverage, float alpha ){
                
                if( alpha <= 0 ){
                    return;
                }
                
                ci::gl::enableAdditiveBlending();
                motionFbo.bindFramebuffer();
                
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow( ci::app::getWindowSize(), false );
                    
                    ci::gl::color( ColorA( 1.0f, 1.0f, 1.0f, alpha ) );
                    
                    vec2 pos    = map.getMaxForcePosition();
                    vec2 center = vec2( pos.x * ci::app::getWindowWidth(), pos.y * ci::app::getWindowHeight() );
                    vec2 force  = map.getAverageForce();
                    float length = force.length() * audioAverage;
                    
                    if( 30.0 < length ){
                        for( int i = 0; i < 20; i++ ){
                            ci::gl::lineWidth(randFloat()*3);
                            vec2 pos = center + vec2(randFloat()*1920.0-960.f, randFloat()*1000.0-500.f);
                            ci::gl::drawLine( pos, pos +  force * 300.0f );
                            ci::gl::drawLine( pos, pos + -force * 300.0f );
                        }
                    }else if( 10.0 < length ){
                        for( int i = 0; i < 40; i++ ){
                            ci::gl::lineWidth(randFloat()* 10);
                            float radian = 6.28 * randFloat();
                            vec2 pos = vec2(cos(radian), sin(radian));
                            ci::gl::drawLine( center + pos * ( 100 + 400.0 * randFloat() ), center + pos * 2000 );
                        }
                    }
                    
                    ci::gl::lineWidth(1);
                
                    ci::gl::popMatrices();
                motionFbo.unbindFramebuffer();
                ci::gl::disableAlphaBlending();
                
            }
        
            void drawKaminari( ForceMap map, float audioAverage, float alpha ){
                
                if( alpha <= 0 ){
                    return;
                }
                
                ci::gl::enableAdditiveBlending();
                motionFbo.bindFramebuffer();
                
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow( ci::app::getWindowSize(), false );
                    
                    ci::gl::color( ColorA( 1.0f, 1.0f, 1.0f, alpha ) );
                    
                    vec2 pos    = map.getMaxForcePosition();
                    vec2 center = vec2( pos.x * ci::app::getWindowWidth(), pos.y * ci::app::getWindowHeight() );
                    vec2 force  = map.getAverageForce();
                    float length = force.length() * audioAverage;
                
                    if( 10.0 < length ){
                        for( int i = 0; i < 10; i++ ){
                            ci::gl::lineWidth(randFloat()* 5);
                            ___drawKaminari___( center + vec2( randFloat(-30,30),randFloat(-30,30) ), 6.28 * randFloat() );
                        }
                    }
                    
                    ci::gl::lineWidth(1);
                    
                    ci::gl::popMatrices();
                
                motionFbo.unbindFramebuffer();
                ci::gl::disableAlphaBlending();
                
            }
        
            void ___drawKaminari___( vec2 from, float firstAngle ){
                vec2 to;
                float angle = firstAngle;
                for( int i=0; i< 70; i++ ){
                    to = from + vec2( cos(angle), sin(angle) ) * (10.0f+30.0f*randFloat());
                    ci::gl::drawLine( from, to );
                    from = to;
                    angle += 1.0 * randFloat() - 0.5;
                }
            }
        
            ci::gl::Texture getTexture(){
                return motionFbo.getTexture();
            }
        
        private :
        
            ci::gl::Fbo motionFbo;
            ParticleController mParticle;
        
    };
    
}}