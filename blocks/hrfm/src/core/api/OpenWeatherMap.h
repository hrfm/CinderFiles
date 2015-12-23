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
        
        class CurrentWeather{
            
        public:
            
            CurrentWeather(){
                _isNoData = true;
            }
            
            bool isNoData();
            void clear();
            virtual void fetch( string city );
            virtual void parse( XmlTree xml );
            
            XmlTree data;
            
            double temperature;;
            double humidity;
            double pressure;
            
        private:
            
            bool _isNoData;
            
        };
        
    }
    
}}