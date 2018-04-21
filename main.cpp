#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"

#define BOARDSIZE 8
#define OFFSET 40
#define TILESIZE 80

#define HIDDEN 0
#define EMPTY 1
#define INVALID 2
#define SHIP 3
#define SHIPFRONT 4
#define SHIPFRONT0 40
#define SHIPFRONT90 41
#define SHIPFRONT180 42
#define SHIPFRONT270 43
#define SHIPMID 5
#define SHIPMID0 50
#define SHIPMID180 51
#define SHIPBACK 6
#define SHIPBACK0 60
#define SHIPBACK90 61
#define SHIPBACK180 62
#define SHIPBACK270 63

class SpriteManager;

class Point;

class Board;

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


class SpriteManager {
public:
    sf::Texture water, ship[3];
    sf::Sprite open, empty, shipMid, shipHead, shipTail;
    static bool midTurn;

    void init() {
        water.loadFromFile("texture/water.jpg");
        ship[0].loadFromFile("texture/shiphead.png");
        ship[1].loadFromFile("texture/shipmid.png");
        ship[2].loadFromFile("texture/shipback.png");
        open = sf::Sprite(water);
        empty = sf::Sprite(water);
        shipMid = sf::Sprite(ship[1]);
        shipHead = sf::Sprite(ship[0]);
        shipTail = sf::Sprite(ship[2]);

        shipHead.setTextureRect((sf::IntRect(0, 0, 64, 64)));
        shipMid.setTextureRect((sf::IntRect(0, 0, 64, 64)));
        shipTail.setTextureRect((sf::IntRect(0, 0, 64, 64)));
        open.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));
        empty.setTextureRect(sf::IntRect(0, 0, TILESIZE, TILESIZE));

        shipHead.scale((float) TILESIZE / 64, (float) TILESIZE / 64);
        shipMid.scale((float) TILESIZE / 64, (float) TILESIZE / 64);
        shipTail.scale((float) TILESIZE / 64, (float) TILESIZE / 64);

        empty.setColor(sf::Color(255, 255, 255, 50));
    }

    SpriteManager() {
        init();
    }

    sf::Sprite getSprite(int type, float angle = 0) {
        sf::Sprite tile;
        if (type == HIDDEN) return open;
        else if (type == EMPTY) return empty;
        else if (type == SHIPBACK) tile = shipTail;
        else if (type == SHIPFRONT) tile = shipHead;
        else if (type == SHIPMID) tile = shipMid;
        tile.setOrigin(64 / 2, 64 / 2);
        tile.setRotation(angle);
        return tile;
    }
};


class Board {
public:
    std::vector<std::vector<int> > pos;
    std::vector<std::vector<bool> > mask;
    int shipCount;
    std::vector<std::vector<sf::Sprite> > tiles;
    SpriteManager sprites;

    void init() {
        pos = std::vector<std::vector<int> >(BOARDSIZE, std::vector<int>(BOARDSIZE, EMPTY));
        mask = std::vector<std::vector<bool> >(BOARDSIZE, std::vector<bool>(BOARDSIZE, true));
        shipCount = 0;
        tiles = std::vector<std::vector<sf::Sprite> >(BOARDSIZE,
                                                      std::vector<sf::Sprite>(BOARDSIZE, sprites.getSprite(EMPTY)));
    }

    void addShip(std::pair<Point, Point> ship) {
        shipCount += (abs(ship.first.gety() - ship.second.gety()) + abs(ship.first.getx() - ship.second.getx()) + 1);
        if (ship.first.getx() == ship.second.getx())
            for (int y = std::min(ship.first.gety(), ship.second.gety());
                 y <= std::max(ship.first.gety(), ship.second.gety()); y++)
                setPos(ship.first.getx(), y, SHIP);
        else
            for (int x = std::min(ship.first.getx(), ship.second.getx());
                 x <= std::max(ship.first.getx(), ship.second.getx()); x++)
                setPos(x, ship.first.gety(), SHIP);
    }

    void addShipTile(std::pair<Point, Point> ship) {
        if (ship.first.getx() == ship.second.getx()) {                              //vertical
            if (ship.first.gety() > ship.second.gety()) {                           //bottom
                setTile(sprites.getSprite(SHIPFRONT, 90), ship.second);
                setTile(sprites.getSprite(SHIPBACK, 90), ship.first);
                for (int y = ship.second.gety() + 1; y < ship.first.gety(); y++)
                    setTile(sprites.getSprite(SHIPMID, 90), ship.first.getx(), y);
            } else {                                                                 //top
                setTile(sprites.getSprite(SHIPFRONT, 270), ship.second);
                setTile(sprites.getSprite(SHIPBACK, 270), ship.first);
                for (int y = ship.first.gety() + 1; y < ship.second.gety(); y++)
                    setTile(sprites.getSprite(SHIPMID, 90), ship.first.getx(), y);
            }
        } else {                                                                    //horizontal
            if (ship.first.getx() > ship.second.getx()) {                           //right
                setTile(sprites.getSprite(SHIPFRONT, 0), ship.second);
                setTile(sprites.getSprite(SHIPBACK, 0), ship.first);
                for (int x = ship.second.getx() + 1; x < ship.first.getx(); x++)
                    setTile(sprites.getSprite(SHIPMID, 0), x, ship.first.gety());
            } else {                                                                 //left
                setTile(sprites.getSprite(SHIPFRONT, 180), ship.second);
                setTile(sprites.getSprite(SHIPBACK, 180), ship.first);
                for (int x = ship.first.getx() + 1; x < ship.second.getx(); x++)
                    setTile(sprites.getSprite(SHIPMID, 0), x, ship.first.gety());
            }
        }
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

public:
    Board() { // NOLINT
        init();
    }

    Board(std::vector<std::pair<Point, Point> > ships) { // NOLINT
        init();
        for (auto ship : ships) {
            addShip(ship);
            addShipTile(ship);
        }
    }

    int getPos(int x, int y) {
        if (x >= BOARDSIZE || y >= BOARDSIZE || x < 0 || y < 0) return INVALID;
        return pos.at(x).at(y);
    }

    int getPos(Point p) {
        if (p.getx() >= BOARDSIZE || p.gety() >= BOARDSIZE || p.getx() < 0 || p.gety() < 0) return INVALID;
        return pos.at(p.getx()).at(p.gety());
    }

    void setPos(int x, int y, int state) {
        pos.at(x).at(y) = state;
    }

    void setPos(Point p, int state) {
        pos.at(p.getx()).at(p.gety()) = state;
    }

    bool isPosFree(int x, int y) {
        if (x >= BOARDSIZE || y >= BOARDSIZE || x < 0 || x < 0) return false;
        return getPos(x, y) == EMPTY;
    }

    bool isPosFree(Point p) {
        if (p.getx() >= BOARDSIZE || p.gety() >= BOARDSIZE || p.getx() < 0 || p.gety() < 0) return false;
        return getPos(p) == EMPTY;
    }

    bool isPosNotFree(int x, int y) {
        return !isPosFree(x, y);
    }

    bool isPosNotFree(Point p) {
        return !isPosFree(p);
    }

    bool isHidden(Point p) {
        return mask.at(p.getx()).at(p.gety());
    }

    bool isHidden(int x, int y) {
        return mask.at(x).at(y);
    }

    int getType(int x, int y) {
        if (isHidden(x, y)) return HIDDEN;
        else return getPos(x, y);
    }

    int getType(Point p) {
        if (isHidden(p)) return HIDDEN;
        else return getPos(p);
    }

    sf::Sprite getTile(int x, int y) {
        if (isHidden(x, y))
            return sprites.getSprite(HIDDEN);

        else
            return tiles.at(x).at(y);
    }

    sf::Sprite getTile(Point p) {
        if (isHidden(p))
            return sprites.getSprite(HIDDEN);

        else
            return tiles.at(p.getx()).at(p.gety());
    }

    void setTile(const sf::Sprite &tile, Point p) {
        tiles.at(p.getx()).at(p.gety()) = tile;
    }

    void setTile(const sf::Sprite &tile, int x, int y) {
        tiles.at(x).at(y) = tile;
    }

    bool shipsLeft() {
        return shipCount > 0;
    }

    void setVisible(int x, int y) {
        if (getPos(x, y) == SHIP && mask.at(x).at(y)) shipCount--;
        mask.at(x).at(y) = false;

    }

    void setVisible(Point p) {
        if (getType(p) == SHIP && mask.at(p.getx()).at(p.gety())) shipCount--;
        mask.at(p.getx()).at(p.gety()) = false;
    }

    void addRandomShips(std::vector<int> shipSizes) {
        std::mt19937 gen{static_cast<unsigned int>(time(nullptr))};
        std::uniform_int_distribution<int> dis{0, BOARDSIZE - 1};
        for (auto size : shipSizes) {
            bool shipAdded = false;

            while (!shipAdded) {
                Point start(dis(gen), dis(gen));
                if (isPosFree(start)) {
                    std::array<Point, 4> directions = {
                            Point(start.getx(), start.gety() - size + 1),  //north
                            Point(start.getx(), start.gety() + size - 1),  //south
                            Point(start.getx() - size + 1, start.gety()),  //west
                            Point(start.getx() + size - 1, start.gety())   //east
                    };
                    std::shuffle(directions.begin(), directions.end(), gen);
                    for (auto end : directions) {
                        if (checkSafe(std::make_pair(start, end))) { ;
                            addShip(std::make_pair(start, end));
                            addShipTile(std::make_pair(start, end));
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


int main() {
    std::vector<std::pair<Point, Point> > ships = {std::make_pair(Point(1, 4), Point(1, 6)),
                                                   std::make_pair(Point(4, 6), Point(6, 6)),
                                                   std::make_pair(Point(6, 3), Point(6, 1)),
                                                   std::make_pair(Point(3, 1), Point(1, 1))};
    Board board = Board(ships);
    int moves = 0;
    //board.addRandomShips({2, 3, 4, 5});
    sf::RenderWindow app(sf::VideoMode(BOARDSIZE * TILESIZE + 2 * OFFSET, BOARDSIZE * TILESIZE + 2 * OFFSET),
                         "Battleships");
    sf::Sprite tile;
    board.printBoard();

    while (app.isOpen()) {
        sf::Event event; // NOLINT
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                app.close();
        }
        if (event.type == sf::Event::MouseButtonPressed)
            if (event.mouseButton.button == sf::Mouse::Left)
                if (event.mouseButton.x >= OFFSET && event.mouseButton.x <= (OFFSET + BOARDSIZE * TILESIZE))
                    if (event.mouseButton.y >= OFFSET && event.mouseButton.y <= (OFFSET + BOARDSIZE * TILESIZE)) {
                        int xpos = (event.mouseButton.x - OFFSET) / TILESIZE;
                        int ypos = (event.mouseButton.y - OFFSET) / TILESIZE;
                        board.setVisible(xpos, ypos);
                        moves++;
                        if (!board.shipsLeft())
                            std::cout << "You win in " << moves << " moves" << std::endl;
                    }

        app.clear(sf::Color::Black);

        //draw tiles
        for (int i = 0; i < BOARDSIZE; i++) {
            for (int j = 0; j < BOARDSIZE; j++) {
                tile = board.getTile(i, j);
                if ((board.isHidden(i, j) || board.getPos(i, j) == EMPTY))
                    tile.setPosition(i * TILESIZE + OFFSET, j * TILESIZE + OFFSET);
                else
                    tile.setPosition(i * TILESIZE + OFFSET + TILESIZE / 2, j * TILESIZE + OFFSET + TILESIZE / 2);

                app.draw(tile);
            }
        }

        //draw borders
        std::array<sf::Vertex, 2> line;
        for (int i = 0; i <= BOARDSIZE; i++) {
            line[0] = sf::Vertex(sf::Vector2f(OFFSET + i * TILESIZE, OFFSET));
            line[1] = sf::Vertex(sf::Vector2f(OFFSET + i * TILESIZE, BOARDSIZE * TILESIZE + OFFSET));
            app.draw(line.begin(), 2, sf::Lines);
        }

        for (int i = 0; i <= BOARDSIZE; i++) {
            line[0] = sf::Vertex(sf::Vector2f(OFFSET, OFFSET + i * TILESIZE));
            line[1] = sf::Vertex(sf::Vector2f(BOARDSIZE * TILESIZE + OFFSET, OFFSET + i * TILESIZE));
            app.draw(line.begin(), 2, sf::Lines);
        }
        app.display();
    }
    return 0;
}

#pragma clang diagnostic pop