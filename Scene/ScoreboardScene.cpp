#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "ScoreboardScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Engine/LOG.hpp"

void ScoreboardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    // title
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 48, halfW, halfH / 2 - 100, 0, 250, 0, 255, 0.5, 0.5));
    // read data to vector
    if(records.empty()) {
        std::ifstream fi("Resource/scoreboard.txt");
        if(fi.is_open()){
            std::string line;
            while (std::getline(fi, line)) {
                records.push_back(line);
                Engine::LOG(Engine::DEBUGGING) << "read line: " << line;
            }
            fi.close();
        }
        else{
            Engine::LOG(Engine::ERROR) << "failed to open scoreboard file";
        }
    }
    int yOffset = halfH / 2 + 50;
    // min and max index of the records on this page
    int start = curPage * pageLimit;
    int end = (start + pageLimit > records.size()) ? records.size() : start + pageLimit;
    for(int i = start; i < end; ++i){
        AddNewObject(new Engine::Label(records[i], "pirulen.ttf", 32, halfW, yOffset + (i - start) * 40, 0, 250, 0, 255, 0.5, 0.5));
    }
    if(records.empty()){
        AddNewObject(new Engine::Label("no scoreboard data.", "pirulen.ttf", 32, halfW, halfH, 255, 0, 0, 255, 0.5, 0.5));
    }

    // prev page button
    // won't show if no prev page
    if(curPage != 0){
        btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 650, halfH * 3 / 2 - 50, 400, 100);
        btn->SetOnClickCallback(std::bind(&ScoreboardScene::PrevPageOnClick, this, 1));
        AddNewControlObject(btn);
        AddNewObject(new Engine::Label("Prev Page", "pirulen.ttf", 40, halfW - 450, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    }

    // back button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    // next page button
    // won't show if no next page
    if(curPage * pageLimit + pageLimit < records.size()){
        btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 250, halfH * 3 / 2 - 50, 400, 100);
        btn->SetOnClickCallback(std::bind(&ScoreboardScene::NextPageOnClick, this, 1));
        AddNewControlObject(btn);
        AddNewObject(new Engine::Label("Next Page", "pirulen.ttf", 40, halfW + 450, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    }

    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}
void ScoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void ScoreboardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void ScoreboardScene::NextPageOnClick(int stage) {
    int maxPage = (records.size() + pageLimit - 1) / pageLimit-1;
    if(curPage < maxPage){
        ++curPage;
        Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
    }
}
void ScoreboardScene::PrevPageOnClick(int stage) {
    if(curPage > 0){
        --curPage;
        Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
    }
}