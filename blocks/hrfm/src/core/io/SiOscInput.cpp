#include "SiOscInput.h"
#include "cinder/Log.h"
#include "Osc.h"

using namespace hrfm::events;

namespace hrfm { namespace io{
    
    void SiOscInput::addListenPort( int port ){
        if( ! listening(port) ){
            cout << "bind:" << port << endl;
            ci::osc::ReceiverUdp * listener = new ci::osc::ReceiverUdp(port);
            try {
                // Bind the receiver to the endpoint. This function may throw.
                listener->bind();
            }
            catch( const ci::osc::Exception &ex ) {
                CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
                return;
            }
            listener->listen(
            []( asio::error_code error, asio::ip::udp::endpoint endpoint ) -> bool {
                if( error ) {
                    CI_LOG_E( "Error Listening: " << error.message() << " val: " << error.value() << " endpoint: " << endpoint );
                    return false;
                }else{
                    return true;
                }
            });
            _listenerMap[port] = listener;
        }
    };
    
    bool SiOscInput::listening(){
        return 0 < _listenerMap.size();
    }
    
    bool SiOscInput::listening( int port ){
        return _listenerMap.find( port ) != _listenerMap.end();
    }
    
    void SiOscInput::setListener( const std::string &address, ci::osc::ReceiverUdp::ListenerFn listener ){
        for( auto it = _listenerMap.begin(); it != _listenerMap.end(); ++it ){
            it->second->setListener( address, listener );
        }
    }
    
}}