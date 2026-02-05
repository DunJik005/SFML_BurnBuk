//
// Created by lazab on 12/22/2025.
//

#include "CardDataBase.h"

void CardDataBase::initCards() {
    sqlite3* db;
    sqlite3_stmt* stmt;
    std::string dbPath = "burnbook.db"; // Putanja do baze
    std::string imageFolder = "assets/cards/"; // Bazni path za slike

    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Baza nije pronadjena: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string query = "SELECT * FROM cards";

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Greška u query-ju: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Mapa za čuvanje tekstura (da ne "puknu" kad izadju iz loop-a)
    // Ako koristiš AssetsManager, koristi njega umesto ove mape
    static std::map<std::string, sf::Texture> textureCache;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // 1. Izvlačenje osnovnih podataka (pazi na indekse kolona!)
        std::string name      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int hp                = sqlite3_column_int(stmt, 2);
        int dmg               = sqlite3_column_int(stmt, 4);
        int cost              = sqlite3_column_int(stmt, 5);
        std::string rarityStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        std::string modStr    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        std::string baseAtkStr= reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        std::string imageName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        int cooldown          = sqlite3_column_int(stmt, 10);
        std::string desc      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 21));

        // 2. Učitavanje slike
        std::string fullImagePath = imageFolder + imageName;

        // Proveravamo da li smo već učitali ovu teksturu da ne trošimo RAM
        if (textureCache.find(imageName) == textureCache.end()) {
            sf::Texture tex;
            if (!tex.loadFromFile(fullImagePath)) {
                std::cerr << "Neuspesno ucitavanje slike: " << fullImagePath << std::endl;
                // Ovdje možeš staviti neku "placeholder.png" teksturu
            }
            textureCache[imageName] = tex;
        }

        // 3. Konverzija stringova u Enume
        Rarity rarity = stringToRarity(rarityStr);
        BaseAttack baseAtk = stringToBaseAttack(baseAtkStr);
        AttackModifier mods = stringToModifiers(modStr);

        // 4. Kreiranje karte i ubacivanje u listu
        // Koristimo teksturu iz keša
        auto newCard = std::make_shared<Card>(
            name,
            textureCache[imageName],
            hp,
            dmg,
            cost,
            rarity,
            baseAtk,
            mods,
            cooldown,
            desc
        );

        // Ovde možeš dodati i ostale parametre (stunned, hitCount, json...)
        // koristeći nove setere u Card klasi ako ih imaš

        allCards.push_back(newCard);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::cout << "Uspesno ucitano " << allCards.size() << " karata iz baze!" << std::endl;
}