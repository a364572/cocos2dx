#include "PokerCard.h"

PokerCard * PokerCard::create(SpriteFrame* frame)
{
	auto card = new PokerCard();
	card->initWithSpriteFrame(frame);
	card->autorelease();
	return card;
}
