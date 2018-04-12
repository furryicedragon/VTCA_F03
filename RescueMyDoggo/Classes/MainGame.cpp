#include "MainGame.h"
#include "ui\CocosGUI.h"
USING_NS_CC;



bool MainGame::init()
{
	if (!Node::init()) 
	{
		return false;
	}


	congratulation = Sprite::create();
	congratulation->setAnchorPoint(Vec2(0, 0));
	auto itsOKMan = RepeatForever::create(animation("Gratz", 1));
	this->congratulation->runAction(itsOKMan);
	if (congratulation)
		this->addChild(congratulation, 99999);
	this->congratulation->runAction(FadeOut::create(0));


	this->canRetry = false;
	this->enemyAdded = false;
	this->gameOver = Sprite::create("/Game Over/0.png");
	gameOver->setAnchorPoint(Vec2(0, 0));
	auto itsOverMan = RepeatForever::create(animation("Game Over",0.1f));
	this->gameOver->runAction(itsOverMan);
	this->gameOver->runAction(FadeOut::create(0));
	if(gameOver)
	this->addChild(gameOver, 99);
	this->isGameOver = false;

	currentMap = 1;
	currentWave = 1;
	currentBoss = 1;
	dashHelper = Sprite::create();
	if(dashHelper)
	this->addChild(dashHelper);
	count = 0;
	isRepeated = false;
	howManyKeyPressed = 0;
	visibleSize = Director::getInstance()->getVisibleSize();
	while(map1==nullptr)
	map1 = TMXTiledMap::create("map1.tmx");
	auto theTest = map1->getContentSize();
	//map1->setScale(1.6f);
	map1->setContentSize(map1->getContentSize()*map1->getScale()); //do nothing but helping *2 that's all
	if(map1)
	this->addChild(map1, 0, 33);
	auto oj = map1->getObjectGroup("Objects");
	auto sPoint = oj->getObject("SpawnPoint");
	float sPx = sPoint["x"].asFloat();
	float sPy = sPoint["y"].asFloat();
	finishPoint = oj->getObject("FinishPoint");
	meta = map1->getLayer("meta");
	while (ppp==nullptr) ppp = Player::create();
	ppp->map1Size = map1->getContentSize();
	ppp->mapScale = map1->getScale();
	ppp->setPosition(sPx*map1->getScale(), sPy*map1->getScale());
	ppp->setAnchorPoint(Vec2(0.5f, 0));
	//ppp->setScale(0.6f);
	ppp->setFlippedX(true);
	if(ppp!=nullptr)
	this->addChild(ppp, 2);



	this->ppp->statPlus = Label::create();
	if (ppp->statPlus) {
		this->ppp->addChild(ppp->statPlus);
		ppp->statPlus->setVisible(false);
	}
	this->ppp->level = Label::create();
	if (this->ppp->level) {
		//this->ppp->level->setScale(2.8f);
		this->ppp->level->setAnchorPoint(Vec2(0.5, 0));
		this->ppp->level->setString("1");
		this->ppp->level->setColor(Color3B(255, 255, 255));
		this->ppp->level->setSystemFontSize(16);
		this->ppp->level->setPosition(this->ppp->getContentSize().width/2.7, ppp->getContentSize().height-69);
		this->ppp->addChild(ppp->level);
	}

	this->setPosition(Vec2(0, 0));
	if(ppp->slash)
	this->addChild(ppp->slash,9);



	while(pppPositionHelper==nullptr) pppPositionHelper = Sprite::create("CloseNormal.png");
	pppPositionHelper->setOpacity(0);
	pppPositionHelper->setAnchorPoint(Vec2(0, 0));
	auto helperPos = Vec2((ppp->getContentSize().width - pppPositionHelper->getContentSize().width) / 2, ppp->getContentSize().height);
	pppPositionHelper->setPosition(helperPos);
	if(pppPositionHelper)
	this->addChild(pppPositionHelper);
	pppPositionHelper->setScale(0.5);
	pppPositionHelper->setAnchorPoint(Vec2(0.5, 0));

	this->startGame = Sprite::create("/Play/0.png");
	this->startGame->setAnchorPoint(Vec2(0, 0));
	auto playIt = RepeatForever::create(animation("Play", 0.5));
	this->startGame->runAction(playIt);
	if(startGame)
	this->addChild(startGame, 100);
	this->startGame->setPosition(0, 0);
	this->isGameStart = false;


	this->setupPressedKeyHandling();
	this->setupTouchHandling();
	
	this->scheduleUpdate();
	
	//this->addChild(ppp->skill1, 3);









	return true;
}

void MainGame::setupTouchHandling() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(MainGame::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool MainGame::onTouchBegan(Touch* touch, Event* event)
{
	auto hud_layer = static_cast<HUDLayer*> (Director::getInstance()->getRunningScene()->getChildByTag(9999));

	if (this->isGameOver && canRetry) {
		canRetry = false;
		this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=]() {this->delAll(); }), nullptr));
	}

	if (!this->isGameStart) {
		isGameStart = true;
		this->startGame->runAction(FadeOut::create(0.9f));
		this->spawnPlayer();
		hud_layer->toggleVisiblity();
	}
	return true;
}

void MainGame::setupPressedKeyHandling() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MainGame::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MainGame::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}
bool MainGame::keyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	if(keyCode==EventKeyboard::KeyCode::KEY_A || keyCode==EventKeyboard::KeyCode::KEY_D){
		howManyKeyPressed++;
		ppp->inputKeys.push_back(keyCode);
		ppp->isHoldingKey = true;
		whatYouWant(keyCode, 2);
		return true;	
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_SHIFT)
	{
		ppp->roll();
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
		this->gameOver->runAction(FadeOut::create(0.1f));
	}

 	//if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) ppp->useSkill(0);
	return true;
}
bool MainGame::keyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D) {
		int doTheMath = 0;
		for each (auto item in ppp->inputKeys)
		{
			if (item == keyCode) {
				ppp->inputKeys.erase(ppp->inputKeys.begin() + doTheMath);
				break;
			}
			doTheMath++;
		}
		if (howManyKeyPressed>0)
			howManyKeyPressed--;
		if (howManyKeyPressed > 0) whatYouWant(ppp->inputKeys[0], 2);

		if (howManyKeyPressed == 0) {
			ppp->isHoldingKey = false;
			whatYouWant(keyCode, 1);
		}
	}
	return true;
}
void MainGame::whatYouWant(EventKeyboard::KeyCode keyCode, int yourStatus) {
	switch (yourStatus)
	{
	case 1: //idle
	{
		ppp->idleStatus();
	}
	case 2: //moving
	{
		EventKeyboard::KeyCode theKey;
			theKey = keyCode;
			switch (theKey)
			{
			case cocos2d::EventKeyboard::KeyCode::KEY_A:
			{
				ppp->lastDuration = 0.25;
				ppp->lastX = -110;
				ppp->lastDirection = "Left";
				break;
			}
			case cocos2d::EventKeyboard::KeyCode::KEY_D:
			{
				ppp->lastDuration = 0.25;
				ppp->lastX = 110;
				ppp->lastDirection = "Right";
				break;
			}
			}
		break;
	}
	}

}

bool MainGame::checkCollision(Player* pPlayer)
{

	Point posDown = pPlayer->getPosition();
	Point posRight = pPlayer->getPosition();
	posRight.x += 44;
	Point posUp = pPlayer->getPosition();
	posUp.y += 44;
	Point posLeft = pPlayer->getPosition();
	posLeft.x -= 44;
	if (pPlayer->getPosition().y < 100) {
		posRight.y += 50;
		posLeft.y += 50;
		posDown.y += 25;
	}
	this->check4Directions(posRight, 1);
	this->check4Directions(posLeft, 3);
	
	return true;
}
void MainGame::check4Directions(Point posDirection, int directionClock) {
	int tileGID = meta->getTileGIDAt(this->tileCoordForPosition(posDirection));
	CCLOG("%d", tileGID);
	if (tileGID) {
		auto properties = map1->getPropertiesForGID(tileGID).asValueMap();

		if (!properties.empty()) {
			bool collide = properties["Collidable"].asBool();
			if (collide) {
				if (ppp->canMoveDirections[directionClock]) {
					ppp->isMoving = false;
				}
				ppp->canMoveDirections[directionClock] = false;
			}

			bool  collect = properties["Collectible"].asBool();
			if (collect) {
				//do something else 
				char* abc = "a";
			}

		}
	}
	if (tileGID == 0 && !ppp->canMoveDirections[directionClock])
		ppp->canMoveDirections[directionClock] = true;
}

Point MainGame::tileCoordForPosition(Point position) {
	int x = position.x/map1->getScale() / map1->getTileSize().width;
	int y = ((map1->getMapSize().height *map1->getScale() * map1->getTileSize().height) - (position.y*map1->getScale())) / (map1->getTileSize().height*map1->getScale());
	return Point(x, y);
}
void MainGame::updatePlayerPosition() {
	//if (ppp->isSpawn) {
		auto pppPos = this->ppp->getPosition();
		auto sth = fabsf(visibleSize.width / 2 - pppPos.y);
		auto movePos = this->pppPositionHelper->getPosition();
		movePos.x = MathUtil::lerp(movePos.x, pppPos.x, 0.8f);
		auto posY = visibleSize.height / 2;
		if (pppPos.y > visibleSize.height / 2) {
			if(pppPos.y>visibleSize.height)
			posY = visibleSize.height;
			else posY = pppPos.y;
		}
		movePos.y = MathUtil::lerp(movePos.y, posY, 0.8f);
		auto theLineRight = map1->getContentSize().width / 4*3;
		auto theLineLeft = map1->getContentSize().width / 4;
			auto followPlayer = Follow::create(pppPositionHelper,Rect::ZERO);
			followPlayer->setTag(99);
			//if ((pppPos.y < 145*map1->getScale() &&!ppp->isRolling) || ((pppPos.y < 145*map1->getScale()+33)&&ppp->isRolling)) 
			//	movePos.y = visibleSize.height/2;
		
			if (pppPos.x < theLineLeft) 
				movePos.x = theLineLeft;
			else if (pppPos.x > theLineRight) movePos.x = theLineRight;

			this->runAction(followPlayer);
			this->pppPositionHelper->setPosition(movePos);
	//}

}

void MainGame::update(float elapsed)
{
	auto hud_layer = static_cast<HUDLayer*> (Director::getInstance()->getRunningScene()->getChildByTag(9999));

	if(this->isGameStart)
	{
		if (ppp->isSpawn && !this->enemyAdded) {
			this->enemyAdded = true;
			this->allEnemyInit();
		}

		if (ppp->isSpawn && !ppp->isDead)
		{
			// joystick
			if (hud_layer->movementStick->getVelocity().x > 0)
			{
				whatYouWant(EventKeyboard::KeyCode::KEY_D, 2);
				this->canIdle = true;
				ppp->isHoldingKey = true;
			}
			if (hud_layer->movementStick->getVelocity().x < 0)
			{
				this->canIdle = true;
				whatYouWant(EventKeyboard::KeyCode::KEY_A, 2);
				ppp->isHoldingKey = true;
			}
			if (hud_layer->movementStick->getVelocity().x == 0)
			{
				if (canIdle) {
					canIdle = false;
					ppp->isHoldingKey = false;
					whatYouWant(EventKeyboard::KeyCode::KEY_YEN, 1);
				}

			}
			
			//HUD buttons
			if (hud_layer->attackBtn->getValue() && !ppp->usingSkill)
			{
				ppp->attack();
			}
			if (hud_layer->rollBtn->getValue() && !ppp->usingSkill)
			{
				ppp->roll();
			}
			if (hud_layer->skill1Btn->getValue() && !ppp->usingSkill)
			{
				ppp->useSkill(1, hud_layer->skill1Btn);
			}
			if (hud_layer->skill2Btn->getValue() && !ppp->usingSkill)
			{
				ppp->useSkill(0, hud_layer->skill2Btn);
			}
		}

		if (this->enemyAdded) {

			this->updatePlayerPosition();

			if (ppp->isHoldingKey && !ppp->isAttacking) {
				ppp->moving();
			}
			checkCollision(ppp);
			if(currentWave!=0)
			this->waveXMapXInit();
			if (allEnemy[9]->isDead && allEnemy[4]->isDead && !congratz) {
				for each (auto item in allEnemy)
				{
					congratz = true;
					item->canRespawn = false;
					item->setHP(0);
					item->dead();
					this->finishPortal = Sprite::create();
					this->addChild(finishPortal, 99);
					finishPortal->setVisible(true);
					finishPortal->setAnchorPoint(Vec2(0, 0));
					finishPortal->runAction(RepeatForever::create(animation("Enemies/Effect/Gate", 0.06f)));
					finishPortal->setPosition(Vec2(finishPoint["x"].asFloat()*this->map1->getScale(), ppp->getPosition().y));
					//finishPortal->setOpacity(222);
					//finishPortal->setScale(2.7f);
				}
			}
			if (congratz && ppp->isSpawn) {
				if (std::fabsf(ppp->getPosition().x - finishPortal->getPosition().x) < 10) {
					ppp->isSpawn = false;
					ppp->forbidAllAction();
					ppp->runAction(this->animation("MainChar/Win Boss", ppp->attackSpeed));
					this->congratulation->setPosition(Vec2(this->getPosition().x*-1,0));
					this->congratulation->runAction(FadeIn::create(1.6f));
					hud_layer->setVisible(false);
				}
			}
		}

		if (ppp->isDead && !this->isGameOver) {
			ppp->isSpawn = false;
			auto where2Put = pppPositionHelper->getPosition().x - visibleSize.width / 2;
			if (where2Put < 0) where2Put = 0;
			this->gameOver->setPosition(Vec2(where2Put,0));
			this->gameOver->runAction(FadeIn::create(2.0f));
			this->isGameOver = true;
			hud_layer->setVisible(false);
			this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=]() {this->canRetry=true; }), nullptr));
			this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=]() {this->canRetry=true;}), nullptr));
		}

		}



}

void MainGame::spawnPlayer()
{
	auto zap = Sprite::create();
	zap->setPosition(Vec2(ppp->getPosition().x, ppp->getPosition().y + 66));
	if(zap)
	this->addChild(zap);
	//zap->setScale(2);
	zap->runAction(Sequence::create(DelayTime::create(0.69f), CallFunc::create([=]() {ppp->spawnEffect(); }), animation("Spawn", 0.1f),
		CallFunc::create([=]() {this->removeChild(zap, true); }), nullptr));
}


bool MainGame::checkRange(Enemy* enemy2Check, int theRange) {

	auto itemWidth = enemy2Check->getContentSize().width*enemy2Check->getScale();
	auto howfarX = (enemy2Check->getPosition().x + itemWidth / 2) - ppp->getPosition().x;
		if ((howfarX < 69 && ppp->isFlippedX() && howfarX>-itemWidth / 2 - theRange)
			|| (howfarX < itemWidth / 2 + theRange && howfarX>-69 && !ppp->isFlippedX()))
			return true;
		else
			return false;
}
void MainGame::checkAttackRange(Enemy * eee, int index)
{
	if ((index != 4 || boss1)||(index!=9 || boss2)&&!ppp->isDead) {
		if (checkRange(eee,100)) 
		{
			if (ppp->isAttacking && !ppp->doneDamage[index]) {
				eee->getHit(ppp->damageCurrent);
				ppp->doneDamage[index] = true;
			}

			if (eee->canDamage && !ppp->isRolling && !eee->isCaster) {
				ppp->getHit(eee->skillDamage, eee->getPosition().x);
				eee->canDamage = false;
			}
		}

		int i = 0;
		float rectPos = ppp->listSkill.at(1)->getPosition().x;
		if (!ppp->listSkill.at(1)->isFlippedX()) 
			rectPos -= ppp->listSkill.at(1)->getContentSize().width/2;
		Rect skillRect = Rect(rectPos, ppp->listSkill.at(1)->getPosition().y, 300, 500);
		Rect eeeRect = eee->getBoundingBox();
		for each  (auto item in ppp->listSkill)
		{
			if (ppp->usingSkill && item->canDamage[index]
				&& (skillRect.intersectsRect(eee->getBoundingBox()) 
					|| (i != 1 && checkRange(eee,item->skillRange))))
			{
					eee->getHit(ppp->damageCurrent / 100 * item->skillDamage);
					item->canDamage[index] = false;
			}
			i++;
		}


		if (eee->isCaster && !ppp->isRolling && !eee->canDamage && std::fabsf(eee->spell->getPosition().x - ppp->getPosition().x) < 22 && eee->mapNumber==2) {
			eee->spell->setPosition(0, 0);
			eee->spell->setVisible(false);
			eee->spell->stopAllActions();
			eee->attackLandedEffect();
		}
		if (eee->isCaster && eee->canDamage && !ppp->isRolling && std::fabsf(eee->spellLanded->getPosition().x-ppp->getPosition().x)<9) {
			ppp->getHit(eee->skillDamage, eee->getPosition().x);
			eee->canDamage = false;
		}

		//if (ppp->skill1->launching)
		//{
		//	//check projectile collision
		//	if (ppp->skill1->getBoundingBox().intersectsRect(eee->getBoundingBox()) && ppp->skill1->canDamage[index])
		//	{
		//		ppp->skill1->canDamage[index] = false;

		//		eee->getHit(ppp->damageCurrent);
		//	}
		//}
	}

}
void MainGame::waveXMapXInit() {
	int i = 0;
		for each (auto item in allEnemy)
		{
			this->checkAttackRange(item, i);
			i++;
		}
		if (ppp->w1kills ==8 && !boss1) {
			this->spawnEffect(allEnemy[4], 1);
			boss1 = true;
		}
		if (ppp->w2kills == 8 && !boss2) { 
			this->spawnEffect(allEnemy[9], 1);
			boss2 = true; 
		}
}

void MainGame::spawnEffect(Enemy* enemy2Spawn,int index)
{
	enemy2Spawn->canSpawn=false;
	Vector<SpriteFrame *> runningFrames;
	Vector<SpriteFrame*> backwardsFrame;
	Vector<SpriteFrame*> loopFrame;
	for (int i = 0; i < 99; i++) {
		auto frameName = "/Enemies/Effect/Spawn/" + std::to_string(i) + ".png";
		Sprite* getSize = Sprite::create(frameName);
		if (!getSize)
			break;

		Size theSize = getSize->getContentSize();
		auto frame = SpriteFrame::create(frameName, Rect(0, 0, theSize.width, theSize.height));

		runningFrames.pushBack(frame);
		if(i>0)
		backwardsFrame.insert(0, frame);
		if (i < 4) loopFrame.pushBack(frame);
	}
	Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, 0.1234f);
	Animation* backwardsAnimation = Animation::createWithSpriteFrames(backwardsFrame, 0.1234f);
	Animate* animF = Animate::create(runningAnimation);
	Animate* animB = Animate::create(backwardsAnimation);
	for (int z = 0; z < 2; z++) {
		for (int j = 0; j < 4; j++)
		{
			loopFrame.pushBack(loopFrame.at(j));
		}
	}
	Animation* loopAnimation = Animation::createWithSpriteFrames(loopFrame, 0.1234f);
	Animate* animLoop = Animate::create(loopAnimation);

	auto spawnGate = Sprite::create();
	spawnGate->setScale(enemy2Spawn->getScale());
	spawnGate->setPosition(Vec2(enemy2Spawn->getPosition().x + enemy2Spawn->getContentSize().width*enemy2Spawn->getScale() / 2, enemy2Spawn->getPosition().y + enemy2Spawn->getContentSize().height*enemy2Spawn->getScale() / 2));
	if(spawnGate)
	this->addChild(spawnGate, 4);

	spawnGate->runAction(Sequence::create(DelayTime::create((index+1)/4),animB,animLoop, CallFunc::create([=]() {enemy2Spawn->setVisible(true); }), animF, CallFunc::create([=]() {this->removeChild(spawnGate); enemy2Spawn->isSpawned = true; }), nullptr));

}

void MainGame::allEnemyInit()
{
	auto oj = map1->getObjectGroup("Objects");
	auto line1 = oj->getObject("Line1");
	auto line2 = oj->getObject("Line2");
	auto line3 = oj->getObject("Line3");
	auto line4 = oj->getObject("Line4");
	for (int i = 0; i < 4; i++) {
		Enemy* wave = Enemy::create(2, 1, 0);
		//wave->setScale(1.6f);
		wave->skillDamage = 20;
		wave->visionRange = 310;
		wave->moveSpeed = 120;
		wave->isCaster = true;
		wave->castSpeed = 0.12f;
		wave->skillSpeed = 0.1f;
		wave->skillCD = 4;
		wave->skillRange = 300;
		wave->setHP(100);
		wave->initOption();
		wave->line1X = line1["x"].asFloat() *map1->getScale();
		wave->line2X = line2["x"].asFloat() *map1->getScale();
		wave->line3X = line3["x"].asFloat() *map1->getScale();
		wave->line4X = line4["x"].asFloat() *map1->getScale();
		wave->setPosition(RandomHelper::random_real(wave->line1X, wave->line2X), this->ppp->getPosition().y);
		wave->ppp = ppp;
		//wave->isSpawned = true;
		wave->setVisible(false);
		wave->isSpawned = false;
		if (wave->spell != nullptr) this->addChild(wave->spell, 9);
		if (wave->spellLanded != nullptr) this->addChild(wave->spellLanded, 9);
		wave->setAnchorPoint(Vec2(0, 0));
		allEnemy.push_back(wave);
		if (allEnemy[i]) {
			this->addChild(allEnemy[i], 0);
			this->spawnEffect(allEnemy[i],i);
		}
	}


	{	//boss 1
		this->boss1m1 = Enemy::create(1, 0, 1);
		boss1m1->visionRange = 420;
		boss1m1->skillDamage = 150;
		//boss1m1->setScale(1.6f);
		boss1m1->moveSpeed = 333;
		boss1m1->isSSMobility = true;
		boss1m1->castSpeed = 0.069f;
		boss1m1->skillCD = 2;
		boss1m1->skillRange = 400;
		boss1m1->mobilitySSAt = 3;
		boss1m1->mobilitySpeed = 4;
		boss1m1->setHP(300);
		boss1m1->initOption();
		auto boss1Pos = oj->getObject("Boss1");
		boss1m1->setPosition(Vec2(boss1Pos["x"].asFloat() *map1->getScale(), boss1Pos["y"].asFloat() *map1->getScale()));
		if (boss1m1->spell != nullptr) this->addChild(boss1m1->spell, 9);
		if (boss1m1->spellLanded != nullptr) this->addChild(boss1m1->spellLanded, 9);
		boss1m1->line1X = line1["x"].asFloat() *map1->getScale();
		boss1m1->line2X = line2["x"].asFloat() *map1->getScale();
		boss1m1->line3X = line3["x"].asFloat() *map1->getScale();
		boss1m1->line4X = line4["x"].asFloat() *map1->getScale();
		boss1m1->ppp = ppp;
		boss1m1->setVisible(false);
		boss1m1->isSpawned = false;
		boss1m1->setAnchorPoint(Vec2(0, 0));
		if (boss1m1) {
			allEnemy.push_back(boss1m1);
			this->addChild(allEnemy[4], 1);
		}
	}


	for (int i = 0; i < 4; i++) {
		Enemy* wave = Enemy::create(1, 2, 0);
		//wave->setScale(1.6f);
		wave->skillDamage = 96;
		wave->visionRange = 350;
		wave->moveSpeed = 196;
		wave->isCaster = true;
		wave->castSpeed = 0.12f;
		wave->skillSpeed = 0.1f;
		wave->skillCD = 4;
		wave->skillRange = 400;
		wave->setHP(175);
		wave->initOption();
		wave->line1X = line1["x"].asFloat() *map1->getScale();
		wave->line2X = line2["x"].asFloat() *map1->getScale();
		wave->line3X = line3["x"].asFloat() *map1->getScale();
		wave->line4X = line4["x"].asFloat() *map1->getScale();
		wave->setPosition(RandomHelper::random_real(wave->line3X, wave->line4X), ppp->getPosition().y);
		wave->ppp = ppp;
		wave->setVisible(false);
		wave->isSpawned = false;
		if (wave->spell != nullptr) this->addChild(wave->spell, 9);
		if (wave->spellLanded != nullptr) this->addChild(wave->spellLanded, 9);
		wave->setAnchorPoint(Vec2(0, 0));
		allEnemy.push_back(wave);
		if (allEnemy[i + 5]) {
			this->addChild(allEnemy[i + 5], 0);
			this->spawnEffect(allEnemy[i + 5], i);
		}
	}

	{	//boss2
		this->boss2m1 = Enemy::create(1, 0, 2);
		//boss2m1->setScale(1.6f);
		boss2m1->skillDamage = 200;
		boss2m1->visionRange = 450;
		boss2m1->moveSpeed = 420;
		boss2m1->isSSMobility = true;
		boss2m1->castSpeed = 0.041f;
		boss2m1->skillCD = 2;
		boss2m1->skillRange = 420;
		boss2m1->mobilitySSAt = 4;
		boss2m1->mobilitySpeed = 2;
		boss2m1->setHP(200);
		boss2m1->initOption();
		auto boss2Pos = oj->getObject("Boss2");
		boss2m1->setPosition(Vec2(boss2Pos["x"].asFloat() *map1->getScale(), boss2Pos["y"].asFloat() *map1->getScale()));
		if (boss2m1->spell != nullptr) this->addChild(boss2m1->spell, 9);
		if (boss2m1->spellLanded != nullptr) this->addChild(boss2m1->spellLanded, 9);
		boss2m1->line1X = line1["x"].asFloat() *map1->getScale();
		boss2m1->line2X = line2["x"].asFloat() *map1->getScale();
		boss2m1->line3X = line3["x"].asFloat() *map1->getScale();
		boss2m1->line4X = line4["x"].asFloat() *map1->getScale();
		boss2m1->ppp = ppp;
		boss2m1->setVisible(false);
		boss2m1->isSpawned = false;
		boss2m1->setAnchorPoint(Vec2(0, 0));
		if (boss2m1) {
			allEnemy.push_back(boss2m1);
			this->addChild(allEnemy[9], 1);
		}
	}

	{	//boss3
		this->bossfm1 = Enemy::create(1, 0, 3);
		bossfm1->norAtkDmgAfterF = 1;
		bossfm1->doneAtkAfterF = 4;
		bossfm1->skillDamage = 169;
		bossfm1->visionRange = 500;
		//bossfm1->setScale(3);
		bossfm1->moveSpeed = 555;
		bossfm1->castSpeed = 0.08f;
		bossfm1->skillCD = 2;
		bossfm1->skillRange = 120;
		bossfm1->setHP(500);
		bossfm1->initOption();
		auto boss3Pos = oj->getObject("Boss3");
		bossfm1->setPosition(Vec2(boss3Pos["x"].asFloat() *map1->getScale(), boss3Pos["y"].asFloat() *map1->getScale()));
		if (bossfm1->spell != nullptr) this->addChild(bossfm1->spell, 9);
		if (bossfm1->spellLanded != nullptr) this->addChild(bossfm1->spellLanded, 9);
		bossfm1->line1X = line1["x"].asFloat() *map1->getScale();
		bossfm1->line2X = line2["x"].asFloat() *map1->getScale();
		bossfm1->line3X = line3["x"].asFloat() *map1->getScale();
		bossfm1->line4X = line4["x"].asFloat() *map1->getScale();
		bossfm1->ppp = ppp;
		bossfm1->setVisible(false);
		bossfm1->isSpawned = false;
		bossfm1->setAnchorPoint(Vec2(0, 0));
		if(bossfm1)
		this->addChild(bossfm1, 1);
	}

	ppp->doneDamage.resize(allEnemy.size(),true);


	if (this->ppp->listSkill.size() < 1) {

		Point testPos = Point(this->ppp->getContentSize().width / 1.8, this->ppp->getContentSize().height / 2);
		ppp->listSkill.insert(0, Skill::create(300,169, 3, 3, 4 , 2, 2, testPos, "MainChar/Effects/Dash Stab","Dash/Dash Attack"));
		this->ppp->addChild(ppp->listSkill.at(0), 3);
		ppp->listSkill.at(0)->setVisible(false);
		//ppp->listSkill.at(0)->setScale(3);

		//Point skillPos = Point(ppp->getPosition().x,ppp->getPosition().y);
		ppp->listSkill.insert(1, Skill::create(333,129, 3, 3, 4, 2, 2, testPos, "MainChar/Effects/Skill 2", "Cast Spell/Cast Spell"));
		ppp->listSkill.at(1)->setAnchorPoint(Vec2(0.5, 0));
		this->addChild(ppp->listSkill.at(1), 6);
		ppp->listSkill.at(1)->setVisible(false);

	}
	for each (auto item in ppp->listSkill)
	{
		item->canDamage.resize(allEnemy.size(), false);
	}
}

Animate * MainGame::animation(std::string actionName,float timeEachFrame)
{
	Vector<SpriteFrame *> runningFrames;
	for (int i = 0; i < 99; i++) {
		auto frameName = "/"+actionName+"/" + std::to_string(i) + ".png";
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

void MainGame::delAll() 
{
	this->allEnemy.clear();
	this->removeAllChildren();


	congratulation = Sprite::create();
	congratulation->setAnchorPoint(Vec2(0, 0));
	auto itsOKMan = RepeatForever::create(animation("Gratz", 0.69f));
	this->congratulation->runAction(itsOKMan);
	if (congratulation)
		this->addChild(congratulation, 99999);
	this->congratulation->runAction(FadeOut::create(0));

	auto hud_layer = static_cast<HUDLayer*> (Director::getInstance()->getRunningScene()->getChildByTag(9999));

	this->stopActionByTag(99); 
	this->setPosition(Vec2(0, 0));
	this->enemyAdded = false;
	this->gameOver = Sprite::create("/Game Over/0.png");
	gameOver->setAnchorPoint(Vec2(0, 0));
	auto itsOverMan = RepeatForever::create(animation("Game Over", 0.1f));
	this->gameOver->runAction(itsOverMan);
	this->gameOver->runAction(FadeOut::create(0));
	if(gameOver)
	this->addChild(gameOver, 99);
	this->isGameOver = false;

	currentMap = 1;
	currentWave = 1;
	currentBoss = 1;
	dashHelper = Sprite::create();
	if(dashHelper)
	this->addChild(dashHelper);
	count = 0;
	isRepeated = false;
	howManyKeyPressed = 0;
	visibleSize = Director::getInstance()->getVisibleSize();
		map1 = TMXTiledMap::create("map1.tmx");
	auto theTest = map1->getContentSize();
	map1->setScale(1.6f);
	map1->setContentSize(map1->getContentSize() *map1->getScale()); //do nothing but helping *2 that's all
	if(map1)
	this->addChild(map1, 0, 33);
	auto oj = map1->getObjectGroup("Objects");
	auto sPoint = oj->getObject("SpawnPoint");
	float sPx = sPoint["x"].asFloat();
	float sPy = sPoint["y"].asFloat();
	auto fPoint = oj->getObject("FinishPoint");
	meta = map1->getLayer("meta");
	ppp = Player::create();
	ppp->map1Size = map1->getContentSize();
	ppp->mapScale = map1->getScale();
	ppp->setPosition(sPx *map1->getScale(), sPy *map1->getScale());
	ppp->setAnchorPoint(Vec2(0.5, 0));
	ppp->setScale(0.6f);
	ppp->setFlippedX(true);
	if (ppp != nullptr)
		this->addChild(ppp, 2);
	this->ppp->statPlus = Label::create();
	if (ppp->statPlus) {
		this->ppp->addChild(ppp->statPlus);
		ppp->statPlus->setVisible(false);
	}
	this->ppp->level = Label::create();
	if (this->ppp->level) {
		this->ppp->level->setScale(2.8f);
		this->ppp->level->setAnchorPoint(Vec2(0.5, 0));
		this->ppp->level->setString("1");
		this->ppp->level->setColor(Color3B(255, 255, 255));
		this->ppp->level->setSystemFontSize(16);
		this->ppp->level->setPosition(this->ppp->getContentSize().width / 2.7, ppp->getContentSize().height - 69);
		this->ppp->addChild(ppp->level);
	}
	if(ppp->slash)
	this->addChild(ppp->slash, 9);

	pppPositionHelper = Sprite::create("CloseNormal.png");
	pppPositionHelper->setOpacity(0);
	pppPositionHelper->setAnchorPoint(Vec2(0, 0));
	auto helperPos = Vec2((ppp->getContentSize().width - pppPositionHelper->getContentSize().width) / 2, ppp->getContentSize().height);
	pppPositionHelper->setPosition(helperPos);
	if(pppPositionHelper)
	this->addChild(pppPositionHelper);
	pppPositionHelper->setScale(0.5);
	pppPositionHelper->setAnchorPoint(Vec2(0.5, 0));

	this->startGame = Sprite::create("/Play/0.png");
	this->startGame->setAnchorPoint(Vec2(0, 0));
	auto playIt = RepeatForever::create(animation("Play", 0.5));
	this->startGame->runAction(playIt);
	if(startGame)
	this->addChild(startGame, 100);
	this->isGameStart = false;

	ppp->w1kills = 0;
	ppp->w2kills = 0;
	boss1 = false;
	boss2 = false;

	//this->addChild(ppp->skill1, 3);
	this->updatePlayerPosition();
	//hud_layer->setupStick();
	hud_layer->skill1Btn->getChildByTag(100)->setVisible(false);
	hud_layer->skill2Btn->getChildByTag(100)->setVisible(false);
	this->scheduleUpdate();
	//this->scheduleUpdate();
}