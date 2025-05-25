#include <functional>
#include <string>
#include <fstream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"
#include "Engine/LOG.hpp"

void WinScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH+50, 300, 300, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));

    AddNewObject(new Engine::Label("Enter your name to be saved into scoreboard: ", "pirulen.ttf", 28, halfW, halfH / 4 + 90, 255, 255, 255, 255, 0.5, 0.5));
    // display user input (input is handled in OnKeyDown)
    input = new Engine::Label(playerName, "pirulen.ttf", 28, halfW, halfH / 4 + 140, 0, 250, 0, 255, 0.5, 0.5);
    AddNewObject(input);
    
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");
}
void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}
void WinScene::BackOnClick(int stage) {
    if(playerName != ""){
        /* Note:
            when the path is `Resource/scoreboard.txt` it will access the file in build/
            write to both build file and actual file
            ensure the file under root dir (actual file) is update correctly
            if only write to root dir file, need to rebuild every time
        */
        std::ofstream fo("../Resource/scoreboard.txt", std::ios::app);      // root dir file
        std::ofstream fo_build("Resource/scoreboard.txt", std::ios::app);   // build dir file
        if(fo.is_open()){
            fo << playerName << std::endl;
            Engine::LOG(Engine::INFO) << "player name " << playerName << " written to scoreboard";
            fo.close();
        }
        else{
            Engine::LOG(Engine::ERROR) << "failed to open scoreboard file";
        }
        if(fo_build.is_open()){
            fo_build << playerName << std::endl;
            Engine::LOG(Engine::INFO) << "player name " << playerName << " written to scoreboard in build";
            fo_build.close();
        }
        else{
            Engine::LOG(Engine::ERROR) << "failed to open scoreboard file in build";
        }
    }
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void WinScene::OnKeyDown(int keyCode){
    Engine::LOG(Engine::DEBUGGING) << "keycode pressed: " << keyCode;
    if(keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z){
        playerName += static_cast<char>(keyCode - ALLEGRO_KEY_A + 'A');
    }
    else if(keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9){
        playerName += static_cast<char>(keyCode - ALLEGRO_KEY_0 + '0');
    }
    else if(keyCode == ALLEGRO_KEY_SPACE){
        playerName += ' ';
    }
    else if(keyCode == ALLEGRO_KEY_MINUS){
        playerName += '_';
    }
    else if(keyCode == ALLEGRO_KEY_BACKSPACE && !playerName.empty()){
        playerName.pop_back();
    }
    Engine::LOG(Engine::DEBUGGING) << "playerName: " << playerName;
    input->Text = playerName;
}
