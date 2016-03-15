#pragma once

#include "DynamicMap.h"
#include "DisplayNode.h"

namespace hrfm{ namespace animation{

    class TimelineAnimator : public hrfm::events::EventDispatcher{
        
    public:
        
        TimelineAnimator(  ):hrfm::events::EventDispatcher(){
            _totalTimeScale = 0.0;
        }
        
        /**
         * 設定されているアニメーションの数かえします
         * @return int
         */
        int numAnimation();
        
        /**
         * アニメーションを追加します
         * @param timeScale アニメーションのタイムスケール.
         * @param func      実行する関数
         */
        void add( float timeScale, std::function<void(float)> func );
        
        /**
         * 追加されたアニメーションの全体の実行時間を 1.0 とし
         * timeScale で指定した位置のアニメーション関数を実行します.
         * @param timeScale
         */
        void animate( float timeScale );
        
        /**
         * idx で指定した順番に設定してあるアニメーションを timeScale で実行します.
         * @param idx
         * @param timeScale
         */
        void animateAt( int idx, float timeScale );
        
        // タイムスケールを気にせずとにかく実行する.
        //void animateAt( ind idx );
        
    protected:
        
        float _totalTimeScale;
        std::vector< std::pair< float, std::function<void(float)> > > _list;
        
    };
    
}};