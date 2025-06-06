#pragma once
#include "Player.hpp"

namespace coup {

    class General : public Player {
    public:
        General(Game& game, const std::string& name);

        void undo(Player& target) override;
    };
}
