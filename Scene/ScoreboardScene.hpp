#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include <vector>

#include "Engine/IScene.hpp"

class ScoreboardScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<std::string> records;
    const int pageLimit = 5;
    int curPage = 0;

public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void NextPageOnClick(int stage);
    void PrevPageOnClick(int stage);
    void BackOnClick(int stage);
};

#endif
