#include "Player.h"
#include "GameManager.h"

Player * Player::create(std::string name)
{
	auto player = new Player();
	player->initWithTexture(TextureCache::getInstance()->getTextureForKey("logo_unknown.png"));
	player->autorelease();
	player->setPlayerName(name);
	return player;
}

bool cardCompare(PokerCard *card1, PokerCard * card2)
{
	return card2->getValueInAll() < card1->getValueInAll();
}
/** type: nongming or dizhu; pos: position in the room; **/
void Player::setAttribute(int type, int roomPos, int index, std::vector<int>& cards)
{
	CC_ASSERT(type < 2 && roomPos >= 0);
	CC_ASSERT(roomPos < 3 && roomPos >= 0);
	CC_ASSERT(index < 3 && index >= 0);
	for (int i = 0; i < 17; i++)
	{
		leftCard.push_back(GameManager::getInstance()->getRawCardArray().at(cards.at(index * 17 + i)));
	}
	setType(type);
	setRoomPos(roomPos);
}

void Player::sortCard()
{
	sort(leftCard.begin(), leftCard.end(), cardCompare);
}
