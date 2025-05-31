#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace coup;

void print_players(Game& game) {
    cout << "\nActive Players:" << endl;
    for (Player* p : game.getPlayers()) {
        cout << "- " << p->getName() << " (" << p->roleName() << ", Coins: " << p->coins() << ", Alive:" <<  p->isAlive() << ")"<<endl;
    }
}

int main() {
    Game game;

    Governor governor(game, "Alice");
    Spy spy(game, "Bob");
    Baron baron(game, "Charlie");
    General general(game, "Dana");
    Judge judge(game, "Eve");
    Merchant merchant(game, "Frank");

    print_players(game);

    try {
        // Turn 1: Alice (Governor)
        governor.tax();

        // Turn 2: Bob (Spy)
        spy.gather();

        // Turn 3: Charlie (Baron)
        baron.tax();

        // Turn 4: Dana (General)
        general.gather();

        // Turn 5: Eve (Judge)
        judge.gather();

        // Turn 6: Frank (Merchant)
        merchant.gather();

        print_players(game);

        // Governor undoes Spy's gather (should fail)
        try {
            governor.undo(spy);
        } catch (const exception& e) {
            cerr << "Expected error: " << e.what() << endl;
        }

        // Governor skips due to sanction (simulate cooldown)
        governor.setCooldown("sanctioned", "true");
        governor.tax();  // Should skip turn

        // Spy uses view_coins
        spy.view_coins(baron);
        spy.tax();

        // Baron invests if has 3 coins
        if (baron.coins() >= 3){
            baron.invest();
            }

        // Merchant resists arrest
        try {
            merchant.arrest(baron);  // Not his turn
        } catch (const exception& e) {
            cerr << "Expected turn error: " << e.what() << endl;
        }


        // Build up coins 
        for (int i = 0; i < 2; ++i) {
            baron.tax();
            general.tax();
            judge.gather();
            merchant.tax();
            governor.tax();
            spy.gather();
            
        }
        print_players(game);
        
        baron.invest();
        baron.tax();
        general.tax();
        judge.gather();
        merchant.tax();
        governor.coup(baron);
        
        spy.gather();
        
        print_players(game);
        
        baron.tax();// Should skip him
        general.undo(baron); // Should get the baron back to the game
        judge.gather();

        print_players(game);

        // Force Merchant to have 10 coins and see if coup is forced
        merchant.addCoins(10);
        try {
            merchant.gather();  // This should trigger a forced coup (if implemented)
        } catch (const exception& e) {
            cerr << "Expected must coup error: " << e.what() << endl;
        }
        merchant.coup(spy);
          

        print_players(game);
        
        // Initialise all players to 0 coins
        for (Player* p : game.getPlayers()) {
        p-> removeCoins(p->coins());
        }
        
        governor.addCoins(4);
        merchant.addCoins(2);
        governor.bribe();
        governor.arrest(merchant);
        governor.tax();
        baron.tax();
        general.tax();
        game.next_turn();// because the merchant couped the spy before one full turn
        game.next_turn();// And the general didn't revive him
        		 // Now Bob the spy should be out of the game(dead)
        
        judge.undo(governor);// Governor should have 0 coins and merchant should have 0
        

    } catch (const exception& e) {
        cerr << "Unexpected error: " << e.what() << endl;
    }
#include <iostream>
#include <vector>
#include <stdexcept>


    cout << "\nFinal state:" << endl;
    print_players(game);

    return 0;
}
#include <iostream>
#include <vector>
#include <stdexcept>


