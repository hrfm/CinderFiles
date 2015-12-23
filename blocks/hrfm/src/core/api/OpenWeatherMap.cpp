#include "OpenWeatherMap.h"

namespace hrfm{ namespace api{
    
    string OpenWeatherMap::APP_ID  = "";
    string OpenWeatherMap::API_URL = "http://api.openweathermap.org/data/2.5/";
    
    void OpenWeatherMap::init( string appId, string url ){
        OpenWeatherMap::APP_ID  = appId;
        OpenWeatherMap::API_URL = url;
    }
    
    namespace openweathermap{
        
        bool CurrentWeather::isNoData(){
            return _isNoData;
        }
        
        void CurrentWeather::clear(){
            _isNoData = true;
        }
            
        void CurrentWeather::fetch( string city ){
            try{
                cout << OpenWeatherMap::API_URL << OpenWeatherMap::APP_ID << endl;
                fs::path path = OpenWeatherMap::API_URL + "weather?q=" + city + "&mode=xml&appid=" + OpenWeatherMap::APP_ID;
                cout << path << endl;
                parse( hrfm::io::XmlLoader::load(path) );
            }catch(...){
                cout << "error" << endl;
                clear();
            }
        }
            
        void CurrentWeather::parse( XmlTree xml ){
            
            try{
                
                data = xml;
                
                // ----------------------------------------------------------------
                
                XmlTree current = xml.getChild("current");
                
                // ---
                
                XmlTree city = current.getChild("city");
                cout << city << endl;
                
                // ---
                
                temperature = current.getChild("temperature").getAttributeValue<double>("value");
                humidity    = current.getChild("humidity").getAttributeValue<double>("value");
                pressure    = current.getChild("pressure").getAttributeValue<double>("value");
                
                cout << (temperature-273.15) << endl;
                cout << humidity << endl;
                cout << pressure << endl;
                
                // ---
                
                XmlTree wind = current.getChild("wind");
                cout << wind << endl;
                
                // ---
                
            }catch(...){
                
                clear();
                
            }
            
        }
    
    }
    
}}