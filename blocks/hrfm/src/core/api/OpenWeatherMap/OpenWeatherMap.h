#pragma once

#include "XmlLoader.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace api{
    
    class OpenWeatherMap{
    public:
        static string APP_ID;
        static string API_URL;
        static void init( string appId, string url = "http://api.openweathermap.org/data/2.5/" );
    };
    
    namespace openweathermap{
        
        class LocationData{
        public:
            LocationData(){}
            LocationData( XmlTree xml ){ setData(xml); }
            void   setData( XmlTree );
            string getName();
            string getType();
            string getCountry();
            string getTimezone();
            float  getAltitude();
            float  getLatitude();
            float  getLongitude();
            string getGeobase();
            int    getGeobaseId();
        protected:
            XmlTree _xml;
        };
        
        class APIBase{
        public:
            APIBase( string type = "weather" ){
                _type = type;
                _isFetched = false;
            };
            virtual bool isFetched();
            virtual void clear();
            virtual void fetchAsXml( string query );
            virtual void parse( XmlTree xml );
            XmlTree data;
        protected:
            string _type;
            bool   _isFetched;
        };
        
        
        class CurrentWeather:public APIBase{
        public:
            CurrentWeather():APIBase(){}
            virtual void parse( XmlTree xml );
            double temperature;;
            double humidity;
            double pressure;
        };
        
    }
    
}}