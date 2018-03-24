#include "MainGame.h"
#include "ui\CocosGUI.h"
USING_NS_CC;


bool MainGame::init()
{
	if (!Node::init()) 
	{
		return false;
	}
	this->enemyAdded = false;
	this->gameOver = Sprite::create("/Game Over/0.png");
	gameOver->setAnchorPoint(Vec2(0, 0));
	auto itsOverMan = RepeatForever::create(animation("Game Over",0.1));
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
	map1->setScale(2);
	map1->setContentSize(map1->getContentSize()*2); //do nothing but helping *2 that's all
	if(map1)
	this->addChild(map1, 0, 33);
	auto oj = map1->getObjectGroup("Objects");
	auto sPoint = oj->getObject("SpawnPoint");
	float sPx = sPoint["x"].asFloat();
	float sPy = sPoint["y"].asFloat();
	auto fPoint = oj->getObject("FinishPoint");
	meta = map1->getLayer("meta");
	while (ppp==nullptr) ppp = Player::create();
	ppp->map1Size = map1->getContentSize();
	ppp->setPosition(sPx*2, sPy*2);
	ppp->setAnchorPoint(Vec2(0.5, 0));
	ppp->setScale(0.6);
	ppp->setFlippedX(true);
	if(ppp!=nullptr)
	this->addChild(ppp, 2);
	this->setPosition(Vec2(-map1->getContentSize().width / 2, 0));
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
	this->startGame->setPosition(Point(map1->getContentSize().width/2, 0));
	this->isGameStart = false;


	this->setupPressedKeyHandling();
	this->setupTouchHandling();
	this->scheduleUpdate();
	//return true;
}

void MainGame::setupTouchHandling() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(MainGame::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}
bool MainGame::onTouchBegan(Touch* touch, Event* event)
{
	if(this->isGameOver)
		this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create([=]() {this->delAll(); }),nullptr));

	if (!this->isGameStart) {
		isGameStart = true;
		this->startGame->runAction(FadeOut::create(0.9));
		this->spawnPlayer();
	}
	ppp->attack();
	return true;
}

void MainGame::setupPressedKeyHandling() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MainGame::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MainGame::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}
bool MainGame::keyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	if(keyCode==EventKeyboard::KeyCode::KEY_A || keyCode==EventKeyboard::KeyCode::KEY_S || keyCode==EventKeyboard::KeyCode::KEY_D || keyCode==EventKeyboard::KeyCode::KEY_W){
		if (howManyKeyPressed == 0) ppp->notCombination = true;
		howManyKeyPressed++;
		ppp->inputKeys.push_back(keyCode);
		ppp->isHoldingKey = true;
		this->checkIfRepeated(keyCode);
		if(!ppp->canDash &&this->isRepeated)
		ppp->checkDash();
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
}
void MainGame::checkIfRepeated(EventKeyboard::KeyCode keyCode) {
	if (repeatedKeys.size() == 0) {
		repeatedKeys.push_back(keyCode);
		this->isRepeated = false;
	}
	else
		if (keyCode == repeatedKeys[0]) this->isRepeated=true;
		else this->isRepeated = false;
		this->dashHelper->runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create([=]() {this->repeatedKeys.clear(); }), nullptr));
}
bool MainGame::keyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_W) {
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
		if (howManyKeyPressed > 0){
			if (howManyKeyPressed == 1) 
				ppp->notCombination = true;
			whatYouWant(ppp->inputKeys[0], 2);
		}

		if (howManyKeyPressed == 0) {
			ppp->isHoldingKey = false;
			whatYouWant(keyCode, 1);
			ppp->notCombination = true;
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
		//if (ppp->notCombination == false) {
		//	ppp->idleStatus(ppp->lastDirection);
		//}
		//else
		//{
		//	switch (keyCode) //last was keyCode
		//	{
		//	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		//		ppp->idleStatus("Left");
		//		break;
		//	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		//		ppp->idleStatus("Right");
		//		break;
		//	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		//		ppp->idleStatus("Down");
		//		break;
		//	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		//		ppp->idleStatus("Up");
		//		break;
		//	}
		//}
	}
	case 2: //moving
	{
		//if (count == 0)
		//	char* abc = "ok";
		//else count++;
		if (ppp->inputKeys.size()>1)
			this->notTheSameKey();
		if ((ppp->inputKeys.size() > 1 && ppp->notCombination == false) || (ppp->inputKeys.size()>2)) {
			this->checkCombinationMovement();
		}
		else
		{
			EventKeyboard::KeyCode theKey;
			if (ppp->inputKeys.size() > 1) {
				theKey = ppp->inputKeys[1];
			}
			else
			{
				theKey = keyCode;
			}
			switch (theKey)
			{
			case cocos2d::EventKeyboard::KeyCode::KEY_A:
			{
				ppp->lastDuration = 0.25;
				ppp->lastX = -45;
				ppp->lastY = 0;
				if (ppp->notCombination == true)
					ppp->lastDirection = "Left";
				break;
			}
			case cocos2d::EventKeyboard::KeyCode::KEY_D:
			{
				ppp->lastDuration = 0.25;
				ppp->lastX = 45;
				ppp->lastY = 0;
				if (ppp->notCombination == true)
					ppp->lastDirection = "Right";
				break;
			}
			case cocos2d::EventKeyboard::KeyCode::KEY_S:
			{
				/*ppp->lastDuration = 0.25;
				ppp->lastX = 0;
				ppp->lastY = -45;
				if (ppp->notCombination == true)
					ppp->lastDirection = "Down";*/
				break;
			}
			case cocos2d::EventKeyboard::KeyCode::KEY_W:
			{
				/*ppp->lastDuration = 0.25;
				ppp->lastX = 0;
				ppp->lastY = 45;
				if (ppp->notCombination == true)
					ppp->lastDirection = "Up";*/
				break;
			}
			}
		}
		//ppp->moving();

		break;
	}
	}

}
void MainGame::checkCombinationMovement() {
	EventKeyboard::KeyCode key1 = ppp->inputKeys[0];
	EventKeyboard::KeyCode key2 = ppp->inputKeys[1];
	EventKeyboard::KeyCode a = EventKeyboard::KeyCode::KEY_A;
	EventKeyboard::KeyCode s = EventKeyboard::KeyCode::KEY_S;
	EventKeyboard::KeyCode d = EventKeyboard::KeyCode::KEY_D;
	EventKeyboard::KeyCode w = EventKeyboard::KeyCode::KEY_W;
	if (ppp->notCombination == true && ppp->inputKeys.size() > 2) {
		key1 = ppp->inputKeys[1];
		key2 = ppp->inputKeys[2];
	}
}
void MainGame::notTheSameKey() {
	EventKeyboard::KeyCode key1 = ppp->inputKeys[0];
	EventKeyboard::KeyCode key2 = ppp->inputKeys[1];
	EventKeyboard::KeyCode a = EventKeyboard::KeyCode::KEY_A;
	EventKeyboard::KeyCode s = EventKeyboard::KeyCode::KEY_S;
	EventKeyboard::KeyCode d = EventKeyboard::KeyCode::KEY_D;
	EventKeyboard::KeyCode w = EventKeyboard::KeyCode::KEY_W;
	if ((key1 == a || key2 == a) && (key1 == d || key2 == d))
		ppp->notCombination = true;
	else ppp->notCombination = false;
	if (ppp->notCombination == false) {
		if ((key1 == w || key2 == w) && (key1 == s || key2 == s))
			ppp->notCombination = true;
		else ppp->notCombination = false;
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
	//int tileGID = meta->getTileGIDAt(this->tileCoordForPosition(posDown));
	//CCLOG("%d", tileGID);
	//if (tileGID) {
	//	auto properties = map1->getPropertiesForGID(tileGID).asValueMap();

	//	if (!properties.empty()) {
	//		bool collide = properties["Collidable"].asBool();
	//		if (collide) {
	//			if (ppp->canMoveDown) {
	//			ppp->stopAllActionsByTag(3);
	//			ppp->moving();
	//			}
	//			ppp->canMoveDown = false;
	//			return true; //-----> need to ask
	//		}

	//		bool  collect = properties["Collectible"].asBool();
	//		if (collect) {
	//			//do something else 
	//			return true;
	//		}

	//	}
	//}
	//if (tileGID == 0 && !ppp->canMoveDown) ppp->canMoveDown = true;
	
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
	int x = position.x/2 / map1->getTileSize().width;
	int y = ((map1->getMapSize().height * map1->getTileSize().height) - (position.y/2)) / map1->getTileSize().height;
	return Point(x, y);
}
void MainGame::updatePlayerPosition() {
	auto pppPos = this->ppp->getPosition();
	auto movePos = this->pppPositionHelper->getPosition();
	movePos.x = MathUtil::lerp(movePos.x, pppPos.x, 0.8);
	movePos.y = MathUtil::lerp(movePos.y, pppPos.y + 168, 0.8);
	auto theLineRight = map1->getContentSize().width - map1->getContentSize().width / 4;
	auto theLineLeft = map1->getContentSize().width / 4;
	auto theLineDown = map1->getContentSize().height / 8;
		auto followPlayer = Follow::create(pppPositionHelper,Rect::ZERO);
		followPlayer->setTag(99);
		if (pppPos.y < 145*map1->getScale()) 
			movePos.y = visibleSize.height/2;
		
		if (pppPos.x < theLineLeft) movePos.x = theLineLeft;
		else if (pppPos.x > theLineRight) movePos.x = theLineRight;

		this->runAction(followPlayer);
		this->pppPositionHelper->setPosition(movePos);
}

void MainGame::update(float elapsed)
{
	if(this->isGameStart)
	{
		if (ppp->isSpawn && !this->enemyAdded) {
			this->enemyAdded = true;
			this->allEnemyInit();
		}

		if (this->enemyAdded) {
			if(!this->isGameOver)
				if (ppp->statUpBox.size() > 0 && ppp->canShowStatUp) {
					ppp->statUpBox[0]->setVisible(true);
					ppp->statUpBox[0]->runAction(Sequence::create(CallFunc::create([=]() {ppp->canShowStatUp=false; }), MoveBy::create(0.69, Vec2(0, 50)),
						CallFunc::create([=]() {ppp->removeChild(ppp->statUpBox[0], true); ppp->statUpBox.erase(ppp->statUpBox.begin()); ppp->canShowStatUp = true; }), nullptr));
				}

			this->updatePlayerPosition();
			this->doTheMath();

			if (ppp->isHoldingKey && !ppp->isAttacking) {
				ppp->moving();
			}
			checkCollision(ppp);
			if(currentWave!=0)
			this->waveXMapXInit();
			this->waveSpawn();
		}

		if (ppp->isDead && !this->isGameOver) {
			auto where2Put = pppPositionHelper->getPosition().x - visibleSize.width / 2;
			if (where2Put < 0) where2Put = 0;
			this->gameOver->setPosition(Vec2(where2Put,0));
			this->gameOver->runAction(FadeIn::create(3.0f));
			this->isGameOver = true;
		}
	}



}
void MainGame::doTheMath()
{
	if (currentMap == 1 && !this->checkDeath(map1Wave2) && boss1m1->isDead && this->currentWave != 2) this->currentWave = 2;
	if (currentMap == 1 && this->checkDeath(map1Wave2) && !boss2m1->isDead && this->currentBoss != 2) this->currentBoss = 2;
	if (currentMap == 1 && boss2m1->isDead && !bossfm1->isDead && this->currentBoss != 3) this->currentBoss = 3;

	if (currentMap == 2 && !this->checkDeath(map2Wave1) && bossfm1->isDead && this->currentBoss != 1) {
		this->currentBoss = 1;
		this->currentWave = 1;
	}

	if (currentMap == 2 && !this->checkDeath(map2Wave2) && boss1m2->isDead && this->currentWave != 2) this->currentWave = 2;
	if (currentMap == 2 && this->checkDeath(map2Wave2) && !boss2m2->isDead && this->currentWave != 2) this->currentBoss = 2;
	if (currentMap == 2 && boss2m2->isDead && !bossfm2->isDead && this->currentBoss != 3) this->currentBoss = 3;
}

void MainGame::spawnPlayer()
{
	auto zap = Sprite::create();
	zap->setPosition(Vec2(ppp->getPosition().x, ppp->getPosition().y + 66));
	if(zap)
	this->addChild(zap);
	zap->setScale(2);
	zap->runAction(Sequence::create(DelayTime::create(0.69), CallFunc::create([=]() {ppp->spawnEffect(); }), animation("Spawn", 0.1),
		CallFunc::create([=]() {this->removeChild(zap, true); }), nullptr));
}


void MainGame::checkAttackRange(Enemy * eee, int index)
{
	auto itemWidth = eee->getContentSize().width*eee->getScale();
	auto howfarX = ppp->getPosition().x - (eee->getPosition().x + itemWidth / 2);
	auto howfarY = ppp->getPosition().y - eee->getPosition().y;
	if (howfarX < 0) howfarX *= -1;
	if (howfarY < 0) howfarY *= -1;
	if (howfarX < itemWidth + 69 && howfarY < 33) {
		if (ppp->isAttacking && !ppp->doneDamage[index]) {
			eee->getHit(ppp->damageCurrent);
			ppp->doneDamage[index] = true;
		}

		if (eee->canDamage && !ppp->isRolling) { 
			ppp->getHit(eee->skillDamage, eee->getPosition().x); 
			eee->canDamage = false;
		}
	}
	if (!this->checkDeath(map1Wave1) || (!this->checkDeath(map1Wave2) && boss1m1->isDead)) {
		if (eee->canDamage && !ppp->isRolling) {
			ppp->getHit(eee->skillDamage, eee->getPosition().x);
			eee->canDamage = false;
		}
	}

}
void MainGame::waveXMapXInit() {
	std::vector<Enemy*> waveXMapX;
	if (currentMap == 1 && currentWave == 1) waveXMapX = map1Wave1;
	else if (currentMap == 1 && currentWave == 2) waveXMapX = map1Wave2;
	else if (currentMap == 2 && currentWave == 1) waveXMapX = map2Wave1;
	else if (currentMap == 2 && currentWave == 2) waveXMapX = map2Wave2;

	Enemy* boss;
	if (currentMap == 1 && currentBoss == 1) boss = boss1m1;
	if (currentMap == 1 && currentBoss == 2) boss = boss2m1;
	if (currentMap == 1 && currentBoss == 3) boss = bossfm1;
	if (currentMap == 2 && currentBoss == 1) boss = boss1m2;
	if (currentMap == 2 && currentBoss == 2) boss = boss2m2;
	if (currentMap == 2 && currentBoss == 3) boss = bossfm2;

	nextWave(waveXMapX,boss);
	int i = 0;
	if (!this->checkDeath(waveXMapX)) {
		for each (auto item in waveXMapX)
		{
			if (item->getPosition().y > ppp->getPosition().y) item->setZOrder(1);
			if (item->getPosition().y < ppp->getPosition().y) item->setZOrder(3);
			if (item->getPosition().y == ppp->getPosition().y) item->setZOrder(2);
			this->checkAttackRange(item, i);
			i++;
		}
	}
	else if (boss->isSpawned)
		this->checkAttackRange(boss, i);

}
void MainGame::nextWave(std::vector<Enemy*> waveXMapX,Enemy* boss)
{
		if (this->checkDeath(waveXMapX) && waveXMapX.size()!=0 && boss->canSpawn) {
			this->bossSpawn();
		}
}

void MainGame::waveSpawn() {
	if (this->checkSpawn(map1Wave2) && boss1m1->isDead) {
		int i = 0;
		for each (auto item in this->map1Wave2) {
			this->spawnEffect(item,i);
			i++;
		}
		currentWave = 2;
	}
	if (this->checkSpawn(map2Wave1) && currentMap==2 && bossfm1->isDead) {
		int i = 0;
		for each (auto item in this->map2Wave1) {
			this->spawnEffect(item,i);
			i++;
		}
		currentWave = 1;
	}
	if (this->checkSpawn(map2Wave2) && currentMap==2 && boss1m2->isDead) {
		int i = 0;
		for each (auto item in this->map2Wave2) {
			this->spawnEffect(item,i);
			i++;
		}
		currentWave = 2;
	}
}
void MainGame::bossSpawn()
{
	if (this->checkDeath(map1Wave1) && !boss1m1->isSpawned) {
		this->spawnEffect(boss1m1,0);
	}
	if (this->checkDeath(map1Wave2) && !boss2m1->isSpawned) {
		this->spawnEffect(boss2m1,0);
		this->currentBoss = 2;
	}
	if (boss2m1->isDead && !bossfm1->isSpawned) {
		this->spawnEffect(bossfm1,0);
		this->currentBoss = 3;
	}

	//if (currentMap == 2 && this->checkDeath(map2Wave1) && !boss1m2->isSpawned) {
	//	this->spawnEffect(boss1m2,0);
	//	//boss1m2->canSpawn = false;
	//}
	//if (this->checkDeath(map2Wave2) && currentMap == 2 && !boss2m2->isSpawned) {
	//	this->spawnEffect(boss2m2,0);
	//	//boss2m2->canSpawn = false;
	//	this->currentBoss = 2;
	//}

	//if (boss2m2->isDead&&currentMap == 2 && !bossfm2->isSpawned) {
	//	this->spawnEffect(bossfm2,0);
	//	//bossfm2->canSpawn = false;
	//	this->currentBoss = 3;
	//}
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
	Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, 0.1234);
	Animation* backwardsAnimation = Animation::createWithSpriteFrames(backwardsFrame, 0.1234);
	Animate* animF = Animate::create(runningAnimation);
	Animate* animB = Animate::create(backwardsAnimation);
	for (int z = 0; z < 2; z++) {
		for (int j = 0; j < 4; j++)
		{
			loopFrame.pushBack(loopFrame.at(j));
		}
	}
	Animation* loopAnimation = Animation::createWithSpriteFrames(loopFrame, 0.1234);
	Animate* animLoop = Animate::create(loopAnimation);

	auto spawnGate = Sprite::create();
	spawnGate->setScale(enemy2Spawn->getScale());
	spawnGate->setPosition(Vec2(enemy2Spawn->getPosition().x + enemy2Spawn->getContentSize().width*enemy2Spawn->getScale() / 2, enemy2Spawn->getPosition().y + enemy2Spawn->getContentSize().height*enemy2Spawn->getScale() / 2));
	if(spawnGate)
	this->addChild(spawnGate, 4);

	spawnGate->runAction(Sequence::create(DelayTime::create((index+1)/4),animB,animLoop, CallFunc::create([=]() {enemy2Spawn->setVisible(true); }), animF, CallFunc::create([=]() {this->removeChild(spawnGate); enemy2Spawn->isSpawned = true; }), nullptr));
}

bool MainGame::checkDeath(std::vector<Enemy*> what2Check) {
	int i = 0;
	for each (auto item in what2Check)
	{
		if (item->isDead) i++;
	}
	if (i == what2Check.size()) return true;
	else return false;
}
bool MainGame::checkSpawn(std::vector<Enemy*> what2Check)
{
	if (what2Check.size() > 0) {
		int i = 0;
		for each (auto item in what2Check)
		{
			if (item->canSpawn) i++;
		}
		if (i == what2Check.size()) return true;
		else return false;
	}
	else return false;

}

void MainGame::allEnemyInit()
{
	auto oj = map1->getObjectGroup("Objects");
	auto line1 = oj->getObject("Line1");
	auto line2 = oj->getObject("Line2");
	auto line3 = oj->getObject("Line3");
	for (int i = 0; i < 8; i++) {
		Enemy* wave = Enemy::create(1, 1, 0);
		wave->setScale(1.6);
		wave->skillDamage = 20;
		wave->visionRange = 310;
		wave->moveSpeed = 120;
		wave->isCaster = true;
		wave->castSpeed = 0.12;
		wave->skillSpeed = 0.1;
		wave->skillCD = 4;
		wave->skillRange = 300;
		wave->setHP(100);
		wave->initOption();
		wave->line1X = line1["x"].asFloat() * 2;
		wave->line2X = line2["x"].asFloat() * 2;
		wave->line3X = line3["x"].asFloat() * 2;
		wave->setPosition(RandomHelper::random_real(wave->line2X, wave->line1X), RandomHelper::random_real(50.f, 129*map1->getScale()));
		wave->ppp = ppp;
		//wave->isSpawned = true;
		wave->setVisible(false);
		wave->isSpawned = false;
		if (wave->spell != nullptr) this->addChild(wave->spell, 9);
		if (wave->spellLanded != nullptr) this->addChild(wave->spellLanded, 9);
		wave->setAnchorPoint(Vec2(0, 0));
		map1Wave1.push_back(wave);
		if (map1Wave1[i]) {
			this->addChild(map1Wave1[i], 0);
			this->spawnEffect(map1Wave1[i],i);
		}
	}

	for (int i = 0; i < 8; i++) {
		Enemy* wave = Enemy::create(1, 2, 0);
		wave->setScale(1.6);
		wave->skillDamage = 96;
		wave->visionRange = 350;
		wave->moveSpeed = 196;
		wave->isCaster = true;
		wave->castSpeed = 0.12;
		wave->skillSpeed = 0.1;
		wave->skillCD = 4;
		wave->skillRange = 400;
		wave->setHP(100);
		wave->initOption();
		wave->line1X = line1["x"].asFloat() * 2;
		wave->line2X = line2["x"].asFloat() * 2;
		wave->line3X = line3["x"].asFloat() * 2;
		wave->setPosition(RandomHelper::random_real(wave->line3X, wave->line2X), RandomHelper::random_real(50.f, 129 * map1->getScale()));
		wave->ppp = ppp;
		wave->setVisible(false);
		wave->isSpawned = false;
		if (wave->spell != nullptr) this->addChild(wave->spell, 9);
		if (wave->spellLanded != nullptr) this->addChild(wave->spellLanded, 9);
		wave->setAnchorPoint(Vec2(0, 0));
		map1Wave2.push_back(wave);
		if(map1Wave2[i])
		this->addChild(map1Wave2[i], 0);
	}

	{	//boss 1
		this->boss1m1 = Enemy::create(1, 0, 1);
		boss1m1->visionRange = 420;
		boss1m1->skillDamage = 150;
		boss1m1->setScale(1.6);
		boss1m1->moveSpeed = 333;
		boss1m1->isSSMobility = true;
		boss1m1->castSpeed = 0.069;
		boss1m1->skillCD = 2;
		boss1m1->skillRange = 400;
		boss1m1->mobilitySSAt = 3;
		boss1m1->mobilitySpeed = 4;
		boss1m1->setHP(250);
		boss1m1->initOption();
		auto boss1Pos = oj->getObject("Boss1");
		boss1m1->setPosition(Vec2(boss1Pos["x"].asFloat()*2,boss1Pos["y"].asFloat()*2));
		if (boss1m1->spell != nullptr) this->addChild(boss1m1->spell, 9);
		if (boss1m1->spellLanded != nullptr) this->addChild(boss1m1->spellLanded, 9);
		boss1m1->line1X = line1["x"].asFloat() * 2;
		boss1m1->line2X = line2["x"].asFloat() * 2;
		boss1m1->line3X = line3["x"].asFloat() * 2;
		boss1m1->ppp = ppp;
		boss1m1->setVisible(false);
		boss1m1->isSpawned = false;
		boss1m1->setAnchorPoint(Vec2(0, 0));
		if(boss1m1)
		this->addChild(boss1m1, 1);
	} 
	{	//boss2
		this->boss2m1 = Enemy::create(1, 0, 2);
		boss2m1->setScale(1.6);
		boss2m1->skillDamage = 222;
		boss2m1->visionRange = 450;
		boss2m1->moveSpeed = 500;
		boss2m1->isSSMobility = true;
		boss2m1->castSpeed = 0.041;
		boss2m1->skillCD = 2;
		boss2m1->skillRange = 420;
		boss2m1->mobilitySSAt = 2;
		boss2m1->mobilitySpeed = 2;
		boss2m1->setHP(180);
		boss2m1->initOption();
		auto boss2Pos = oj->getObject("Boss2");
		boss2m1->setPosition(Vec2(boss2Pos["x"].asFloat() * 2, boss2Pos["y"].asFloat() * 2));
		if (boss2m1->spell != nullptr) this->addChild(boss2m1->spell, 9);
		if (boss2m1->spellLanded != nullptr) this->addChild(boss2m1->spellLanded, 9);
		boss2m1->line1X = line1["x"].asFloat() * 2;
		boss2m1->line2X = line2["x"].asFloat() * 2;
		boss2m1->line3X = line3["x"].asFloat() * 2;
		boss2m1->ppp = ppp;
		boss2m1->setVisible(false);
		boss2m1->isSpawned = false;
		boss2m1->setAnchorPoint(Vec2(0, 0));
		if(boss2m1)
		this->addChild(boss2m1, 1);
	}

	{	//boss3
		this->bossfm1 = Enemy::create(1, 0, 3);
		bossfm1->norAtkDmgAfterF = 1;
		bossfm1->doneAtkAfterF = 4;
		bossfm1->skillDamage = 169;
		bossfm1->visionRange = 500;
		bossfm1->setScale(3);
		bossfm1->moveSpeed = 555;
		bossfm1->castSpeed = 0.08;
		bossfm1->skillCD = 2;
		bossfm1->skillRange = 120;
		bossfm1->setHP(500);
		bossfm1->initOption();
		auto boss3Pos = oj->getObject("Boss3");
		bossfm1->setPosition(Vec2(boss3Pos["x"].asFloat() * 2, boss3Pos["y"].asFloat() * 2));
		if (bossfm1->spell != nullptr) this->addChild(bossfm1->spell, 9);
		if (bossfm1->spellLanded != nullptr) this->addChild(bossfm1->spellLanded, 9);
		bossfm1->line1X = line1["x"].asFloat() * 2;
		bossfm1->line2X = line2["x"].asFloat() * 2;
		bossfm1->line3X = line3["x"].asFloat() * 2;
		bossfm1->ppp = ppp;
		bossfm1->setVisible(false);
		bossfm1->isSpawned = false;
		bossfm1->setAnchorPoint(Vec2(0, 0));
		if(bossfm1)
		this->addChild(bossfm1, 1);
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

void MainGame::delAll() {
	this->map1Wave1.clear();
	this->map1Wave2.clear();
	this->map2Wave1.clear();
	this->map2Wave2.clear();
	this->removeAllChildren();
	this->enemyAdded = false;
	this->gameOver = Sprite::create("/Game Over/0.png");
	gameOver->setAnchorPoint(Vec2(0, 0));
	auto itsOverMan = RepeatForever::create(animation("Game Over", 0.1));
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
	map1->setScale(2);
	map1->setContentSize(map1->getContentSize() * 2); //do nothing but helping *2 that's all
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
	ppp->setPosition(sPx * 2, sPy * 2);
	ppp->setAnchorPoint(Vec2(0.5, 0));
	ppp->setScale(0.6);
	ppp->setFlippedX(true);
	if (ppp != nullptr)
		this->addChild(ppp, 2);
	this->setPosition(Vec2(-map1->getContentSize().width / 2, 0));
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
	this->startGame->setPosition(Point(map1->getContentSize().width / 2, 0));
	this->isGameStart = false;

	this->scheduleUpdate();
}