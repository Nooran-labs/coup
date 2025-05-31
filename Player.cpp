#include "Player.hpp"
#include <stdexcept>
#include <random>
#include <ctime>

using namespace std;

namespace coup {



    Player::Player(Game& game, const string& name)
    : game(game), name(name), coin_count(0), alive(true), turn_delay(1) {
    game.add_player(this);  // role should already be set by subclass
    }



    string Player::roleName() const {
        switch (role) {
            case RoleType::Governor: return "Governor";
            case RoleType::Spy: return "Spy";
            case RoleType::Baron: return "Baron";
            case RoleType::General: return "General";
            case RoleType::Judge: return "Judge";
            case RoleType::Merchant: return "Merchant";
            default: return "Unknown";
        }
    }

    void Player::log_action(const std::string& action) {
    if (last_actions.size() >= 5) {
        last_actions.pop_front();
    }
    last_actions.push_back(action);
    }

    const string& Player::getName() const {
        return name;
    }

    int Player::coins() const {
        return coin_count;
    }

    bool Player::isAlive() const {
        return alive;
    }

    void Player::setAlive(bool status) {
        alive = status;
    }

    RoleType Player::getRole() const {
        return role;
    }
    // Turn Delay
    int Player::getTurnDelay() const {
        return turn_delay;
    }

    void Player::setTurnDelay(int delay) {
        turn_delay = delay;
    }

    // Coin Control
    void Player::addCoins(int amount) {
        coin_count += amount;
    }

    void Player::removeCoins(int amount) {
        if (coin_count < amount) throw std::runtime_error("Not enough coins");
        coin_count -= amount;
    }

    // Action History
    const std::deque<std::string>& Player::getLastActions() const {
        return last_actions;
    }

    void Player::clearLastActions() {
        last_actions.clear();
    }

    void Player::popLastAction() {
        if (!last_actions.empty()) {
            last_actions.pop_back();
        }
    }

    // Cooldowns
    std::string Player::getCooldown(const std::string& action) const {
        auto it = cooldowns.find(action);
        if (it != cooldowns.end()) {
            return it->second;
        }
        return "";
    }

    void Player::setCooldown(const std::string& action, const std::string& target) {
        cooldowns[action] = target;
    }

    void Player::clearCooldown(const std::string& action) {
        cooldowns.erase(action);
    }

    void Player::clear_turn_based_cooldowns() {
    if (cooldowns["arrest_blocked"] == "true") {
        clearCooldown("arrest_blocked");
    }
    if (cooldowns["sanctioned"] == "true") {
        clearCooldown("sanctioned");
    }
    if (cooldowns.find("arrest") != cooldowns.end()) {
    clearCooldown("arrest");
    }
    if (cooldowns["undone"] == "true") {
        clearCooldown("undone");
    }
    if (cooldowns["block"] == "true") {
        clearCooldown("block");
    }
    if (cooldowns["view"] == "true") {
        clearCooldown("view");
    }
    if (cooldowns["invested"] == "true") {
        clearCooldown("invested");
    }

    }

    // Helper functions(HF)
    void Player::checkMandatoryCoup() const {
    if (game.turn() != name) return;
    if (coin_count >= 10) {
        throw std::runtime_error("You must perform a coup — 10 or more coins held.");
    }
    }

    bool Player::isTemporarilyDead() const {
    auto it = cooldowns.find("couped");
    return it != cooldowns.end() && it->second == "pending";
    }


    // Cancel/Revers a given action(HF)
    void Player::cancelAction(const std::string& actionName) {
    if (!alive) throw std::runtime_error("Dead player cannot cancel actions");

    if (actionName == "gather") {
        coin_count -= 1;
    }
    else if (actionName == "tax") {
        if (role == RoleType::Governor) {
            coin_count -= 3;
        } else {
            coin_count -= 2;
        }
    }
    else if (actionName == "bribe") {
        // Already paid the 4 coins; nothing to do unless you want to refund it
        // Usually we just skip bribe bonus turns, not reverse coin loss
    }
    else if (actionName == "arrest") {
        std::string target_name = cooldowns["arrest"];
        if (!target_name.empty()) {
            Player* target = game.getPlayerByName(target_name);
            if (target && target->isAlive()) {
                // Reverse the coin transfer
                this->coin_count -= 1;
                target->addCoins(1);

                // Remove cooldown tracking
                clearCooldown("arrest");
            }
        }
    }
    else if (actionName == "sanction") {
        std::string target_name = cooldowns["sanctioned"];
        if (!target_name.empty()) {
            Player* target = game.getPlayerByName(target_name);
            if (target) {
                target->clearCooldown("sanctioned");
                clearCooldown("sanctioned");
            }
        }
    }
    else if (actionName == "coup") {
        std::string target_name = cooldowns["coup"];
        if (!target_name.empty()) {
            Player* target = game.getPlayerByName(target_name);
            if (target) {
                target->clearCooldown("couped");
                target->setAlive(true);  // revive
                clearCooldown("coup");
                game.clear_all_coup_targets_for(target_name);  // clear globally
            }
        }

    }
    else if (actionName == "skipped") {return;}
    else {
        throw std::runtime_error("Unknown action: " + actionName);
    }

    // Remove the action from log (latest matching)
    for (auto it = last_actions.rbegin(); it != last_actions.rend(); ++it) {
        if (*it == actionName) {
            last_actions.erase(std::next(it).base());
            break;
        }
    }
    }




    // --- Base Actions (all roles can do them) ---
    void Player::gather() {
        // If the player was couped and there is a chance
        // That he may get revived by a General this round
         if(isTemporarilyDead()){
            game.next_turn();
            return;
        }
        if (!alive) throw runtime_error("Dead player cannot act");
        if (game.turn() != name) throw runtime_error("Not your turn");

        checkMandatoryCoup();

        if (cooldowns["sanctioned"] == "true") {
            log_action("skipped");
            game.next_turn();
        }
        else{
        coin_count += 1;
        log_action("gather");

        game.next_turn();
        }

    }

    void Player::tax() {
         if(isTemporarilyDead()){
            game.next_turn();
            return;
        }
        if (!alive) throw runtime_error("Dead player cannot act");
        if (game.turn() != name) throw runtime_error("Not your turn");

        checkMandatoryCoup();

        if (cooldowns["sanctioned"] == "true") {
            log_action("skipped");

            game.next_turn();
        }
        else{
        coin_count += 2;
        log_action("tax");

        game.next_turn();
        }

    }

    void Player::bribe() {
        if(isTemporarilyDead()){
            game.next_turn();
            return;
        }
        if (coin_count < 4) throw runtime_error("Not enough coins to bribe");
        if (game.turn() != name) throw runtime_error("Not your turn");

        checkMandatoryCoup();

        coin_count -= 4;
        turn_delay += 2;
        log_action("bribe");

        game.next_turn();
    }

    void Player::arrest(Player& target) {
        if(isTemporarilyDead()){
            game.next_turn();
            return;
        }
        if (!alive || !target.isAlive() || target.isTemporarilyDead()) throw runtime_error("Invalid arrest");
        if (game.turn() != name) throw runtime_error("Not your turn");

        checkMandatoryCoup();

        if (cooldowns["arrest_blocked"] == "true") {
        throw runtime_error("Arrest is blocked by Spy");
        }
        if (cooldowns["arrest"] == target.getName()) {
            throw runtime_error("Cannot arrest same player twice in a row");
        }
        if (target.getRole() == RoleType::Merchant) {
        if (target.coins() < 2) throw runtime_error("Merchant has insufficient coins to be arrested");
        target.removeCoins(2); // loss to treasury
        } else {
        if (target.coins() > 0) {
        target.removeCoins(1);
        this->addCoins(1);
        }

        // Refund if target is a General
        if (target.getRole() == RoleType::General) {
            this->removeCoins(1);
            target.addCoins(1);  // refunded
        }
        }


        log_action("arrest");
        game.next_turn();
        cooldowns["arrest"] = target.getName();

    }

    void Player::sanction(Player& target) {
        if(isTemporarilyDead()){
            game.next_turn();
            return;
        }
        if (coin_count < 3) throw runtime_error("Not enough coins to sanction");
        if (!target.isAlive() || target.isTemporarilyDead()) throw runtime_error("Target is dead");
        if (game.turn() != name) throw runtime_error("Not your turn");

        checkMandatoryCoup();

        coin_count -= 3;
        // For now we just track this action symbolically
        log_action("sanction");

        target.setCooldown("sanctioned", "true");
        if (target.getRole() == RoleType::Baron) {
        target.addCoins(1);
        }
        game.next_turn();
    }

    void Player::coup(Player& target) {
        if(isTemporarilyDead()){
            game.next_turn();
            return;
        }
        if (coin_count < 7) throw runtime_error("Not enough coins to coup");
        if (!target.isAlive() || target.isTemporarilyDead()) throw runtime_error("Target is already dead");
        if (game.turn() != name) throw runtime_error("Not your turn");

        coin_count -= 7;
        if(game.has_general()){
            log_action("coup");
            target.setCooldown("couped", "pending");// A state between True and False
            cooldowns["coup"]=target.getName();
            game.next_turn();
        }
        else{

        log_action("coup");

        target.log_action("couped");
        target.setCooldown("couped", "true");
        game.remove_player(&target);
        game.next_turn();
        }
    }

    void Player::undo(Player& source) {
        throw runtime_error("Undo action not implemented in this Player class");
    }
    void Player::invest() {
    throw runtime_error("Invest action not implemented in this Player class");
    }
    void Player::view_coins(const Player& target) {
    throw runtime_error("View action not implemented in thsi Player class");
    }
    
    
    std::vector<std::string> Player::getSpecialAbilities() const {
    if (roleName() == "Spy") {
        return {"View", "Undo"};
    }
    if (roleName() == "Baron") {
        return {"Invest"};
    }
    if (roleName() == "Governor" || roleName() == "General" || roleName() == "Judge") {
        return {"Undo"};
    }
    return {}; // Merchant and others
}



}
