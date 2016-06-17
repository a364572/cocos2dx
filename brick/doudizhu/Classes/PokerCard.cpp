#include "PokerCard.h"

PokerCard * PokerCard::create(SpriteFrame* frame)
{
	auto card = new PokerCard();
	card->setSelect(false);
	card->initWithSpriteFrame(frame);
	card->autorelease();
	return card;
}

bool PokerArray::isSingle(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 1)
	{
		pokerArray->lengthOfType = 1;
		return true;
	}
	return false;
}

bool PokerArray::isPair(PokerArray *pokerArray)
{
	//two card with no queen
	if (pokerArray->cardArray.size() == 2)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == 
			pokerArray->cardArray.at(1)->getValueInType() &&
			!pokerArray->cardArray.at(0)->isQueen())
		{
			return true;
			pokerArray->lengthOfType = 1;
		}
	}
	return false;
}

bool PokerArray::isSequence(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() < 5)
	{
		return false;
	}
	//the first card should not be the queen
	if (!pokerArray->cardArray.at(0)->isQueen())
	{
		int value = pokerArray->cardArray.at(0)->getValueInType();
		for (int i = 1; i < pokerArray->cardArray.size(); i++)
		{
			auto nextValue = pokerArray->cardArray.at(i)->getValueInType();
			if (value - nextValue != 1)
			{
				return false;
			}
			value = nextValue;
		}
		pokerArray->lengthOfType = pokerArray->cardArray.size();
		return true;
	}
	return false;
}

bool PokerArray::isMultiplePair(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() >= 6 && pokerArray->cardArray.size() % 2 == 0)
	{
		if (!pokerArray->cardArray.at(0)->isQueen())
		{
			int value = pokerArray->cardArray.at(0)->getValueInType();
			for (int i = 0; i < pokerArray->cardArray.size(); i = i + 2)
			{
				int first = pokerArray->cardArray.at(i)->getValueInType();
				int second = pokerArray->cardArray.at(i + 1)->getValueInType();
				if (first != second)
				{
					return false;
				}
				if (i > 0)
				{
					if (value - first != 1)
					{
						return false;
					}
					value = first;
				}
			}
			pokerArray->lengthOfType = pokerArray->cardArray.size() / 2;
			return true;
		}
	}
	return false;
}

/**Three card should be the same**/
bool PokerArray::isThreeWithNone(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 3)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType())
		{
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

/** call sortByLength first **/
bool PokerArray::isThreeWithSingle(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 4)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType())
		{
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

/** call sortByLength first **/
bool PokerArray::isThreeWithPair(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 5)
	{
		std::vector<PokerCard *> first(pokerArray->cardArray.begin(), pokerArray->cardArray.begin() + 3);
		std::vector<PokerCard *> second(pokerArray->cardArray.begin() + 3, pokerArray->cardArray.end());
		PokerArray arry1(first);
		PokerArray arry2(second);
		if (isThreeWithNone(&arry1) && isPair(&arry2))
		{
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

/** call sortByLength first **/
bool PokerArray::isPlaneWithNone(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() > 0 && pokerArray->cardArray.size() % 3 == 0)
	{
		int prevValue = pokerArray->cardArray.at(0)->getValueInType();
		for (int i = 0; i < pokerArray->cardArray.size(); i += 3)
		{
			std::vector<PokerCard *> cards(pokerArray->cardArray.begin() + i, pokerArray->cardArray.begin() + i + 3);
			PokerArray array(cards);
			if (!isThreeWithNone(&array))
			{
				return false;
			}
			if (i > 0)
			{
				int nextValue = pokerArray->cardArray.at(i)->getValueInType();
				if (prevValue - nextValue != 1)
				{
					return false;
				}
				prevValue = nextValue;
			}

		}
		pokerArray->lengthOfType = pokerArray->cardArray.size() / 3;
		return true;
	}
	return false;
}

/** call sortByLength first **/
bool PokerArray::isPlaneWithSingle(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() > 0 && pokerArray->cardArray.size() % 4 == 0)
	{
		int prevValue = pokerArray->cardArray.at(0)->getValueInType();
		int length = pokerArray->cardArray.size() / 4;
		for (int i = 0; i < length; i++)
		{
			std::vector<PokerCard *> cards(pokerArray->cardArray.begin() + i * 3, pokerArray->cardArray.begin() + i * 3 + 3);
			PokerArray array(cards);
			if (!isThreeWithNone(&array))
			{
				return false;
			}
			if (i > 0)
			{
				int nextValue = pokerArray->cardArray.at(i * 3)->getValueInAll();
				if (prevValue - nextValue != 1)
				{
					return false;
				}
				prevValue = nextValue;
			}
		}
		pokerArray->lengthOfType = length;
		return true;
	}
	return false;
}

/** call sortByLength first **/
bool PokerArray::isPlaneWithPair(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() > 0 && pokerArray->cardArray.size() % 5 == 0)
	{
		int prevValue = pokerArray->cardArray.at(0)->getValueInType();
		int length = pokerArray->cardArray.size() / 5;
		for (int i = 0; i < length; i++)
		{
			std::vector<PokerCard *> cards(pokerArray->cardArray.begin() + i * 3, pokerArray->cardArray.begin() + i * 3 + 3);
			PokerArray array(cards);
			if (!isThreeWithNone(&array))
			{
				return false;
			}
			if (i > 0)
			{
				int nextValue = pokerArray->cardArray.at(i * 3)->getValueInAll();
				if (prevValue - nextValue != 1)
				{
					return false;
				}
				prevValue = nextValue;
			}
		}
		//pair
		int beginIndex = length * 3;
		for (int i = 0; i < length; i++)
		{
			std::vector<PokerCard *> cards(pokerArray->cardArray.begin() + beginIndex + i * 2, pokerArray->cardArray.begin() + beginIndex +  i * 2 + 2);
			PokerArray array(cards);
			if (!isPair(&array))
			{
				return false;
			}
		}
		pokerArray->lengthOfType = length;
		return true;
	}
	return false;
}

bool PokerArray::isBomb(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 4)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(1)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType() &&
			pokerArray->cardArray.at(2)->getValueInType() == pokerArray->cardArray.at(3)->getValueInType())
		{
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

bool PokerArray::isBombWithSingle(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 5)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(1)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType() &&
			pokerArray->cardArray.at(2)->getValueInType() == pokerArray->cardArray.at(3)->getValueInType())
		{
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

bool PokerArray::isBombWithPair(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 6)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(1)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType() &&
			pokerArray->cardArray.at(2)->getValueInType() == pokerArray->cardArray.at(3)->getValueInType() &&
			pokerArray->cardArray.at(4)->getValueInType() == pokerArray->cardArray.at(5)->getValueInType())
		{
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

bool PokerArray::isQueenBomb(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 0)
	{
		if (pokerArray->cardArray.at(1)->getValueInAll() >= 52)
		{
			pokerArray->lengthOfType = 1;
			return true;
		}
		return false;
	}
}

/**	count the length of each value, and sort them by the length
	values of the same length will be further sorted by their value **/
void PokerArray::sortByLength()
{
	if (cardArray.size() <= 1)
	{
		return;
	}
	//sort the value firstly for secondary sort
	sortByValue();

	//count the number of each value 
	std::map<int, int> map;
	for (auto card : cardArray)
	{
		int value = card->getValueInType();
		if (map.find(value) == map.end())
		{
			map[value] = 0;
		}
		map[value] = map[value] + 1;
	}
	
	//get all the key
	std::vector<int> keyVector;
	for (auto element : map)
	{
		keyVector.push_back(element.first);
	}

	//sort the key and insert it into the vector
	int count = keyVector.size();
	std::vector<int> sortedKey;
	for (int i = 0; i < count; i++)
	{
		int max = 0;
		int maxIndex = 0;
		for (int j = 0; j < keyVector.size(); j++)
		{
			int key = keyVector.at(j);
			int value = map[key];
			if (value > max)
			{
				max = value;
				maxIndex = j;
			}
			if (value == max)
			{
				if (key > keyVector.at(maxIndex))
				{
					max = value;
					maxIndex = j;
				}
			}
		}
		int maxKey = keyVector.at(maxIndex);
		sortedKey.push_back(maxKey);
		keyVector.erase(keyVector.begin() + maxIndex);
	}

	//resort the card
	std::vector<PokerCard *> tmpCards(cardArray);
	cardArray.clear();
	for (auto element : sortedKey)
	{
		for (auto card : tmpCards)
		{
			if (card->getValueInType() == element)
			{
				cardArray.push_back(card);
			}
		}
	}
}

void PokerArray::sortByValue()
{
	sort(cardArray.begin(), cardArray.end(), PokerArray::compareByLocalValue);
}

