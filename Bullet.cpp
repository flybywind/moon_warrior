//
//  Bullet.cpp
//  MoonWarriorsx
//
//
//

#include "Bullet.h"
#include "Resource.h"
#include "Config.h"


Bullet::Bullet():m_active(true),m_velocityx(0),m_velocityy(200),m_power(1),m_Hp(1),m_zorder(3000)
{
    
}
/**子弹构造器，可以用create重写下，懒的写了*/
Bullet::Bullet(int speed, const char *weapon, int attactMode)
{
    m_active = true;
    m_velocityx = 0;
    m_power = 1;
    m_Hp = 1;
    m_zorder = 3000;
    
    m_velocityy = -speed;
    m_attackMode = attactMode;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_bullet_plist);
    this->initWithSpriteFrameName(weapon);
    BlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
    this->setBlendFunc(cbl);
    setTag(Bullet::Tag);
}

void Bullet::update(float dt)
{
    Vec2 position = this->getPosition();
    position.x -= m_velocityx * dt;
    position.y -= m_velocityy * dt;
    setPosition(position);
    if (m_Hp <= 0) {
        m_active = false;
    }
}
bool Bullet::init()
{
    return true;
}

void Bullet::hurt()
{
    m_Hp--;
}

void Bullet::destroy()
{
    
    // 子弹爆炸特效
    Sprite *explode = Sprite::create(s_hit);
    BlendFunc cb = {GL_SRC_ALPHA, GL_ONE };
    explode->setBlendFunc(cb);
    explode->setPosition(this->getPosition());
    explode->setRotation(CCRANDOM_0_1() * 360);
    explode->setScale(0.75);
    getParent()->addChild(explode, 9999);

    play_bullet->removeObject(this);
    enemy_bullet->removeObject(this);
    this->removeFromParent();
    
    CallFunc *removeExplode =  CallFunc::create(CC_CALLBACK_0(
                                    Bullet::removeExplode, this, explode));
    explode->runAction(ScaleBy::create(0.3, 2, 2));
    explode->runAction(Sequence::create(FadeOut::create(0.3), removeExplode, NULL));
    
}

void Bullet::removeExplode(Node *pSender)
{
    pSender->removeFromParent();
}
bool Bullet::isActive()
{
    return m_active;
}

Rect Bullet::collideRect()
{
    Vec2 pos = this->getPosition();
    return Rect(pos.x - 3, pos.y - 3, 6, 6);
}