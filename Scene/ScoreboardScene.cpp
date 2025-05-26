#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>

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
    if(!records.empty()){
        records.clear();
    }
    std::ifstream fi("../Resource/scoreboard.txt");
    if(fi.is_open()){
        std::string line;
        while(std::getline(fi, line)){
            // <date> <name> <score>
            std::istringstream iss(line);
            std::string date;
            std::string name;
            int score;
            if (iss >> date >> name >> score){
                records.push_back({date, name, score});
                Engine::LOG(Engine::DEBUGGING) << "read line: " << date << ' ' << name << ' ' << score;
            }
        }
        fi.close();
        if(!records.empty()) SortRecords();
    }
    else{
        Engine::LOG(Engine::ERROR) << "failed to open scoreboard file";
    }
    int yOffset = halfH / 2 + 50;
    // min and max index of the records on this page
    int start = curPage * pageLimit;
    int end = (start + pageLimit > records.size()) ? records.size() : start + pageLimit;
    for(int i = start; i < end; ++i){
        auto [date, name, score] = records[i];  // sturctured binding
        AddNewObject(new Engine::Label(date, "pirulen.ttf", 28, halfW - 300, yOffset + (i - start) * 40, 0, 250, 0, 255, 0.5, 0.5));
        AddNewObject(new Engine::Label(name, "pirulen.ttf", 32, halfW + 100, yOffset + (i - start) * 40, 0, 250, 0, 255, 0.5, 0.5));
        AddNewObject(new Engine::Label(std::to_string(score), "pirulen.ttf", 32, halfW + 400, yOffset + (i - start) * 40, 0, 250, 0, 255, 0.5, 0.5));
    }
    if(records.empty()){
        AddNewObject(new Engine::Label("No Scoreboard Data", "pirulen.ttf", 32, halfW, halfH, 255, 0, 0, 255, 0.5, 0.5));
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
}
void ScoreboardScene::Terminate() {
    IScene::Terminate();
}
void ScoreboardScene::BackOnClick(int stage) {
    curPage = 0;
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
void ScoreboardScene::SortRecords(){
    std::sort(records.begin(), records.end(), [](const auto& a, const auto& b) {
        return std::get<2>(a) > std::get<2>(b); // sort by score descending
    });
}