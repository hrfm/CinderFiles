#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <regex>

#include "EventDispatcher.h"
#include "Singleton.h"

using namespace std;
using namespace boost;

namespace hrfm{ namespace utils{
    
    /*
     
     /Volumes を監視して USB メモリを検知する処理を行います.
     
     */
    class SiFileManager : public Singleton<SiFileManager>, public hrfm::events::EventDispatcher{
        
    public:
        
        vector<filesystem::path> getFileList( filesystem::path dir, bool ignoreDotFiles = true, bool sortByNumeric = true, filesystem::path createCopyInto = "" );
        
    private:
        
        friend class Singleton<SiFileManager>;
        SiFileManager():hrfm::events::EventDispatcher(){}
        
    };
    
}}