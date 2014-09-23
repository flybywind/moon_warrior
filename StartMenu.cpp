#include "StartMenu.h"
#include "SimpleAudioEngine.h"
#include "Resource.h"
//#include "GameLayer.h"
USING_NS_CC;
using namespace CocosDenshion;

Scene* StartMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // 还是用驼峰命名法吧：
    Sprite *sp_load = Sprite::create(s_loading);
    Sprite* sp_logo = Sprite::create(s_logo);
    // 不设AnchorPoint，sprite默认是中点
    sp_load->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    // zorder决定了layer顺序，越大的越靠上，就会掩盖zorder小的
    addChild(sp_load, 0);
    sp_logo->setAnchorPoint(Vec2(0.5, 1));
    sp_logo->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.8));
    addChild(sp_logo, 10);
    
    Sprite * sp_new_game_crate = Sprite::create(s_menu, Rect(0, 0, 126,33));
    Sprite * sp_new_game_sel = Sprite::create(s_menu, Rect(0, 33, 126, 33));
//    Sprite * sp_new_game_disa = Sprite::create(s_menu, Rect(0, 66, 126, 33));
    
    auto new_game = MenuItemSprite::create(sp_new_game_crate, sp_new_game_sel, CC_CALLBACK_1(StartMenu::newGame, this));
    auto menu = Menu::createWithItem(new_game);
    this->addChild(menu, 1);
    menu->alignItemsVertically();

    menu->setPosition(visibleSize.width/2,
                      sp_logo->getPosition().y - sp_logo->getContentSize().height - 33);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("飞机大战", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    return true;
}


void StartMenu::newGame(Ref* pSender)
{
    // TODO: 检查音量是否打开
    // 记得勾选“create folder references for any added folder”，然后文件夹显示蓝色
    // 说明文件才正确地加入到project了。否则会提示无法找到文件
    SimpleAudioEngine::getInstance()->playEffect(s_buttonEffect);
    auto scene = Scene::create();
//    scene->addChild(GameLayer::create());
    Director::getInstance()->replaceScene(scene);
}
