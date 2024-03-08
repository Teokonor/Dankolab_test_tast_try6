#include "Block.h"

Block::Block(size_t row, size_t col, float x0, float y0, float blockSize, size_t _colorNumber) : Row(row), Col(col),
colorNumber(_colorNumber), BlockSize(blockSize) {
	me = Sprite::create(Const::blockFileNames[colorNumber]);
	me->setAnchorPoint(Vec2(0, 0));
	me->setPosition(Vec2(x0 + blockSize * (float)col, y0 + blockSize * (float)row));
	me->setScale(blockSize / me->getContentSize().width);
}

void Block::initNeighbors(Block* left, Block* up, Block* right, Block* down) {
	Left = left; Up = up; Right = right; Down = down;
}

void Block::setConnections(Block* thisOrNull) {
	if (Left) {
		Left->Right = thisOrNull;
	}
	if (Up) {
		Up->Down = thisOrNull;
	}
	if (Right) {
		Right->Left = thisOrNull;
	}
	if (Down) {
		Down->Up = thisOrNull;
	}
}

Block::~Block() {
	setConnections(NULL);
}

bool Block::notCheckedAndColorNumberIs(size_t colorNum) {
	if (checked) {
		return false;
	}
	else {
		checked = (colorNumber == colorNum);
		return checked;
	}
}

void Block::move(size_t newRow, size_t newCol, Block* left, Block* up, Block* right, Block* down) {
	setConnections(NULL);
	Row = newRow; Col = newCol;
	initNeighbors(left, up, right, down);
	setConnections(this);
}