//
//  Effect.cpp
//  MoonWarriorsx
//

#include "Effect.h"
#include "Resource.h"
#include <string>
using namespace std;

Effect::Effect()
{
}

Effect::~Effect()
{
}

Effect* Effect::create()
{
    Effect* effect = new Effect();
    if (effect) {
        effect->autorelease();
        effect->sharedExplosion();
        return effect;
    }else{
        delete effect;
        effect = NULL;
        return NULL;
    }
}

void Effect::flareEffect(Node *parent, CallFunc *callback)
{
    Size winSize = Director::getInstance()->getWinSize();
    // 特效图片
    Sprite *flare = Sprite::create(s_flare);
    
    // 设置混合模式
    BlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
    flare->setBlendFunc(cbl);
    
    // 添加到父节点
    parent->addChild(flare, 10);
    
    // 设置初始透明度
    flare->setOpacity(0);
    
    // 设置初始位置
    flare->setPosition(Vec2(-30, winSize.height - 130));
    
    // 设置初始角度
    flare->setRotation(-120);
    
    // 设置初始放大系数
    flare->setScale(0.2);
    
    // 透明度渐变
    ActionInterval *opacityAnim = FadeTo::create(0.5, 255);
    ActionInterval *opacDim = FadeTo::create(1, 0);
    
    // 大小渐变
    ActionInterval *bigAnim = ScaleBy::create(0.7, 1.2, 1.2);
    
    // 渐变速度
    EaseSineOut *biggerEase = EaseSineOut::create(bigAnim);
    ActionInterval *moveAnim = MoveBy::create(0.5, Vec2(328, 0));
    EaseSineOut *moveEase = EaseSineOut::create(moveAnim);
    
    // 角度旋转
    ActionInterval *roteAnim = RotateBy::create(2.5, 90);
    
    // 角度旋转速度，指数变化
    EaseExponentialOut * rotateEase = EaseExponentialOut::create(roteAnim);
    
    // 放大到原大小
    ScaleTo *bigger = ScaleTo::create(0.5, 1);
    
    // 动画完成后回调函数,干掉精灵
    CallFuncN *removeFunc =  CallFuncN::create(CC_CALLBACK_0(Effect::killSprite, this, flare));
    
    // 执行动画和回调函数
    flare->runAction(Sequence::create(
                        opacityAnim,
                        biggerEase,
                        opacDim,
                        removeFunc, callback,  NULL));
    flare->runAction(moveEase);
    flare->runAction(rotateEase);
    flare->runAction(bigger);

}

void Effect::explode(Node *parent, Vec2 pos)
{
    // 第一帧
    SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("explosion_01.png");
    Sprite *explosion = Sprite::createWithSpriteFrame(pFrame);
    parent->addChild(explosion);
    explosion->setPosition(pos);
    
    Size cs = explosion->getContentSize();
    
    // 完成之后的回调
    CallFuncN *removeFunc =  CallFuncN::create(CC_CALLBACK_0(Effect::killSprite, this, explosion));
    
    // 爆炸动画
    Animation *animation = AnimationCache::getInstance()->getAnimation("Explosion");
    explosion->runAction(Sequence::create(Animate::create(animation), removeFunc, NULL));
    
}

// 闪光动画
void Effect::spark(Point pos, Node *parent, float scale, float duration)
{
    // spark 精灵
    Sprite *one = Sprite::create(s_explode1);
    Sprite *two = Sprite::create(s_explode2);
    Sprite *three = Sprite::create(s_explode3);
    
    // 混合模式
    ccBlendFunc cb = { GL_SRC_ALPHA, GL_ONE };
    one->setBlendFunc(cb);
    two->setBlendFunc(cb);
    three->setBlendFunc(cb);
    
    one->setPosition(pos);
    two->setPosition(pos);
    three->setPosition(pos);
    
    //这个为什么不加？加上去感觉太亮了吧
//    parent->addChild(one);
    parent->addChild(two);
    parent->addChild(three);
    
    one->setScale(scale);
    two->setScale(scale);
    three->setScale(scale);
    
    three->setRotation(CCRANDOM_0_1() * 360);
    
    RotateBy *left = RotateBy::create(duration, -45);
    RotateBy *right = RotateBy::create(duration, 45);
    ScaleBy *scaleBy = ScaleBy::create(duration, 3);
    FadeOut *fadeOut = FadeOut::create(duration);
    

//    CallFuncN *removeOne =  CallFuncN::create(this, callfuncN_selector(Effect::killSprite));
//    CallFuncN *removeTwo =  CallFuncN::create(this, callfuncN_selector(Effect::killSprite));
//    CallFuncN *removeThree =  CallFuncN::create(this, callfuncN_selector(Effect::killSprite));
    CallFuncN *removeOne =  CallFuncN::create(CC_CALLBACK_0(Effect::killSprite, this, one));
    CallFuncN *removeTwo =  CallFuncN::create(CC_CALLBACK_0(Effect::killSprite, this, two));
    CallFuncN *removeThree =  CallFuncN::create(CC_CALLBACK_0(Effect::killSprite, this, three));
    Action *seqOne = Sequence::create(fadeOut, removeOne, NULL);
    Action *seqTwo = Sequence::create(fadeOut->clone(), removeTwo, NULL);
    Action *seqThree = Sequence::create(fadeOut->clone(), removeThree, NULL);
    
    one->runAction(left);
    two->runAction(right);
    
    // 看起来在对象O执行runAction A后，A就和O建立了结合关系
    // 如果A被用于另外一个对象O2，那么A和O的关系就断了。
    // 所以必须使用clone复制一个同样的对象
    one->runAction(scaleBy);
    two->runAction(scaleBy->clone());
    three->runAction(scaleBy->clone());
    
    one->runAction(seqOne);
    two->runAction(seqTwo);
    three->runAction(seqThree);
    
    
}

//动画加入缓存
void Effect::sharedExplosion()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_explosion_plist);
    Vector<SpriteFrame*> animFrames;
    
    char str[64] = {0};
    
    for (int i = 1; i < 35; ++i) {
        sprintf(str, "explosion_%02d.png", i);
        SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.04);
    // 帧动画命名
    AnimationCache::getInstance()->addAnimation(animation, "Explosion");
    
}

void Effect::killSprite(Node *pSender)
{
    pSender->removeFromParent();
}

