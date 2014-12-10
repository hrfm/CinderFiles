#pragma once

#include "cinder/Xml.h"
#include "Singleton.h"
#include "XmlLoader.h"

using namespace ci;

namespace hrfm { namespace app{
    
    class ISettingValue{
    public:
        ISettingValue(){
            init();
        };
        ~ISettingValue(){};
        virtual void init(){}
    };
    
    template <class T> class SettingValue : public ISettingValue{
    public:
        SettingValue( T value ){
            ISettingValue();
            _value = value;
        }
        virtual T getValue(){
            return _value;
        }
    private:
        T _value;
    };
    
    class SiSetting : public hrfm::utils::Singleton<SiSetting>{
        
    public:
        
        XmlTree load( ci::fs::path settingXmlPath );
        
        XmlTree getXmlTree();
        
        bool hasValue( const string key );
        
        template <class T> void setValue( const string key, T value ){
            _values[key] = new SettingValue<T>( value );
        }
        
        template <class T> T getValue( const string key ){
            if( hasValue( key ) ){
                cout << dynamic_cast<SettingValue<T>*>( _values[key] ) << endl;
                if( SettingValue<T> * v = dynamic_cast<SettingValue<T>*>( _values[key] ) ){
                    return v->getValue();
                }
            }
            return NULL;
        }
        
    private:
        
        friend class hrfm::utils::Singleton<SiSetting>;
        
        SiSetting(){}
        
        XmlTree _xml;
        
        map<const string, ISettingValue * > _values;
        
    };
    
}}