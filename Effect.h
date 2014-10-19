//
//  Effect.h
//  MoonWarriorsx
//
//

#ifndef __MoonWarriorsx__Effect__
#define __MoonWarriorsx__Effect__

#include "cocos2d.h"
USING_NS_CC;

class Effect : public Node
{
    
private:
    Sprite *m_sprite;
    
public:
    Effect(void);
    ~Effect(void);
    
    // flare effect
    void flareEffect(Node* parent, CallFunc* callback);
    void explode(Node *parent, Point pos);
    void spark(Point pos, Node *parent, float scale, float duration);
    
    void killSprite(Node *pSender);

    
    
    static void sharedExplosion();
    static Effect* create();
    
};
#endif /* defined(__MoonWarriorsx__Effect__) */
