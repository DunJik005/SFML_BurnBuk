// #ifndef SFMLPROJECT_CARDDATA_H
// #define SFMLPROJECT_CARDDATA_H
//
// #include <string>
// #include <vector>
// #include <memory>
//
// enum class Rarity {
//     Common,
//     Rare,
//     Epic,
//     Legendary,
//     Hero
// };
//
// // forward deklaracija
// class Ability;
//
// class CardData {
// public:
//     std::string name;
//     Rarity rarity;
//     int cost;
//     int damage;
//     int health;
//     std::string artTexturePath; // putanja do slike karte
//
//     std::vector<std::shared_ptr<Ability>> abilities;
//
//     CardData(const std::string& name,
//              Rarity rarity,
//              int cost,
//              int damage,
//              int health,
//              const std::string& artPath)
//         : name(name), rarity(rarity), cost(cost),
//           damage(damage), health(health),
//           artTexturePath(artPath) {}
//
//     Rarity getRarity() const { return rarity; }
//     std::string getName() const { return name; }
//     int getHealth() const { return health; }
//     int getDamage() const { return damage; }
//     int getCost() const { return cost; }
// };
//
// #endif //SFMLPROJECT_CARDDATA_H