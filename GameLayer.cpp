//
//  GameLayer.cpp
//  MoonWarriorsx
//

#include "GameLayer.h"
#include "SimpleAudioEngine.h"
//#include "Bullet.h"
#include "Resource.h"
#include "Config.h"
//#include "Enemy.h"
//#include "Effect.h"
//#include "GameOver.h"
//#include "PauseLayer.h"

USING_NS_CC;
using namespace CocosDenshion;


bool isPaused = false;

GameLayer::GameLayer():
    m_state(Playing),
    m_time(0),
    m_ship(NULL),
    m_backSky(NULL),
    m_backSkyHeight(0),
    m_backSkyRe(NULL),
    m_backTileMap(NULL),
    m_backTileMapHeight(0),
    m_backTileMapRe(NULL),
    m_isBackSkyReload(false),
    m_isBackTileReload(false),
    m_lbScore(NULL),
    m_lifeCount(NULL),
    m_tempScore(0)
{
    
}
GameLayer::~GameLayer()
{
//    if (m_levelManager) {
//        delete m_levelManager;
//    }
    
//    play_bullet->release();
//    enemy_bullet->release();
//    enemy_items->release();
}

bool GameLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    // 开启触摸
    this->setTouchEnabled(true);
    
    // 创建数组，需要retain一下
    play_bullet = Array::create();
    play_bullet->retain();
    
    enemy_bullet = Array::create();
    enemy_bullet->retain();
    
    enemy_items = Array::create();
    enemy_items->retain();
    
    m_state = Playing;
    
//    Enemy::sharedEnemy();
//    Effect::sharedExplosion();
    
//    Config::sharedConfig()->resetConfig();
    
    winSize = Director::getInstance()->getWinSize();
//    m_levelManager = new LevelManager(this);
    
    //初始化背景
    initBackground();

    m_screenRec = Rect(0, 0,  winSize.x, winSize.y + 10);
    
    // score
    m_lbScore = LabelBMFont::create("Score:0", s_arial14_fnt);
    m_lbScore->setAnchorPoint(Vec2(1, 0));
    m_lbScore->setAlignment(TextHAlignment::RIGHT);
    addChild(m_lbScore, 1000);
    m_lbScore->setPosition(winSize.x - 5, winSize.y - 30);
    
    // ship life
    Texture2D *shipTexture = TextureCache::getInstance()->addImage(s_ship01);
    Sprite *life = Sprite::createWithTexture(shipTexture, Rect(0, 0, 60, 38));
    life->setScale(0.6);
    life->setPosition(Vec2(30,winSize.y-23));
    addChild(life, 1, 5);
    
    // ship life count
    char lifecount[2];
    sprintf(lifecount, "%d",Config::sharedConfig()->getLifeCount());
    m_lifeCount = CCLabelTTF::create(lifecount, "Arial", 20);
    m_lifeCount->setPosition(Vec2(60, winSize.y -20));
    m_lifeCount->setColor(ccc3(255,0, 0));
    addChild(m_lifeCount, 1000);
    
    // ship
    Ship::BornPosition = Vec2(winSize.x/2, winSize.y/2);
    m_ship = Ship::create();
    
    addChild(m_ship, m_ship->getZoder(), 1001);
    
    MenuItemImage *pause = MenuItemImage::create("pause.png", "pause.png",
                                                 CC_CALLBACK_1(GameLayer::doPause, this));
    pause->setAnchorPoint(Vec2(1, 0));
    pause->setPosition(Vec2(winSize.x, 0));
    Menu *menu = Menu::create(pause, NULL);
    menu->setAnchorPoint(Vec2(0, 0));
    addChild(menu, 1, 10);
    menu->setPosition(CCPointZero);
    
    // 调 update函数
    scheduleUpdate();
    
    // 每秒调一次 scoreCounter函数
    //schedule(schedule_selector(GameLayer::scoreCounter), 1);
    
    if (Config::sharedConfig()->getAudioState()) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(s_bgMusic, true);
    }
    // 单点触控监听：
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchMoved = CC_CALLBACK_2(GameLayer::TouchMoved, this);
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::TouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}



void GameLayer::update(float dt)
{
    if (m_state == Playing) {
        checkIsCollide();
        removeInactiveUnit(dt);
        checkIsReborn();
        updateUI();
    }
    
}

//void GameLayer::scoreCounter()
//{
//    if (m_state == Playing) {
//        m_time++;
//        m_levelManager->loadLevelResource(m_time);
//    }
//}

void GameLayer::checkIsCollide()
{
    Ref *units;
    Ref *bullets;
    Ref *enemybs;
    CCARRAY_FOREACH(enemy_items, units)
    {
        UnitSprite *enemy = dynamic_cast<UnitSprite*>(units);
        CCARRAY_FOREACH(play_bullet, bullets)
        {
            UnitSprite *bullet = dynamic_cast<UnitSprite*>(bullets);
            if (this->collide(enemy, bullet)) {
                enemy->hurt();
                bullet->hurt();
            }
            if (!(m_screenRec.intersectsRect(bullet->boundingBox()))) {
                bullet->destroy();
            }
        }
        if (collide(enemy, m_ship)) {
            if (m_ship->isActive()) {
                enemy->hurt();
                m_ship->hurt();
            }
            
        }
        if (!(m_screenRec.intersectsRect(enemy->boundingBox()))) {
            enemy->destroy();
        }
    }
    
    CCARRAY_FOREACH(enemy_bullet, enemybs)
    {
        UnitSprite *enemyb = dynamic_cast<UnitSprite*>(enemybs);
        if (enemyb) {
            if (collide(enemyb, m_ship)) {
                if (m_ship->isActive()) {
                    enemyb->hurt();
                    m_ship->hurt();
                }
            }
            if (!m_screenRec.intersectsRect(enemyb->boundingBox())) {
                enemyb->destroy();
            }
        }
        
    }
}

void GameLayer::removeInactiveUnit(float dt)
{
        
    auto children = this->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        Sprite *selChild =  dynamic_cast<Sprite*>(children.at(i));
        if (selChild) {
            selChild->update(dt);
            int tag = selChild->getTag();
            if (( tag == 900) || (tag == 901 )|| (tag == 1000)) {
                if (!((UnitSprite*)selChild)->isActive()) {
                    ((UnitSprite*)selChild)->destroy();
                }
            }
        }
    }
    
    if (m_ship) {
        if (!m_ship->isActive()) {
            m_ship->destroy();
            m_ship = NULL;
        }
    }
    
}

void GameLayer::checkIsReborn()
{    
    if (Config::sharedConfig()->getLifeCount() > 0) {
        if (!m_ship) {
            m_ship = Ship::create();
            this->addChild(m_ship, m_ship->getZoder(), 1001);
        }
        
    } else if (Config::sharedConfig()->getLifeCount() <= 0 ) {
        m_state = GameOver;
        m_ship = NULL;
        //CallFunc *gameOver = CallFunc::create(this, callfunc_selector(GameLayer::gameOver));
        //this->runAction(CCSequence::create(CCDelayTime::create(0.2), gameOver, NULL));
    }
    
}

bool GameLayer::collide(UnitSprite *a, UnitSprite *b)
{
    if(!a || !b)
    {
        return false;
    }
    Rect aRect = a->collideRect();
    Rect bRect = b->collideRect();
    if (aRect.intersectsRect(bRect)) {
        return true;
    }
    return false;
}

void GameLayer::updateUI()
{
    if (m_tempScore < Config::sharedConfig()->getScoreValue()) {
        m_tempScore += 5;
    }
    char score[20];
    char s[] = "Score:";
    sprintf(score, "%d", m_tempScore);
    m_lbScore->setString(strcat(s, score));
    char lifecount[2];
    sprintf(lifecount, "%d",Config::sharedConfig()->getLifeCount());
    m_lifeCount->setString(lifecount);
    
}

//void GameLayer::onEnter()
//{
//    CCDirector* pDirector = CCDirector::sharedDirector();
//    pDirector->addTargetedDelegate(this, 0, true);
//    Layer::onEnter();
//}
//
//void GameLayer::onExit()
//{
////    CCDirector* pDirector = CCDirector::sharedDirector();
////    pDirector->getTouchDispatcher()->removeDelegate(this);
//    Layer::onExit();
//}
//
//
bool GameLayer::TouchBegan(Touch *touch, Event *event)
{
    log("touch began!");
    return true;
}

void GameLayer::TouchMoved(Touch *touch, Event *event)
{
    if ((m_state == Playing) && m_ship) {
        Vec2 pos = touch->getDelta();
        Vec2 currentPos = m_ship->getPosition();
        currentPos = currentPos + pos;
        currentPos = currentPos.getClampPoint(Vec2::ZERO, Vec2(winSize.x, winSize.y));
        m_ship->setPosition(currentPos);
    }
    
}

// 无限滚动地图，采用两张图循环加载滚动
void GameLayer::initBackground()
{
    m_backSky = Sprite::create(s_bg01);
    m_backSky->setAnchorPoint(Vec2(0, 0));
    m_backSkyHeight = m_backSky->getContentSize().height;
    addChild(m_backSky, -10);
    
    // Tile map
    m_backTileMap = TMXTiledMap::create(s_level01);
    addChild(m_backTileMap, -9);
    m_backTileMapHeight = m_backTileMap->getMapSize().height * m_backTileMap->getTileSize().height;
    
    
    
    m_backSkyHeight -= 48;
    m_backTileMapHeight -= 200;
    m_backSky->runAction(CCMoveBy::create(3, Vec2(0, -48)));
    m_backTileMap->runAction(CCMoveBy::create(3, Vec2(0, -200)));
    
    schedule(schedule_selector(GameLayer::movingBackground),3);
    
}

// 这里就是视差背景了
void GameLayer::movingBackground(float)
{
    m_backSky->runAction(CCMoveBy::create(3, Vec2(0, -48)));
    m_backTileMap->runAction(CCMoveBy::create(3, Vec2(0, -200)));
    
    // 每次移动48
    m_backSkyHeight -= 48;
    
    // 每次移动200
    m_backTileMapHeight -= 200;
    
    // m_backSkyHeight表示back图片最上缘的位置，开始等于576，initBackground里面move一次
    // 变成528，此处又move一次，正好变成480，等于图像高度。此时如果不加入第二张图片，上部将出现
    // 黑色背景，变得很难看
    // 图的顶部到达屏幕顶部时
    if (m_backSkyHeight <= winSize.y - 50) {
        if (!m_isBackSkyReload) {
            
            // 如果另一张图还没加载则create一个
            m_backSkyRe = Sprite::create(s_bg01);
            m_backSkyRe->setAnchorPoint(Vec2(0, 0));
            addChild(m_backSkyRe, -10);
            m_backSkyRe->setPosition(Vec2(0, winSize.y));
            
            // 加载标志位
            m_isBackSkyReload = true;
        }
        // 第二张图紧接着第一张图滚动
        m_backSkyRe->runAction(CCMoveBy::create(3, Vec2(0, -48)));
    }
    
    // 第一张图完全经过屏幕
    if (m_backSkyHeight <= 0) {
        m_backSkyHeight = m_backSky->getContentSize().height;
        // 移除第一张的精灵
        this->removeChild(m_backSky, true);
        
        // 指向第二张图的精灵
        m_backSky = m_backSkyRe;
        
        // 第二张的精灵指针置空
        m_backSkyRe = NULL;
        
        // 反转标志位
        m_isBackSkyReload = false;
    }
    
    if (m_backTileMapHeight <= winSize.y) {
        if (!m_isBackTileReload) {
            m_backTileMapRe = TMXTiledMap::create(s_level01);
            this->addChild(m_backTileMapRe, -9);
            m_backTileMapRe->setPosition(0, winSize.y);
            m_isBackTileReload = true;
        }
        m_backTileMapRe->runAction(CCMoveBy::create(3, Vec2(0, -200)));
    }
    
    if (m_backTileMapHeight <= 0) {
        m_backTileMapHeight = m_backTileMap->getMapSize().height * m_backTileMap->getTileSize().height;
        this->removeChild(m_backTileMap, true);
        m_backTileMap = m_backTileMapRe;
        m_backTileMapRe = NULL;
        m_isBackTileReload = false;
    }
}

void GameLayer::gameOver()
{
//    auto scene = GameOver::scene();
//    Director::getInstance()->replaceScene(TransitionFade::create(1.2, scene));
}

void GameLayer::doPause(Ref *pSender)
{
    Director::getInstance()->pause();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
//    PauseLayer *pauseLayer = PauseLayer::create();
//    addChild(pauseLayer,9999);
}

/*Ship* GameLayer::getShip()
{
    return m_ship;
}*/