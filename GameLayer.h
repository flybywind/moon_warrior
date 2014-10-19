//
//  GameLayer.h
//  MoonWarriorsx
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
    
    TMXTiledMap *m_backTileMap;
    TMXTiledMap *m_backTileMapRe;
    LabelBMFont *m_lbScore;
    LabelTTF *m_lifeCount;
    cocos2d::Rect m_screenRec;
    LevelManager *m_levelManager;
    
    bool m_isBackSkyReload;
    bool m_isBackTileReload;
    int m_tempScore;
    
    
public:
    GameLayer();
    ~GameLayer();
    virtual bool init();
    virtual void update(float dt);
    void scoreCounter(float);
    Ship* getShip();
    void checkIsCollide();
    void removeInactiveUnit(float dt);
    void checkIsReborn();
    bool collide(UnitSprite *a, UnitSprite *b);
    void updateUI();
    void gameOver();
    void doPause(cocos2d::Ref *pSender);
    void initBackground();
    void movingBackground(float);
    virtual bool TouchBegan(Touch* touch, Event* event);
    virtual void TouchMoved(Touch* touch, Event* event);
    CREATE_FUNC(GameLayer);
};

#endif /* defined(__MoonWarriorsx__GameLayer__) */
