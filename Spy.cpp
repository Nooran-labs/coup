#include "Spy.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

namespace coup {

    Spy::Spy(Game& game, const string& name)
        : Player(game, name) {
        this->role = RoleType::Spy;
    }

    void Spy::view_coins(const Player& target) {
        if(isTemporarilyDead()) {return;}
        if(getCooldown("view") == "true") throw runtime_error("Already viewed");
        if (!this->isAlive() || !target.isAlive() || target.isTemporarilyDead()) {
            throw runtime_error("Dead players cannot use Spy powers");
        }
        cout << target.getName() << " has " << target.coins() << " coins." << endl;
        setCooldown("view", "true") ;
    }

    void Spy::undo(Player& target) {
        if(isTemporarilyDead()) return;
        if(getCooldown("block") == "true") throw runtime_error("Already blocked a player from getting arrested");
        if (!this->isAlive() || !target.isAlive() || target.isTemporarilyDead()) {
            throw runtime_error("Invalid block target");
        }
        target.setCooldown("arrest_blocked", "true");
        setCooldown("block", "true") ;
    }
}
