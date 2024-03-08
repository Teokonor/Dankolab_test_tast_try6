#pragma once
#include "Const.h"
#include "cocos2d.h"

USING_NS_CC;

class Block {
	size_t Row, Col;
	Block* Left = NULL, * Up = NULL, * Right = NULL, * Down = NULL;
	Sprite* me;
	size_t colorNumber;
	float BlockSize;
	bool checked = false;
public:
	Block(size_t row, size_t col, float x0, float y0, float blockSize, size_t _colorNumber);
	void initNeighbors(Block* left, Block* up, Block* right, Block* down);
	void setConnections(Block* thisOrNull);
	~Block();

	size_t row() { return Row; }
	size_t col() { return Col; }
	Sprite* getSprite() { return me; }
	size_t colorNum() { return colorNumber; }
	Block* left() { return Left; }
	Block* up() { return Up; }
	Block* right() { return Right; }
	Block* down() { return Down; }

	bool notCheckedAndColorNumberIs(size_t colorNum);
	void uncheck() { checked = false; }
	void move(size_t newRow, size_t newCol, Block* left, Block* up, Block* right, Block* down);
};