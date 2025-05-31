#pragma once
#include <deque> // for action history
#include <string>
#include <unordered_map>
#include <vector>
#include "Game.hpp"

namespace coup {

    class Game;

    

    class Player {
    protected:
        Game& game;
        std::string name;
        int coin_count;
        bool alive;
        RoleType role;
        int turn_delay; // 1 by default, higher if bribe used
        std::deque<std::string> last_actions;
        void log_action(const std::string& action);

        std::unordered_map<std::string, std::string> cooldowns; // e.g., "arrest" → "last_target"

    public:
        Player(Game& game, const std::string& name);
        virtual ~Player() = default;

        virtual void gather();
        virtual void tax();
        virtual void bribe();
        virtual void arrest(Player& target);
        virtual void sanction(Player& target);
        virtual void coup(Player& target);
        virtual void undo(Player& source); // for blocking
        virtual void invest();                    // For roles like Baron
        virtual void view_coins(const Player& target);        // For roles like Spy
        virtual std::vector<std::string> getSpecialAbilities() const;



        // Helper function
        void checkMandatoryCoup() const;
        bool isTemporarilyDead() const;
        void cancelAction(const std::string& actionName);
        // Getters
        const std::string& getName() const;
        int coins() const;
        bool isAlive() const;
        std::string roleName() const;
        void setAlive(bool status);
        RoleType getRole() const;
        // Turn Delay
        int getTurnDelay() const;
        void setTurnDelay(int delay);
        // Coin manipulation
        void addCoins(int amount);
        void removeCoins(int amount);
        // Action History
        const std::deque<std::string>& getLastActions() const;
        void clearLastActions();
        void popLastAction();
        // Cooldowns
        std::string getCooldown(const std::string& action) const;
        void setCooldown(const std::string& action, const std::string& target);
        void clearCooldown(const std::string& action);
        void clear_turn_based_cooldowns();


    };
}
