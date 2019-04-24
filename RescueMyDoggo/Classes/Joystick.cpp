#include "cocos2d.h"
#include "Joystick.h"

using namespace cocos2d;

#define SJ_PI 3.14159265359f
#define SJ_PI_X_2 6.28318530718f
#define SJ_RAD2DEG 180.0f/SJ_PI
#define SJ_DEG2RAD SJ_PI/180.0f

Joystick::~Joystick()
{

}

bool Joystick::initWithRect(Rect rect)
{
	stickPosition = cocos2d::Vec2::ZERO;
	degrees = 0.0f;
	velocity = cocos2d::Vec2::ZERO;
	autoCenter = true;
	isDPad = false;
	hasDeadzone = false;
	numberOfDirections = 4;

	this->setJoystickRadius(rect.size.width / 2);
	this->setThumbRadius(32.0f);
	this->setDeadRadius(0.0f);

	//Cocos node stuff
	setPosition(rect.origin);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = Joystick::onTouchBegan;
	listener->onTouchMoved = Joystick::onTouchMoved;
	listener->onTouchEnded = Joystick::onTouchEnded;
	listener->onTouchCancelled = Joystick::onTouchCancelled;

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

float round_custom(float r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

void Joystick::updateVelocity(Point point)
{
	// Calculate distance and angle from the center.
	float dx = point.x;
	float dy = point.y;
	float dSq = dx * dx + dy * dy;

	if (dSq <= deadRadiusSq)
	{
		velocity = cocos2d::Size::ZERO;
		degrees = 0.0f;
		stickPosition = point;
		return;
	}

	float angle = atan2f(dy, dx); // in radians
	if (angle < 0)
	{
		angle += SJ_PI_X_2;
	}
	float cosAngle;
	float sinAngle;

	if (isDPad)
	{
		float anglePerSector = 360.0f / numberOfDirections * SJ_DEG2RAD;
		angle = round_custom(angle / anglePerSector) * anglePerSector;
	}

	cosAngle = cosf(angle);
	sinAngle = sinf(angle);

	// NOTE: Velocity goes from -1.0 to 1.0.
	if (dSq > joystickRadiusSq || isDPad)
	{
		dx = cosAngle * joystickRadius;
		dy = sinAngle * joystickRadius;
	}

	velocity = Point(dx / joystickRadius, dy / joystickRadius);
	degrees = angle * SJ_RAD2DEG;

	// Update the thumb's position
	stickPosition = Vec2(dx, dy);
}

void Joystick::setIsDPad(bool b)
{
	isDPad = b;
	if (isDPad) {
		hasDeadzone = true;
		this->setDeadRadius(10.0f);
	}
}

void Joystick::setJoystickRadius(float r)
{
	joystickRadius = r;
	joystickRadiusSq = r*r;
}

void Joystick::setThumbRadius(float r)
{
	thumbRadius = r;
	thumbRadiusSq = r*r;
}

void Joystick::setDeadRadius(float r)
{
	deadRadius = r;
	deadRadiusSq = r*r;
}

bool Joystick::onTouchBegan(Touch *touch, Event *event)
{
	Point location = Director::getInstance()->convertToGL(touch->getLocationInView());

	auto target = static_cast<Joystick*>(event->getCurrentTarget());

	location = target->convertToNodeSpace(location);
	//Do a fast rect check before doing a circle hit check:
	if (location.x < -target->getJoystickRadius() || location.x > target->getJoystickRadius() || location.y < -target->getJoystickRadius() || location.y > target->getJoystickRadius())
	{
		return false;
	}
	else
	{
		float dSq = location.x*location.x + location.y*location.y;
		if (target->getJoystickRadiusSq() > dSq)
		{
			target->updateVelocity(location);
			return true;
		}
	}
	return false;
}

void Joystick::onTouchMoved(Touch *touch, Event *event)
{
	Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
	auto target = static_cast<Joystick*>(event->getCurrentTarget());
	location = target->convertToNodeSpace(location);
	target->updateVelocity(location);
}

void Joystick::onTouchEnded(Touch *touch, Event *event)
{
	Point location = cocos2d::Size::ZERO;
	auto target = static_cast<Joystick*>(event->getCurrentTarget());

	if (!target->getAutoCenter())
	{
		Point location = Director::getInstance()->convertToGL(touch->getLocationInView());
		location = target->convertToNodeSpace(location);
	}
	target->updateVelocity(location);
}

void Joystick::onTouchCancelled(Touch *touch, Event *event)
{
	auto target = static_cast<Joystick*>(event->getCurrentTarget());

	target->onTouchEnded(touch, event);
}

void Joystick::touchDelegateRelease()
{
	this->release();
}

void Joystick::touchDelegateRetain()
{
	this->retain();
}
