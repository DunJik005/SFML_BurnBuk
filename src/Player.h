#ifndef SFMLPROJECT_PLAYER_H
#define SFMLPROJECT_PLAYER_H

class Player {

private:

    int elixir = 0;
    int hp = 50;
    int vutra = 0;
    int hp_shild = 0;

public:
    Player() = default;

    void addElixir(int amount = 1) {
        elixir += amount;
    }

    bool spendElixir(int amount) {
        if (elixir < amount) return false;
        elixir -= amount;
        return true;
    }

    int getElixir() const {
        return elixir;
    }

    void setElixir(int amount) {
        elixir = amount;
    }

    int getHP()const {
        return hp;
    }

    void setHP(int amount){
        hp = amount;
    }

    void takeDamage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    void heal(int amount) {
        hp += amount;
    }

    int getVutra() const {
        return vutra;
    }

    void setVutra(int amount) {
        vutra = amount;
    }
    void removeVutra(int amount) {
        vutra -= amount;
        if (vutra < 0) vutra = 0;
    }
    int getHPShield() const { return hp_shild; }
    void setHPShield(int amount) { hp_shild = amount; }
    void addHPShield(int amount) { hp_shild += amount; }
    void removeHPShield(int amount) {
        hp_shild -= amount;
        if(hp_shild < 0) hp_shild = 0;
    }

};

#endif