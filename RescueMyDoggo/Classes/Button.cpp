#include "Button.h"

using namespace cocos2d;

bool Button::initWithRect(Rect rect)
{
    bounds = Rect(0, 0, rect.size.width, rect.size.height);
    center = Point(rect.size.width/2, rect.size.height/2);
    status = 1; //defaults to enabled
    active = false;
    value = 0;
    isHoldable = 0;
    isToggleable = 0;
    radius = 32.0f;
    rateLimit = 1.0f/120.0f;
    
    setPosition(rect.origin); //not sure about this
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = Button::onTouchBegan;
    listener->onTouchMoved = Button::onTouchMoved;
    listener->onTouchEnded = Button::onTouchEnded;
    listener->onTouchCancelled = Button::onTouchCancelled;
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
	return true;
}

void Button::limiter(float delta)
{
	value = 0;
	this->unschedule(schedule_selector(Button::limiter));
	active = false;
}

void Button::setRadius(float r)
{
	radius = r;
	radiusSq = r*r;
}

bool Button::onTouchBegan(Touch *touch, Event *event)
{
    auto target = static_cast<Button*>(event->getCurrentTarget());

	if (target->getIsActive())
    {
        return false;
    }
	
    Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
	location = target->convertToNodeSpace(location);
		//Do a fast rect check before doing a circle hit check:
	if(location.x < -target->getRadius() || location.x > target->getRadius() || location.y < -target->getRadius() || location.y > target->getRadius())
    {
		return false;
	}
    else
    {
		float dSq = location.x*location.x + location.y*location.y;
		if(target->getRadiusSq() > dSq){
			target->setIsActive(true);
			if (!target->getIsHoldable() && !target->getIsToggleable())
            {
				target->setValue(1);
				target->schedule(schedule_selector(Button::limiter), target->getRateLimit());
			}
			if (target->getIsHoldable())
            {
                target->setValue(1);
            }
			if (target->getIsToggleable())
            {
                target->setValue(!target->getValue());
            }
			return true;
		}
	}
return false;
}

void Button::onTouchMoved(Touch *touch, Event *event)
{
    auto target = static_cast<Button*>(event->getCurrentTarget());

	if (!target->getIsActive())
    {
        return;
    }
    
	Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
	location = target->convertToNodeSpace(location);
		//Do a fast rect check before doing a circle hit check:
	if(location.x < -target->getRadius() || location.x > target->getRadius() || location.y < -target->getRadius() || location.y > target->getRadius())
    {
		return;
	}
    else
    {
		float dSq = location.x*location.x + location.y*location.y;
		if(target->getRadiusSq() > dSq)
        {
			if (target->getIsHoldable())
            {
                target->setValue(1);
            }
		}
		else
        {
			if (target->getIsHoldable())
            {
                target->setValue(0);
                target->setIsActive(false);
            }
		}
	}
}

void Button::onTouchEnded(Touch *touch, Event *event)
{
    auto target = static_cast<Button*>(event->getCurrentTarget());

	if (!target->getIsActive())
    {
        return;
    }
	if (target->getIsHoldable())
    {
        target->setValue(0);
    }
	if (target->getIsHoldable()||target->getIsToggleable())
    {
        target->setIsActive(false);
    }
}

void Button::onTouchCancelled(Touch *touch, Event *event)
{
    auto target = static_cast<Button*>(event->getCurrentTarget());

	target->onTouchEnded(touch, event);
}

void Button::touchDelegateRelease()
{
	this->release();
}

void Button::touchDelegateRetain()
{
	this->retain();
}
