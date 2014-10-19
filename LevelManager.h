//
//  LevelManager.h
//  MoonWarriorsx
//
//
//

#ifndef __MoonWarriorsx__LevelManager__
#define __MoonWarriorsx__LevelManager__

#include <iostream>
#include <string>
#include <vector>
#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;

class LevelManager{
private:
    vector<EnemyInfo> m_enemies;
    vector<EnemyType> m_enemyType;
    cocos2d::CCLayer *m_gameLayer;
    
public:
    LevelManager(cocos2d::CCLayer *gameLayer);
   ~LevelManager();


    void loadLevelResource(int dt);
    void addEnemyToGameLayer(int type);
    void repeatAction(CCNode *pSender);

};
#endif /* defined(__MoonWarriorsx__LevelManager__) */
