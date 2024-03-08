#include "Field.h"
#include <random>

using namespace CocosDenshion;

void Field::init(Scene* _scene, float width, float height, size_t lines, size_t columns, size_t colorsQuantity, Label* scoreLabel) {
	blockSize = MIN(height * (1 - Const::buttonStartZoneHeight - Const::indentToPLayingField) / (float)lines, 
		(width - height * Const::indentToPLayingField) / (float)columns);
	Lines = lines, Columns = columns;
	X0 = (width - blockSize * (float)columns) / 2;
	Y0 = (height * (1 - Const::buttonStartZoneHeight) - blockSize * (float)lines) / 2;
	scene = _scene;
	ColorsQuantity = colorsQuantity;
	ScoreLabel = scoreLabel;
	blocks.reserve(lines * sizeof(std::vector<Block*>));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, colorsQuantity - 1);
	for (size_t i = 0; i < lines; i++) {
		std::vector<Block*> line;
		line.reserve(columns * sizeof(Block*));
		for (size_t j = 0; j < columns; j++) {
			Block* bl = new Block(i, j, X0, Y0, blockSize, dist(gen));
			scene->addChild(bl->getSprite(), 10);
			line.push_back(bl);
		}
		blocks.push_back(line);
	}
	for (size_t i = 0; i < lines; i++) {
		for (size_t j = 0; j < columns; j++) {
			blocks[i][j]->initNeighbors(j > 0 ? blocks[i][j - 1] : NULL, i < lines - 1 ? blocks[i + 1][j] : NULL,
				j < columns - 1 ? blocks[i][j + 1] : NULL, i > 0 ? blocks[i - 1][j] : NULL);
		}
	}
}

Field::~Field() {
	for(size_t i = 0; i < Lines; i++) {
		for (size_t j = 0; j < Columns; j++) {
			if (blocks[i][j] != NULL) {
				delete(blocks[i][j]);
				blocks[i][j] = NULL;
			}
		}
	}
}



std::vector<std::pair<Block*, size_t>> Field::findNeighborsSameColor(size_t row, size_t col) {
	size_t colorNum = blocks[row][col]->colorNum();
	std::vector<std::pair<Block*, size_t>> founded;
	size_t counter_deleting_blocks = 1;
	founded.push_back({ blocks[row][col], 0 });
	founded[0].first->notCheckedAndColorNumberIs(colorNum);
	for (size_t i = 0; i < counter_deleting_blocks; i++) {
		Block* bl = founded[i].first;
		size_t num = founded[i].second;
		Block* neighbors[4] = { bl->left(), bl->up(), bl->right(), bl->down() };
		for (Block* neighbor : neighbors) {
			if (neighbor && neighbor->notCheckedAndColorNumberIs(colorNum)) {
				founded.push_back({ neighbor, num + 1 });
				counter_deleting_blocks++;
			}
		}
	}
	return founded;
}

void Field::moveBlocks(std::vector<std::pair<Block*, size_t>>& deleting) {
	std::vector<bool> columnChanged(Columns, false);
	std::vector<size_t> lowerBlocks(Columns, Lines);
	size_t maxDeletingBlockNum = 0;
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("blockBreak.mp3");
	score += deleting.size();
	ScoreLabel->setString("Score: " + std::to_string(score));
	for (std::pair<Block*, size_t> elem : deleting) {
		Block* bl = elem.first;
		columnChanged[bl->col()] = true;
		lowerBlocks[bl->col()] = MIN(bl->row(), lowerBlocks[bl->col()]);
		maxDeletingBlockNum = MAX(maxDeletingBlockNum, elem.second);
		blocks[bl->row()][bl->col()] = NULL;
		auto animationSprite = Sprite::create(Const::blockFileNames[bl->colorNum()]);
		auto delay = DelayTime::create(Const::fadeDelay * elem.second);
		auto fadeOut = FadeOut::create(Const::fadeDuration);
		animationSprite->setAnchorPoint(Vec2(0, 0));
		animationSprite->setPosition(Vec2(X0 + bl->col() * blockSize, Y0 + bl->row() * blockSize));
		animationSprite->setScale(blockSize / animationSprite->getContentSize().width);
		animationSprite->runAction(Sequence::create(delay, fadeOut, nullptr));
		scene->removeChild(bl->getSprite());
		scene->addChild(animationSprite);
		delete(bl);
	}
	size_t dropMaxHeight = 0;
	lastAnimationDur = 0;
	for (size_t col = 0; col < Columns; col++) {
		if (columnChanged[col]) {
			size_t vertical_offset = 0;
			for (size_t row = lowerBlocks[col]; row < Lines; row++) {
				Block* bl = blocks[row][col];
				if (bl == NULL) {
					vertical_offset++;
				}
				else {
					dropMaxHeight = MAX(dropMaxHeight, vertical_offset);
					size_t row_new = row - vertical_offset;
					bl->move(row_new, col, col > 0 ? blocks[row_new][col - 1] : NULL, NULL, 
						col + 1 < Columns ? blocks[row_new][col + 1] : NULL, row_new > 0 ? blocks[row_new - 1][col] : NULL);
					auto delay = DelayTime::create(Const::fadeDuration + Const::fadeDelay * maxDeletingBlockNum);
					auto moveTo = MoveTo::create((1. / (float)Const::blocksFallingSpeed) * vertical_offset, 
						Vec2(X0 + col * blockSize, Y0 + row_new * blockSize));
					bl->getSprite()->runAction(Sequence::create(delay, moveTo, nullptr));
					blocks[row_new][col] = bl;
					blocks[row][col] = NULL;
				}
			}
		}
	}
	lastAnimationDur = clock_t(1000. * (Const::fadeDuration + Const::fadeDelay * maxDeletingBlockNum) + 
		1000. / (float)Const::blocksFallingSpeed * (float)dropMaxHeight);
	checkHavingMoves();
}

bool Field::changeByClick(float x, float y) {
	if (x < X0 || y < Y0 || x > X0 + blockSize * (float)Columns || y > Y0 + blockSize * (float)Lines || std::clock() < lastMovement + lastAnimationDur) {
		return false;
	}
	size_t col = (size_t)((x - X0) / blockSize + 0.05), row = (size_t)((y - Y0) / blockSize + 0.05);
	if (blocks[row][col] == NULL) {
		return false;
	}
	std::vector<std::pair<Block*, size_t>> deleting = findNeighborsSameColor(row, col);
	size_t quantityDeleting = deleting.size();
	if (quantityDeleting < 3) {
		for (size_t i = 0; i < quantityDeleting; i++) {
			deleting[i].first->uncheck();
		}
		return false;
	}
	lastMovement = std::clock();
	moveBlocks(deleting);
	return true;
}

bool Field::checkHavingMoves() {
	for (size_t i = 0; i < Lines; i++) {
		for (size_t j = 0; j < Columns; j++) {
			Block* bl = blocks[i][j];
			if (bl) {
				size_t colorNum = bl->colorNum(), heighborsWithSameColor = 0;
				Block* neighbors[4] = { bl->left(), bl->up(), bl->right(), bl->down() };
				for (Block* neighbor : neighbors) {
					if (neighbor) {
						heighborsWithSameColor += (neighbor->colorNum() == colorNum);
					}
				}
				if (heighborsWithSameColor >= 2) {
					return true;
				}
			}
		}
	}
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("applause.mp3");
	auto labelGameOver = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 280);
	labelGameOver->setScale(blockSize * Columns / labelGameOver->getContentSize().width);
	labelGameOver->setAnchorPoint(Vec2(0.5, 0.5));
	labelGameOver->setPosition(Vec2(X0 + blockSize * Columns / 2, Y0 + blockSize * Lines / 2));
	scene->addChild(labelGameOver, 15);
	return false;
}