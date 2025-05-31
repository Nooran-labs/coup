#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

namespace coup {

    Game::Game() : current_index(0), game_started(false) {}

    // Role drawing
    RoleType Game::draw_random_role() {
        static mt19937 rng(static_cast<unsigned int>(time(nullptr)));
        uniform_int_distribution<> dist(0, 5);
        return static_cast<RoleType>(dist(rng));
    }

    // player creation
    Player* Game::create_player(const string& name) {
        if (game_started) throw runtime_error("Cannot add players after game started");
        if (players_list.size() >= 6) throw runtime_error("Max players reached (6)");
        RoleType role = draw_random_role();
        Player* p = nullptr;

        switch (role) {
            case RoleType::Governor:
                p = new Governor(*this, name);
                break;
            case RoleType::Spy:
                p = new Spy(*this, name);
                break;
            case RoleType::Baron:
                p = new Baron(*this, name);
                break;
            case RoleType::General:
                p = new General(*this, name);
                break;
            case RoleType::Judge:
                p = new Judge(*this, name);
                break;
            case RoleType::Merchant:
                p = new Merchant(*this, name);
                break;
        }

        if (!p) throw runtime_error("Failed to create player");

        return p;
    }

    void Game::add_player(Player* player) {
        if (game_started) throw runtime_error("Cannot add players after game started");
        if (players_list.size() >= 6) throw runtime_error("Max players reached (6)");
        players_list.push_back(player);
    }

    string Game::turn() const {
        if (players_list.empty()) throw runtime_error("No players in game");
        return players_list[current_index]->getName();
    }

    vector<string> Game::players() const {
        vector<string> active_names;
        for (const auto& p : players_list) {
            if (p->isAlive()) {
                active_names.push_back(p->getName());
            }
        }
        return active_names;
    }

    string Game::winner() const {
        int alive_count = 0;
        string last_name;
        for (const auto& p : players_list) {
            if (p->isAlive()) {
                alive_count++;
                last_name = p->getName();
            }
        }
        if (alive_count > 1) throw runtime_error("The game is still ongoing");
        return last_name;
    }

    void Game::clear_all_coup_targets_for(const std::string& name) {
    for (Player* p : players_list) {
        if (p->getCooldown("coup") == name) {
            p->clearCooldown("coup");
        }
    }
    }


    bool Game::has_general() const {
    for (const Player* p : players_list) {
        if (p->isAlive() && p->getRole() == RoleType::General) {
            return true;
        }
    }
    return false;
    }
    
    void Game::play_action(const std::string& action) {
    // Call the original function with empty target name or nullptr as needed
    play_action(action, "");
    }
    
    void Game::play_action(const std::string& action, const std::string& targetName) {
    Player* current = current_player();
    Player* target = nullptr;

    if (!targetName.empty()) {
        for (Player* p : getPlayers()) {
            if (p->getName() == targetName) {
                target = p;
                break;
            }
        }
        if (!target) throw std::runtime_error("Target player not found: " + targetName);
    }

    if (action == "Tax") {
        current->tax();
    } else if (action == "Coup") {
        if (!target) throw std::runtime_error("Coup requires a target");
        current->coup(*target);
    } else if (action == "Bribe") {
        current->bribe();
    } else if (action == "Arrest") {
        if (!target) throw std::runtime_error("Arrest requires a target");
        current->arrest(*target);
    } else if (action == "Sanction") {
        if (!target) throw std::runtime_error("Sanction requires a target");
        current->sanction(*target);
    } else if (action == "Gather") {
        current->gather();
    }else if (action == "Undo") {
        if (!target) throw std::runtime_error("Undo requires a target");
        current->undo(*target);
    } else if (action == "Invest") {
        current->invest();
    } else if (action == "View") {
        if (!target) throw std::runtime_error("Undo requires a target");
        current->view_coins(*target);
    }
    else {
        throw std::runtime_error("Unknown action: " + action);
    }
}



   void Game::next_turn() {
    game_started = true;

    Player* current = players_list[current_index];

    if (current->getTurnDelay() > 1) {
        current->setTurnDelay(current->getTurnDelay() - 1);
        return;
    }
    cout << turn() << endl;
    current->clear_turn_based_cooldowns();

    // Advance to next alive player
    do {
        current_index = (current_index + 1) % players_list.size();
    } while (!players_list[current_index]->isAlive() ||(players_list[current_index]->isTemporarilyDead() && players_list[current_index]-> roleName() != "General"));

    Player* next_player = players_list[current_index];
    next_player->setTurnDelay(1);
    // If Merchant and has at least 3 coins, grant passive bonus
    if (next_player->getRole() == RoleType::Merchant && next_player->coins() >= 3) {
        next_player->addCoins(1);
    }

    // Coup condition from cooldown["coup"]
    // Coup cooldown isn't empty only if there is a general on the table
    // Thus the coup action waits a full turn to be done
    std::string target_name = next_player->getCooldown("coup");
    if (!target_name.empty()) {
        for (Player* p : players_list) {
            if (p->getName() == target_name && p->isAlive()) {
                remove_player(p); // eliminate the player
                break;
            }
        }
        next_player->clearCooldown("coup"); // clear the coup directive
    }
}


    void Game::remove_player(Player* player) {
	player->setCooldown("couped", "true");
        player->setAlive(false);
    }
    
    std::vector<Player*>& Game::getPlayers() {
    return players_list;
    }


    bool Game::is_game_over() const {
        int alive_count = 0;
        for (const auto& p : players_list) {
            if (p->isAlive()) ++alive_count;
        }
        return alive_count <= 1;
    }

    Player* Game::current_player() const {
        return players_list[current_index];
    }

    Player* Game::getPlayerByName(const std::string& name) const {
    for (Player* p : players_list) {
        if (p->getName() == name) return p;
    }
    return nullptr;
    }


}
