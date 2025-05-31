#pragma once
#include <string>
#include <vector>
#include <stdexcept>

namespace coup {

    enum class RoleType {
        Governor,
        Spy,
        Baron,
        General,
        Judge,
        Merchant
    };
    
    class Player;

    class Game {
    private:
        std::vector<Player*> players_list;
        size_t current_index;
        bool game_started;

    public:
        Game();
        

        void add_player(Player* player);
	Player* create_player(const std::string& name);

        std::string turn() const;
        std::vector<std::string> players() const;
        std::string winner() const;
        bool has_general() const;
        void clear_all_coup_targets_for(const std::string& name);
        RoleType draw_random_role();
        void play_action(const std::string& action); // No target
        void play_action(const std::string &action, const std::string &target); // for targeted 





        void next_turn();
        void remove_player(Player* player);
        bool is_game_over() const;
        Player* current_player() const;
        Player* getPlayerByName(const std::string& name) const;
        std::vector<Player*>& getPlayers();


    };
}
