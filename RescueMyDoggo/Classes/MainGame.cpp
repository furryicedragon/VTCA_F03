#include "MainGame.h"
#include <ui/CocosGUI.h>
#include "MainMenuScene.h"
#include "MainScene.h"
USING_NS_CC;

#define MaxMap 3
#define BYPASS 1

MainGame * MainGame::instance = NULL;
auto def = UserDefault::getInstance();

bool MainGame::init()
{
	doneAddingEnemy = false;
	if (!Node::init()) 
	{
		return false;
	}

	MainGame::instance = this;
	highScore = def->getIntegerForKey("highScore", 0);

	cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("damage.plist");
	cache->addSpriteFramesWithFile("drops.plist");
	cache->addSpriteFramesWithFile("Map1.plist");
	cache->addSpriteFramesWithFile("Map2.plist");
	cache->addSpriteFramesWithFile("Map3-0.plist");
	cache->addSpriteFramesWithFile("Map3-1.plist");
	cache->addSpriteFramesWithFile("Map3-2.plist");
	cache->addSpriteFramesWithFile("Map3-3.plist");
	cache->addSpriteFramesWithFile("Map3-4.plist");
	cache->addSpriteFramesWithFile("ppp.plist");
	cache->addSpriteFramesWithFile("teleportation.plist");

	this->canRetry = false;
	this->enemyAdded = false;
	this->isGameOver = false;

	currentMap = 1;
	currentWave = 1;
	currentBoss = 1;
	this->boss1 = false;
	this->boss2 = false;

	dashHelper = Sprite::create();
	if(dashHelper)
	this->addChild(dashHelper);
	count = 0;
	isRepeated = false;
	howManyKeyPressed = 0;
	visibleSize = Director::getInstance()->getVisibleSize();
	map = TMXTiledMap::create("map" + std::to_string(currentMap) + ".tmx");
	if(map)
	this->addChild(map, 0, 33);
	auto oj = map->getObjectGroup("Objects");
	auto groundOj = map->getObjectGroup("Grounds");
	auto sPoint = oj->getObject("SpawnPoint");
	float sPx = sPoint["x"].asFloat();
	float sPy = sPoint["y"].asFloat();
	finishPoint = oj->getObject("FinishPoint");

	for (auto item : groundOj->getObjects()) {
		grounds.push_back(Rect(item.asValueMap()["x"].asFloat(), item.asValueMap()["y"].asFloat(), item.asValueMap()["width"].asFloat(), item.asValueMap()["height"].asFloat()));
	}
	meta = map->getLayer("meta");
	ppp = Player::create();
	ppp->mapWidth = map->getContentSize().width;
	ppp->mapScale = map->getScale();
	ppp->setPosition(sPx, sPy);
	if(ppp)
	this->addChild(ppp, 2);
	this->ppp->w1kills = 0;
	this->ppp->w2kills = 0;


	this->ppp->statPlus = Label::create();
	if (ppp->statPlus) {	
		ppp->statPlus->setVisible(false);
		this->ppp->addChild(ppp->statPlus);
	}
	this->ppp->level = Label::create();
	if (this->ppp->level) {
		this->ppp->level->setString("1");
		this->ppp->level->setColor(Color3B(255, 255, 255));
		this->ppp->level->setSystemFontSize(16);
		this->ppp->level->setVisible(false);
		this->ppp->addChild(ppp->level);
	}

	this->HPonHead = ppp->HPonHead;
	this->HitDame = ppp->HitDame;
	this->nothingBar = ppp->nothingBar;
	this->HPonHead->setVisible(false);
	this->HitDame->setVisible(false);
	this->nothingBar->setVisible(false);

	if(HPonHead)
	this->addChild(HPonHead, 5);
	if(HitDame)
	this->addChild(HitDame, 4);
	if(nothingBar)
	this->addChild(nothingBar, 3);

	this->setPosition(Vec2(0, 0));

	this->finishPortal = Sprite::create("Enemies/Effect/Gate/0.png");
	if (finishPortal)
		this->addChild(finishPortal, 99);
	finishPortal->setAnchorPoint(Vec2(0, 0));
	finishPortal->runAction(RepeatForever::create(animation("Enemies/Effect/Gate", 0.06f)));
	finishPortal->setPosition(Vec2(finishPoint["x"].asFloat()*this->map->getScale(), finishPoint["y"].asFloat()*this->map->getScale()));
	finishPortal->setVisible(false);

	this->isGameStart = false;

	this->setupPressedKeyHandling();
	this->setupTouchHandling();

	this->scheduleUpdate();
	auto followPlayer = Follow::create(ppp, Rect(0, 0, map->getContentSize().width, map->getContentSize().height));
	followPlayer->setTag(99);
	this->runAction(followPlayer);
	
	return true;
}

MainGame * MainGame::GetInstance() {
	return MainGame::instance;
}

void MainGame::setupTouchHandling() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(MainGame::onTouchBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool MainGame::onTouchBegan(Touch* touch, Event* event)
{
	if (this->isGameOver && canRetry) {
		canRetry = false;
		this->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([=]() {this->delAll(); }), nullptr));
	}

	//Start game click true

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
		/*this->gameOver->runAction(FadeOut::create(0.1f));*/
	}

 	//if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) ppp->useSkill(0);
	return true;
}
bool MainGame::keyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D) {
		int doTheMath = 0;
		for (auto item : ppp->inputKeys)
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
				//ppp->lastDuration = 0.15;
				//ppp->lastX = -45;
				ppp->isMoving = true;
				ppp->lastDirection = "Left";
				break;
			}
			case cocos2d::EventKeyboard::KeyCode::KEY_D:
			{
				//ppp->lastDuration = 0.15;
				//ppp->lastX = 45;
				ppp->isMoving = true;
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
	if (tileGID) {
		auto properties = map->getPropertiesForGID(tileGID).asValueMap();

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
			}
		}
	}
	if (tileGID == 0 && !ppp->canMoveDirections[directionClock])
		ppp->canMoveDirections[directionClock] = true;
}

Point MainGame::tileCoordForPosition(Point position) {
	int x = position.x/map->getScale() / map->getTileSize().width;
	int y = ((map->getMapSize().height *map->getScale() * map->getTileSize().height) - (position.y*map->getScale())) / (map->getTileSize().height*map->getScale());
	return Point(x, y);
}

void MainGame::update(float elapsed)
{

	if(this->isGameStart)
	{
		auto pos = ppp->getPosition();
		pos.y += 60;
		this->HPonHead->setPosition(pos);
		this->HitDame->setPosition(pos);
		this->nothingBar->setPosition(pos);
		
		if (doneAddingEnemy) {
			this->dropMoneyInit();
			this->collectMoney();
		}
		if (ppp->lastSeenLife != std::stoi(ppp->hp->getString()) / ppp->baseHP * 100) {
			ppp->lastSeenLife = std::stoi(ppp->hp->getString()) / ppp->baseHP * 100;
			HUDLayer::GetInstance()->statPlayer->DameHit->runAction(ProgressTo::create(1.0f, ppp->lastSeenLife));
			this->HitDame->runAction(ProgressTo::create(0.5f, ppp->lastSeenLife));

			HUDLayer::GetInstance()->statPlayer->DameHit->setPercentage(ppp->lastSeenLife);
			HUDLayer::GetInstance()->statPlayer->HPplayer->setPercentage(ppp->lastSeenLife);

			this->HPonHead->setPercentage(ppp->lastSeenLife);
			this->HitDame->setPercentage(ppp->lastSeenLife);
		}
		if (ppp->lastSeenExp != ppp->currentEXP / ppp->baseEXP * 100) {
			ppp->lastSeenExp = ppp->currentEXP / ppp->baseEXP * 100;
			HUDLayer::GetInstance()->statPlayer->EXPplayer->runAction(ProgressTo::create(0.3f, ppp->lastSeenExp));
		}
		if (ppp->isDead && this->isGameOver) {
			this->HPonHead->setVisible(false);
			this->HitDame->setVisible(false);

			HUDLayer::GetInstance()->statPlayer->DameHit->setPercentage(100);
			HUDLayer::GetInstance()->statPlayer->HPplayer->setPercentage(100);
			this->HPonHead->setPercentage(100);
			this->HitDame->setPercentage(100);
		}
		if (ppp->isSpawn && !this->enemyAdded) {
			this->enemyAdded = true;
			this->allEnemyInit();
		}
		if (ppp->isSpawn && !ppp->isDead)
		{
			// joystick
			if (HUDLayer::GetInstance()->movementStick->getVelocity().x > 0)
			{
				whatYouWant(EventKeyboard::KeyCode::KEY_D, 2);
				this->canIdle = true;
				ppp->isHoldingKey = true;
			}
			if (HUDLayer::GetInstance()->movementStick->getVelocity().x < 0)
			{
				this->canIdle = true;
				whatYouWant(EventKeyboard::KeyCode::KEY_A, 2);
				ppp->isHoldingKey = true;
			}
			if (HUDLayer::GetInstance()->movementStick->getVelocity().x == 0)
			{
				if (canIdle) {
					canIdle = false;
					ppp->isHoldingKey = false;
					whatYouWant(EventKeyboard::KeyCode::KEY_YEN, 1);
				}

			}
			
			//HUD buttons
			if (HUDLayer::GetInstance()->attackBtn->getValue() && !ppp->usingSkill)
			{
				ppp->attack();
			}
			if (HUDLayer::GetInstance()->rollBtn->getValue() && !ppp->usingSkill)
			{
				ppp->roll();
			}
			if (HUDLayer::GetInstance()->skill1Btn->getValue() && !ppp->usingSkill)
			{
				ppp->useSkill(1, HUDLayer::GetInstance()->skill1Btn);
			}
			if (HUDLayer::GetInstance()->skill2Btn->getValue() && !ppp->usingSkill)
			{
				ppp->useSkill(0, HUDLayer::GetInstance()->skill2Btn);
			}
		}
		if (this->enemyAdded) {
			checkGravity();

			if (this->currentMap > 1)
				if (teleportation->getBoundingBox().containsPoint(ppp->getPosition())) 
					ppp->setPosition(teleportation2->getPosition());

			auto gravity = RepeatForever::create(Sequence::create(CallFunc::create([=]() { 
				if (this->checkGravity()) { ppp->setPositionY(ppp->getPositionY() - 1); 
				}}), DelayTime::create(0.05f), nullptr));
			gravity->setTag(99);
			if(ppp->isFalling)
			this->map->runAction(gravity);
			

			if (ppp->isHoldingKey && !ppp->isAttacking) {
				ppp->isMoving = true;
				ppp->moving(elapsed);
			}
			checkCollision(ppp);
			if(currentWave!=0)
			this->waveXMapXInit();
			if ( ( (allEnemy[9]->isDead && allEnemy[4]->isDead) || BYPASS ) && !congratz) {
				for (auto item : allEnemy)
				{
					congratz = true;
					item->setHP(0);
					item->dead();
					item->canRespawn = false;
					
					finishPortal->setVisible(true);

					
					if (currentMap == 3)
					{
						auto hud = HUDLayer::GetInstance();

						auto atkbtn = hud->getChildByTag(1);
						auto rollbtn = hud->getChildByTag(2);
						auto skill1btn = hud->getChildByTag(3);
						auto skill2btn = hud->getChildByTag(4);

						atkbtn->setPositionX(atkbtn->getPositionX() - 5);
						skill1btn->setPositionX(skill1btn->getPositionX() - 5);
						skill2btn->setPositionX(skill2btn->getPositionX() - 5);
						rollbtn->setPositionY(rollbtn->getPositionY() - 15);
					}

				}
			}
			if (congratz && ppp->isSpawn && finishPortal->isVisible()) 
			{
				// change level
				if (finishPortal->getBoundingBox().containsPoint(ppp->getPosition())) 
				{
					congratz = false;
					HUDLayer::GetInstance()->setVisible(false);
					this->saveHighScore();

					if (currentMap + 1 <= MaxMap)
					{
						this->lastLevel = std::stoi( ppp->level->getString() );
						this->lastHP = ppp->baseHP;
						this->lastExp = ppp->lastSeenExp;
						this->delAll(++currentMap);
						this->gameStarto();
					}
					else
					{
						// congrats scene?
						this->isGameStart = false;

						auto btMenu = (ui::Button*)this->congratulation->getChildByTag(1);
						auto endLabel = (Label*)this->congratulation->getChildByTag(2);
						this->congratulation->setPosition(Vec2(this->getPosition().x*-1, 0));
						this->congratulation->runAction(Sequence::create(FadeIn::create(1.6f), 
							CallFunc::create([=]() { ppp->forbidAllAction(); 
							  btMenu->setVisible(true); btMenu->setEnabled(true); endLabel->setVisible(true); }), nullptr));
					}
				}
			}
		}
		if (ppp->isDead && !this->isGameOver) {
			ppp->isSpawn = false;
			this->isGameOver = true;
			HUDLayer::GetInstance()->setVisible(false);

			auto highScoreLabel = (Label*)MainScene::GetInstance()->gameOverLayer->getChildByTag(100)->getChildByTag(10);
			int hs = def->getIntegerForKey("highScore");
			highScoreLabel->setString(std::to_string(hs));

			this->runAction(Sequence::create(DelayTime::create(2.0f), 
				CallFunc::create([=]() {MainScene::GetInstance()->gameOverLayer->setVisible(true); experimental::AudioEngine::play2d("sounds/defeat.mp3", false, 0.7f); }), nullptr));
			
		}
		}



}
bool MainGame::checkGravity()
{
	int i = 0;
	for (auto item : grounds) {
		if (!Rect(ppp->getPositionX()-11, ((int)ppp->getPositionY()) - 20, 22, 80).intersectsRect(item) || ppp->getPositionY() - 20 < item.getMaxY())
			i++;
	}
	if (i == grounds.size()) { //neu ko dung tren ground
		if (!ppp->isRolling) {
			ppp->isFalling = true;
			if(!ppp->isDead &&!ppp->usingSkill)
			ppp->setSpriteFrame(ppp->pppFrames->getSpriteFrameByName(std::to_string(ppp->direction) + "jump0.png"));
		}
		return true;
	}
	else { //neu dang tren ground
		if (!ppp->isRolling) {
			ppp->isFalling = false;
			map->stopAllActionsByTag(99); //stop gravity reapeat forever
			ppp->timePassedInSecond = 1;
			if (!ppp->isIdle) 
				ppp->idleStatus();
			return false;
		}
		}
}

void MainGame::spawnPlayer()
{
	auto zap = Sprite::create();
	zap->setPosition(Vec2(ppp->getPosition().x, ppp->getPosition().y /*+ 66*/));
	if(zap)
	this->addChild(zap);
	zap->runAction(Sequence::create(DelayTime::create(0.69f), CallFunc::create([=]() {ppp->spawnEffect(); }), animation("Spawn", 0.1f),
		CallFunc::create([=]() {this->removeChild(zap, true); HUDLayer::GetInstance()->toggleVisiblity(); 
								this->HPonHead->setVisible(true); this->HitDame->setVisible(true); this->nothingBar->setVisible(true); }), nullptr));
}


bool MainGame::checkRange(Enemy* enemy2Check, int theRange) {

	auto itemWidth = enemy2Check->getContentSize().width;
	auto eeePosY = enemy2Check->getPositionY() - 43;
	auto calculateIt = 40;
	if (enemy2Check->mapNumber == 3 && enemy2Check->bossNumber != 1) {
		if (enemy2Check->bossNumber == 2) eeePosY -= 69;
			else
		eeePosY -= 40;
	}
	if (enemy2Check->bossNumber == 1 && enemy2Check->mapNumber==1) calculateIt = 150;
	auto howfarX = enemy2Check->getPosition().x - ppp->getPosition().x;
		if (((howfarX < 69 && ppp->direction==0 && howfarX>-itemWidth / 2 - theRange)
			|| (howfarX < itemWidth / 2 + theRange && howfarX>-69 && ppp->direction==1))&& ppp->getPositionY() - eeePosY<calculateIt && ppp->getPositionY()-eeePosY>0)
			return true;
		else
			return false;
}
void MainGame::checkAttackRange(Enemy * eee, int index)
{
	if ((index != 4 || boss1)||(index!=9 || boss2)&&!ppp->isDead) {
		if (checkRange(eee,69)) 
		{
			if (ppp->isAttacking && ppp->canAADamage[index])
			{
				if (!eee->isDead && eee->isSpawned && !eee->invulnerable)
					this->displayDamage(ppp->damageCurrent, "grey", eee->getPosition(),eee->getContentSize());
				eee->getHit(ppp->damageCurrent);

				ppp->canAADamage[index] = false;
			}
		}
		if (eee->canDamage && !ppp->isRolling && !eee->isCaster
			&& (((eee->direction == 1||(eee->mapNumber==3 && eee->waveNumber==1)) && ppp->getPositionX()  - eee->getPositionX() <= eee->skillRange) 
				|| ((eee->direction==0 || (eee->mapNumber==3 && eee->waveNumber==1)) && eee->getPositionX() - ppp->getPositionX() > eee->skillRange) ) ) {
			if (!ppp->isDead && ppp->state != 1)
				this->displayDamage(eee->skillDamage, "blue", ppp->getPosition(), Size(0, 0));
			ppp->getHit(eee->skillDamage, eee->getPosition().x);

			eee->canDamage = false;
		}
		int i = 0;
		float rectPos = ppp->listSkill.at(1)->getPosition().x;
		if (!ppp->listSkill.at(1)->isFlippedX()) 
			rectPos -= ppp->listSkill.at(1)->getContentSize().width/2;
		Rect skillRect = Rect(rectPos, ppp->listSkill.at(1)->getPosition().y-69, 300, /*100*/269);
		Rect eeeRect = eee->getBoundingBox();
		for (auto item : ppp->listSkill)
		{
			if ((ppp->usingSkill||i==1) && item->canDamage[index]
				&& ((skillRect.intersectsRect(eee->getBoundingBox()) && i==1)
					|| (i ==0 && checkRange(eee,item->skillRange))))
			{
					if (!eee->isDead && eee->isSpawned && !eee->invulnerable)
						this->displayDamage(ppp->damageCurrent / 100 * item->skillDamage, "grey", eee->getPosition(),eee->getContentSize());
					eee->getHit(ppp->damageCurrent / 100 * item->skillDamage);
					
					item->canDamage[index] = false;
			}
			i++;
		}


		if (eee->isCaster && !ppp->isRolling && !eee->canDamage && std::fabsf(eee->spell->getPosition().x - ppp->getPosition().x) < 22 && std::fabsf(eee->spell->getPositionY() - ppp->getPositionY())<40 && ((eee->mapNumber==1 && eee->waveNumber==1) || (eee->mapNumber==2 && eee->bossNumber==2))) {
			eee->spell->setPosition(0, 0);
			eee->spell->setVisible(false);
			eee->spell->stopAllActions();
			eee->attackLandedEffect();
		}
		if (eee->isCaster && eee->canDamage && !ppp->isRolling && std::fabsf(eee->spellLanded->getPosition().x-ppp->getPosition().x)<9) {
			
			if (!ppp->isDead && ppp->state != 1)
				this->displayDamage(eee->skillDamage, "blue", ppp->getPosition(),Size(0,0));
			ppp->getHit(eee->skillDamage, eee->getPosition().x);
			
			eee->canDamage = false;
		}
	}

}
void MainGame::waveXMapXInit() {
	int i = 0;
		for (auto item : allEnemy)
		{
			this->checkAttackRange(item, i);
			i++;
		}
		if (ppp->w1kills > 7 && !boss1) {
			this->spawnEffect(allEnemy[4], 1);
			boss1 = true;
		}
		if (ppp->w2kills > 7 && !boss2) { 
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
		auto frameName = "Enemies/Effect/Spawn/" + std::to_string(i) + ".png";
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
	spawnGate->setPosition(Vec2(enemy2Spawn->getPosition().x , enemy2Spawn->getPosition().y));
	if(spawnGate)
	this->addChild(spawnGate, 4);

	spawnGate->runAction(Sequence::create(DelayTime::create((index+1)/4),animB,animLoop, CallFunc::create([=]() {enemy2Spawn->setVisible(true); }), animF, CallFunc::create([=]() {this->removeChild(spawnGate); enemy2Spawn->isSpawned = true; }), nullptr));

}

void MainGame::allEnemyInit()
{
	auto oj = map->getObjectGroup("Objects");

	std::vector<ValueMap> listLine;
	
	for (int i = 0; i < 99; i++)
	{
		auto line = oj->getObject("Line" + std::to_string(i + 1));

		if (line.empty())
			break;

		listLine.push_back(line);
	}
	//wave 1
	for (int i = 0; i < 4; i++) {
		Enemy* wave = Enemy::create(currentMap, 1, 0);
		for (auto line : listLine)
		{
			wave->listLineX.push_back(line["x"].asFloat());
		}

		switch (this->currentMap)
		{
		case 1:
			wave->skillDamage = 11;
			wave->visionRange = 310;
			wave->moveSpeed = 120;
			wave->isCaster = true;
			wave->castSpeed = 0.12f;
			wave->skillSpeed = 0.1f;
			wave->skillAtkAfterF = 8;
			wave->skillCD = 4;
			wave->skillRange = 300;
			wave->setHP(100);
			wave->moneyRank = 1;
			wave->expReward = 25;
			break;

		case 2:
			wave->skillDamage = 99;
			wave->visionRange = 310;
			wave->norAtkDmgAfterF = 3;
			wave->doneAtkAfterF = 2;
			wave->moveSpeed = 120;
			wave->castSpeed = 0.12f;
			wave->skillSpeed = 0.1f;
			wave->skillCD = 3;
			wave->skillRange = 240;
			wave->setHP(150);
			wave->moneyRank = 2;
			wave->expReward = 100;
			break;

		case 3:
			wave->skillDamage = 222;
			wave->visionRange = 310;
			wave->moveSpeed = 120;
			wave->norAtkDmgAfterF = 9;
			wave->doneAtkAfterF = 5;
			wave->castSpeed = 0.09f;
			wave->skillSpeed = 0.1f;
			wave->skillCD = 4;
			wave->skillRange = 150;
			wave->setHP(666);
			wave->moneyRank = 3;
			wave->expReward = 300;
			break;
		}
		
		wave->initOption();
		wave->setPosition(RandomHelper::random_real(wave->listLineX.at(0), wave->listLineX.at(1)), listLine.at(0)["y"].asFloat() + 43);
		wave->ppp = ppp;
		wave->setVisible(false);
		wave->isSpawned = false;
		if (wave->spell) this->addChild(wave->spell, 9);
		if (wave->spellLanded) this->addChild(wave->spellLanded, 9);
		//wave->setAnchorPoint(Vec2(0, 0));
		allEnemy.push_back(wave);
		if (allEnemy[i]) {
			this->addChild(allEnemy[i], 0);
			this->spawnEffect(allEnemy[i],i);
		}
	}

	//boss 1
	{
		this->boss1m1 = Enemy::create(currentMap, 0, 1);
		for (auto line : listLine)
		{
			boss1m1->listLineX.push_back(line["x"].asFloat());
		}

		switch (this->currentMap)
		{
		case 1:
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
			boss1m1->mobilityDoneAfterF = 3;
			boss1m1->setHP(300);
			boss1m1->moneyRank = 2;
			boss1m1->expReward = 150;
			break;

		case 2:
			boss1m1->visionRange = 420;
			boss1m1->skillDamage = 150;
			//boss1m1->setScale(1.6f);
			boss1m1->moveSpeed = 333;
			boss1m1->isCaster = true;
			boss1m1->skillAtkAfterF = 8;
			boss1m1->castSpeed = 0.1f;
			boss1m1->skillCD = 2;
			boss1m1->skillRange = 400;
			boss1m1->setHP(333);
			boss1m1->moneyRank = 3;
			boss1m1->expReward = 500;
			break;

		case 3:
			boss1m1->visionRange = 200;
			boss1m1->skillDamage = 240;
			//boss1m1->setScale(1.6f);
			boss1m1->moveSpeed = 333;
			boss1m1->norAtkDmgAfterF = 5;
			boss1m1->doneAtkAfterF = 6;
			boss1m1->castSpeed = 0.09f;
			boss1m1->skillCD = 2;
			boss1m1->skillRange = 150;
			boss1m1->setHP(550);
			boss1m1->moneyRank = 4;
			boss1m1->expReward = 1200;
			break;
		}
		
		boss1m1->initOption();
		auto boss1Pos = oj->getObject("Boss1");
		boss1m1->setPosition(Vec2(boss1Pos["x"].asFloat(), boss1Pos["y"].asFloat()+43));
		if (boss1m1->spell) this->addChild(boss1m1->spell, 9);
		if (boss1m1->spellLanded) this->addChild(boss1m1->spellLanded, 9);
		boss1m1->ppp = ppp;
		boss1m1->setVisible(false);
		boss1m1->isSpawned = false;
		//boss1m1->setAnchorPoint(Vec2(0, 0));
		if (boss1m1) {
			allEnemy.push_back(boss1m1);
			this->addChild(allEnemy[4], 1);
		}
	}

	//wave2
	for (int i = 0; i < 4; i++) {
		Enemy* wave = Enemy::create(currentMap, 2, 0);
		for (auto line : listLine)
		{
			wave->listLineX.push_back(line["x"].asFloat());
		}
		switch (this->currentMap)
		{
		case 1:
			wave->skillDamage = 96;
			wave->visionRange = 350;
			wave->moveSpeed = 100;
			wave->isCaster = true;
			wave->castSpeed = 0.1f;
			wave->skillSpeed = 0.1f;
			wave->skillCD = 4;
			wave->skillRange = 400;
			wave->setHP(175);
			wave->moneyRank = 1;
			wave->expReward = 50;
			break;

		case 2:
			wave->skillDamage = 300;
			wave->visionRange = 222;
			wave->moveSpeed = 99;
			wave->norAtkDmgAfterF = 4;
			wave->doneAtkAfterF = 3;
			wave->isCaster = false;
			wave->castSpeed = 0.12f;
			wave->skillSpeed = 0.1f;
			wave->skillCD = 5;
			wave->skillRange = 120;
			wave->setHP(450);
			wave->moneyRank = 2;
			wave->expReward = 200;
			break;

		case 3:
			wave->skillDamage = 290;
			wave->visionRange = 250;
			wave->moveSpeed = 100;
			wave->norAtkDmgAfterF = 3;
			wave->doneAtkAfterF = 4;
			wave->castSpeed = 0.11f;
			wave->skillSpeed = 0.1f;
			wave->skillCD = 4;
			wave->skillRange = 222;
			wave->setHP(500);
			wave->moneyRank = 3;
			wave->expReward = 600;
			break;
		}

		wave->initOption();
		wave->setPosition(RandomHelper::random_real(wave->listLineX.at(2), wave->listLineX.at(3)), listLine.at(2)["y"].asFloat()+43);
		wave->ppp = ppp;
		wave->setVisible(false);
		wave->isSpawned = false;
		if (wave->spell) this->addChild(wave->spell, 9);
		if (wave->spellLanded) this->addChild(wave->spellLanded, 9);
		//wave->setAnchorPoint(Vec2(0, 0));
		allEnemy.push_back(wave);
		if (allEnemy[i + 5]) {
			this->addChild(allEnemy[i + 5], 0);
			this->spawnEffect(allEnemy[i + 5], i);
		}
	}
	
	//boss2
	{
		this->boss2m1 = Enemy::create(currentMap, 0, 2);
		for (auto line : listLine)
		{
			boss2m1->listLineX.push_back(line["x"].asFloat());
		}
		switch (this->currentMap)
		{
		case 1:
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
			boss2m1->mobilityDoneAfterF = 3;
			boss2m1->setHP(200);
			boss2m1->moneyRank = 2;
			boss2m1->expReward = 300;
			break;

		case 2:
			//boss2m1->setScale(1.6f);
			boss2m1->skillDamage = 222;
			boss2m1->visionRange = 380;
			boss2m1->moveSpeed = 300;
			boss2m1->isCaster = true;
			boss2m1->skillAtkAfterF = 10;
			boss2m1->castSpeed = 0.04f;
			boss2m1->skillCD = 2;
			boss2m1->skillRange = 380;
			boss2m1->setHP(500);
			boss2m1->moneyRank = 3;
			boss2m1->expReward = 1000;
			break;

		case 3:
			//boss2m1->setScale(1.6f);
			boss2m1->skillDamage = 280;
			boss2m1->visionRange = 321;
			boss2m1->moveSpeed = 369;
			boss2m1->norAtkDmgAfterF = 11;
			boss2m1->doneAtkAfterF = 3;
			boss2m1->castSpeed = 0.8f;
			boss2m1->skillCD = 2;
			boss2m1->skillRange = 300;
			boss2m1->mobilitySSAt = 4;
			boss2m1->mobilitySpeed = 2;
			boss2m1->setHP(800);
			boss2m1->moneyRank = 4;
			boss2m1->expReward = 2345;
			break;
		}
     	
		boss2m1->initOption();
		auto boss2Pos = oj->getObject("Boss2");
		boss2m1->setPosition(Vec2(boss2Pos["x"].asFloat(), boss2Pos["y"].asFloat()+43));
		if (boss2m1->spell) this->addChild(boss2m1->spell, 9);
		if (boss2m1->spellLanded) this->addChild(boss2m1->spellLanded, 9);
		boss2m1->ppp = ppp;
		boss2m1->setVisible(false);
		boss2m1->isSpawned = false;
		//boss2m1->setAnchorPoint(Vec2(0, 0));
		if (boss2m1) {
			allEnemy.push_back(boss2m1);
			this->addChild(allEnemy[9], 1);
		}
	}

	for (auto eee : allEnemy)
	{
		if ((eee->waveNumber == 1 || eee->bossNumber == 1) && (eee->mapNumber == 1 || eee->mapNumber == 3)) {
			eee->spotPlayerLineLeft = eee->listLineX.at(0);
			eee->spotPlayerLineRight = eee->listLineX.at(1);
		}
		if ((eee->waveNumber == 2 || eee->bossNumber > 1) && eee->mapNumber == 1 || eee->mapNumber == 3) {
			eee->spotPlayerLineLeft = eee->listLineX.at(2);
			eee->spotPlayerLineRight = eee->listLineX.at(3);
		}
		if (eee->waveNumber == 1 && eee->mapNumber == 2) {
			eee->spotPlayerLineLeft = eee->listLineX.at(0);
			eee->spotPlayerLineRight = eee->listLineX.at(1);
		}
		if(eee->waveNumber==2 && eee->mapNumber ==2){
			eee->spotPlayerLineLeft = eee->listLineX.at(2);
			eee->spotPlayerLineRight = eee->listLineX.at(3);
		}
		if (eee->bossNumber > 0 && eee->mapNumber == 2) {
			eee->spotPlayerLineLeft = eee->listLineX.at(4);
			eee->spotPlayerLineRight = eee->listLineX.at(5);
		}
	}

	ppp->canAADamage.resize(allEnemy.size(),true);


	if (this->ppp->listSkill.size() < 1) {

		Point testPos = Point(this->ppp->getContentSize().width / 1.8, this->ppp->getContentSize().height / 2);
		//ppp->listSkill.insert(0, Skill::create(300,169, 3, 3, 4 , 2, 2, testPos, "MainChar/Effects/DashStab","Dash/Dash Attack"));
		ppp->listSkill.insert(0, Skill::create(120,169, 3, 1, 2 , 1, 1, testPos, "MainChar/Effects/DashStab","attack0"));
		if(ppp->listSkill.at(0))
		this->ppp->addChild(ppp->listSkill.at(0), 3);
		ppp->listSkill.at(0)->setVisible(false);
		ppp->listSkill.at(0)->setScale(0.5);

		//Point skillPos = Point(ppp->getPosition().x,ppp->getPosition().y);
		ppp->listSkill.insert(1, Skill::create(333,129, 3, 3, 4, 2, 2, Point(ppp->getPosition().x, ppp->getPosition().y), "MainChar/Effects/Skill2", "attack5"));
		ppp->listSkill.at(1)->setAnchorPoint(Vec2(0.5, 0.5));
		ppp->listSkill.at(1)->setScale(0.4f);
		if(ppp->listSkill.at(1))
		this->addChild(ppp->listSkill.at(1), 3);
		ppp->listSkill.at(1)->setVisible(false);

	}
	for (auto item : ppp->listSkill)
	{
		item->canDamage.resize(allEnemy.size(), false);
	}
	doneAddingEnemy = true;
}

Animate * MainGame::animation(std::string actionName,float timeEachFrame)
{
	Vector<SpriteFrame *> runningFrames;
	for (int i = 0; i < 99; i++) {
		auto frameName = actionName+"/" + std::to_string(i) + ".png";
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

void MainGame::displayDamage(int damage, std::string color, Vec2 where,Size size)
{

	auto digit = std::to_string(damage);
	std::vector<int> digits;
	auto test = digit.length();
	while (digit.length()>0)
	{
		test = digit.length();
		auto what2add = digit;
		digits.push_back(std::stoi(what2add.erase(1, digit.length())));
		digit.erase(0,1);
	}

	auto sprite_size = cache->getSpriteFrameByName(color + "0.png")->getOriginalSize();

	auto start = where;
	start.x += size.width / 2;
	start.y += RandomHelper::random_int(0, 50);

	Vector<Sprite*> digitSprites;
	for (int i = 0; i < (int)digits.size(); i++)
	{
		auto sprite = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(color + std::to_string(digits[i]) + ".png"));
		if (i > 0) {
			if(size.width>0)
				start.x += sprite_size.width;
			else start.x += sprite_size.width*0.4f;
		}
		sprite->setPosition(start);

		if (size.width == 0) sprite->setScale(0.4f);
		digitSprites.pushBack(sprite);
		if(sprite)
		this->addChild(sprite, 2);
	}

	auto theY = RandomHelper::random_int(50, 150);

	for (auto item : digitSprites)
		item->runAction(Sequence::create(
			MoveBy::create(0.5f, Vec2(0, theY)),
			FadeOut::create(0.5f),
			CallFunc::create([=]() { item->removeFromParentAndCleanup(true); }), nullptr));
}

void MainGame::delAll() 
{
	this->allEnemy.clear();
	this->listDrops.clear();
	this->grounds.clear();
	this->removeAllChildrenWithCleanup(true);

	this->init();
	
	HUDLayer::GetInstance()->resetHUDstate();
}

void MainGame::delAll(int level)
{
	this->allEnemy.clear();
	this->listDrops.clear();
	this->grounds.clear();
	this->removeAllChildrenWithCleanup(true);

	this->isGameStart = false;
	this->changeMap(level);

	auto followPlayer = Follow::create(ppp, Rect(0, 0, map->getContentSize().width, map->getContentSize().height));
	followPlayer->setTag(99);
	this->runAction(followPlayer);

	auto hud_layer = HUDLayer::GetInstance();
	hud_layer->resetHUDstate();
	hud_layer->statPlayer->level->setString(ppp->level->getString());
	hud_layer->statPlayer->HPplayer->setPercentage(100);
	hud_layer->statPlayer->EXPplayer->setPercentage(lastExp);
	hud_layer->scoreLabel->setString(std::to_string(lastScore));
}

void MainGame::gameStarto()
{
	if (!ppp->statPlayer)
		ppp->statPlayer = HUDLayer::GetInstance()->statPlayer;
	if (!this->isGameStart) 
	{
		congratz = false;
		isGameStart = true;
		this->spawnPlayer();
	}
}

void MainGame::restartGame()
{
	canRetry = false;
	this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {this->delAll(); this->gameStarto(); }), nullptr));
	
}

void MainGame::dropMoneyInit()
{
		for (auto item : allEnemy) {
			if (item->canDrop) {
				auto moneyDrop = Sprite::create();
				/*if (item->mapNumber != 2 || (item->mapNumber == 2 && item->bossNumber == 2) || item->waveNumber>0)
					moneyDrop->setPosition(item->getPosition());
				else
					moneyDrop->setPosition(Vec2(item->getPositionX(), item->getPositionY() + 58));*/

				if (item->mapNumber == 1) {
					if (item->waveNumber > 0) {
						moneyDrop->setPosition(item->getPosition());
					}
					else if (item->bossNumber == 1 || item->bossNumber == 2) {
						moneyDrop->setPosition(item->getPositionX(), item->getPositionY() + 5);
					}
				}
				if (item->mapNumber == 2) {
					if (item->waveNumber > 0 || item->bossNumber == 2) {
						moneyDrop->setPosition(item->getPositionX(), item->getPositionY() - 12);
					}
					if (item->bossNumber == 1) {
						moneyDrop->setPosition(item->getPositionX(), item->getPositionY() + 42);
					}
				}
				if (item->mapNumber == 3) {
					if (item->waveNumber > 0) {
						moneyDrop->setPosition(item->getPositionX(), item->getPositionY() - 40);
					}
					else if (item->bossNumber == 1)
					{
						moneyDrop->setPosition(item->getPositionX(), item->getPositionY() - 10);
					}
					else if (item->bossNumber == 2)
					{
						moneyDrop->setPosition(item->getPositionX(), item->getPositionY() - 50);
					}
				}

				this->addChild(moneyDrop, 9);
				item->canDrop = false;
				if (item->moneyRank == 1) {
					moneyDrop->setTag(1);
					listDrops.pushBack(moneyDrop);
					moneyDrop->runAction(RepeatForever::create(makeAnimation("silvercoin", 0.15f)));
				}
				if (item->moneyRank == 2) {
					moneyDrop->setTag(2);
					listDrops.pushBack(moneyDrop);
					moneyDrop->runAction(RepeatForever::create(makeAnimation("goldcoin", 0.15f)));
				}
				if (item->moneyRank == 3) {
					moneyDrop->setTag(3);
					listDrops.pushBack(moneyDrop);
					moneyDrop->runAction(RepeatForever::create(makeAnimation("money", 0.15f)));
				}
				if (item->moneyRank == 4) {
					moneyDrop->setTag(4);
					listDrops.pushBack(moneyDrop);
					moneyDrop->runAction(RepeatForever::create(makeAnimation("treasure", 0.15f)));
				}
				auto jumpAction = JumpBy::create(0.5, Vec2(0, -30), 100, 1);
				moneyDrop->runAction(jumpAction);			
		}
	}
}

void MainGame::collectMoney() {
	if(listDrops.size()>0)
	for (auto item : listDrops) {
		if (item->getTag() != 69) {
			Rect moneyRect = item->getBoundingBox();
			Rect playerRect = ppp->getBoundingBox();
			if (playerRect.intersectsRect(moneyRect)) {
				if (item->getTag() == 1) {
					ppp->score += 10;
				}
				if (item->getTag() == 2) {
					ppp->score += 20;
				}
				if (item->getTag() == 3) {
					ppp->score += 100;
				}
				if (item->getTag() == 4) {
					ppp->score += 500;
				}
				HUDLayer::GetInstance()->scoreLabel->setString(std::to_string(ppp->score));

				this->runAction(Sequence::create(DelayTime::create(0.3f),
						CallFunc::create([=]() {experimental::AudioEngine::play2d("sounds/coin_pickup.mp3", false, 0.7f); }), nullptr));
				
				item->setTag(69);
				item->runAction(Sequence::create(DelayTime::create(0.2f),MoveBy::create(0.2f, Vec2(0, 40)), FadeOut::create(0.5f), 
					CallFunc::create([=]() { this->removeChild(item, true); }), nullptr));
			}
		}
		else
		{
			listDrops.eraseObject(item, false);
			break;
		}
	}
}

Animate* MainGame::makeAnimation(std::string actionName, float timeEachFrame)
{
	Vector<SpriteFrame*> runningFrames;
	for (int i = 0; i < 99; i++) {
		auto frameName = actionName + std::to_string(i) + ".png";
		SpriteFrame* frame = cache->getSpriteFrameByName(frameName);
		if (!frame)
			break;
		runningFrames.pushBack(frame);
	}
	Animation* runningAnimation = Animation::createWithSpriteFrames(runningFrames, timeEachFrame);
	Animate* anim = Animate::create(runningAnimation);
	return anim;
}

void MainGame::changeMap(int level)
{
	if (level == 3)
	{
		congratulation = Sprite::create("Gratz/0.png");
		congratulation->setAnchorPoint(Vec2(0, 0));
		auto itsOKMan = RepeatForever::create(animation("Gratz", 1));
		this->congratulation->runAction(itsOKMan);
		this->addChild(congratulation, 99999);
		this->congratulation->runAction(FadeOut::create(0));

		auto btMenu = ui::Button::create(BT_exitnomal, BT_exitclick);
		btMenu->setPosition(Vec2(congratulation->getContentSize().width / 2, congratulation->getContentSize().height * 0.08f));
		btMenu->setTag(1);
		btMenu->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				experimental::AudioEngine::play2d("sounds/button_click.mp3");
				break;
			case ui::Widget::TouchEventType::ENDED:
				MainGame::GetInstance()->delAll();
				MainGame::GetInstance()->setVisible(false);

				MainMenuScene::GetInstance()->bgAudio();
				MainMenuScene::GetInstance()->setVisible(true);

				HUDLayer::GetInstance()->resetHUDstate();

				MainScene::GetInstance()->gameOverLayer->setVisible(false);
				MainScene::GetInstance()->gamePauseLayer->setVisible(false);

				break;
			}
		});
		btMenu->setVisible(false);
		btMenu->setEnabled(false);

		auto endLabel = Label::createWithSystemFont("Thanks for playing!", "Arial", 40.0f, Size::ZERO, TextHAlignment::CENTER);
		endLabel->setPosition(congratulation->getContentSize().width / 2, congratulation->getContentSize().height * 0.18f);
		endLabel->setVisible(false);
		endLabel->setTag(2);

		congratulation->addChild(btMenu);
		congratulation->addChild(endLabel);
	}

	doneAddingEnemy = false;

	this->canRetry = false;
	this->enemyAdded = false;
	this->isGameOver = false;

	currentMap = level;
	currentWave = 1;
	currentBoss = 1;
	this->boss1 = false;
	this->boss2 = false;

	dashHelper = Sprite::create();
	if (dashHelper)
		this->addChild(dashHelper);
	count = 0;
	isRepeated = false;
	howManyKeyPressed = 0;
	visibleSize = Director::getInstance()->getVisibleSize();

	this->map = TMXTiledMap::create("map" + std::to_string(currentMap) + ".tmx");


	if (map)
		this->addChild(map, 0, 33);
	auto oj = map->getObjectGroup("Objects");
	auto groundOj = map->getObjectGroup("Grounds");
	auto sPoint = oj->getObject("SpawnPoint");
	float sPx = sPoint["x"].asFloat();
	float sPy = sPoint["y"].asFloat();
	finishPoint = oj->getObject("FinishPoint");

	auto TeleportPoint1 = oj->getObject("teleportation1");
	float sTP1x = TeleportPoint1["x"].asFloat();
	float sTP1y = TeleportPoint1["y"].asFloat();

	auto TeleportPoint2 = oj->getObject("teleportation2");
	float sTP2x = TeleportPoint2["x"].asFloat();
	float sTP2y = TeleportPoint2["y"].asFloat();


	for (auto item : groundOj->getObjects()) {
		grounds.push_back(Rect(item.asValueMap()["x"].asFloat(), item.asValueMap()["y"].asFloat(), item.asValueMap()["width"].asFloat(), item.asValueMap()["height"].asFloat()));
	}
	meta = map->getLayer("meta");
	meta->setVisible(false);
	ppp = Player::create();
	ppp->mapWidth = map->getContentSize().width;
	ppp->mapScale = map->getScale();
	ppp->setPosition(sPx, sPy);

	ppp->damageCurrent = 16 * lastLevel;
	ppp->baseHP = lastHP;
	ppp->hp->setString(std::to_string(lastHP));
	ppp->attackSpeed = 0.14f - ((0.14f / 20)*lastLevel);
	ppp->score = this->lastScore;

	if (ppp)
		this->addChild(ppp, 2);
	this->ppp->w1kills = 0;
	this->ppp->w2kills = 0;


	this->ppp->statPlus = Label::create();
	if (ppp->statPlus) {
		this->ppp->addChild(ppp->statPlus);
		ppp->statPlus->setVisible(false);
	}
	this->ppp->level = Label::create();
	if (this->ppp->level) {
		this->ppp->level->setString(std::to_string(this->lastLevel));
		this->ppp->level->setColor(Color3B(255, 255, 255));
		this->ppp->level->setSystemFontSize(16);
		this->ppp->level->setVisible(false);
		this->ppp->addChild(ppp->level);
	}

	this->HPonHead = ppp->HPonHead;
	this->HitDame = ppp->HitDame;
	this->nothingBar = ppp->nothingBar;
	this->HPonHead->setVisible(false);
	this->HitDame->setVisible(false);
	this->nothingBar->setVisible(false);
	if (HPonHead)
		this->addChild(HPonHead, 5);
	if (HitDame)
		this->addChild(HitDame, 4);
	if (nothingBar)
		this->addChild(nothingBar, 3);

	this->setPosition(Vec2(0, 0));

	this->teleportation = Sprite::create();
	teleportation->runAction(RepeatForever::create(makeAnimation("teleportation", 0.06f)));	
	teleportation->setPosition(Vec2(sTP1x * this->map->getScale(), sTP1y *this->map->getScale()));
	teleportation->setScale(0.8f);
	teleportation->setVisible(true);
	this->addChild(teleportation);
	
	this->teleportation2 = Sprite::create();
	teleportation2->runAction(RepeatForever::create(makeAnimation("teleportation", 0.06f)));
	teleportation2->setPosition(Vec2(sTP2x *this->map->getScale(), sTP2y *this->map->getScale()));
	teleportation2->setScale(0.8f);
	teleportation2->setVisible(true);
	this->addChild(teleportation2);

	this->finishPortal = Sprite::create("Enemies/Effect/Gate/0.png");
	if (finishPortal)
		this->addChild(finishPortal, 99);
	finishPortal->setAnchorPoint(Vec2(0, 0));
	finishPortal->runAction(RepeatForever::create(animation("Enemies/Effect/Gate", 0.06f)));
	finishPortal->setPosition(Vec2(finishPoint["x"].asFloat()*this->map->getScale(), finishPoint["y"].asFloat()*this->map->getScale()));
	finishPortal->setVisible(false);
}

void MainGame::saveHighScore()
{
	this->lastScore = ppp->score;
	if (this->lastScore > this->highScore)
	{
		highScore = lastScore;
	}
	def->setIntegerForKey("highScore", highScore);
	def->flush();
}