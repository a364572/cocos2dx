#include "Player.h"
#include "GameManager.h"

Player * Player::create(std::string name, int roomPos)
{
	auto player = new Player();
	player->initWithTexture(TextureCache::getInstance()->getTextureForKey("logo_unknown.png"));
	player->setPlayerName(name);
	player->setRoomPos(roomPos);
	player->autorelease();
	return player;
}

/** type: nongming or dizhu; pos: position in the room; **/
/** 传递的serverPos表示用户在服务器房间的位置，cards是牌的编号 **/
void Player::setAttribute(int serverPos, std::vector<int>& cards)
{
	CC_ASSERT(serverPos < 3 && serverPos >= 0);
	for (int i = 0; i < 17; i++)
	{
		leftCard.push_back(GameManager::getInstance()->rawCardArray.at(cards.at(i)));
	}
}

void Player::sortCard()
{
	sort(leftCard.begin(), leftCard.end(), PokerArrayUtil::compareByLocalValue);
}
