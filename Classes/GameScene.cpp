#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "ui/UITextField.h"

static size_t cols = Const::startColumns, rows = Const::startLines, colors = Const::startColors;

Scene* Game::createScene()
{
    return Game::create();
}


static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

Menu* Game::createMenu(const std::string normalImage, const std::string selectedImage, const ccMenuCallback& callback, float targetHeight, 
    float anchorPointX, float anchorPointY, float positionX, float positionY) {
    auto menuItem = MenuItemImage::create(normalImage, selectedImage, callback);
    menuItem->setScale(targetHeight / menuItem->getContentSize().height);
    menuItem->setAnchorPoint(Vec2(anchorPointX, anchorPointY));
    menuItem->setPosition(Vec2(positionX, positionY));
    auto menu = Menu::create(menuItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 0);
    return menu;
}

Label* Game::createLabel(const std::string text, const std::string fontFile, float targetHeight,
    float anchorPointX, float anchorPointY, float positionX, float positionY) {
    auto label = Label::createWithTTF(text, fontFile, 80);
    label->setScale(targetHeight / label->getContentSize().height);
    label->setAnchorPoint(Vec2(anchorPointX, anchorPointY));
    label->setPosition(Vec2(positionX, positionY));
    this->addChild(label, 1);
    return label;
}

bool Game::init()
{
    if (!Scene::init())
    {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("BG.png");
    this->addChild(background, -10);

    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();

    createMenu("buttonStart.png", "buttonStartPressed.png", CC_CALLBACK_1(Game::buttonStartCallback, this),
        Const::buttonStartHeight * visibleSize.height, 0.5, 0, visibleSize.width / 2, visibleSize.height * Const::buttonStartY);
    createMenu("close.png", "closePressed.png", CC_CALLBACK_1(Game::buttonCloseCallback, this),
        Const::buttonStartHeight * visibleSize.height, 1, 1, visibleSize.width, visibleSize.height);
    createMenu("plus.png", "plusPressed.png", CC_CALLBACK_1(Game::plusLinesCallback, this),
        visibleSize.height / 40., 0.5, 0.5, visibleSize.height / 60., visibleSize.height * 59. / 60.);
    createMenu("minus.png", "minusPressed.png", CC_CALLBACK_1(Game::minusLinesCallback, this),
        visibleSize.height / 40., 0.5, 0.5, visibleSize.height * 3. / 60., visibleSize.height * 59. / 60.);
    createMenu("plus.png", "plusPressed.png", CC_CALLBACK_1(Game::plusColumnsCallback, this),
        visibleSize.height / 40., 0.5, 0.5, visibleSize.height / 60., visibleSize.height * 57. / 60.);
    createMenu("minus.png", "minusPressed.png", CC_CALLBACK_1(Game::minusColumnsCallback, this),
        visibleSize.height / 40., 0.5, 0.5, visibleSize.height * 3. / 60., visibleSize.height * 57. / 60.);
    createMenu("plus.png", "plusPressed.png", CC_CALLBACK_1(Game::plusColorsCallback, this),
        visibleSize.height / 40., 0.5, 0.5, visibleSize.height / 60., visibleSize.height * 55. / 60.);
    createMenu("minus.png", "minusPressed.png", CC_CALLBACK_1(Game::minusColorsCallback, this),
        visibleSize.height / 40., 0.5, 0.5, visibleSize.height * 3. / 60., visibleSize.height * 55. / 60.);


    linesLabel = createLabel("Lines: " + std::to_string(rows), "fonts/Marker Felt.ttf", visibleSize.height / 30.,
        0, 0, visibleSize.height * 2. / 30, visibleSize.height * 29. / 30.);
    columnsLabel = createLabel("Columns: " + std::to_string(cols), "fonts/Marker Felt.ttf", visibleSize.height / 30.,
        0, 0, visibleSize.height * 2. / 30, visibleSize.height * 28. / 30.);
    colorsLabel = createLabel("Colors: " + std::to_string(colors), "fonts/Marker Felt.ttf", visibleSize.height / 30.,
        0, 0, visibleSize.height * 2. / 30, visibleSize.height * 27. / 30.);
    auto scoreLabel = createLabel("Score: 0", "fonts/Marker Felt.ttf", visibleSize.height / 20.,
        0, 0, visibleSize.width / 4., visibleSize.height * 28. / 30.);


    field.init(this, visibleSize.width, visibleSize.height, rows, cols, colors, scoreLabel);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseUp = CC_CALLBACK_1(Game::mouseClickCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    
    return true;
}

void Game::buttonStartCallback(cocos2d::Ref* pSender) {
    auto next_scene = Game::createScene();
    Director::getInstance()->replaceScene(next_scene);
}

void Game::buttonCloseCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->end();
}

void Game::mouseClickCallback(Event* event) {
    EventMouse* ev = (EventMouse*)event;
    field.changeByClick(ev->getCursorX(), ev->getCursorY());
}

void Game::plusLinesCallback(cocos2d::Ref* pSender) {
    if (rows < Const::maxLines) {
        rows++;
    }
    linesLabel->setString("Lines: " + std::to_string(rows));
}

void Game::minusLinesCallback(cocos2d::Ref* pSender) {
    if (rows > Const::minLines) {
        rows--;
    }
    linesLabel->setString("Lines: " + std::to_string(rows));
}

void Game::plusColumnsCallback(cocos2d::Ref* pSender) {
    if (cols < Const::maxCols) {
        cols++;
    }
    columnsLabel->setString("Columns: " + std::to_string(cols));
}

void Game::minusColumnsCallback(cocos2d::Ref* pSender) {
    if (cols > Const::minCols) {
        cols--;
    }
    columnsLabel->setString("Columns: " + std::to_string(cols));
}

void Game::plusColorsCallback(cocos2d::Ref* pSender) {
    if (colors < Const::maxColors) {
        colors++;
    }
    colorsLabel->setString("Colors: " + std::to_string(colors));
}

void Game::minusColorsCallback(cocos2d::Ref* pSender) {
    if (colors > Const::minColors) {
        colors--;
    }
    colorsLabel->setString("Colors: " + std::to_string(colors));
}