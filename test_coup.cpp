#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Player.hpp"

// Helper to create initialized game
coup::Game setup_game_with(std::vector<std::string> names) {
    coup::Game game;
    for (const std::string& name : names) {
        game.create_player(name);
    }
    for (const std::string& name : names) {
        coup::Player* p = game.getPlayerByName(name);
        p->addCoins(7);
    }
    return game;
}

TEST_CASE("Player creation and coin setup") {
    coup::Game game = setup_game_with({"A", "B", "C"});
    CHECK(game.players().size() == 3);
    for (const std::string& name : game.players()) {
        CHECK(game.getPlayerByName(name)->coins() == 7);
    }
}

TEST_CASE("Game progresses and action is applied") {
    coup::Game game = setup_game_with({"X", "Y"});
    coup::Player* x = game.getPlayerByName("X");
    coup::Player* y = game.getPlayerByName("Y");

    CHECK(x->coins() == 7);
    CHECK_NOTHROW(game.play_action("Coup", "Y"));
    CHECK_FALSE(y->isAlive());
}

TEST_CASE("Game ends with one player remaining") {
    coup::Game game = setup_game_with({"P1", "P2"});
    coup::Player* p1 = game.getPlayerByName("P1");
    coup::Player* p2 = game.getPlayerByName("P2");

    // Kill P2
    game.play_action("Coup", "P2");

    CHECK(game.is_game_over());
    CHECK(game.winner() == "P1");
}

