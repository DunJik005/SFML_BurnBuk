//
// Created by lazab on 12/9/2025.
//

#ifndef SFMLPROJECT_ASSETSMANAGER_H
#define SFMLPROJECT_ASSETSMANAGER_H
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AssetsManager {
private:
    std::map<std::string, sf::Texture> textures;

public:
    bool loadTexture(const std::string& name, const std::string& filename) {
        sf::Texture texture;
        if (!texture.loadFromFile(filename))
            return false;

        texture.setSmooth(true); // lepše skaliranje
        textures[name] = texture;
        return true;
    }

    sf::Texture& getTexture(const std::string& name) {
        return textures.at(name);
    }
};


#endif //SFMLPROJECT_ASSETSMANAGER_H