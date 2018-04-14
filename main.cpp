#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#define BOARDSIZE 8
#define OFFSET 40
#define TILESIZE 80

#define HIDDEN 0
#define SHIP 1
#define EMPTY 2
#define INVALID 3

class Point {
    int x, y;
public:
    Point() {
        x = y = 0;
    }

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    int getx() {
        return this->x;
    }

    int gety() {
        return this->y;
    }

    void setx(int x) {
        this->x = x;
    }

    void sety(int y) {
        this->y = y;
    }

};

class Board {
    std::vector<std::vector<int> > pos;

    void init() {
        pos = std::vector<std::vector<int> >(BOARDSIZE, std::vector<int>(BOARDSIZE, EMPTY));
    }

public:
    Board() {
        init();
    }

    Board(std::vector<std::pair<Point, Point> > ships) {
        init();
        for (auto ship : ships)
            addShip(ship);
    }

    void addShip(std::pair<Point, Point> ship) {
        if (ship.first.getx() == ship.second.getx())
            for (int y = std::min(ship.first.gety(), ship.second.gety());
                 y <= std::max(ship.first.gety(), ship.second.gety()); y++)
                setPos(ship.first.getx(), y, SHIP);
        else
            for (int x = std::min(ship.first.getx(), ship.second.getx());
                 x <= std::max(ship.first.getx(), ship.second.getx()); x++)
                setPos(x, ship.first.gety(), SHIP);
    }

    bool checkSafe(std::pair<Point, Point> ship) {
        if (ship.first.getx() == ship.second.getx())
            for (int y = std::min(ship.first.gety(), ship.second.gety());
                 y <= std::max(ship.first.gety(), ship.second.gety()); y++)
                if (isPosNotFree(ship.first.getx(), y)) return false;
                else;
        else if (ship.first.gety() == ship.second.gety())
            for (int x = std::min(ship.first.getx(), ship.second.getx());
                 x <= std::max(ship.first.getx(), ship.second.getx()); x++)
                if (isPosNotFree(x, ship.first.gety())) return false;
                else;
        else return false;
        return true;
    }

    int getPos(int x, int y) {
        if (x >= BOARDSIZE || y >= BOARDSIZE || x < 0 || y < 0) return INVALID;
        return pos.at(x).at(y);
    }

    void setPos(int x, int y, int s) {
        pos.at(x).at(y) = s;
    }

    bool isPosFree(int x, int y) {
        if (x >= BOARDSIZE || y >= BOARDSIZE || x < 0 || x < 0) return false;
        return getPos(x, y) == EMPTY;
    }

    bool isPosNotFree(int x, int y) {
        return !isPosFree(x, y);
    }

    int getPos(Point p) {
        if (p.getx() >= BOARDSIZE || p.gety() >= BOARDSIZE || p.getx() < 0 || p.gety() < 0) return INVALID;
        return pos.at(p.getx()).at(p.gety());
    }

    void setPos(Point p, int state) {
        pos.at(p.getx()).at(p.gety()) = state;
    }

    bool isPosFree(Point p) {
        if (p.getx() >= BOARDSIZE || p.gety() >= BOARDSIZE || p.getx() < 0 || p.gety() < 0) return false;
        return getPos(p) == EMPTY;
    }

    bool isPosNotFree(Point p) {
        return !isPosFree(p);
    }

    void addRandomShips(std::vector<int> shipSizes) {
        std::random_device rd;
        std::mt19937 gen{rd()};
        std::uniform_int_distribution<int> dis{0, BOARDSIZE - 1};
        for (auto size : shipSizes) {
            bool shipAdded = false;
            while (!shipAdded) {
                Point start(dis(gen), dis(gen));
                if (isPosFree(start)) {
                    std::vector<Point> directions = {
                            Point(start.getx(), start.gety() - size + 1),  //north
                            Point(start.getx(), start.gety() + size - 1),  //south
                            Point(start.getx() - size + 1, start.gety()),  //west
                            Point(start.getx() + size - 1, start.gety())   //east
                    };
                    std::shuffle(directions.begin(), directions.end(), gen);
                    for (auto end : directions) {
                        if (checkSafe(std::make_pair(start, end))) {
                            std::cout << "Adding ship of size " << size << " start at(" << start.getx() << ","
                                      << start.gety() << ") and end at (" << end.getx() << "," << end.gety()
                                      << ")"
                                      << std::endl;
                            addShip(std::make_pair(start, end));
                            shipAdded = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    void printBoard() {  // DEBUG
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++)
                std::cout << getPos(j, i) << " ";
            std::cout << std::endl;
        }
    }
};

class SpriteManager {
public:
    sf::Texture water;
    sf::Sprite open, empty, ship;

    SpriteManager() {
        water.loadFromFile("texture/water.jpg");
        open = sf::Sprite(water);
        empty = sf::Sprite(water);
        open = sf::Sprite(water);
        open.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));
        empty.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));
        ship.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));
        empty.setColor(sf::Color(255, 255, 255, 50));
        ship.setColor(sf::Color(255, 255, 255));
    }

    sf::Sprite getSprite(int type) {
        if (type == HIDDEN) return open;
        if (type == EMPTY) return empty;
        else return ship;
    }
};

int main() {
    Board board = Board();
    board.addRandomShips({2, 3, 4, 5});
    sf::RenderWindow window(sf::VideoMode(BOARDSIZE * TILESIZE + 2 * OFFSET, BOARDSIZE * TILESIZE + 2 * OFFSET),
                            "Battleships");
    SpriteManager sprites = SpriteManager();
    sf::Sprite tile;
    while (window.isOpen()) {
        sf::Event event; // NOLINT
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        //draw tiles
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                tile = sprites.getSprite(board.getPos(i, j));
                tile.setPosition(sf::Vector2f(OFFSET + i * TILESIZE, OFFSET + j * TILESIZE));
                window.draw(tile);
            }
        }
        //draw borders
        sf::Vertex line[2];
        for (int i = 0; i <= BOARDSIZE; i++) {
            line[0] = sf::Vertex(sf::Vector2f(OFFSET + i * TILESIZE, OFFSET));
            line[1] = sf::Vertex(sf::Vector2f(OFFSET + i * TILESIZE, BOARDSIZE * TILESIZE + OFFSET));
            window.draw(line, 2, sf::Lines);
        }

        for (int i = 0; i <= BOARDSIZE; i++) {
            line[0] = sf::Vertex(sf::Vector2f(OFFSET, OFFSET + i * TILESIZE));
            line[1] = sf::Vertex(sf::Vector2f(BOARDSIZE * TILESIZE + OFFSET, OFFSET + i * TILESIZE));
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
    return 0;
}

#pragma clang diagnostic pop