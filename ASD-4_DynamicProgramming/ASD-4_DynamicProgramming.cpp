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
    void DrawMap(RenderWindow&, Texture&);
};

Game::Game() {
    
    int w = 10 + (rand() % 11);
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

void Game::DrawMap(RenderWindow& w, Texture& tileset) {

    Sprite sprite(tileset);
    int tileSize = 32;



    float offsetX = (w.getSize().x - this->map.size() * tileSize) / 2;
    float offsetY = (w.getSize().y - this->map[0].size() * tileSize) / 2;

    for (int i = 0; i < this->map.size(); i++) {
        for (int j = 0; j < this->map[i].size(); j++) {
            int tileName = this->map[i][j];
            
           /* if (tileName == 1 || tileName == 10) sprite.setTextureRect(IntRect({ 0, 1 * tileSize }, { tileSize, tileSize }));
            else if (tileName == 2 || tileName == 3) sprite.setTextureRect(IntRect({ 0, 1 * tileSize }, { tileSize, tileSize }));
            else if (tileName == 4 || tileName == 5) sprite.setTextureRect(IntRect({ 0, 2 * tileSize }, { tileSize, tileSize }));
            else if (tileName == 6 || tileName == 7) sprite.setTextureRect(IntRect({ 0, 3 * tileSize }, { tileSize, tileSize }));
            else if (tileName == 8 || tileName == 9) sprite.setTextureRect(IntRect({ 0, 4 * tileSize }, { tileSize, tileSize }));*/
            
            sprite.setTextureRect(IntRect({ 1 * tileSize, 0 }, { tileSize, tileSize }));
            //sprite.setTextureRect(IntRect({ 1 * tileSize, 50 * tileSize }, { tileSize, tileSize }));
            sprite.setPosition(Vector2f(i * tileSize + offsetX, j * tileSize + offsetY));
            
            w.draw(sprite);
        }
    }
}

int main() {
    
    srand(static_cast<unsigned>(time(0)));
    
    RenderWindow w(VideoMode({ 1000, 800 }), "The best game");
    Texture tileset;

    if (!tileset.loadFromFile("./media/BaseChip.png")) {
        cout << "Error loading tileset!" << endl;
        return -1;
    }
    Color bg(87, 71, 30);

    Game g;
    g.ShowMap();

    while (w.isOpen())
    {
        while (const std::optional event = w.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) w.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->scancode == Keyboard::Scancode::F11) {
                    if (w.getSize().x != VideoMode::getDesktopMode().size.x) {
                        w.create(sf::VideoMode::getDesktopMode(), "The best game");
                    }
                    else {
                        w.create(sf::VideoMode({ 1000, 800 }), "The best game", sf::Style::Close | sf::Style::Resize);
                    }
                }
            }
            if (event->getIf<Event::Resized>()) {
                unsigned int width = w.getSize().x;
                unsigned int height = w.getSize().y;
                w.create(sf::VideoMode({ width, height }), "The best game", sf::Style::Close | sf::Style::Resize);
            }
        }
       
        w.clear(bg);
        g.DrawMap(w, tileset);
        w.display();
    }

   
}
