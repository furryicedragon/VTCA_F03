#include"Enemy.h"
USING_NS_CC;
using namespace std;
Enemy* Enemy::create(int xMapNumber, int xWaveNumber, int xBossNumber)
{
	Enemy* pSprite = new Enemy();
	pSprite->eeeFrames = SpriteFrameCache::getInstance();
	if (pSprite && pSprite->initWithSpriteFrame(pSprite->eeeFrames->getSpriteFrameByName
	(std::to_string(xMapNumber) + std::to_string(xWaveNumber) + std::to_string(xBossNumber) + "0_idle0.png")))
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
	this->direction = 0;
	this->canDrop = false;
	this->canChase = true;
	this->canRespawn = true;
	this->getHitTime = 0;
	this->inviTime = 1.8;
	if (this->bossNumber == 1) inviTime = 6;
	if (this->bossNumber == 2) inviTime = 8;
	if (this->bossNumber == 3) inviTime = 10;
	this->isDead = false;
	this->getLastFrameNumberOf("attack");
	this->getLastFrameNumberOf("projectile");
	spell = Sprite::create();
	//spell->setAnchorPoint(Vec2(0.5, 0));
	//if (this->waveNumber == 2 && this->mapNumber == 1) spell->setAnchorPoint(Vec2(0, 0));
	spellLanded = Sprite::create();
	spellLanded->setAnchorPoint(Vec2(0.5f, 0.3f));
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

	this->hp->setVisible(false);

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

	if (!hpBar && hp)
	{
		this->addChild(hp, 1);
		this->hpBar = true;
	}
}


void Enemy::getLastFrameNumberOf(std::string actionName)
{
	for (int i = 0; i < 99; i++) {
		auto frameName = 
			std::to_string(mapNumber) + std::to_string(waveNumber) + std::to_string(bossNumber) +std::to_string(this->direction) +"_" + actionName + std::to_string(i) + ".png";
		SpriteFrame* test = eeeFrames->getSpriteFrameByName(frameName);
		if (!test) {
			if (actionName == "attack")
				useSkillLastFN = i;
			if (actionName == "projectile")
				skillLastFN = i;
			break;
		}
	}
}


void Enemy::idleStatus() {
	if (!this->isIdle &&( !this->isDead || this->canRespawn)) {
		this->stopAllActionsByTag(1);
		this->stopAllActionsByTag(3);
		//auto idleState = RepeatForever::create(animation("Idle", 0.12f));
		auto idleState = RepeatForever::create(makeAnimation("idle", 0.12f));
		idleState->setTag(1);
		this->runAction(idleState);
		this->isIdle = true;
		this->canMove = true;
		this->canChase = true;
	}
}

void Enemy::movingAnimation()
{
	if (this->mapNumber == 2 && this->waveNumber == 1) {
		//what does the tree do?
	}
	else if (this->canMove) {
		this->stopAllActionsByTag(1);
		this->stopAllActionsByTag(3);
		//auto anim = RepeatForever::create(animation("Moving", 0.12f));
		auto anim = RepeatForever::create(makeAnimation("moving", 0.12f));
		anim->setTag(3);
		this->runAction(anim);
		this->isMoving = true;
		canMove = false;
	}

}
void Enemy::chasing()
{
	float howFar = std::fabsf(ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width / 2));

	if (this->canChase && !this->isMoving && !this->isAttacking &&this->isChasing && howFar>skillRange-69) {
		float pppX = ppp->getPosition().x;
		this->canChase = false;
		this->isIdle = false;
		this->isMoving = true;
		this->breakTime = false;
		float moveByX = pppX - (this->getPosition().x + this->getContentSize().width / 2);
		if (moveByX < 0)  this->direction = 0;
		else this->direction = 1;
		this->movingAnimation();
		this->stopAllActionsByTag(4);

		if (this->mapNumber == 2 && this->waveNumber == 1) {
			//what a tree should do
		}
		else
		{
			auto move2 = Sequence::create(MoveTo::create(std::fabsf(moveByX)/moveSpeed, Vec2(ppp->getPosition().x, this->getPosition().y)),
				CallFunc::create([=]() 
			{this->canChase = true; this->isMoving = false; if (ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width*this->getScale() / 2) > visionRange + 200) this->isChasing = false; else this->idleStatus(); }), nullptr);
			//could need some fix?! nah
			move2->setTag(4);
			this->runAction(move2);
		}

	}
}
void Enemy::randomMoving() {

	if(this->waveNumber==1 || this->bossNumber==1)
	randomX = RandomHelper::random_real(listLineX.at(0), listLineX.at(1)); //di chuyen trong 1 khoang giua line1 va line2 trong tiledMap
	if(this->waveNumber==2 || this->bossNumber>1) randomX = RandomHelper::random_real(listLineX.at(2), listLineX.at(3));
	float eX = this->getPosition().x;
	float moveByX = randomX - eX;
	this->isIdle = false;
	this->stopAllActionsByTag(4);

	if (moveByX > 0) this->direction = 1; //done
	else {
		moveByX *= -1;
		this->direction = 0;
	}
	this->movingAnimation();
	if (this->mapNumber == 2 && this->waveNumber == 1) {
		//wat a tree shoud do
	}
	else
	{
		auto seq = Sequence::create(MoveTo::create(moveByX/moveSpeed, Vec2(randomX, this->getPosition().y)),
			CallFunc::create([=]() {this->isMoving = false; this->breakTime = false; }), /*DelayTime::create(1),*/ nullptr);
		seq->setTag(4);
		this->runAction(seq);
	}

}
void Enemy::moving() {
	float howFar = std::fabsf(ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width / 2));
	if (howFar < skillRange && !this->isAttacking && !this->isOnCD 
		&& std::fabsf(ppp->getPosition().y - this->getPosition().y) < 145 && (ppp->getPositionY() - (this->getPositionY() - 43))>0 && ppp->getPositionX() > spotPlayerLineLeft && ppp->getPositionX()<spotPlayerLineRight) {
		this->attack();
	}
	if (ppp->getPosition().y - this->getPosition().y > 145 || ppp->getPositionX() < spotPlayerLineLeft || ppp->getPositionX() > spotPlayerLineRight)
		this->isChasing = false;

	if (howFar < visionRange && !this->isChasing && !this->isAttacking && std::fabsf(ppp->getPosition().y - this->getPosition().y) < 145 && (ppp->getPositionY() - (this->getPositionY() - 43))>0 && ppp->getPositionX() > spotPlayerLineLeft && ppp->getPositionX()<spotPlayerLineRight) {
		this->isChasing = true;
	}
	else {
		if (!this->isMoving && !this->isAttacking && !this->isChasing && !this->breakTime) 
		{
			this->breakTime = true;
			this->idleStatus();
			this->runAction(Sequence::create(DelayTime::create(RandomHelper::random_real(0.5f,1.0f)), CallFunc::create([=]() {this->randomMoving(); }), nullptr));
		}
		else
		{
			this->chasing();
		}
	}

}


void Enemy::attack() {
	if (this->isSpawned) {
		float howFar = ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width / 2);
		if (howFar < 0) this->direction = 0;
		else this->direction = 1;
		//if (checkFrame("projectile"))
		//	this->spell->setFlippedX(this->isFlippedX());
		this->stopAllActions();
		this->isIdle = false;
		this->isAttacking = true;
		this->isOnCD = true;
		this->isMoving = false;
		this->canMove = true;
		this->breakTime = false;

		//this->runAction(Sequence::create(animation("SkillUsing", castSpeed),
		this->runAction(Sequence::create(makeAnimation("attack", castSpeed),
			CallFunc::create([=]() {this->canDamage = false; this->isAttacking = false; this->idleStatus(); }), nullptr));
		if (this->isCaster) {
			this->casterSpell();
		}
		if (this->isSSMobility) {
			this->mobilitySS();
		}
		if (!this->isCaster && !this->isSSMobility) {
			this->runAction(Sequence::create(DelayTime::create(castSpeed*norAtkDmgAfterF), CallFunc::create([=]() {this->canDamage = true; }),
				DelayTime::create(castSpeed*doneAtkAfterF), CallFunc::create([=]() {this->canDamage = false; }), nullptr));
		}

		this->attackHelper->runAction(Sequence::create(DelayTime::create(0.12*useSkillLastFN*skillCD), CallFunc::create([=]() {this->isOnCD = false; }), nullptr));
	}

}
void Enemy::casterSpell()
	{
	float range = 500.f;
	if (this->direction==0) range *= -1;
	float move2X = this->getPosition().x;
	if (this->direction==1) {
		move2X += this->getContentSize().width/2;
	}
	else move2X -= this->getContentSize().width / 2;
	if(this->waveNumber==2 && this->mapNumber == 1)
	this->spell->runAction(Sequence::create(
		MoveTo::create(0, Vec2(move2X, this->getPosition().y+this->getContentSize().height/2)),
		CallFunc::create([=]() {this->spell->setVisible(true); }),
		makeAnimation("projectile", castSpeed), CallFunc::create([=]() {this->spell->setVisible(false); this->attackLandedEffect(); }), nullptr));
	
	if (this->waveNumber == 1 && this->mapNumber == 1) {
			this->spell->runAction(Sequence::create(
				CallFunc::create([=]() {this->interuptable=true; }),
				DelayTime::create(castSpeed * 8),
				CallFunc::create([=]() {
			this->interuptable = false;
			this->spell->setPosition(move2X, this->getPosition().y+10);
			this->spell->setVisible(true); 
			this->spell->runAction(RepeatForever::create(makeAnimation("projectile", castSpeed)));
		}),
				MoveBy::create(0.69f, Vec2(range, 0)),
			CallFunc::create([=]() {this->spell->setVisible(false);  this->spell->setPosition(0, 0); }), nullptr));
	}


		//this->spell->runAction(RepeatForever::create(animation("Spell", castSpeed)));

}
void Enemy::mobilitySS()
{
	this->mobilityUsing = true;
	this->invulnerable = true;
	this->movementHelper->stopActionByTag(123);
	float howFar = ppp->getPosition().x - (this->getPosition().x + this->getContentSize().width / 2);
	if (howFar < 0) howFar *= -1;
	if (howFar < skillRange) {
		if (this->bossNumber == 1 && this->mapNumber == 1)
			this->runAction(Sequence::create(DelayTime::create(castSpeed*(mobilitySSAt + 1)),
				JumpTo::create(castSpeed*mobilitySpeed, Vec2(ppp->getPosition().x - this->getContentSize().width / 2, this->getPosition().y), 72, 1), CallFunc::create([=]() {this->mobilityUsing = false; this->canDamage = true;  this->invulnerable = false; }), nullptr));
		else
		this->runAction(Sequence::create(DelayTime::create(castSpeed*(mobilitySSAt + 1)),
			MoveTo::create(castSpeed*mobilitySpeed, Vec2(ppp->getPosition().x - this->getContentSize().width / 2, this->getPosition().y)), CallFunc::create([=]() {this->mobilityUsing = false; this->canDamage = true; this->invulnerable = false; }), nullptr));
	}
			
}
void Enemy::attackLandedEffect() {
	if (this->checkFrame("impact")) {
		this->spellLanded->setPosition(Vec2(ppp->getPosition().x, ppp->getPosition().y));
		this->spellLanded->runAction(Sequence::create(
			//CallFunc::create([=]() {this->canDamage = true; this->spellLanded->setVisible(true); }), animation("SkillLanded", 0.12f), CallFunc::create([=]() {this->canDamage = false; this->spellLanded->setVisible(false); this->spellLanded->setPosition(0, 0); }), nullptr));
		CallFunc::create([=]() {this->canDamage = true; this->spellLanded->setVisible(true); }), makeAnimation("impact", 0.12f), CallFunc::create([=]() {this->canDamage = false; this->spellLanded->setVisible(false); this->spellLanded->setPosition(0, 0); }), nullptr));
	}
}


void Enemy::getHit(int damage) {
	if (!this->isDead && this->isSpawned && !this->invulnerable) {
/*		if(this->mapNumber==1 || this->mapNumber==2) this->stopAllActions();
		else */
		this->forbidAllAction();
		this->canChase = true;
		this->isIdle = false;
		this->isAttacking = false;
		this->isMoving = false;
		this->getHitTime++;

		if (getHitTime == 3) {
			getHitTime = 0;
			this->invulnerable = true;
			auto doIt = Sequence::create(DelayTime::create(inviTime), CallFunc::create([=]() {this->invulnerable = false; }), nullptr);
			doIt->setTag(123);
			this->movementHelper->runAction(doIt);
		}

		SpriteFrame * hit = eeeFrames->getSpriteFrameByName(std::to_string(mapNumber) + std::to_string(waveNumber) 
			+ std::to_string(bossNumber) + std::to_string(this->direction) + "_hurt0.png");
		if(hit)
		this->setSpriteFrame(hit);
		experimental::AudioEngine::play2d("sounds/hit.mp3", false, 1.0f);
		int x = -16;
		if (ppp->getPositionX() - ppp->getContentSize().width / 2 < this->getPositionX()) 
		{
			x *= -1;
			this->direction = 0;
		}
		else this->direction = 1;
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
		this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() { this->idleStatus(); }), DelayTime::create(0.4f), CallFunc::create([=]() {this->isMoving=false; this->canMove = true; this->breakTime = false; }), nullptr));
	}

}

void Enemy::autoRespawn()
{
	if (this->canRespawn && this->bossNumber==0) 
	{
		float timeTillRespawn = RandomHelper::random_real(5.0f, 10.0f);
		//this->setVisible(true);
		//this->setOpacity(0);

		this->runAction(
			Sequence::create(
				DelayTime::create(timeTillRespawn),
				CallFunc::create([=]() {this->idleStatus(); }),
				FadeIn::create(1.0f), nullptr));

		this->runAction(
			Sequence::create(
				DelayTime::create(timeTillRespawn + 1.0f),
				CallFunc::create([=]() { 
			this->isDead = false; 
			this->isSpawned = true; 
			this->setHP(100);
			this->canRespawn = false; }), nullptr));
	}

}

void Enemy::dead() {
	if (this->waveNumber == 1) ppp->w1kills++;
	if (this->waveNumber == 2) ppp->w2kills++;
		this->isDead = true;
		this->isSpawned = false;
		this->canRespawn = true;
		this->canDrop = true;
		//this->stopAllActions();
		this->forbidAllAction();
		if(this->checkFrame("die"))
			this->runAction(Sequence::create(makeAnimation("die", 0.12f), FadeOut::create(0), CallFunc::create([=]() {this->autoRespawn(); }), nullptr));

		

		auto howFar = ppp->getPosition().x - this->getPosition().x;
		auto theX = 40.f;
		if (howFar > 0) theX *= -1;
		this->runAction(MoveBy::create(0.5, Vec2(theX, 0)));
		if (this->waveNumber == 1)
		{
			moneyRank = 1;
			ppp->currentEXP += 25;
		}
			
		if (this->waveNumber == 2) 
		{
			moneyRank = 1;
			ppp->currentEXP += 50;

		}
			
		if (this->bossNumber == 1)
		{
			moneyRank = 2;
			ppp->currentEXP += 150;

		}		
		if (this->bossNumber == 2) 
		{
			moneyRank = 3;
			ppp->currentEXP += 300;
		}
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
	action = "_" + action + "0.png";
	//auto checkSprite = Sprite::create(combination +"/"+ action);
	auto checkSprite = eeeFrames->getSpriteFrameByName
	(std::to_string(this->mapNumber) + std::to_string(this->waveNumber) + std::to_string(this->bossNumber) + std::to_string(this->direction) + action);
	if (checkSprite) return true;
	else return false;
}


void Enemy::update(float elapsed) 
{
	if(!this->isDead && this->isSpawned)
	moving();
	if (this->isChasing && !this->canChase && !this->isAttacking) {
		if (this->direction==0 && ppp->getPositionX() > this->getPositionX()+this->getContentSize().width)
		{
			this->stopAllActionsByTag(3);
			this->stopAllActionsByTag(4);
			if(!this->mobilityUsing)
			this->direction = 1;
			this->canChase = true;
			this->isMoving = false;
			this->idleStatus();
		}
		else if (this->direction==1 && ppp->getPositionX() < this->getPositionX() - this->getContentSize().width) {
			this->stopAllActionsByTag(3);
			this->stopAllActionsByTag(4);
			if(!this->mobilityUsing)
			this->direction = 0;
			this->canChase = true;
			this->isMoving = false;
			this->idleStatus();
		}
	}
}

Animate* Enemy::makeAnimation(std::string actionName, float timeEachFrame)
{
	std::string key = std::to_string(this->mapNumber) + std::to_string(this->waveNumber) + std::to_string(this->bossNumber) 
		+ std::to_string(this->direction) + "_" + actionName;

	Animate* anim = listAnimations.at(key);

	if (anim == nullptr)
	{
		Vector<SpriteFrame*> runningFrames;
		for (int i = 0; i < 99; i++)
		{
			auto frameName = key + std::to_string(i) + ".png";
			SpriteFrame* frame = eeeFrames->getSpriteFrameByName(frameName);
			if (!frame)
				break;
			runningFrames.pushBack(frame);
		}
		Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, timeEachFrame);

		anim = Animate::create(runningAnimation);

		listAnimations.insert(key, anim);
	}
	return anim;
}