#include "Baron.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Baron::Baron(Game& game, const string& name)
        : Player(game, name) {
        this->role = RoleType::Baron;
    }

    void Baron::invest() {
        if (isTemporarilyDead()){return;}
        if (!this->isAlive()) throw runtime_error("Dead player cannot act");
        if (game.turn() != this->name) throw runtime_error("Not your turn");

        if (this->coins() < 3) throw runtime_error("Not enough coins to invest");
        if(cooldowns["invested"] == "true") throw runtime_error("Already invested");

        cooldowns["invested"] = "true" ;


        this->removeCoins(3);
        this->addCoins(6);
    }

}
