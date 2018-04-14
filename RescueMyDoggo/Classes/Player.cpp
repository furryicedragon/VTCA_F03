#include"Player.h"
USING_NS_CC;
using namespace std;
Player* Player::create()
{
	Player* pSprite = new Player();
	if (pSprite && pSprite->initWithFile("/MainChar/0idle0.png"))
	{
		pSprite->setAnchorPoint(Vec2(0.70930, 0.41095));
		pSprite->autorelease();
		pSprite->initOption();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}

void Player::initOption()
{
	timePassedInSecond = 1;
	this->setFlippedX(false);
	pppFrames = SpriteFrameCache::getInstance();
	pppFrames->addSpriteFramesWithFile("/MainChar/ppp.plist");
	this->lastSeenLife = 100;
	this->isSpawning = true;
	this->setOpacity(255);
	this->runAction(FadeOut::create(0));
	this->canShowStatUp = true;
	this->setHP(100);
	this->baseHP = 100;
	this->baseEXP = 100;
	this->damageCurrent = 16;

	canAADamage.resize(8, false);


	attackRange = 100;
	attackSpeed = 0.14f;


	for(int i=0;i<4;i++){
	this->canMoveDirections.push_back(true);
	}
	this->count = 0;
	slash = Sprite::create("/MainChar/Effects/Slash/1.png");
	slash->setVisible(false);
	slash->setScale(1.9f);

	attackHelper = Sprite::create();
	if(attackHelper)
	this->addChild(attackHelper);

	//skillHelper = Sprite::create();
	//skillHelper->setVisible(false);
	//skillHelper->setScale(3);
	//if (skillHelper)
	//	this->addChild(skillHelper, 3);
	//this->skillHelper->setPosition(this->getContentSize().width / 1.8, this->getPosition().y + this->getContentSize().height / 2);

	movementHelper = Sprite::create();
	if(movementHelper)
	this->addChild(movementHelper);
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

	this->makeAnimation("walk",0.1);

	this->cd_reduction = 1.0f;

	this->scheduleUpdate();
}

void Player::setHP(int HP)
{
	hp = Label::create();
	//hp->setScale(2.8f);
	//hp->setAnchorPoint(Vec2(0.5, 0));
	hp->setString(std::to_string(HP));
	//hp->setPosition(this->getContentSize().width/2, this->getPosition().y + this->getContentSize().height);
	hp->setColor(Color3B(255, 0, 0));
	hp->setSystemFontSize(16);
	if(hp)
	this->addChild(hp, 1);
}

Animate * Player::animation(std::string actionName, float timeEachFrame) {
	
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

Animate * Player::allAnimation(std::string actionName, float timeEachFrame)
{
	Vector<SpriteFrame *> runningFrames;
	for (int i = 1; i < 99; i++) {
		auto frameName = "/" + actionName + "/(" + to_string(i) + ").png";
		Sprite* getSize = Sprite::create(frameName);
		if (!getSize)
			break;

		Size theSize = getSize->getContentSize();
		auto frame = SpriteFrame::create(frameName, Rect(0, 0, theSize.width, theSize.height));
		runningFrames.pushBack(frame);
	}
	Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, timeEachFrame);
	Animate* animAll = Animate::create(runningAnimation);
	return animAll;
}


void Player::idleStatus() {

	if (!this->isAttacking 
		&& !this->isRolling 
		&& !this->isDead 
		&& !this->isHit 
		&& this->isSpawn
		&& !this->usingSkill) 
	{
		this->isAttacking = false;
		this->isMoving = false;
		this->stopAllActions();
		//auto repeatIdle = RepeatForever::create(animation("Idle", 0.16969f));
		auto repeatIdle = RepeatForever::create(makeAnimation("idle", 0.16969f));
		repeatIdle->setTag(1);
		this->runAction(repeatIdle);
	}
	this->secondLastDirection = "";
}

void Player::moving() {

	if (this->isSpawn
		&& !this->usingSkill
		&& !this->isHit 
		&& !this->isRolling 
		&& !this->isAttacking 
		&& !this->isDead 
		&&(!this->isMoving || this->secondLastDirection!=lastDirection)) 
	{

		this->stopAllActionsByTag(1);
		if (!this->canMoveDirections[1]) {
			if (lastX > 0) lastX = 0;
		}
		if (!this->canMoveDirections[3]) {
			if (lastX < 0) lastX = 0;
		}

		if (lastDirection == "Left") {
			this->direction = 0;
			//this->slash->setFlippedX(false);
			//this->setFlippedX(false);
		}
		if (lastDirection == "Right") {
			this->direction = 1;
			//this->slash->setFlippedX(true);
			//this->setFlippedX(true);
		}

		smootherMove();
		secondLastDirection = lastDirection;
		this->stopAllActionsByTag(3);
		auto moveBy = MoveBy::create(lastDuration, Vec2(lastX,0));
		auto repeatMove = RepeatForever::create(moveBy);
		repeatMove->setTag(3);
		this->runAction(repeatMove);
		//this->runAction(Sequence::create(DelayTime::create(0.1) , CallFunc::create([=]() {this->notCombination = true; }), nullptr));
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
		//auto dashIt = RepeatForever::create(animation("Dash/Dash Normal/Dash", 0.14f));
		auto dashIt = RepeatForever::create(makeAnimation("walk",0.14f));
			dashIt->setTag(4);
			this->runAction(dashIt);
	}
}
int Player::checkDirectionInNumber(std::string direction) {
	int directionNumber = 0;
	
	if (direction == "Left") directionNumber = 2;
	if (direction == "Right") directionNumber = 3;
	return directionNumber;
}


void Player::attack() {
	if (this->isSpawn && !this->isAttacking && !this->isRolling && !this->isDead && !this->isHit && !this->usingSkill) {
		this->isAttacking = true;
		//this->slashEffect();                 //thuc hien animation cua slash
		this->stopAllActions();				//stop all hanh dong de attack
		//std::string name = "Attack/Attack Chain/" + std::to_string(this->attackChainNumber); //name = Folder chua Animate cua (attack)
		//this->runAction(Sequence::create(animation(name, attackSpeed),
		this->runAction(Sequence::create(makeAnimation("attack" + std::to_string(this->attackChainNumber), attackSpeed),
			CallFunc::create([=]() 
			{
				std::fill(canAADamage.begin(), canAADamage.end(), false);
				this->isAttacking = false;
				this->idleStatus();  
		}),	nullptr));
		this->runAction(Sequence::create(DelayTime::create(0.1), DelayTime::create(this->animationDelay - 0.1), CallFunc::create([=]() {
			std::fill(canAADamage.begin(), canAADamage.end(), true); }), nullptr));
		this->attackCount(); //dung de tang them 1 don vi moi khi attack (**)
	}
}
void Player::attackCount() {
		this->attackHelper->stopAllActionsByTag(1); //neu attack lien tiep se stop "helperAction(sau vai giay khong danh thu tu combo se ve 1)"
	if (attackChainNumber == 6) //(**)
		attackChainNumber = 0; //sau don danh thu 3 se tu dong quay ve don danh thu nhat (**)
	else {
		attackChainNumber++; //tang thu tu don danh them 1
		//int frameNumbers = attackFrameNumber[this->attackChainNumber - 1]; //check so luong frame animation trong folder cua Attack dua theo 
							//																									attack ChainNumber
		auto helperAction(Sequence::create(DelayTime::create(0.12/**(frameNumbers-1)*/ +10.1), CallFunc::create([=]() {this->attackChainNumber = 1; }), nullptr));
		helperAction->setTag(1);
		this->attackHelper->runAction(helperAction);
	}

}

void Player::getHit(int damage, float eeePosX) {
	if (!this->isDead) {
		this->stopAllActions();
		this->isAttacking = false;
		this->isMoving = false;
		if (this->usingSkill) {
			this->listSkill.at(currentSkillID)->stopAllActions();
			if (currentSkillID == 1) {
				std::fill(listSkill.at(currentSkillID)->canDamage.begin(), listSkill.at(currentSkillID)->canDamage.end(), false);
				listSkill.at(currentSkillID)->setPosition(0, 0);
			}
			listSkill.at(currentSkillID)->setVisible(false);
		}
		this->usingSkill = false;

		std::fill(canAADamage.begin(), canAADamage.end(), false);
		//this->setDoneDamageTo(true);
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
			this->runAction(Sequence::create(animation("Get hit/Get Hit Stand", 0.14f), CallFunc::create([=]() {this->isHit = false; this->idleStatus(); }), nullptr));
		}
	}
}

void Player::roll() {
	if (!this->isRolling && !this->isDead && !this->isAttacking && !this->isHit &&this->isSpawn && !this->usingSkill &&!isFalling) {
		this->stopAllActions();
		this->jump2Height = this->getPositionY() + 200;
		isRolling = true;
		int theX = /*20*/40;
		if (this->getPosition().x < theX + 44 + 32 *mapScale && this->direction==0)
			theX = this->getPosition().x - (44 + 32 *mapScale);
		if (map1Size.width - this->getPosition().x < theX + 44 + 32 *mapScale && this->direction == 1)
			theX = map1Size.width - this->getPosition().x - (44 + 32 *mapScale); //dung de check xem neu shift evade co gan diem cuoi cung cua map k, "to make sure we can't jump out of the map"
		//if (this->getPosition().x < 44 + 32*2)
		//	theX = 0;
		if (this->direction==0) theX *= -1; // nhan voi -1 de nhay dung' huong'
		this->runAction(Sequence::create(DelayTime::create(attackSpeed),JumpBy::create(attackSpeed * 2, Vec2(theX, 200),0/*33*/,1),nullptr));
		//this->runAction(Sequence::create(animation("Roll", attackSpeed), CallFunc::create([=]() {this->isRolling = false; this->idleStatus(); }), nullptr));
		this->runAction(Sequence::create(makeAnimation("evade", attackSpeed), CallFunc::create([=]() {this->isRolling = false; this->idleStatus(); }), nullptr));
	}
}


void Player::dead()
{
	this->isDead = true;
	//this->stopAllActions();
	this->forbidAllAction();
		//this->runAction(Sequence::create(animation("Death", 0.12), CallFunc::create([=]() {this->runAction(FadeOut::create(1.0)); }), nullptr));
		//this->runAction(animation("Death", 0.12f));
	this->setSpriteFrame(pppFrames->getSpriteFrameByName(std::to_string(this->direction)+"dead.png"));
}

void Player::forbidAllAction()
{
	this->stopAllActions();
	this->slash->stopAllActions();
	this->slash->setVisible(false);
}

void Player::statUp()
{
	this->damageCurrent += 10;
	this->attackSpeed = 0.08f - (0.08f / 20);
	baseHP += 80;

	std::stringstream sstream;
	sstream << std::fixed << std::setprecision(0) << baseHP;

	this->hp->setString(sstream.str());
	
	
	//statPlus->setScale(3);
	//statPlus->setAnchorPoint(Vec2(0.5, 0));
	statPlus->setPosition(this->getContentSize().width / 2, this->getPosition().y + this->getContentSize().height);
	statPlus->setSystemFontSize(20);

		this->statPlus->runAction(Sequence::create(
			CallFunc::create([=]() 
		{
			statPlus->setColor(Color3B(0, 0, 255));
			statPlus->setString("+1 Level");
			statPlus->setVisible(true);
		}),
			MoveBy::create(1, Vec2(0, 100)),
			MoveBy::create(0, Vec2(0, -100)),
			CallFunc::create([=]() {
			statPlus->setColor(Color3B(204, 0, 62));
			statPlus->setString("+ 10 Damage");
		}),
			MoveBy::create(1,Vec2(0,100)),
			MoveBy::create(0, Vec2(0, -100)),
			CallFunc::create([=]() {
			statPlus->setColor(Color3B(0, 255, 0));
			statPlus->setString("+ 80 Health Point");
		}),
			MoveBy::create(1, Vec2(0, 100)),
			MoveBy::create(0, Vec2(0, -100)),
			CallFunc::create([=]() {
			statPlus->setColor(Color3B(255, 255, 0));
			statPlus->setString("+ 20% Speed");
		}),
			MoveBy::create(1, Vec2(0, 100)),
			CallFunc::create([=]() {statPlus->setVisible(false); }), nullptr));




}

void Player::levelUp() {
	this->statUp();
	int lvl = std::stoi(level->getString());
	this->baseEXP += 100 * lvl;
	level->setString(std::to_string(lvl+1));
}


void Player::spawnEffect(){
	if (this->isSpawning)
		this->isSpawning = false;
		//this->runAction(FadeIn::create(0.8f));
	this->runAction(Sequence::create(FadeIn::create(0.8f), CallFunc::create([=]() {this->isSpawn = true; this->idleStatus(); }), nullptr));
		//this->runAction(Sequence::create(animation("Spawned", 0.1f), CallFunc::create([=]() {  this->isSpawn = true;this->idleStatus(); }), nullptr));
}

void Player::update(float elapsed)
{
	if (this->currentEXP > this->baseEXP || this->currentEXP==this->baseEXP) {
		this->currentEXP = 0 + currentEXP - baseEXP;
		this->levelUp();
	}
	if (isRolling) {
		char* a = "";
	}
	auto a = fabsf(this->getPositionY() - this->jump2Height);
	if (isRolling && fabsf(this->getPositionY() - this->jump2Height)<3) {
		this->isFalling = true;
	}
}


void Player::useSkill(int skillID, Button* button)
{
	if (this->isSpawn && !this->isAttacking && !this->isRolling && !this->isDead && !this->listSkill.at(skillID)->onCD) 
	{
		currentSkillID = skillID;
		Skill* skill = listSkill.at(skillID);
		skill->setupCD(button);

		skill->onCD = true;

		this->isHit = false;
		this->skillDamage = skill->skillDamage;
		int range = 269;
		if (skillID != 0) range = 0;
		if (this->getPosition().x < range + 44 + 32 * mapScale && this->direction==0)
			range = this->getPosition().x - (44 + 32 * mapScale);
		if (map1Size.width - this->getPosition().x < 259 + 44 + 32 * mapScale && this->direction==1)
			range = map1Size.width - this->getPosition().x - (44 + 32 * mapScale);

		if (this->direction == 0) {
			range *= -1;
			skill->setFlippedX(false); 
		}
		else skill->setFlippedX(true);
		this->stopAllActions();
		this->usingSkill = true;


		//this->runAction(Sequence::create(animation(skill->castAName,attackSpeed), 
		this->runAction(Sequence::create(makeAnimation(skill->castAName,attackSpeed), 
			CallFunc::create([=]() 
			{this->usingSkill = false; this->idleStatus(); }), nullptr));

		if(skillID!=1)
		this->runAction(Sequence::create(
			DelayTime::create(skill->mobilityDelayTime*attackSpeed), 
			MoveBy::create(skill->mobilityTime*attackSpeed, Vec2(range, 0)),nullptr));

		if(skillID!=1)
		skill->setPosition(skill->skillPosition);


		skill->runAction(Sequence::create(DelayTime::create(skill->skillAppearTime*attackSpeed), 
			CallFunc::create([=]() 
			{ 
			skill->setVisible(true); 
			std::fill(skill->canDamage.begin(), skill->canDamage.end(), true); 
			}),
			allAnimation(skill->skillAName,skill->skillDisappearTime*attackSpeed/skill->frames), 
				CallFunc::create([=]() {skill->setVisible(false);
			std::fill(skill->canDamage.begin(), skill->canDamage.end(), false); }), nullptr));

		skill->runAction(Sequence::create(DelayTime::create(skill->coolDownTime), CallFunc::create([=]() {this->skill2CD = false; }), nullptr));

		if (skillID == 1)
		{
			//listSkill.at(skillID)->setPosition;
			int moveRange = skill->skillRange;
			if (this->direction == 0)  moveRange = skill->skillRange * -1;
			listSkill.at(skillID)->runAction(Sequence::create(DelayTime::create(skill->skillAppearTime*attackSpeed),MoveTo::create(0,Vec2(this->getPosition().x, this->getPosition().y)),MoveBy::create(0.5, Vec2(moveRange, 0)),nullptr));
		}
	}
}


Animate* Player::makeAnimation(std::string actionName, float timeEachFrame) {
	Vector<SpriteFrame *> runningFrames;
	for (int i = 0; i < 99; i++) {
		auto frameName = std::to_string(this->direction)+actionName+std::to_string(i)+".png";
		SpriteFrame* frame = pppFrames->getSpriteFrameByName(frameName);
		if (!frame)
			break;
		runningFrames.pushBack(frame);
	}
	Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, timeEachFrame);
	Animate* anim = Animate::create(runningAnimation);
	this->animationDelay = timeEachFrame * (runningFrames.size() - 1);

	return anim;
}