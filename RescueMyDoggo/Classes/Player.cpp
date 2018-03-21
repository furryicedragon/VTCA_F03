#include"Player.h"
USING_NS_CC;
using namespace std;
Player* Player::create()
{
	Player* pSprite = new Player();
	if (pSprite && pSprite->initWithFile("/MainChar/Idle/onehanded/(1).png"))
	{
		pSprite->autorelease();
		pSprite->initOption();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}

void Player::initOption()
{
	this->isSpawning = true;
	this->setOpacity(255);
	this->runAction(FadeOut::create(0));
	this->canShowStatUp = true;
	this->additionalDmg = 0;
	this->additionalHP = 0;
	this->additionalAS = 0;
	this->setHP(100);
	this->damageCurrent = 16;

	for (int i = 0; i < 8; i++) {
		doneDamage.push_back(true);
	}



	attackSpeed = 0.08;


	for(int i=0;i<4;i++){
	this->canMoveDirections.push_back(true);
	}
	this->count = 0;
	slash = Sprite::create("/MainChar/Effects/Slash/1.png");
	slash->setVisible(false);
	slash->setScale(1.9);
	isDashing = false;
	attackHelper = Sprite::create();
	if(attackHelper)
	this->addChild(attackHelper);
	movementHelper = Sprite::create();
	if(movementHelper)
	this->addChild(movementHelper);
	canDash = false;
	int one = 5;
	int two = 7;
	int three = 8;
	this->attackFrameNumber.push_back(one);
	this->attackFrameNumber.push_back(two);
	this->attackFrameNumber.push_back(three);
	this->attackChainNumber = 1;
	this->weaponKind = "onehanded";
	isAttacking = false;
	this->idleStatus();
}

void Player::setHP(int HP)
{
	hp = Label::create();
	hp->setScale(2.8);
	hp->setAnchorPoint(Vec2(0.5, 0));
	hp->setString(std::to_string(HP));
	hp->setPosition(this->getContentSize().width/2, this->getPosition().y + this->getContentSize().height);
	hp->setColor(Color3B(255, 0, 0));
	hp->setSystemFontSize(16);
	if(hp)
	this->addChild(hp, 1);
}

Animate * Player::animation(std::string actionName, float timeEachFrame) {
	//{
	//	Vector<SpriteFrame *> runningFrames;
	//	for (int i = 1; i < frameNumbers; i++) {
	//		auto frameName = "/" + actionName + "/(" + to_string(i) + ").png";
	//		auto frame = SpriteFrame::create(frameName, Rect(0, 0, fWidth, fHeight));
	//		runningFrames.pushBack(frame);
	//	}
	//	Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, timeEachFrame);
	//	Animate* anim = Animate::create(runningAnimation);
	//	if (repeat) {
	//		Action* theAction = (RepeatForever::create(anim));
	//		theAction->setTag(tag);
	//		this->runAction(theAction);
	//	}
	//	else
	//		this->runAction(anim);
	//}
	Vector<SpriteFrame *> runningFrames;
	for (int i = 1; i < 99; i++) {
		auto frameName = "/MainChar/" + actionName +"/"+ this->weaponKind +"/(" + to_string(i) + ").png";
		Sprite* getSize = Sprite::create(frameName);
		if (!getSize)
			break;

		Size theSize = getSize->getContentSize();
		auto frame = SpriteFrame::create(frameName, Rect(0, 0, theSize.width, theSize.height));
		runningFrames.pushBack(frame);
	}
	Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, timeEachFrame);
	Animate* anim = Animate::create(runningAnimation);
	return anim;
}


void Player::idleStatus() {

	if (!this->isAttacking && !this->isRolling && !this->isDead && !this->isHit && this->isSpawn) {
		this->isAttacking = false;
		this->isMoving = false;
		this->isDashing = false;
		this->stopAllActions();
		auto repeatIdle = RepeatForever::create(animation("Idle", 0.16969));
		repeatIdle->setTag(1);
		this->runAction(repeatIdle);
		}
	this->secondLastDirection = "";
}

void Player::moving() {

	if (this->isSpawn && !this->isHit && !this->isRolling && !this->isAttacking && !this->isDead &&(!this->isMoving || this->secondLastDirection!=lastDirection)) {
		//this->stopAllActions();
		this->stopAllActionsByTag(1);
		if (this->canDash||this->isDashing) {
			this->isDashing = true;
			if(lastX==-45||lastX==45 ||lastX==32||lastX==-32)
			lastX = lastX*2.4;
			if(lastY==-45||lastY==45||lastY==32||lastY==-32)
			lastY = lastY*2.4;
		}
		if (!this->canMoveDirections[0]) {
			if (lastY > 0) lastY = 0;
		}
		if (!this->canMoveDirections[1]) {
			if (lastX > 0) lastX = 0;
		}
		if (!this->canMoveDirections[2]) {
			if (lastY < 0) lastY = 0;
		}
		if (!this->canMoveDirections[3]) {
			if (lastX < 0) lastX = 0;
		}
		if (lastX == 80 && lastY==0) lastX = 45 * 2.5;
		if (lastX == -80 && lastY==0) lastX = -45 * 2.5;

		smootherMove();
		secondLastDirection = lastDirection;
		this->stopAllActionsByTag(3);
		auto moveBy = MoveBy::create(lastDuration, Vec2(lastX,lastY));
		auto repeatMove = RepeatForever::create(moveBy);
		repeatMove->setTag(3);
		this->runAction(repeatMove);
		if (lastDirection == "Left" || lastDirection == "DownLeft" || lastDirection == "UpLeft") {
			this->slash->setFlippedX(false);
			this->setFlippedX(true);
		}
		if (lastDirection == "Right" || lastDirection == "DownRight" || lastDirection == "UpRight") {
			this->slash->setFlippedX(true);
			this->setFlippedX(false);
		}
		this->runAction(Sequence::create(DelayTime::create(0.1) , CallFunc::create([=]() {this->notCombination = true; }), nullptr));
		this->isMoving = true;

	}

}
void Player::smootherMove() {
	previousDirectionInNumber = this->checkDirectionInNumber(secondLastDirection);
	presentDirectionInNumber = this->checkDirectionInNumber(lastDirection);
	bool OK = false;
	switch (previousDirectionInNumber)
	{
		case 0:
		case 1:
			OK = true;
		break;
		case 2:
			if (presentDirectionInNumber == 3) OK = true;
				break;
		case 3:
			if (presentDirectionInNumber == 2) OK = true;
			break;
	}
	if(OK) {
		this->stopAllActionsByTag(2);
		this->stopAllActionsByTag(4);
		if (this->canDash || this->isDashing) {
			auto dashIt = RepeatForever::create(animation("Dash/Dash Normal/Dash", 0.14));
			dashIt->setTag(4);
			this->runAction(dashIt);
		}
		else {
			auto walkIt = RepeatForever::create(animation("Walk/Walk Forward/", 0.14));
			walkIt->setTag(2);
			this->runAction(walkIt);
		}
	}
}
int Player::checkDirectionInNumber(std::string direction) {
	int directionNumber = 0;
	if (direction == "Up") directionNumber = 1; // 1 = no change
	if (direction == "Down")
		directionNumber = 1;
	if (direction == "UpLeft") directionNumber = 2;
	if (direction == "Left") directionNumber = 2;
	if (direction == "DownLeft") directionNumber = 2;
	if (direction == "DownRight") directionNumber = 3;
	if (direction == "Right") directionNumber = 3;
	if (direction == "UpRight") directionNumber = 3;
	return directionNumber;
}


void Player::checkDash(){
	this->movementHelper->runAction(Sequence::create(CallFunc::create([=]() {this->canDash = true; }), DelayTime::create(0.2), CallFunc::create([=]() {this->canDash = false; }), nullptr));
}

void Player::attack() {
	if (this->isSpawn && !this->isAttacking && !this->isRolling && !this->isDead && !this->isHit) {
		this->isAttacking = true;
		this->slashEffect();
		this->stopAllActions();
		std::string name = "Attack/Attack Chain/" + std::to_string(this->attackChainNumber);
		int frameNumbers = attackFrameNumber[this->attackChainNumber - 1];
		this->runAction(Sequence::create(animation(name, attackSpeed), CallFunc::create([=]() {this->isAttacking = false; this->idleStatus(); }),nullptr));
		this->attackCount();
	}
}
void Player::attackCount() {
		this->attackHelper->stopAllActionsByTag(1);
	if (attackChainNumber == 3) 
		attackChainNumber = 1;
	else {
		attackChainNumber++;
		int frameNumbers = attackFrameNumber[this->attackChainNumber - 1];
		auto helperAction(Sequence::create(DelayTime::create(0.12*(frameNumbers-1) +0.1), CallFunc::create([=]() {this->attackChainNumber = 1; }), nullptr));
		helperAction->setTag(1);
		this->attackHelper->runAction(helperAction);
	}

}
void Player::slashEffect() {
	Vector<SpriteFrame *> runningFrames;
	for (int i = 1; i < 4; i++) {
		auto frameName = "/MainChar/Effects/Slash/" + to_string(i) + ".png";
		auto frame = SpriteFrame::create(frameName, Rect(0, 0, 192, 192));
		runningFrames.pushBack(frame);
	}
	Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, attackSpeed*2/3);
	Animate* anim = Animate::create(runningAnimation);
	float delayTime1 = 0;
	float delayTime2 = 0;
	int xAngle = 0;
	int yAngle = 0;
	int xPos = 0;
	int yPos = 0;
	bool flippedY = true;
	switch (this->attackChainNumber)
	{
	case 1:
	{
		//delayTime1 = 0.08 * 3 + 0.1;
		delayTime1 = attackSpeed * 4;
		xAngle = -18;
		yAngle = -36;
		xPos = -63;
		yPos = 210;
		//this->slash->runAction(Sequence::create(CallFunc::create([=]() { slash->setVisible(true); }), anim, CallFunc::create([=]() { slash->setVisible(false); }), nullptr));
		//this->slash->runAction(Sequence::create(RotateBy::create(0, -18, -36), DelayTime::create(0.08 * 3 + 0.1), RotateTo::create(0, 0, 0), nullptr));
		//this->slash->setPosition(Vec2(this->getPosition().x - 63, this->getPosition().y + 210));
		//this->slash->setFlippedY(true);
		break;
	}
	case 2:
	{
		//delayTime1 = 0.08 * 3 + 0.11+0.05;
		delayTime1 = attackSpeed * 4;
		delayTime2 = attackSpeed*2;
		xAngle = -8;
		yAngle = 11;
		xPos = -36;
		yPos = 99;
		flippedY = false;
		//this->slash->runAction(Sequence::create(RotateBy::create(0, -44,15), DelayTime::create(0.08 * 3 + 0.145+0.05), RotateTo::create(0, 0, 0), nullptr));
		//this->slash->setPosition(Vec2(this->getPosition().x + 25, this->getPosition().y + 128));
		//this->slash->setFlippedY(false);
		//this->slash->runAction(Sequence::create(DelayTime::create(0.145), CallFunc::create([=]() { slash->setVisible(true); }), anim, CallFunc::create([=]() { slash->setVisible(false); }), nullptr));
		break;
	}
	case 3:
	{
		//delayTime1 = 0.08 * 3 + 0.05 + 0.15;
		delayTime1 = attackSpeed * 5;
		delayTime2 = attackSpeed*3;
		xAngle = -144;
		yAngle = -155;
		xPos = -55;
		yPos = 88;
		break;
	}
	}

	if (this->slash->isFlippedX()) {
		xAngle *= -1;
		yAngle *= -1;
		xPos *= -1;
	}

		this->slash->runAction(Sequence::create(RotateBy::create(0, xAngle, yAngle), DelayTime::create(delayTime1), RotateTo::create(0, 0, 0), nullptr));
		this->slash->setPosition(Vec2(this->getPosition().x + xPos, this->getPosition().y + yPos));
		this->slash->setFlippedY(flippedY);
		this->slash->runAction(Sequence::create(DelayTime::create(delayTime2),
			CallFunc::create([=]() { slash->setVisible(true); this->setDoneDamageTo(false); }), anim,
			CallFunc::create([=]() { slash->setVisible(false); this->setDoneDamageTo(true); }), nullptr));
}

void Player::getHit(int damage, float eeePosX) {
	if (!this->isDead) {
		this->stopAllActions();
		this->isAttacking = false;
		this->isMoving = false;
		//int x = -16;
		//if (this->getPosition().x < eeePosX)
		//{
		//	x *= -1;
		//	this->setFlippedX(true);
		//}
		//else this->setFlippedX(false);
		//this->runAction(MoveBy::create(0.33, Vec2(x, 0)));
		int healthP = std::stoi(this->hp->getString());
		healthP -= damage;
		if (healthP < 0 || healthP == 0) {
			this->hp->setString("0");
			this->dead();
		}
		else
			this->hp->setString(std::to_string(healthP));
		if (!this->isDead) {
			this->isHit = true;
			this->runAction(Sequence::create(animation("Get hit/Get Hit Stand", 0.14), CallFunc::create([=]() {this->isHit = false; this->idleStatus(); }), nullptr));
		}
	}
}

void Player::roll() {
	if (!this->isRolling && !this->isDead && !this->isAttacking && !this->isHit) {
		this->stopAllActions();
		isRolling = true;
		int theX = 259;
		if (this->getPosition().x < 259 + 44 + 32 * 2 && this->isFlippedX())
			theX = this->getPosition().x - (44 + 32 * 2);
		if (map1Size.width - this->getPosition().x < 259 + 44 + 32 * 2 && !this->isFlippedX())
			theX = map1Size.width - this->getPosition().x - (44 + 32 * 2);
		//if (this->getPosition().x < 44 + 32*2)
		//	theX = 0;
		if (this->isFlippedX()) theX *= -1;
		this->runAction(Sequence::create(DelayTime::create(attackSpeed),JumpBy::create(attackSpeed * 6, Vec2(theX, 0),33,1),nullptr));
		this->runAction(Sequence::create(animation("Roll", attackSpeed), CallFunc::create([=]() {this->isRolling = false; this->idleStatus(); }), nullptr));
	}
}

void Player::setDoneDamageTo(bool whatYouWant) {
	int i = 0;
	for each (auto item in doneDamage)
	{
		doneDamage[i] = whatYouWant;
		i++;
	}
}

void Player::dead()
{
	this->isDead = true;
	//this->stopAllActions();
	this->forbidAllAction();
		//this->runAction(Sequence::create(animation("Death", 0.12), CallFunc::create([=]() {this->runAction(FadeOut::create(1.0)); }), nullptr));
		this->runAction(animation("Death", 0.12));
}

void Player::forbidAllAction()
{
	this->stopAllActions();
	this->slash->stopAllActions();
	this->slash->setVisible(false);
}

void Player::statUp(int damage, int HP, int speed)
{
	this->additionalHP += HP;
	this->additionalDmg += damage;
	this->additionalAS += speed;
	this->damageCurrent = 16 + additionalDmg;
	this->attackSpeed = 0.08 - (additionalAS / 5000);
	this->hp->setString(std::to_string(100+this->additionalHP));
	auto statPlus = Label::create();
	statPlus = Label::create();
	statPlus->setScale(3);
	statPlus->setAnchorPoint(Vec2(0.5, 0));
	statPlus->setPosition(this->getContentSize().width / 2, this->getPosition().y + this->getContentSize().height);
	statPlus->setSystemFontSize(20);

	if (damage > 0) {
		statPlus->setColor(Color3B(204, 0, 62));
		statPlus->setString("+ " + std::to_string(damage) +"DMG");
	}
	else
	{
		if (HP > 0) {
			statPlus->setColor(Color3B(0, 255, 0));
			statPlus->setString("+ " + std::to_string(HP) +"HP");
		}
		else {
			statPlus->setColor(Color3B(255, 255, 0));
			statPlus->setString("+ " + std::to_string(speed) + "AS");
		}

	}
	statPlus->setVisible(false);
	statUpBox.push_back(statPlus);
	if(statUpBox[statUpBox.size()-1])
	this->addChild(statUpBox[statUpBox.size()-1], 1);

}

void Player::spawnEffect(){
	if (this->isSpawning)
		this->isSpawning = false;
		this->runAction(FadeIn::create(0.8f));
		this->runAction(Sequence::create(animation("Spawned", 0.1), CallFunc::create([=]() {  this->isSpawn = true;this->idleStatus(); }), nullptr));
}