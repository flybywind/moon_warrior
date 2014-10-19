//
//  LevelManager.cpp
//  MoonWarriorsx
//
//
//

#include "LevelManager.h"
#include "Enemy.h"
#include "GameLayer.h"

using namespace cocos2d;

LevelManager::LevelManager(Layer *gameLayer)
{
    CCAssert(gameLayer != NULL, "game layer must be non-null");
    m_gameLayer = gameLayer;
    m_enemies = Config::sharedConfig()->getEnemise();
    m_enemyType = Config::sharedConfig()->getEnemyType();

}
LevelManager::~LevelManager()
{
    
}



void LevelManager::loadLevelResource(int dt)
{
    for (auto iter = m_enemies.begin();
         iter != m_enemies.end(); ++iter) {
        if (iter->showType == "Repeate") {
            if (dt % iter->showTime == 0) {
                // 数组长度
                for (int i = 0; i != 3; ++i) {
                    addEnemyToGameLayer(iter->types[i]);
                }
            }
        }
    }
}



void LevelManager::addEnemyToGameLayer(int type)
{
    Size winSize = Director::getInstance()->getWinSize();
    EnemyType enemyType = m_enemyType[type];
    Enemy * enemy = Enemy::create(enemyType);
    
    Vec2 enemyPos = Vec2( 80 + (winSize.width - 160) * CCRANDOM_0_1(), winSize.height);
    Size eSize = enemy->getContentSize();
    enemy->setPosition(enemyPos);
    
    Vec2 offset; //= ((GameLayer*)m_gameLayer)->getShip()->getPosition();
    Action *tempAction; //= CCMoveTo::create(1, offset);
    MoveBy *a0;
    MoveBy *a1;
    CallFuncN *onComplete;
    switch (enemy->getMoveType()) {
        case 0:
            //  直接冲着你过去
            if (((GameLayer*)m_gameLayer)->getShip()) {
                offset = ((GameLayer*)m_gameLayer)->getShip()->getPosition();
            }else{
                offset = Vec2(winSize.width / 2, 0);
            }
            tempAction = MoveTo::create(1, offset);
            break;
            
        case 1:
            offset = Vec2(0, -winSize.height - eSize.height);
            tempAction = MoveBy::create(4, offset);
            break;
            
        case 2:
            offset = Vec2(0, -100 - 200 * CCRANDOM_0_1());
            a0 = MoveBy::create(0.5, offset);
            a1 = MoveBy::create(1, Vec2(-50 - 100 * CCRANDOM_0_1(), 0));
            
            onComplete = CallFuncN::create(CC_CALLBACK_0(LevelManager::repeatAction, this, enemy));
            tempAction = Sequence::create(a0, a1, onComplete, NULL);
            break;
            
        case 3:
            int newX = (enemyPos.x <= winSize.width / 2) ?
                        (winSize.width - enemyPos.x) : -enemyPos.x;
            a0 = MoveBy::create(4, Vec2(newX, -240));
            a1 = MoveBy::create(4, Vec2(-newX, -320));
            tempAction = Sequence::create(a0, a1, NULL);
            break;
    
    }
    
    m_gameLayer->addChild(enemy,enemy->getZOrder());
    enemy_items->addObject(enemy);
    enemy->runAction(tempAction);
}

void LevelManager::repeatAction(Node *pSender)
{
    DelayTime *delay =  DelayTime::create(1);
    MoveBy *mv = MoveBy::create(1, Vec2(100 + 100 * CCRANDOM_0_1(), 0));
    FiniteTimeAction *seq = Sequence::create(delay, mv, delay->clone(), mv->reverse(), NULL);
    pSender->runAction(RepeatForever::create((ActionInterval*)seq));
}
