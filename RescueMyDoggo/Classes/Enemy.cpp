#include"Enemy.h"
USING_NS_CC;
using namespace std;
Enemy* Enemy::create(int xMapNumber, int xWaveNumber, int xBossNumber)
{
	Enemy* pSprite = new Enemy();

	string xFile2Init = "/Enemies/Map " + std::to_string(xMapNumber);
	string xBossOrWave = "";
	if (xBossNumber == 0) xBossOrWave = "/Wave " + std::to_string(xWaveNumber) + "/Moving/0.png";
	else xBossOrWave = "/Boss " + std::to_string(xBossNumber) + "/Moving/0.png";
	string xCombination = xFile2Init + xBossOrWave;
	if (pSprite && pSprite->initWithFile(xCombination))
	{
		pSprite->autorelease();
		pSprite->mapNumber = xMapNumber;
		pSprite->waveNumber = xWaveNumber;
		pSprite->bossNumber = xBossNumber;
		pSprite->isSpawned = false;
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}

void Enemy::initOption()
{
	this->canRespawn = true;
	this->getHitTime = 0;
	this->inviTime = 1.8;
	if (this->bossNumber == 1) inviTime = 6;
	if (this->bossNumber == 2) inviTime = 8;
	if (this->bossNumber == 3) inviTime = 10;
	this->isDead = false;
	this->getFolderName();
	spell = Sprite::create();
	spell->setAnchorPoint(Vec2(0.5, 0));
	if (this->waveNumber > 0 && this->mapNumber == 1) spell->setAnchorPoint(Vec2(-0.5, 0));
	spellLanded = Sprite::create();
	spellLanded->setAnchorPoint(Vec2(0.5, 0));
	spellLanded->setScale(2);
	spell->setScale(2);

	this->isOnCD = false;
	this->isAttacking = false;
	this->isMoving = false;
	this->canDamage = false;
	this->canSpawn = true;

	attackHelper = Sprite::create();
	if(attackHelper)
	this->addChild(attackHelper);
	movementHelper = Sprite::create();
	if(movementHelper)
	this->addChild(movementHelper);

	auto getHitFrame = Sprite::create(combination + "/Get Hit/0.png");
	if(getHitFrame)
	this->getHitFrameSize = getHitFrame->getContentSize();
	this->idleStatus();
	this->scheduleUpdate();
}

void Enemy::setHP(int HP)
{
	if (hp == nullptr)	
	{ 
		hp = Label::create();

		hp->setAnchorPoint(Vec2(0.5, 0));

		hp->setPosition(this->getPosition().x +
			(this->getContentSize().width / 2), this->getPosition().y + this->getContentSize().height);
		hp->setColor(Color3B(255, 0, 0));
		//hp->setColor(Color4B::RED);
		hp->setSystemFontSize(16);

		this->hpBar = false;
	}

	hp->setString(std::to_string(HP));

	if (!hpBar)
	{
		this->addChild(hp, 1);
		this->hpBar = true;
	}
}

void Enemy::getFolderName()
{
	string file2Init = "/Enemies/Map " + std::to_string(this->mapNumber);
	string bossOrWave = "";
	if (bossNumber == 0) bossOrWave = "/Wave " + std::to_string(this->waveNumber);
	else bossOrWave = "/Boss " + std::to_string(this->bossNumber);
	combination = file2Init + bossOrWave;
	this->getLastFrameNumberOf("Skill Using");
	this->getLastFrameNumberOf("Spell");
}

void Enemy::getLastFrameNumberOf(std::string actionName)
{
	for (int i = 0; i < 99; i++) {
		auto frameName = combination + "/" + actionName + "/" + to_string(i) + ".png";
		Sprite* getSize = Sprite::create(frameName);
		if (!getSize) {
			if (actionName == "Skill Using")
				useSkillLastFN = i;
			if (actionName == "Spell")
				skillLastFN = i;
			break;
		}
	}
}

Animate* Enemy::animation(std::string actionName, float timeEachFrame) {
	Vector<SpriteFrame *> runningFrames;
	for (int i = 0; i < 99; i++) {
		auto frameName = combination + "/"+ actionName + "/" + to_string(i) + ".png";
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

void Enemy::idleStatus() {
	if (!this->isIdle && !this->isDead) {
		this->stopAllActionsByTag(1);
		this->stopAllActionsByTag(3);
		auto idleState = RepeatForever::create(animation("Idle", 0.12f));
		idleState->setTag(1);
		this->runAction(idleState);
		this->isIdle = true;
		this->canMove = true;
	}
}

void Enemy::movingAnimation()
{
	if (this->canMove) {
		this->stopAllActionsByTag(1);
		auto anim = RepeatForever::create(animation("Moving", 0.12f));
		anim->setTag(3);
		this->runAction(anim);
		this->isMoving = true;
		canMove = false;
	}

}
void Enemy::chasing()
{
	float howFar = ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width / 2);
	if (howFar < 0) howFar *= -1;

	if (!this->isMoving && !this->isAttacking &&this->isChasing && howFar>skillRange-69) {
		float pppX = ppp->getPosition().x;
		float moveByX = pppX - (this->getPosition().x + this->getContentSize().width / 2);
		if (moveByX < 0) {
			moveByX *= -1;
			this->setFlippedX(true);
		}
		else this->setFlippedX(false);

		this->isIdle = false;
		this->isMoving = true;
		this->breakTime = false;
		this->movingAnimation();
		auto move2 = Sequence::create(MoveTo::create(moveByX/moveSpeed, Vec2(ppp->getPosition().x, this->getPosition().y)),
			CallFunc::create([=]() 
		{this->isMoving = false; if (ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width / 2) > visionRange + 200) this->isChasing = false; else this->idleStatus(); }), nullptr);
		//could need some fix?! nah
		move2->setTag(2);
		this->runAction(move2);
		//this->movementHelper->runAction(Sequence::create(DelayTime::create(calculateDuration(moveByX, 0)), CallFunc::create([=]() {this->isMoving = false; }), nullptr));
	}
}
void Enemy::randomMoving() {
	if(this->waveNumber==1 || this->bossNumber==1)
	randomX = RandomHelper::random_real(line1X, line2X); //di chuyen trong 1 khoang giua line1 va line2 trong tiledMap
	if(this->waveNumber==2 || this->bossNumber>1) randomX = RandomHelper::random_real(line3X, line4X);
	float eX = this->getPosition().x;
	float moveByX = randomX - eX;

	this->isIdle = false;

	this->movingAnimation();
	if (moveByX > 0) this->setFlippedX(false); //done
	else {
		moveByX *= -1;
		this->setFlippedX(true);
	}
	auto seq = Sequence::create(MoveTo::create(moveByX/moveSpeed, Vec2(randomX, this->getPosition().y)),
		CallFunc::create([=]() {this->isMoving = false; this->breakTime = false; }), /*DelayTime::create(1),*/ nullptr);
	seq->setTag(2);
	this->runAction(seq);
}
void Enemy::moving() {
	float howFar = ppp->getPosition().x - (this->getPosition().x+this->getContentSize().width/2);
	if (howFar < 0) howFar *= -1; 
	if (howFar < skillRange && !this->isAttacking && !this->isOnCD) {
		this->attack();
		this->isAttacking = true;
	}

	if (howFar < visionRange && !this->isChasing && !this->isAttacking) {
		this->isChasing = true;
	}
	else {
		if (!this->isMoving && !this->isAttacking && !this->isChasing && !this->breakTime) 
		{
			this->breakTime = true;
			this->idleStatus();
			this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=]() {this->randomMoving(); }), nullptr));
		}
		else
		{
			this->chasing();
		}
	}

}


void Enemy::attack() {
		float howFar = ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width / 2);
		if (howFar < 0) this->setFlippedX(true);
		else this->setFlippedX(false);
		if (checkFrame("Spell"))
			this->spell->setFlippedX(this->isFlippedX());
		this->stopAllActions();
		this->isIdle = false;
		this->isAttacking = true;
		this->isOnCD = true;
		this->isMoving = false;
		this->canMove = true;
		this->breakTime = false;

		this->runAction(Sequence::create(animation("Skill Using", castSpeed),
			CallFunc::create([=]() {this->canDamage = false; this->isAttacking = false; this->idleStatus(); }), nullptr));
		if (this->isCaster) {
			this->casterSpell();
		}
		if (this->isSSMobility) {
			this->mobilitySS();
		}
		if (!this->isCaster && !this->isSSMobility) {
			this->runAction(Sequence::create(DelayTime::create(castSpeed*norAtkDmgAfterF), CallFunc::create([=]() {this->canDamage=true; }),
				DelayTime::create(castSpeed*doneAtkAfterF), CallFunc::create([=]() {this->canDamage = false; }), nullptr));
		}

		this->attackHelper->runAction(Sequence::create(DelayTime::create(0.12*useSkillLastFN*skillCD), CallFunc::create([=]() {this->isOnCD = false; }), nullptr));
}
void Enemy::casterSpell()
	{
	float range = 500.f;
	if (this->isFlippedX()) range *= -1;
	float move2X = this->getPosition().x;
	if (this->isFlippedX()) {
		move2X -= this->getContentSize().width;
	}
	if(this->mapNumber==1)
	this->spell->runAction(Sequence::create(
		MoveTo::create(0, Vec2(move2X, this->getPosition().y)),
		CallFunc::create([=]() {this->spell->setVisible(true); }),
		animation("Spell", castSpeed), CallFunc::create([=]() {this->spell->setVisible(false); this->attackLandedEffect(); }), nullptr));
	if (this->mapNumber == 2)
		this->spell->runAction(Sequence::create(
			CallFunc::create([=]() {this->interuptable=true; }),
			DelayTime::create(castSpeed * 8),
			CallFunc::create([=]() {
		this->interuptable = false;
		this->spell->setPosition(this->getPosition().x+this->getContentSize().width/2, this->getPosition().y+40);
		this->spell->setVisible(true); 
		this->spell->runAction(RepeatForever::create(animation("Spell", castSpeed)));
	}),
			MoveBy::create(0.69, Vec2(range, 0)),
		CallFunc::create([=]() {this->spell->setVisible(false);  this->spell->setPosition(0, 0); }), nullptr));

		//this->spell->runAction(RepeatForever::create(animation("Spell", castSpeed)));

}
void Enemy::mobilitySS()
{
	this->invulnerable = true;
	float howFar = ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width / 2);
	if (howFar < 0) howFar *= -1;
	if (howFar < skillRange) {
		if (this->bossNumber == 1 && this->mapNumber == 1)
			this->runAction(Sequence::create(DelayTime::create(castSpeed*(mobilitySSAt + 1)),
				JumpTo::create(castSpeed*mobilitySpeed, Vec2(ppp->getPosition().x - this->getContentSize().width / 2, this->getPosition().y), 44, 1), CallFunc::create([=]() {this->canDamage = true; this->invulnerable = false; }), nullptr));
		else
		this->runAction(Sequence::create(DelayTime::create(castSpeed*(mobilitySSAt + 1)),
			MoveTo::create(castSpeed*mobilitySpeed, Vec2(ppp->getPosition().x - this->getContentSize().width / 2, this->getPosition().y)), CallFunc::create([=]() {this->canDamage = true; this->invulnerable = false; }), nullptr));
	}
			
}
void Enemy::attackLandedEffect() {
	if (this->checkFrame("Skill Landed")) {
		this->spellLanded->setPosition(Vec2(ppp->getPosition().x, ppp->getPosition().y));
		this->spellLanded->runAction(Sequence::create(
			CallFunc::create([=]() {this->canDamage = true; this->spellLanded->setVisible(true); }), animation("Skill Landed", 0.12f), CallFunc::create([=]() {this->canDamage = false; this->spellLanded->setVisible(false); this->spellLanded->setPosition(0, 0); }), nullptr));
	}
}


void Enemy::getHit(int damage) {
	if (!this->isDead && this->isSpawned && !this->invulnerable) {
/*		if(this->mapNumber==1 || this->mapNumber==2) this->stopAllActions();
		else */
		this->forbidAllAction();

		this->isIdle = false;
		this->isAttacking = false;
		this->isMoving = false;
		this->canMove = true;
		this->breakTime = false;
		this->getHitTime++;

		if (getHitTime == 3) {
			getHitTime = 0;
			this->invulnerable = true;
			this->movementHelper->runAction(Sequence::create(DelayTime::create(inviTime),CallFunc::create([=]() {this->invulnerable = false; }), nullptr));
		}

		SpriteFrame * hit = SpriteFrame::create(combination + "/Get Hit/0.png", Rect(0, 0, this->getHitFrameSize.width,this->getHitFrameSize.height));
		if(hit)
		this->setSpriteFrame(hit);
		int x = -16;
		if (ppp->getPosition().x < this->getPosition().x) 
		{
			x *= -1;
			this->setFlippedX(true);
		}
		else this->setFlippedX(false);
		this->runAction(MoveBy::create(0.33f,Vec2(x, 0)));
		int healthP = std::stoi(this->hp->getString());
		healthP -= damage;
		if (healthP < 0 || healthP == 0) {
			this->hp->setString("0");
			this->dead();
		}
		else
		this->hp->setString(std::to_string(healthP));
		if(!this->isDead)
		this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() { this->idleStatus(); }), DelayTime::create(0.4f), CallFunc::create([=]() {this->isMoving=false;}), nullptr));
	}

}

void Enemy::autoRespawn()
{
	if (this->canRespawn && this->bossNumber==0) {
		this->runAction(
			Sequence::create(DelayTime::create(RandomHelper::random_int(10,20)), 
				CallFunc::create([=]()
				{
					this->setVisible(true);
					this->isDead = false;
					this->setHP(100);
					this->isSpawned = true;
		}), nullptr));
	}

}

void Enemy::dead() {
	if (this->waveNumber == 1) ppp->w1kills++;
	if (this->waveNumber == 2) ppp->w2kills++;
		this->isDead = true;
		this->isSpawned = false;
		//this->stopAllActions();
		this->forbidAllAction();
		if(this->checkFrame("Dead"))
		this->runAction(Sequence::create(animation("Dead", 0.12f), CallFunc::create([=]() {this->setVisible(false); }), nullptr));

		this->autoRespawn();

		auto howFar = ppp->getPosition().x - this->getPosition().x;
		auto theX = 40.f;
		if (howFar > 0) theX *= -1;
		this->runAction(MoveBy::create(0.5, Vec2(theX, 0)));
		if (this->waveNumber == 1) 
			ppp->currentEXP += 25;
		if (this->waveNumber == 2) 
			ppp->currentEXP += 50;
		if (this->bossNumber == 1) 
			ppp->currentEXP += 150;
		if (this->bossNumber == 2) 
			ppp->currentEXP += 300;
}

void Enemy::forbidAllAction()
{
	this->stopAllActions();
	if ((this->mapNumber == 1 && this->waveNumber > 0)||this->interuptable) {
		this->spell->stopAllActions();
		this->spell->setVisible(false);
	}

	//this->spellLanded->stopAllActions();
	//this->spellLanded->setVisible(false);
}

bool Enemy::checkFrame(std::string action)
{
	action += "/0.png";
	auto checkSprite = Sprite::create(combination +"/"+ action);
	if (checkSprite) return true;
	else return false;
}


void Enemy::update(float elapsed) 
{
	if(!this->isDead && this->isSpawned)
	moving();
}