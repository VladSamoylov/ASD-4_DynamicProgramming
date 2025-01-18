#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iomanip>

using namespace std;
using namespace sf;

class Game {
private:
    vector<vector<int>> map;
    int xP;
    int yP;
    int xG;
    int yG;
public:
    Game();
    void ShowMap();
};

Game::Game() {
    
    int w = 10 + (rand() % 21);
    int h = w;
    cout << "width map : " << w << " height map : " << h << endl;
    this->map.resize(w, vector<int>(h, 0));
    for (int i = 0; i < this->map.size(); i++) {
        for (int j = 0; j < this->map[i].size(); j++) {
            this->map[i][j] = rand() % 10 + 1;
        }
    }
}

void Game::ShowMap() {

    cout << "The map-matrix: " << endl;
    for (int i = 0; i < this->map.size(); i++) {
        for (int j = 0; j < this->map[i].size(); j++) {
            cout << setw(2) << this->map[i][j] << " ";
        }cout << endl;
    }
}

int main() {
    
    srand(static_cast<unsigned>(time(0)));
    
    RenderWindow w(VideoMode({ 1000, 800 }), "The best game");
    Game g;
    g.ShowMap();

    while (w.isOpen())
    {
        while (const std::optional event = w.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                w.close();
        }

        w.clear();
       


        w.display();
    }

   
}
