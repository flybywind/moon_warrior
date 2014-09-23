//
//  GameLayer.h
//  MoonWarriors
//
//  Created by flybywind on 14-9-23.
//
//

#ifndef MoonWarriors_GameLayer_h
#define MoonWarriors_GameLayer_h

#include "cocos2d.h"
#include "Ship.h"
#include "LevelManager.h"
USING_NS_CC;

enum GameState{
    Playing = 0,
    GameOver= 1,
};

class GameLayer : public cocos2d::Layer
{
private:

    cocos2d::Vec2 winSize;
    cocos2d::Sprite *m_backSkyRe;
    cocos2d::Sprite * m_backSky;
    
    Ship *m_ship;
    int m_state;
    int m_time;
    float m_backSkyHeight;
    float m_backTileMapHeight;
    
    CCTMXTiledMap *m_backTileMap;
    CCTMXTiledMap *m_backTileMapRe;
    CCLabelBMFont *m_lbScore;
    CCLabelTTF *m_lifeCount;
    cocos2d::Rect m_screenRec;
//    LevelManager *m_levelManager;
    
    bool m_isBackSkyReload;
    bool m_isBackTileReload;
    int m_tempScore;
    
    
public:
    GameLayer();
    ~GameLayer();
    virtual bool init();
    virtual void update(float dt);
    void scoreCounter();
    Ship* getShip();
    void checkIsCollide();
    void removeInactiveUnit(float dt);
    void checkIsReborn();
    bool collide(UnitSprite *a, UnitSprite *b);
    void updateUI();
    void gameOver();
    void doPause(CCObject *pSender);
    virtual void onEnter();
    virtual void onExit();
    void initBackground();
    void movingBackground();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    CREATE_FUNC(GameLayer);
};
#endif
