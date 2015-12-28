#pragma once

#include "OpenWeatherMap.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace ci;

namespace hrfm{ namespace api{ namespace openweathermap{
    
    namespace forecast{
        
        /*/
        <time from="2015-12-29T06:00:00" to="2015-12-29T09:00:00">
             <symbol number="801" name="few clouds" var="02n"/>
             <precipitation/>
             <windDirection deg="319.001" code="NW" name="Northwest"/>
             <windSpeed mps="1.86" name="Light breeze"/>
             <temperature unit="celsius" value="4.21" min="4.21" max="4.21"/>
             <pressure unit="hPa" value="1039.59"/>
             <humidity value="68" unit="%"/>
             <clouds value="few clouds" all="20" unit="%"/>
        </time>
        /*/
        class TimeData{
        public:
            TimeData(){};
            TimeData( XmlTree xml ){ setData(xml); };
            virtual void setData( XmlTree xml );
            virtual XmlTree getData();
            
            virtual string       getTimeFromString();
            virtual std::tm      getTimeFrom();
            virtual unsigned int getTimeFromAsUnixTime();
            
            virtual string       getTimeToString();
            virtual std::tm      getTimeTo();
            virtual unsigned int getTimeToAsUnixTime();
            
            virtual float  getWindDirectionDeg();
            virtual string getWindDirectionCode();
            virtual string getWindDirectionName();
            
            virtual float  getWindSpeed();
            virtual string getWindSpeedName();
            
            virtual float  getTemperature();
            virtual float  getTemperatureMin();
            virtual float  getTemperatureMax();
            virtual string getTemperatureUnit();
            
            virtual float  getPressure();
            virtual string getPressureUnit();
            
            virtual float  getHumidity();
            virtual string getHumidityUnit();
            
        protected:
            XmlTree _xml;
            std::tm _from;
            std::tm _to;
        };
        
    }
    
    class Forecast:public APIBase{
        
    public:
        
        Forecast():APIBase("forecast"){}
        
        virtual void parse( XmlTree xml );
        
        virtual int numTimeData();
        virtual forecast::TimeData * getTimeDataAt( int index );
        
        virtual int numDays();
        virtual vector<forecast::TimeData*> getTimeDataByDayAt(int index);
        
    protected:
        
        vector<forecast::TimeData*> _times;
        vector<vector<forecast::TimeData*>> _timesByDay;
        LocationData _location;
        
    };
    
}}}