#ifndef __STARTMENU_SCENE_H__
#define __STARTMENU_SCENE_H__

#include "cocos2d.h"

class StartMenu : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    void newGame(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(StartMenu);
    
    void update(float dt);
private:
    cocos2d::Sprite* spShip;
    cocos2d::Size visibleSize;
};

#endif // __STARTMENU_SCENE_H__
