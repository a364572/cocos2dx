#include "GameManager.h"

GameManager* GameManager::instance = nullptr;
GameManager* GameManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new GameManager();
	}
	return instance;
}

void GameManager::increaseCount(Ref *ref)
{
	numberOfLoadRes++;
}
void GameManager::initCard(Ref * ref)
{
	int type = 0;
	auto texture = Director::getInstance()->getTextureCache()->getTextureForKey("puke_whole.png");
	auto cardWidth = texture->getContentSize().width / 9;
	auto cardHeight = texture->getContentSize().height / 6;
	for (int i = 0; i < 54; i++)
	{
		if (i % 13 == 0)
		{
			type++;
		}
		int row = i / 9;
		int col = i - 9 * row;

		auto originX = texture->getContentSize().width - (col + 1) * cardWidth;
		auto originY = texture->getContentSize().height - (6 - row) * cardHeight;
		Rect rect(originX, originY, cardWidth, cardHeight);
		auto card = PokerCard::create(SpriteFrame::createWithTexture(texture, rect));
		card->setType(type);
		card->setValueInAll(i);
		card->setValueInType(i % 13 + 1);
		rawCardArray.pushBack(card);
	}
	log("Total Card : %d", rawCardArray.size());
	increaseCount(ref);
}

void GameManager::initNumbers(Ref * ref)
{
	auto texture = Director::getInstance()->getTextureCache()->getTextureForKey("poker_number.png");
	auto width = texture->getContentSize().width / 10;
	auto height = texture->getContentSize().height;
	for (int i = 0; i < 10; i++)
	{
		auto sprite = Sprite::createWithTexture(texture, Rect(i * width, 0, width, height));
		numberArray.pushBack(sprite);
	}
	increaseCount(ref);
}

GameManager::GameManager()
{
	//Ô¤¼ÓÔØ×ÊÔ´
	instance = nullptr;
	numberOfTotalRes = 7;
	numberOfLoadRes = 0;
	srand(time(nullptr));

	TextureCache::getInstance()->addImageAsync("logo.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("logo_dizhu.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("logo_nongmin.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("logo_unknown.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("poke_back_header.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("poker_number.png", CC_CALLBACK_1(GameManager::initNumbers, this));
	TextureCache::getInstance()->addImageAsync("puke_whole.png", CC_CALLBACK_1(GameManager::initCard, this));
	
}