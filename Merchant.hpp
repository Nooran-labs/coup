#pragma once
#include "Player.hpp"

namespace coup {
    class Merchant : public Player {
    public:
        Merchant(Game& game, const std::string& name);
    };
}
