#include "Judge.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Judge::Judge(Game& game, const string& name)
        : Player(game, name) {
        this->role = RoleType::Judge;
    }

    void Judge::undo(Player& target) {
        if (isTemporarilyDead()){return;}
        if(cooldowns["undone"] == "true") throw runtime_error("Already undid a player's bribe");
        if (!this->isAlive() || !target.isAlive() || target.isTemporarilyDead()) {
            throw runtime_error("Cannot undo: one of the players is dead");
        }

        const auto& history = target.getLastActions();
        size_t len = history.size();

        if (len < 3) {
            throw runtime_error("Not enough actions to undo a bribe");
        }

        // Case: two consecutive bribes at the end (positions 3 and 4, i.e., index 3 and 4)
        if (len >= 5 &&
            history[len - 2] == "bribe" &&
            history[len - 1] == "bribe") {
            // Cancel last 3 actions (done after the bribes)
            for (int i = 0; i < 3; ++i) {
                const string& act = target.getLastActions().back();
                target.cancelAction(act);
            }
            cooldowns["undone"] = "true";
            // No need to pop the bribes themselves, they already cost coins and time
        }

        // Case: one bribe at position 2 (index 2), and two actions followed
        else if (history[len - 3] == "bribe") {
            // Cancel last 2 actions
            for (int i = 0; i < 2; ++i) {
                const string& act = target.getLastActions().back();
                target.cancelAction(act);
            }
            cooldowns["undone"] = "true";
        }

        else {
            throw runtime_error("No recent bribe found to undo");
        }
    }

}
