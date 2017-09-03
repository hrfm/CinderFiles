#include "Forecast.h"

namespace hrfm{ namespace api{ namespace openweathermap{
    
    namespace forecast{
        
        void   TimeData::setData( XmlTree xml ){
            
            _xml = xml;
            
            cout << "TimeData:" << endl << _xml << endl;
            
            // ---
            
            string from = getTimeFromString();
            std::tm tmFrom;
            tmFrom.tm_year  = stoi(from.substr( 0,4))-1900;
            tmFrom.tm_mon   = stoi(from.substr( 5,2))-1;
            tmFrom.tm_mday  = stoi(from.substr( 8,2));
            tmFrom.tm_hour  = stoi(from.substr(11,2));
            tmFrom.tm_min   = stoi(from.substr(14,2));
            tmFrom.tm_sec   = stoi(from.substr(17,2));
            tmFrom.tm_isdst = -1;
            _from = tmFrom;
            
            // ---
            
            string to = getTimeToString();
            std::tm tmTo;
            tmTo.tm_year  = stoi(to.substr(0,4))-1900;
            tmTo.tm_mon   = stoi(to.substr( 5,2))-1;
            tmTo.tm_mday  = stoi(to.substr( 8,2));
            tmTo.tm_hour  = stoi(to.substr(11,2));
            tmTo.tm_min   = stoi(to.substr(14,2));
            tmTo.tm_sec   = stoi(to.substr(17,2));
            tmTo.tm_isdst = -1;
            _to = tmTo;
            
            // ---
            
        };
        XmlTree TimeData::getData(){
            return _xml;
        }
        string TimeData::getTimeFromString(){
            return _xml.getAttributeValue<string>("from");
        };
        std::tm TimeData::getTimeFrom(){
            return _from;
        };
        unsigned int TimeData::getTimeFromAsUnixTime(){
            return mktime(&_from);
        };
        string TimeData::getTimeToString(){
            return _xml.getAttributeValue<string>("to");
        };
        std::tm TimeData::getTimeTo(){
            return _to;
        };
        unsigned int TimeData::getTimeToAsUnixTime(){
            return mktime(&_to);
        };
        float  TimeData::getWindDirectionDeg(){
            return _xml.getChild("windDirection").getAttributeValue<float>("deg");
        };
        string TimeData::getWindDirectionCode(){
            return _xml.getChild("windDirection").getAttributeValue<string>("code");
        };
        string TimeData::getWindDirectionName(){
            return _xml.getChild("windDirection").getAttributeValue<string>("name");
        };
        float  TimeData::getWindSpeed(){
            return _xml.getChild("windSpeed").getAttributeValue<float>("mps");
        };
        string TimeData::getWindSpeedName(){
            return _xml.getChild("windSpeed").getAttributeValue<string>("name");
        };
        float  TimeData::getTemperature(){
            return _xml.getChild("temperature").getAttributeValue<float>("value");
        };
        float  TimeData::getTemperatureMin(){
            return _xml.getChild("temperature").getAttributeValue<float>("min");
        };
        float  TimeData::getTemperatureMax(){
            return _xml.getChild("temperature").getAttributeValue<float>("max");
        };
        string TimeData::getTemperatureUnit(){
            return _xml.getChild("temperature").getAttributeValue<string>("unit");
        };
        float  TimeData::getPressure(){
            return _xml.getChild("pressure").getAttributeValue<float>("value");
        };
        string TimeData::getPressureUnit(){
            return _xml.getChild("pressure").getAttributeValue<string>("unit");
        };
        float  TimeData::getHumidity(){
            return _xml.getChild("humidity").getAttributeValue<float>("value");
        };
        string TimeData::getHumidityUnit(){
            return _xml.getChild("humidity").getAttributeValue<string>("unit");
        };
        
    }
    
    int Forecast::numTimeData(){
        return _times.size();
    }
    
    forecast::TimeData * Forecast::getTimeDataAt(int index){
        return _times.at(index);
    }
    
    int Forecast::numDays(){
        return _timesByDay.size();
    }
    
    vector<forecast::TimeData*> Forecast::getTimeDataByDayAt(int index){
        return _timesByDay.at(index);
    }
    
    void Forecast::parse( XmlTree xml ){
        
        APIBase::parse(xml);
        
        XmlTree weatherdata = xml.getChild("weatherdata");
        
        cout << weatherdata << endl;
        
        if( weatherdata.hasChild("location") ){
            _location.setData(weatherdata.getChild("location"));
        }
        
        if( weatherdata.hasChild("forecast") ){
            XmlTree times = weatherdata.getChild("forecast");
            int day = -1;
            for( XmlTree::Iter item = times.begin(); item != times.end(); ++item ){
                forecast::TimeData * timeData = new forecast::TimeData(*item);
                if( day != timeData->getTimeFrom().tm_mday ){
                    day = timeData->getTimeFrom().tm_mday;
                    _timesByDay.push_back(vector<forecast::TimeData*>());
                }
                _timesByDay.at(_timesByDay.size()-1).push_back(timeData);
                _times.push_back(timeData);
            }
        }
        
    }
    
}}}