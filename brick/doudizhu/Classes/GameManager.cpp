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

GameManager::GameManager()
{
	//Ô¤¼ÓÔØ×ÊÔ´
	instance = nullptr;
	numberOfTotalRes = 5;
	numberOfLoadRes = 0;
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("", "");
	//numberOfLoadRes++;

	TextureCache::getInstance()->addImageAsync("logo.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("logo_unknown.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("puke_whole.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("logo_dizhu.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	TextureCache::getInstance()->addImageAsync("logo_nongmin.png", CC_CALLBACK_1(GameManager::increaseCount, this));

}