#include "OpenWeatherMap.h"

namespace hrfm{ namespace api{
    
    string OpenWeatherMap::APP_ID  = "";
    string OpenWeatherMap::API_URL = "http://api.openweathermap.org/data/2.5/";
    
    void OpenWeatherMap::init( string appId, string url ){
        OpenWeatherMap::APP_ID  = appId;
        OpenWeatherMap::API_URL = url;
    }
    
    namespace openweathermap{
        
        void   LocationData::setData( XmlTree xml ){
            _xml = xml;
            cout << "LocationData:" << endl << xml << endl;
        }
        string LocationData::getName(){
            if( _xml.hasChild("name") ){
                return _xml.getChild("name").getValue<string>();
            }else{
                return "undefined";
            }
        };
        string LocationData::getType(){
            if( _xml.hasChild("type") ){
                return _xml.getChild("type").getValue<string>();
            }else{
                return "undefined";
            }
        };
        string LocationData::getCountry(){
            if( _xml.hasChild("country") ){
                return _xml.getChild("country").getValue<string>();
            }else{
                return "undefined";
            }
        };
        string LocationData::getTimezone(){
            if( _xml.hasChild("timezone") ){
                return _xml.getChild("timezone").getValue<string>();
            }else{
                return "undefined";
            }
        };
        float  LocationData::getAltitude(){
            if( _xml.hasChild("location") ){
                return _xml.getChild("location").getAttributeValue<float>("altitude");
            }else{
                return 0.0f;
            }
        };
        float  LocationData::getLatitude(){
            if( _xml.hasChild("location") ){
                return _xml.getChild("location").getAttributeValue<float>("latitude");
            }else{
                return 0.0f;
            }
        };
        float  LocationData::getLongitude(){
            if( _xml.hasChild("location") ){
                return _xml.getChild("location").getAttributeValue<float>("longitude");
            }else{
                return 0.0f;
            }
        };
        string LocationData::getGeobase(){
            if( _xml.hasChild("location") ){
                return _xml.getChild("location").getAttributeValue<string>("geobase");
            }else{
                return "undefined";
            }
        };
        int    LocationData::getGeobaseId(){
            if( _xml.hasChild("location") ){
                return _xml.getChild("location").getAttributeValue<int>("geobaseid");
            }else{
                return 0;
            }
        };
        
        // ----------
        // =====================================================================================
        // ----------
        
        // ----------
        // =====================================================================================
        // ----------
        
        bool APIBase::isFetched(){ return _isFetched; }
        void APIBase::clear(){ _isFetched = false; }
        void APIBase::fetchAsXml( string query ){
            try{
                cout << OpenWeatherMap::API_URL << OpenWeatherMap::APP_ID << endl;
                fs::path path = OpenWeatherMap::API_URL + _type + "?q=" + query + "&mode=xml&appid=" + OpenWeatherMap::APP_ID;
                cout << path << endl;
                parse( hrfm::io::XmlLoader::load(path) );
            }catch(...){
                cout << "error" << endl;
                clear();
            }
        }
        void APIBase::parse( XmlTree xml ){
            data = xml;
        };
        
        // ----------
        // =====================================================================================
        // ----------
        
        void CurrentWeather::parse( XmlTree xml ){
            
            APIBase::parse(xml);
            
            cout << xml << endl;
            
            XmlTree current = xml.getChild("current");
            
            // ---
            
            XmlTree city = current.getChild("city");
            //cout << city << endl;
            
            // ---
            
            temperature = current.getChild("temperature").getAttributeValue<double>("value");
            humidity    = current.getChild("humidity").getAttributeValue<double>("value");
            pressure    = current.getChild("pressure").getAttributeValue<double>("value");
            
            //cout << (temperature-273.15) << endl;
            //cout << humidity << endl;
            //cout << pressure << endl;
            
            // ---
            
            XmlTree wind = current.getChild("wind");
            //cout << wind << endl;
            
        }
        
    }
    
}}