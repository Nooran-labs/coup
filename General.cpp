#include "General.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    General::General(Game& game, const string& name)
        : Player(game, name) {
        this->role = RoleType::General;
    }

    void General::undo( Player& target) {
        if (!this->isAlive() || !target.isAlive() || !target.isTemporarilyDead()) {
            throw runtime_error("Invalid block target");
        }

        if(cooldowns["block"] == "true") throw runtime_error("Already blocked a player from getting couped");

        if (this->coins() < 5) {
            throw runtime_error("General does not have enough coins to block the coup");
        }

        this->removeCoins(5);

        // Reverse the coup effect:
        target.clearCooldown("couped");
        // Remove coup plans against the target from all players
        game.clear_all_coup_targets_for(target.getName());
        // The attacker already paid 7 coins — they lose those
        // Optionally, we could log this event for audit

        // Note: This doesn't affect turn order or delay
        cooldowns["block"] = "true";
    }
}
