#include "GameManager.h"

GameManager* GameManager::_gameManager = nullptr;
GameManager* GameManager::getInstance()
{
	if (!_gameManager)
	{
		_gameManager = new GameManager();
	}
	return _gameManager;
};