//
// Created by Dun Jik on 2/12/2026.
//

#ifndef BURNBOOKALFA_MUSICMANAGER_H
#define BURNBOOKALFA_MUSICMANAGER_H


#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <map>
#include "Card.h"

class MusicManager {
public:
    static MusicManager& instance();

    void loadBackgroundPlaylist(const std::string& folder);
    void playBackground();
    void update(); // proverava da li je pesma završila

    void playCardSound(const Card& card);

    void setBackgroundVolume(float volume);

private:

    float bgVolume = 100.f; // početna jačina background muzike


    MusicManager();

#include <memory>

    sf::Music background;        // može default konstruktor
    sf::SoundBuffer soundBuffer; // može default konstruktor
    sf::Sound sound;             // OK, može default konstruktor jer ćemo mu setBuffer kasnije


    std::vector<std::string> bgTracks;
    int currentTrack = 0;

    std::map<BaseAttack, std::string> baseAttackToSound;

    void nextTrack();
};


#endif //BURNBOOKALFA_MUSICMANAGER_H