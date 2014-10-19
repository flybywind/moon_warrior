//
//  Ship.cpp
//  MoonWarriorsx
//
//  Created by 王 磊 on 12-12-29.
//
//

#include "Ship.h"
#include "Resource.h"
#include "Bullet.h"
#include "Config.h"
#include "Effect.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
TextureCache* Ship::g_texture_cache = Director::getInstance()->getTextureCache();
Vec2 Ship::BornPosition;
Ship::Ship():m_speed(220),
             m_bulletSpeed(900),
             m_HP(5),
             m_bulletTypeValue(1),
             m_bulletPowerValue(1),
             m_throwBombing(false),
             m_canBeAttack(true),
             m_isThrowBomb(false),
             m_zOder(3000),
             m_maxBulletPowerValue(4),
             m_appearPosition(BornPosition),
             m_hurtColorLife(0),
             m_active(true)
{
}

Ship::~Ship()
{
}

bool Ship::init()
{
    // super init first
    if ( !Sprite::init() )
    {
        return false;
    }
    
    
    // init life
    Texture2D * shipTextureCache = g_texture_cache->addImage(s_ship01);
    Rect rec = Rect(0, 0, 60, 38);
    this->initWithTexture(shipTextureCache,  rec);
    this->setPosition(m_appearPosition);

    
    // set frame
    Vector<SpriteFrame*> animFrames;
    animFrames.pushBack(
                          SpriteFrame::createWithTexture(shipTextureCache, Rect(0, 0, 60, 38))
        );
    animFrames.pushBack(
                          SpriteFrame::createWithTexture(shipTextureCache,Rect(60, 0, 60, 38))
    );
    
    // ship animate
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1);
    Animate *animate = Animate::create(animation);
    this->runAction(RepeatForever::create(animate));
    
    // 子弹发射
    this->schedule(schedule_selector(Ship::shoot), 0.16);
    
    // revive effect
    this->m_canBeAttack = false;
    Sprite *ghostSprite = Sprite::createWithTexture(shipTextureCache, Rect(0, 45, 60, 38));
    BlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
    ghostSprite->setBlendFunc(cbl);
    ghostSprite->setScale(8);
    ghostSprite->setPosition(Vec2(this->getContentSize().width / 2, 12));
    this->addChild(ghostSprite, 3000, 99999);
    ghostSprite->runAction(ScaleTo::create(1, 1, 1));
    
    // 闪烁动画
    Blink *blinks = Blink::create(3, 9);
    
    // 回调，攻击使能
    CallFunc *makeBeAttack = CallFunc::create(CC_CALLBACK_0(Ship::makeAttack, this));
    
    this->runAction(Sequence::create(DelayTime::create(0.5), blinks, makeBeAttack, NULL));
    return true;
}


void Ship::update(float dt)
{

    if (m_HP <= 0) {
        m_active = false;
    }
    
}

void Ship::makeAttack()
{
//    CCLog("pSend tag %d",pSender->getTag());
    this->m_canBeAttack = true;
    this->setVisible(true);
    this->removeChildByTag(99999);
//    CCLog("make Attack !");
    
}

void Ship::shoot(float dt)
{
    int offset = 13;
    Vec2 position = this->getPosition();
    Size contentSize = this->getContentSize();
    Bullet *bullet_a = new Bullet(m_bulletSpeed, "W1.png", 1);
    if (bullet_a) {
        bullet_a->autorelease();
        play_bullet->addObject(bullet_a);
        this->getParent()->addChild(bullet_a, bullet_a->m_zorder);
        bullet_a->setPosition(Vec2(position.x + offset,
                                   position.y + 3 + contentSize.height * 0.3));

    }else{
        delete bullet_a;
        bullet_a = 0;
    }
    
    
    Bullet *bullet_b = new Bullet(m_bulletSpeed, "W1.png", 1);
    if (bullet_b) {
        bullet_b->autorelease();
        play_bullet->addObject(bullet_b);
        this->getParent()->addChild(bullet_b, bullet_b->m_zorder);
        bullet_b->setPosition(Vec2(position.x - offset,
                                   position.y + 3 + contentSize.height * 0.3));
    }else{
        delete bullet_b;
        bullet_a = 0;
    }

}

void Ship::hurt()
{
    if (m_canBeAttack) {
        CCLog("under fire!");
        m_HP--;
        this->setColor(Color3B(255, 0, 0));
    }
    
}

void Ship::destroy()
{
    CCLOG("destroy one ship");
    Config::sharedConfig()->updateLifeCount();

    CCLOG("life count is %d",Config::sharedConfig()->getLifeCount());
    
    Effect *effect = Effect::create();    
    effect->explode(this->getParent(), this->getPosition());

    this->removeFromParent();
    if (Config::sharedConfig()->getAudioState()){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(s_shipDestroyEffect);
        
    }

}

Rect Ship::collideRect()
{
    Vec2 pos = getPosition();
    Size cs = getContentSize();
    // AnchorPoint在中心点
    return Rect(pos.x - cs.width / 2 , pos.y - cs.height / 2, cs.width, cs.height / 2);
}

bool Ship::isActive()
{
    return m_active;
}

int Ship::getZoder()
{
    return m_zOder;
}