#include"Player.h"
USING_NS_CC;
using namespace std;
Player* Player::create()
{
	Player* pSprite = new Player();
	if (pSprite && pSprite->initWithFile("0idle0.png"))
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
	this->doneWalking = true;
	timePassedInSecond = 1;
	this->setFlippedX(false);
	pppFrames = SpriteFrameCache::getInstance();
	pppFrames->addSpriteFramesWithFile("ppp.plist");
	this->lastSeenLife = 100;
	this->isSpawning = true;
	this->setOpacity(255);
	this->runAction(FadeOut::create(0));
	this->canShowStatUp = true;
	this->setHP(100);
	this->baseHP = 100;
	this->baseEXP = 100;
	this->damageCurrent = 16;
	this->score = 0;

	canAADamage.resize(8, false);
	canAct = true;

	attackRange = 100;
	attackSpeed = 0.14f;


	for(int i=0;i<4;i++){
	this->canMoveDirections.push_back(true);
	}
	this->count = 0;

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
	//this->attackChainNumber = 0;
	this->weaponKind = "onehanded";
	isAttacking = false;
	this->idleStatus();

	this->makeAnimation("walk",0.1);

	this->state = 0;

	this->cd_reduction = 1.0f;

	this->scheduleUpdate();
}

void Player::setHP(int HP)
{

	hp = Label::create();
	hp->setScale(2.8f);
	hp->setAnchorPoint(Vec2(0.5, 0));
	hp->setString(std::to_string(HP));
	hp->setPosition(this->getContentSize().width/2, this->getPosition().y + this->getContentSize().height);
	hp->setColor(Color3B(255, 0, 0));
	hp->setSystemFontSize(16);
	hp->setVisible(false);
	if(hp)
	this->addChild(hp, 1);
	HPonHead = ProgressTimer::create(Sprite::create("HUD/Bars/hp2.png"));
	HPonHead->setType(ProgressTimerType::BAR);
	HPonHead->setPercentage(100);
	HPonHead->setBarChangeRate(Vec2(1, 0));
	HPonHead->setMidpoint(Vec2(0.0, 0.0));
	HPonHead->setScale(0.12f);
	HPonHead->setPosition(this->getContentSize().width / 2,this->getContentSize().height + 20);

	HitDame = ProgressTimer::create(Sprite::create("HUD/Bars/hitDame2.png"));
	HitDame->setType(ProgressTimerType::BAR);
	HitDame->setPercentage(100);
	HitDame->setBarChangeRate(Vec2(1, 0));
	HitDame->setMidpoint(Vec2(0.0, 0.0));
	HitDame->setScale(0.12f);
	HitDame->setPosition(this->getContentSize().width / 2,this->getContentSize().height + 20);

	nothingBar = Sprite::create("HUD/Bars/nothing2.png");
	nothingBar->setPosition(this->getContentSize().width / 2,this->getContentSize().height + 20);
	nothingBar->setScale(0.12f);

	HPonHead->retain();
	HitDame->retain();
	nothingBar->retain();

	/*this->addChild(nothingBar);
	this->addChild(HitDame);
	this->addChild(HPonHead);*/
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
		&& this->canAct 
		&& this->isSpawn
		&& !this->usingSkill) 
	{
		this->doneWalking = true;
		this->isIdle = true;
		this->isAttacking = false;
		this->isMoving = false;
		this->stopAllActions();
		//auto repeatIdle = RepeatForever::create(animation("Idle", 0.16969f));
		auto repeatIdle = RepeatForever::create(makeAnimation("idle", 0.16969f));
		repeatIdle->setTag(1);
		this->runAction(repeatIdle);
		this->secondLastDirection = "";
	}
}

void Player::moving(float dt) {

	if (this->isSpawn
		&& !this->usingSkill
		&& canAct
		//&& !this->isRolling 
		&& !this->isAttacking 
		&& !this->isDead 
		&& this->isMoving) 
	{
		this->stopAllActionsByTag(1);
		float speed;
		if (lastDirection == "Left") {
			this->direction = 0;
			speed = 275 * dt * -1;
		}
		if (lastDirection == "Right") {
			this->direction = 1;
			speed = 275 * dt;
		}

		if (!this->canMoveDirections[1]) {
			if (speed > 0) speed = 0;
		}
		if (!this->canMoveDirections[3]) {
			if (speed < 0) speed = 0;
		}

		this->setPositionX(this->getPositionX() + speed);
		smootherMove();
		secondLastDirection = lastDirection;
		//this->isMoving = true;
		//this->stopAllActionsByTag(3);
		//auto moveBy = MoveBy::create(lastDuration, Vec2(lastX,0));
		//auto repeatMove = RepeatForever::create(moveBy);
		//repeatMove->setTag(3);
		//this->runAction(repeatMove);

	}

}
void Player::smootherMove() {
		//this->stopAllActionsByTag(2);
	if (this->lastDirection != secondLastDirection) {
		this->stopAllActionsByTag(4);
		this->doneWalking = true;
	}
	if (this->doneWalking) {
		doneWalking = false;
		auto walkIt = Sequence::create(makeAnimation("walk", 0.14f), CallFunc::create([=]() {doneWalking = true; }), nullptr);
		walkIt->setTag(4);
		this->runAction(walkIt);
	}
}


void Player::attack() {
	if (this->isSpawn && !this->isAttacking && !this->isRolling && !this->isDead && this->canAct && !this->usingSkill) {
		this->isMoving = false;
		this->isAttacking = true;
		this->doneWalking = true;
		//this->slashEffect();                 //thuc hien animation cua slash
		this->stopAllActions();				//stop all hanh dong de attack
		//std::string name = "Attack/Attack Chain/" + std::to_string(this->attackChainNumber); //name = Folder chua Animate cua (attack)
		//this->runAction(Sequence::create(animation(name, attackSpeed),
		this->runAction(Sequence::create(makeAnimation("attack" + std::to_string(RandomHelper::random_int(0, 6))/*std::to_string(this->attackChainNumber)*/, attackSpeed),
			CallFunc::create([=]() 
			{
				std::fill(canAADamage.begin(), canAADamage.end(), false);
				this->isAttacking = false;
				this->idleStatus();  
		}),	nullptr));
		this->runAction(Sequence::create(DelayTime::create(0.1), DelayTime::create(this->animationDelay - 0.1), CallFunc::create([=]() {
			std::fill(canAADamage.begin(), canAADamage.end(), true); }), nullptr));
		//this->attackCount(); //dung de tang them 1 don vi moi khi attack (**)
	}
}

//void Player::attackCount() {
//		this->attackHelper->stopAllActionsByTag(1); //neu attack lien tiep se stop "helperAction(sau vai giay khong danh thu tu combo se ve 1)"
//	if (attackChainNumber == 6) //(**)
//		attackChainNumber = 0; //sau don danh thu 3 se tu dong quay ve don danh thu nhat (**)
//	else {
//		attackChainNumber++; //tang thu tu don danh them 1
//		//int frameNumbers = attackFrameNumber[this->attackChainNumber - 1]; //check so luong frame animation trong folder cua Attack dua theo 
//							//																									attack ChainNumber
//		auto helperAction(Sequence::create(DelayTime::create(0.12/**(frameNumbers-1)*/ +10.1), CallFunc::create([=]() {this->attackChainNumber = 0; }), nullptr));
//		helperAction->setTag(1);
//		this->attackHelper->runAction(helperAction);
//	}
//
//}

void Player::knockback(float eeePosX)
{
	// blink
	this->movementHelper->runAction(
		Sequence::create(
		Repeat::create(Sequence::create(DelayTime::create((float) 1 / ((255-90)*2 *9)), 
			CallFunc::create([=]() 
			{ if (this->getOpacity() == 90 && !fade)
					fade = true;
				else if (this->getOpacity() == 255 && fade)
					fade = false;

				if (fade) this->setOpacity(this->getOpacity() + 1);
				else this->setOpacity(this->getOpacity() - 1);
			}), nullptr), (255 - 90)*2 *9),
			CallFunc::create([=]() { this->state = 0; this->idleStatus(); }), nullptr));

	// knockback
	int theX = (this->getPositionX() - this->getContentSize().width / 2 < eeePosX) ? -1 : 1;

	this->movementHelper->runAction(
		Sequence::create(
			Repeat::create(Sequence::create( DelayTime::create((float)0.16/69), 
				CallFunc::create([=]() {
					if ((this->getPositionX() > 32 + 44) 
						&& (this->getPositionX() < mapWidth - (32 + 44)))
						this->setPositionX(this->getPositionX() + theX); 
				}), nullptr), 69),
			CallFunc::create([=]() {this->canAct = true; this->idleStatus(); }), nullptr));

}

void Player::getHit(int damage, float eeePosX) {
	if (!this->isDead && this->state != 1) 
	{
		this->doneWalking = true;
		tester = true;
		this->state = 1;
		this->stopAllActions();
		this->isAttacking = false;
		//this->aa = false;
		this->canAct = false;
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

			this->knockback(eeePosX);
			
		}
	}
}

void Player::roll() {
	if (!this->isRolling && !this->isDead && !this->isAttacking && this->canAct &&this->isSpawn && !this->usingSkill &&!isFalling) {
		this->stopAllActions();
		this->setSpriteFrame(pppFrames->getSpriteFrameByName(std::to_string(direction) + "jump0.png"));
		this->isIdle = false;
		//this->isMoving = false;
		this->doneWalking = true;
		//this->jump2Height = this->getPositionY() + 200;
		isRolling = true;
		int theX = /*20*/0;
		if (this->getPosition().x < theX + 44 + 32 *mapScale && this->direction==0)
			theX = this->getPosition().x - (44 + 32 *mapScale);
		if (mapWidth - this->getPosition().x < theX + 44 + 32 *mapScale && this->direction == 1)
			theX = mapWidth - this->getPosition().x - (44 + 32 *mapScale); //dung de check xem neu shift evade co gan diem cuoi cung cua map k, "to make sure we can't jump out of the map"
		//if (this->getPosition().x < 44 + 32*2)
		//	theX = 0;
		if (this->direction==0) theX *= -1; // nhan voi -1 de nhay dung' huong'
		//this->runAction(Sequence::create(DelayTime::create(attackSpeed),JumpBy::create(attackSpeed * 2, Vec2(theX, 80),0/*33*/,1),nullptr));
		//auto a = pppFrames->getSpriteFrameByName(std::to_string(direction) + "jump.png");
		//this->setSpriteFrame(pppFrames->getSpriteFrameByName(std::to_string(direction)+"jump0.png"));
		this->runAction(Sequence::create(JumpBy::create(0.2, Vec2(theX, 109), 0/*33*/, 1), CallFunc::create([=]() {this->isRolling = false; this->isFalling = true; }), nullptr));
		//this->runAction(Sequence::create(animation("Roll", attackSpeed), CallFunc::create([=]() {this->isRolling = false; this->idleStatus(); }), nullptr));
		//this->runAction(Sequence::create(makeAnimation("evade", attackSpeed), CallFunc::create([=]() {this->isRolling = false; this->idleStatus(); }), nullptr));
	}
}


void Player::dead()
{
	this->doneWalking = true;
	this->isDead = true;
	//this->stopAllActions();
	this->forbidAllAction();
		//this->runAction(Sequence::create(animation("Death", 0.12), CallFunc::create([=]() {this->runAction(FadeOut::create(1.0)); }), nullptr));
		//this->runAction(animation("Death", 0.12f));
	this->setSpriteFrame(pppFrames->getSpriteFrameByName(std::to_string(this->direction)+"dead0.png"));
}

void Player::forbidAllAction()
{
	this->stopAllActions();
}

void Player::statUp()
{
	this->damageCurrent += 10;
	this->attackSpeed = 0.14f - (0.14f / 20);
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

	int lvl1 = std::stoi(statPlayer->level->getString());
	statPlayer->level->setString(std::to_string(lvl1 + 1));
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
	//if (isFalling || isRolling) 
	//	this->setSpriteFrame(pppFrames->getSpriteFrameByName(std::to_string(direction) + "jump0.png"));
}


void Player::useSkill(int skillID, Button* button)
{
	if (this->isSpawn && !this->isAttacking && !this->isRolling && !this->isDead && this->canAct && !this->listSkill.at(skillID)->onCD) 
	{
		this->doneWalking = true;
		currentSkillID = skillID;
		Skill* skill = listSkill.at(skillID);
		skill->setupCD(button);

		skill->onCD = true;

		this->skillDamage = skill->skillDamage;
		int range = 269;
		if (skillID != 0) range = 0;
		if (this->getPosition().x < range + 44 + 32 * mapScale && this->direction==0)
			range = this->getPosition().x - (44 + 32 * mapScale);
		if (mapWidth - this->getPosition().x < 259 + 44 + 32 * mapScale && this->direction==1)
			range = mapWidth - this->getPosition().x - (44 + 32 * mapScale);

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
		auto frameName = std::to_string(this->direction)+actionName+std::to_string(i)+std::to_string(state)+".png";
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