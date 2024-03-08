#pragma once
#include "Block.h"
#include "SimpleAudioEngine.h"
#include <vector>
#include <chrono>

/* ����� Field - ����� �������� ����. �� �������� � ���� ���������� � ������������ �������� �������� ���� ������ ����,
   � ����� ������ �����, ������ �� ������� �������� ��������, ����� ���������� � �������� ��������� �� ������� */
class Field {
	Scene* scene;
	float X0, Y0, blockSize;
	size_t Lines, Columns, ColorsQuantity;
	std::vector<std::vector<Block*>> blocks;
	clock_t lastMovement = 0, lastAnimationDur = 0;
	size_t score = 0;
	Label* ScoreLabel;

	std::vector<std::pair<Block*, size_t>> findNeighborsSameColor(size_t row, size_t col);
	void moveBlocks(std::vector<std::pair<Block*, size_t>>& deleting);
	bool checkHavingMoves();
public:
	void init(Scene* _scene, float width, float height, size_t lines, size_t columns, size_t colorsQuantity, Label* scoreLabel);
	~Field();
	bool changeByClick(float x, float y);
};