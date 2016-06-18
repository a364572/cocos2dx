#include "PokerCard.h"

PokerArrayUtil *PokerArrayUtil::instance = nullptr;
std::map<PokerArrayType, JUDGE_FUNCTION> PokerArrayUtil::funMap = std::map<PokerArrayType, JUDGE_FUNCTION>();	//easy to get the counterpart function
std::vector<PokerArrayType> PokerArrayUtil::funList = std::vector<PokerArrayType>();				//card type priority
PokerCard * PokerCard::create(SpriteFrame* frame)
{
	auto card = new PokerCard();
	card->setSelect(false);
	card->initWithSpriteFrame(frame);
	card->autorelease();
	return card;
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
	sort(cardArray.begin(), cardArray.end(), PokerArrayUtil::compareByLocalValue);
}

PokerArray::PokerArray()
{
	this->type = ILLEGAL;
}

PokerArray::PokerArray(std::vector<PokerCard *> cards)
{
	this->type = ILLEGAL;
	for (auto card : cards)
	{
		cardArray.push_back(card);
	}
	PokerArrayUtil::judgeTypeOfCards(this);
}


void PokerArrayUtil::judgeTypeOfCards(PokerArray *pokerArray)
{
	pokerArray->sortByValue();
	
	for (int i = 0; i < funList.size(); i++)
	{
		auto fun = funList[i];
		if (fun == PokerArrayType::PLANE_WITH_PAIR)
		{
			log("Sort the card by length!");
			pokerArray->sortByLength();
		}
		if (funMap[fun](pokerArray))
		{
			break;
		}
	}
}

void PokerArrayUtil::initFunction()
{
	funList.clear();
	funMap.clear();
	
	//function for sortByValue
	funList.push_back(PokerArrayType::QUEEN_BOMB);			// == 2
	funList.push_back(PokerArrayType::MULTIPLE_PAIR);		// >= 6
	funList.push_back(PokerArrayType::SEQUENCE);			// >= 5
	funList.push_back(PokerArrayType::BOMB);				// == 4
	funList.push_back(PokerArrayType::THREE_WITH_NONE);		// == 3
	funList.push_back(PokerArrayType::PAIR);				// == 2
	funList.push_back(PokerArrayType::SINGLE);			// == 1

	//function for sortByLength
	funList.push_back(PokerArrayType::PLANE_WITH_PAIR);		// >= 10
	funList.push_back(PokerArrayType::PLANE_WITH_SINGLE);	// >= 8
	funList.push_back(PokerArrayType::BOMB_WITH_PAIR);		// == 8
	funList.push_back(PokerArrayType::PLANE_WITH_NONE);		// >= 6
	funList.push_back(PokerArrayType::BOMB_WITH_SINGLE);	// == 6
	funList.push_back(PokerArrayType::THREE_WITH_PAIR);		// ==5
	funList.push_back(PokerArrayType::THREE_WITH_SINGLE);	// == 4

	
	funMap[PokerArrayType::SINGLE] = isSingle;
	funMap[PokerArrayType::QUEEN_BOMB] = isQueenBomb;
	funMap[PokerArrayType::PAIR] = isPair;
	funMap[PokerArrayType::THREE_WITH_NONE] = isThreeWithNone;
	funMap[PokerArrayType::BOMB] = isBomb;
	funMap[PokerArrayType::THREE_WITH_SINGLE] = isThreeWithSingle;
	funMap[PokerArrayType::THREE_WITH_PAIR] = isThreeWithPair;
	funMap[PokerArrayType::SEQUENCE] = isSequence;
	funMap[PokerArrayType::BOMB_WITH_SINGLE] = isBombWithSingle;
	funMap[PokerArrayType::MULTIPLE_PAIR] = isMultiplePair;
	funMap[PokerArrayType::PLANE_WITH_NONE] = isPlaneWithNone;
	funMap[PokerArrayType::BOMB_WITH_PAIR] = isBombWithPair;
	funMap[PokerArrayType::PLANE_WITH_SINGLE] = isPlaneWithSingle;
	funMap[PokerArrayType::PLANE_WITH_PAIR] = isPlaneWithPair;
}

bool PokerArrayUtil::isSingle(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 1)
	{
		pokerArray->type = PokerArrayType::SINGLE;
		pokerArray->lengthOfType = 1;
		return true;
	}
	return false;
}

bool PokerArrayUtil::isPair(PokerArray *pokerArray)
{
	//two card with no queen
	if (pokerArray->cardArray.size() == 2)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() ==
			pokerArray->cardArray.at(1)->getValueInType() &&
			!pokerArray->cardArray.at(0)->isQueen())
		{
			pokerArray->type = PokerArrayType::PAIR;
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

bool PokerArrayUtil::isSequence(PokerArray *pokerArray)
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
			if (value == 15)
			{
				return false;
			}
			auto nextValue = pokerArray->cardArray.at(i)->getValueInType();
			if (value - nextValue != 1)
			{
				return false;
			}
			value = nextValue;
		}
		pokerArray->type = PokerArrayType::SEQUENCE;
		pokerArray->lengthOfType = pokerArray->cardArray.size();
		return true;
	}
	return false;
}

bool PokerArrayUtil::isMultiplePair(PokerArray *pokerArray)
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
			pokerArray->type = PokerArrayType::MULTIPLE_PAIR;
			pokerArray->lengthOfType = pokerArray->cardArray.size() / 2;
			return true;
		}
	}
	return false;
}

/**Three card should be the same**/
bool PokerArrayUtil::isThreeWithNone(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 3)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType())
		{
			pokerArray->type = PokerArrayType::THREE_WITH_NONE;
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

/** call sortByLength first **/
bool PokerArrayUtil::isThreeWithSingle(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 4)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType())
		{
			pokerArray->type = PokerArrayType::THREE_WITH_SINGLE;
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

/** call sortByLength first **/
bool PokerArrayUtil::isThreeWithPair(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 5)
	{
		std::vector<PokerCard *> first(pokerArray->cardArray.begin(), pokerArray->cardArray.begin() + 3);
		std::vector<PokerCard *> second(pokerArray->cardArray.begin() + 3, pokerArray->cardArray.end());
		PokerArray arry1(first);
		PokerArray arry2(second);
		if (isThreeWithNone(&arry1) && isPair(&arry2))
		{
			pokerArray->type = PokerArrayType::THREE_WITH_PAIR;
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

/** call sortByLength first **/
bool PokerArrayUtil::isPlaneWithNone(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() >= 6 && pokerArray->cardArray.size() % 3 == 0)
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
		pokerArray->type = PokerArrayType::PLANE_WITH_NONE;
		pokerArray->lengthOfType = pokerArray->cardArray.size() / 3;
		return true;
	}
	return false;
}

/** call sortByLength first **/
bool PokerArrayUtil::isPlaneWithSingle(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() >= 8 && pokerArray->cardArray.size() % 4 == 0)
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
				int nextValue = pokerArray->cardArray.at(i * 3)->getValueInType();
				if (prevValue - nextValue != 1)
				{
					return false;
				}
				prevValue = nextValue;
			}
		}
		pokerArray->type = PokerArrayType::PLANE_WITH_SINGLE;
		pokerArray->lengthOfType = length;
		return true;
	}
	return false;
}

/** call sortByLength first **/
bool PokerArrayUtil::isPlaneWithPair(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() >= 10 && pokerArray->cardArray.size() % 5 == 0)
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
				int nextValue = pokerArray->cardArray.at(i * 3)->getValueInType();
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
			std::vector<PokerCard *> cards(pokerArray->cardArray.begin() + beginIndex + i * 2, pokerArray->cardArray.begin() + beginIndex + i * 2 + 2);
			PokerArray array(cards);
			if (!isPair(&array))
			{
				return false;
			}
		}
		pokerArray->type = PokerArrayType::PLANE_WITH_PAIR;
		pokerArray->lengthOfType = length;
		return true;
	}
	return false;
}

bool PokerArrayUtil::isBomb(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 4)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(1)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType() &&
			pokerArray->cardArray.at(2)->getValueInType() == pokerArray->cardArray.at(3)->getValueInType())
		{
			pokerArray->type = PokerArrayType::BOMB;
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

bool PokerArrayUtil::isBombWithSingle(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 6)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(1)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType() &&
			pokerArray->cardArray.at(2)->getValueInType() == pokerArray->cardArray.at(3)->getValueInType())
		{
			pokerArray->type = PokerArrayType::BOMB_WITH_SINGLE;
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

bool PokerArrayUtil::isBombWithPair(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 8)
	{
		if (pokerArray->cardArray.at(0)->getValueInType() == pokerArray->cardArray.at(1)->getValueInType() &&
			pokerArray->cardArray.at(1)->getValueInType() == pokerArray->cardArray.at(2)->getValueInType() &&
			pokerArray->cardArray.at(2)->getValueInType() == pokerArray->cardArray.at(3)->getValueInType() &&
			pokerArray->cardArray.at(4)->getValueInType() == pokerArray->cardArray.at(5)->getValueInType() &&
			pokerArray->cardArray.at(6)->getValueInType() == pokerArray->cardArray.at(7)->getValueInType())
		{
			pokerArray->type = PokerArrayType::BOMB_WITH_PAIR;
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

bool PokerArrayUtil::isQueenBomb(PokerArray *pokerArray)
{
	if (pokerArray->cardArray.size() == 2)
	{
		if (pokerArray->cardArray.at(1)->getValueInAll() >= 52)
		{
			pokerArray->type = PokerArrayType::QUEEN_BOMB;
			pokerArray->lengthOfType = 1;
			return true;
		}
	}
	return false;
}

bool PokerArrayUtil::compareByLocalValue(PokerCard *card1, PokerCard *card2)
{
	if (card2->getValueInType() == card1->getValueInType())
	{
		return card2->getType() < card1->getType();
	}
	return card2->getValueInType() < card1->getValueInType();
}
