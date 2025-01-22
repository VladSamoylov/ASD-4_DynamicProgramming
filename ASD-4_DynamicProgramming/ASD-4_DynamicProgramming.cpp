#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include <iomanip>

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_PURPLE "\033[35m"
#define ANSI_COLOR_LIGHTBLUE "\033[94m"

using namespace std;
using namespace sf;

const int INF = numeric_limits<int>::max();

class Game {
private:
    vector<vector<int>> map;
    int xP;
    int yP;
    int xG;
    int yG;
    int currentFramePlayer;
    int currentFramePlayerJump;
    int currentFrameGoal;
    float animationTimer;
    float animationSpeed;
    stack<string> move;
    int minPrice;
public:
    Game();
    void ShowMap();
    void DrawMap(RenderWindow&, const string&);
    void DrawPlayer(RenderWindow&, const string&, const string&, const string&, Clock);
    void DrawGoal(RenderWindow&, const string&, const string&, Clock);
    void MoveRight(Vector2f&, int&);
    void MoveDown(Vector2f&, int&);
    void MoveDownRight(Vector2f&, int&);
    void FindPath();
    void LeaveFootprints(RenderWindow&);
};

Game::Game() {
    
    int w = 10;
    int h = w;
    cout << "width map : " << w << " height map : " << h << endl;
    this->map.resize(w, vector<int>(h, 0));
    for (int i = 0; i < this->map.size(); i++) {
        for (int j = 0; j < this->map[i].size(); j++) {
            this->map[i][j] = rand() % 10 + 1;
        }
    }
    do {
        this->xP = rand() % this->map.size();
    } while (this->xP == this->map.size() - 1);
    do {
        this->yP = rand() % this->map.size();
    } while (this->yP == this->map.size() - 1);
    do {
        this->xG = rand() % this->map.size();
    } while (this->xG <= this->xP);
    do {
        this->yG = rand() % this->map.size();
    } while (this->yG <= this->yP);
    cout << "Player coord : x(" << this->xP << ") y(" << this->yP << ")" << endl;
    cout << "Goal coord : x(" << this->xG << ") y(" << this->yG << ")" << endl;
    //this->map[this->xP][this->yP] = 0;
    //this->map[this->xG][this->yG] = 99;
    this->currentFramePlayer = 0;
    this->currentFramePlayerJump = 0;
    this->currentFrameGoal = 0;
    this->animationTimer = 0.0f;
    this->animationSpeed = 0.1f;
    this->minPrice = INF;
}

void Game::ShowMap() {

    cout << "The map-matrix: " << endl;
    for (int i = 0; i < this->map.size(); i++) {
        for (int j = 0; j < this->map[i].size(); j++) {
            cout << setw(2) << this->map[i][j] << " ";
        }cout << endl;
    }
}

void Game::FindPath() {

    vector<vector<int>> minPath(this->map.size(), vector<int>(this->map[0].size(), INF));

    minPath[this->xP][this->yP] = this->map[this->xP][this->yP];

    cout << "The dp-matrix: " << endl;
    for (int i = 0; i < minPath.size(); i++) {
        for (int j = 0; j < minPath[i].size(); j++) {
            if(minPath[i][j] == INF) cout << setw(4) << "INF";
            else cout << setw(4) << minPath[i][j];
        }cout << endl;
    }
   
    for (int x = this->xP; x <= this->xG; x++) {
        for (int y = this->yP; y <= this->yG; y++) {
            if (x > this->xP) minPath[x][y] = min(minPath[x][y], minPath[x - 1][y] + this->map[x][y]);
            if (y > this->yP) minPath[x][y] = min(minPath[x][y], minPath[x][y - 1] + this->map[x][y]);
            if (x > this->xP && y > this->yP) minPath[x][y] = min(minPath[x][y], minPath[x - 1][y - 1] + this->map[x][y]);
        }
    }

    cout << "The dp-matrix-2: " << endl;
    for (int i = 0; i < minPath.size(); i++) {
        for (int j = 0; j < minPath[i].size(); j++) {
            if (minPath[i][j] == INF) cout << setw(4) << "INF";
            else cout << setw(4) << minPath[i][j];
        }cout << endl;
    }

    this->minPrice = minPath[this->xG][this->yG];

    int x = this->xG, y = this->yG;
    while (x != this->xP || y != this->yP) {
        if (x > this->xP && minPath[x][y] == minPath[x - 1][y] + this->map[x][y]) {
            x--;
            this->move.push("d");
        }
        else if (y > this->yP && minPath[x][y] == minPath[x][y - 1] + this->map[x][y]) {
            y--;
            this->move.push("r");
        }
        else if (x > this->xP && y > this->yP && minPath[x][y] == minPath[x - 1][y - 1] + this->map[x][y]) {
            x--;
            y--;
            this->move.push("dr");
        }
    }
    cout << "Min dist cost : " << this->minPrice << endl;
}

void Game::DrawMap(RenderWindow& w, const string& fileName) {

    Texture tileset;
    if (!tileset.loadFromFile(fileName)) throw "Error loading tileset!";
    
    Sprite sprite(tileset);
    int tileSize = 32;

    vector<vector<int>> mapFull(this->map.size() + 2, vector<int>(this->map[0].size() + 2, -1));
    for (int i = 0; i < this->map.size(); i++) {
        for (int j = 0; j < this->map[i].size(); j++) {
            mapFull[i + 1][j + 1] = this->map[i][j];
        }
    }

    float offsetX = (w.getSize().x - mapFull.size() * tileSize * 2) / 2;
    float offsetY = (w.getSize().y - mapFull[0].size() * tileSize * 2) / 2;
    for (int i = 0; i < mapFull.size(); i++) {
        for (int j = 0; j < mapFull.size(); j++) {
            int tileName = mapFull[i][j];          
            
            if(tileName == -1) sprite.setTextureRect(IntRect({ 1 * tileSize, 50 * tileSize }, { tileSize, tileSize }));
            else sprite.setTextureRect(IntRect({ 1 * tileSize, 0 }, { tileSize, tileSize }));
            
            sprite.setScale(Vector2f(2.f, 2.f));
            sprite.setPosition(Vector2f(j * tileSize * 2 + offsetX, i * tileSize * 2 + offsetY));
            
            w.draw(sprite);

            if (tileName != -1) {
                RectangleShape tileBorder(Vector2f(tileSize * 2, tileSize * 2));
                Color cl(87, 71, 30);
                tileBorder.setPosition(Vector2f(j * (tileSize * 2) + offsetX, i * (tileSize * 2) + offsetY));
                tileBorder.setFillColor(sf::Color::Transparent);
                tileBorder.setOutlineThickness(1);
                tileBorder.setOutlineColor(cl);

                w.draw(tileBorder);
            }

            Font font;
            if (!font.openFromFile("./fonts/chilispepper.ttf")) {
                throw "Не вдалося завантажити шрифт!";
            }
            Text text(font);
            text.setCharacterSize(16);
            text.setFillColor(Color::Yellow);
            if (tileName != -1) {
                text.setString(to_string(tileName));
                text.setPosition(Vector2f(j * (tileSize * 2) + (tileSize * 2) / 2 + offsetX - 2, i * (tileSize * 2) + (tileSize * 2) / 4 + offsetY));
                w.draw(text);
            }
        }
    }
}

void Game::DrawGoal(RenderWindow& w, const string& fileName, const string& fileName2, Clock clock) {

    static Texture goal, goal2;
    if (!goal.loadFromFile(fileName)) throw "Error loading goal!";
    if (!goal2.loadFromFile(fileName2)) throw "Error loading goal!";

    static Sprite sprite(goal);
    int tileSize = 128;
    int mapTileSize = 64;
    static bool IDLE = false;
    static int wait = 0;
    static int doit = 0;
    this->animationTimer += clock.restart().asSeconds();
    if (this->animationTimer >= this->animationSpeed) {
        this->currentFrameGoal++;
        if (this->currentFrameGoal >= 7 && doit <= 14 && !IDLE) { this->currentFrameGoal = 0; doit++; }
        else if (this->currentFrameGoal >= 7 && !IDLE) { IDLE = true; sprite.setTexture(goal2);  this->currentFrameGoal = 0; }
        else if(IDLE && this->currentFrameGoal >= 9) { IDLE = false; sprite.setTexture(goal);  this->currentFrameGoal = 0; }
        
        if (IDLE && this->currentFrameGoal == 5 && wait < 30) { this->currentFrameGoal = 4; wait++; }
        this->animationTimer = 0.0f;

        if (wait >= 30) { wait = 0; this->currentFrameGoal++; }
        if (doit >= 14 && IDLE) doit = 0;
    }

    float offsetX = (w.getSize().x - (this->map.size() ) * mapTileSize) / 2;
    float offsetY = (w.getSize().y - (this->map[0].size() ) * mapTileSize) / 2;
    sprite.setTextureRect(IntRect({ this->currentFrameGoal * tileSize, 1 * (tileSize / 2) }, { tileSize, tileSize / 2}));
    sprite.setScale(Vector2f(-1.f, 1.f));
    sprite.setPosition(Vector2f(this->yG * mapTileSize + offsetX + 112, this->xG * mapTileSize + offsetY));
    w.draw(sprite);
}

void Game::DrawPlayer(RenderWindow& w, const string& fileName, const string& fileName2, const string& fileName3, Clock clock) {

    Texture playerT, playerW, playerJ;
    if (!playerT.loadFromFile(fileName)) throw "Error loading player!";
    if (!playerW.loadFromFile(fileName2)) throw "Error loading player walks!";
    if (!playerJ.loadFromFile(fileName3)) throw "Error loading player jump!";

    static bool isMoving = false;
    static bool isJump = false;
    static bool isMidJump = false;
    static bool isEndJump = false;
    static int currentPos = 0;

    Sprite player(playerT);
    int tileSize = 128;
    int mapTileSize = 64;

    if (Keyboard::isKeyPressed(Keyboard::Key::Space) && !isMoving) {
        isMoving = true;
    }

    if (isJump) { player.setTexture(playerJ); }
    else if(isMoving) { player.setTexture(playerW); }
    else player.setTexture(playerT);

    this->animationTimer += clock.restart().asSeconds();
    if (isMoving && !isJump) {
        if (this->animationTimer >= this->animationSpeed) {
            this->currentFramePlayer++;
            if (this->currentFramePlayer >= 8) this->currentFramePlayer = 0;
            this->animationTimer = 0.0f;
        }
    }
    else if (isMoving || isJump) {
        if (this->animationTimer >= this->animationSpeed) {
            this->currentFramePlayer++;
            if (this->currentFramePlayer >= 8 && !isEndJump) { this->currentFramePlayer = 7; isMidJump = true; }
            if (this->currentFramePlayer >= 15) { isJump = false; isEndJump = false; }
            this->animationTimer = 0.0f;
        }
    }
    else {
        if (this->animationTimer >= this->animationSpeed) {
            this->currentFramePlayer++;
            if (this->currentFramePlayer >= 11) this->currentFramePlayer = 0;
            this->animationTimer = 0.0f;
        }
    }
    
    float offsetX = (w.getSize().x - (this->map.size()) * mapTileSize) / 2;
    float offsetY = (w.getSize().y - (this->map[0].size()) * mapTileSize) / 2;
    player.setTextureRect(IntRect({ this->currentFramePlayer * tileSize, 1 * (tileSize / 2) }, {tileSize, tileSize / 2}));
    
    static Vector2u prevWindowSize = w.getSize();
    static Vector2f current(this->yP * mapTileSize + offsetX - 40, this->xP * mapTileSize + offsetY);
    
    if (prevWindowSize != w.getSize()) {
        current.x = this->yP * mapTileSize + offsetX - 40;
        current.y = this->xP * mapTileSize + offsetY;
        prevWindowSize = w.getSize();
    }

    if (isMoving) {
        if (!this->move.empty()) {
            if (this->move.top() == "r" && !isJump) this->MoveRight(current, currentPos);
            else if (this->move.top() == "d") {
                if (isMidJump) this->MoveDown(current, currentPos);
                else isJump = true;
            } 
            else if (this->move.top() == "dr") {
                if(isMidJump) this->MoveDownRight(current, currentPos);
                else isJump = true;
            }
        }
        
        if (currentPos >= mapTileSize && this->move.top() == "r") { this->yP++; currentPos = 0; this->move.pop(); }
        else if (currentPos >= mapTileSize && this->move.top() == "d") { isMidJump = false; isEndJump = true; this->xP++; currentPos = 0; this->move.pop(); }
        else if (currentPos >= mapTileSize && this->move.top() == "dr") { isMidJump = false; isEndJump = true; this->xP++; this->yP++; currentPos = 0; this->move.pop(); }

        if(this->move.empty()) { isMoving = false; }
    }
    
    //cout << "Player coord : x(" << this->xP << ") y(" << this->yP << ")" << endl;

    player.setPosition(current);
    w.draw(player);

    Font font;
    if (!font.openFromFile("./fonts/chilispepper.ttf")) {
        throw "Не вдалося завантажити шрифт!";
    }
    Text text(font);
    text.setCharacterSize(66);
    text.setFillColor(Color::Cyan);
    if (this->xP == this->xG && this->yP == this->yG) {
        text.setString("Your min path price : " + to_string(this->minPrice));
        text.setPosition(Vector2f(offsetX - 15, offsetY));
        w.draw(text);
    }
}

void Game::MoveRight(Vector2f& current, int& currentPos) {
    
    float playerSpeed = 8.0f;
    current.x += playerSpeed;
    currentPos += playerSpeed;
}

void Game::MoveDown(Vector2f& current, int& currentPos) {

    float playerSpeed = 8.0f;
    current.y += playerSpeed;
    currentPos += playerSpeed;
}

void Game::MoveDownRight(Vector2f& current, int& currentPos) {

    float playerSpeed = 8.0f;
    current.x += playerSpeed;
    current.y += playerSpeed;
    currentPos += playerSpeed;
}

void Game::LeaveFootprints(RenderWindow& w) {
    static vector<CircleShape> footprints;
    CircleShape footprint(5);
    Color cl(87, 71, 30);
    footprint.setFillColor(cl);

    float mapTileSize = 64;
    float offsetX = (w.getSize().x - (this->map[0].size()) * mapTileSize) / 2;
    float offsetY = (w.getSize().y - (this->map.size()) * mapTileSize) / 2;

    footprint.setPosition(Vector2f(this->yP * mapTileSize + offsetX - 10 + mapTileSize / 2 - footprint.getRadius(), this->xP * mapTileSize + offsetY + mapTileSize / 2 - footprint.getRadius()));
    footprints.push_back(footprint);

    for (const auto& f : footprints) {
        w.draw(f);
    }
}

int main() {
    
    srand(static_cast<unsigned>(time(0)));
    
    RenderWindow w(VideoMode({ 1000, 800 }), "The best game");
    
    Color bg(87, 71, 30);

    Game g;
    g.ShowMap();
    g.FindPath();

    Clock clock;

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
        try {
            g.DrawMap(w, "./media/BaseChip.png");
            g.LeaveFootprints(w);
            g.DrawGoal(w, "./media/Goal/Idle.png", "./media/Goal/Idle_2.png", clock);
            g.DrawPlayer(w, "./media/Player/Special.png", "./media/Player/Walk.png", "./media/Player/Jump.png", clock);
        }
        catch (const char* err) {
            cout << err << endl;
            w.close();
        }
        w.display();
    }
}
