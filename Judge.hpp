#pragma once
#include "Player.hpp"

namespace coup {

    class Judge : public Player {
    public:
        Judge(Game& game, const std::string& name);

        void undo(Player& target) override;
    };
}
