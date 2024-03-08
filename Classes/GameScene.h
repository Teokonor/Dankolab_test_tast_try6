#pragma once
#include "Field.h"

class Game : public cocos2d::Scene
{
private:
    Field field;
    Label* linesLabel, * columnsLabel, * colorsLabel;
    Menu* createMenu(const std::string normalImage, const std::string selectedImage, const ccMenuCallback& callback, float targetHeight,
        float anchorPointX, float anchorPointY, float positionX, float positionY);
    Label* createLabel(const std::string text, const std::string fontFile, float targetHeight,
        float anchorPointX, float anchorPointY, float positionX, float positionY);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void buttonStartCallback(cocos2d::Ref* pSender);
    void buttonCloseCallback(cocos2d::Ref* pSender);
    void mouseClickCallback(Event* event);

    void plusLinesCallback(cocos2d::Ref* pSender);
    void minusLinesCallback(cocos2d::Ref* pSender);
    void plusColumnsCallback(cocos2d::Ref* pSender);
    void minusColumnsCallback(cocos2d::Ref* pSender);
    void minusColorsCallback(cocos2d::Ref* pSender);
    void plusColorsCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);
};