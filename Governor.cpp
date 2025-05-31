#include "Governor.hpp"
#include <stdexcept>
using namespace std;

namespace coup {

    Governor::Governor(Game& game, const string& name)
        : Player(game, name) {
        this->role = RoleType::Governor;
    }

    void Governor::tax() {
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
        coin_count += 3;
        log_action("tax");
        game.next_turn();
        }

    }

    void Governor::undo(Player& target) {
        if(isTemporarilyDead()){return;}
        if (!alive || !target.isAlive() || target.isTemporarilyDead()) throw runtime_error("Invalid undo");
        if(getCooldown("undone") == "true") throw runtime_error("Already undone the tax");

        if (target.getLastActions().back() == "tax") {
            throw runtime_error("Governor can only undo tax actions");
        }

        int refund = 2;
        if (target.getRole() == RoleType::Governor) {
            refund = 3;
        }

        if (target.coins() < refund) {
            throw runtime_error("Target does not have enough coins to refund");
        }

        target.removeCoins(refund) ;
        setCooldown("undone","true") ;
    }
}
