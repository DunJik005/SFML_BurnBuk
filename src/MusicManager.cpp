//
// Created by Dun Jik on 2/12/2026.
//

#include "MusicManager.h"
#include <filesystem>
#include <iostream>
#include <thread>

using namespace std;
namespace fs = std::filesystem;

MusicManager& MusicManager::instance() {
    static MusicManager inst;
    return inst;
}

MusicManager::MusicManager() : sound(soundBuffer)
{
    baseAttackToSound = {
        {BaseAttack::Chomper,   "assets/music/sounds/ChomperPlace.ogg"},
        {BaseAttack::Peashooter,"assets/music/sounds/PeashooterPlace.ogg"},
        {BaseAttack::Catapult,  "assets/music/sounds/CatapultPlace.ogg"},
        {BaseAttack::Nut,       "assets/music/sounds/NutPlace.ogg"},
        {BaseAttack::Jelepeno,  "assets/music/sounds/JelepenoPlace.ogg"},
        {BaseAttack::Pijavica,  "assets/music/sounds/DefaultPlace.ogg"},
        {BaseAttack::Hemisfera, "assets/music/sounds/DefaultPlace.ogg"},
        {BaseAttack::Sniper,    "assets/music/sounds/DefaultPlace.ogg"},
        {BaseAttack::Pump,      "assets/music/sounds/DefaultPlace.ogg"}
    };
}





void MusicManager::setBackgroundVolume(float volume) {
    bgVolume = volume;
    background.setVolume(bgVolume);
}


void MusicManager::loadBackgroundPlaylist(const string& folder) {
    for (auto& p : fs::directory_iterator(folder)) {
        if (p.path().extension() == ".ogg")
            bgTracks.push_back(p.path().string());
    }
}

void MusicManager::playBackground() {
    if (bgTracks.empty())
        return;
    if (!background.openFromFile(bgTracks[currentTrack])) {
        cout << "Ne mogu da otvorim: " << bgTracks[currentTrack] << endl;
        return;
    }
    background.setVolume(bgVolume);
    background.play();
}



void MusicManager::nextTrack() {
    if (bgTracks.empty())
        return;
    currentTrack = (currentTrack + 1) % bgTracks.size();
    if (!background.openFromFile(bgTracks[currentTrack])) {
        cout << "Ne mogu da otvorim: " << bgTracks[currentTrack] << endl;
        return;
    }
    background.setVolume(bgVolume);
    background.play();
}



void MusicManager::update() {
    if (background.getStatus() == sf::SoundSource::Status::Stopped)
        nextTrack();
}




void MusicManager::playCardSound(const Card& card) {
    string path;

    if (!card.getSoundPath().empty()) {
        path = card.getSoundPath();
    } else {
        auto it = baseAttackToSound.find(card.getBaseAttack());
        path = (it != baseAttackToSound.end())
            ? it->second
            : "assets/music/sounds/DefaultPlace.ogg";
    }

    if (!soundBuffer.loadFromFile(path)) {
        cout << "Ne mogu da ucitam sound: " << path << endl;
        return;
    }

    // 1. Duck background
    background.setVolume(bgVolume * 0.05f); // smanji na 20%

    // 2. Postavi i odigraj zvuk karte jače
    sound.setBuffer(soundBuffer);
    sound.setVolume(100.f); // jačina zvuka karte
    sound.play();

    // 3. Thread koji vraća jačinu background-a kad zvuk završi
    sf::Sound* s = &sound;
    sf::Music* bg = &background;
    float originalVolume = bgVolume;

    std::thread([s, bg, originalVolume]() {
        while (s->getStatus() == sf::SoundSource::Status::Playing) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        bg->setVolume(originalVolume); // vrati pozadinu
    }).detach();
}