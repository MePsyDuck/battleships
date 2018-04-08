#include <SFML/Graphics.hpp>
#include <iostream>

#define SIZE 6

using namespace std;

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

class Board{
    // 0 = open/unknown
    // 1 = ship
    //-1 = cell empty/invalid
    vector< vector<int> > pos;

    void init() {
        pos = vector<vector<int> >(SIZE, vector<int>(SIZE, 0));
    }
public:
    Board(){
        init();
    }

    Board(vector<pair<Point, Point> > ships) {
        init();
        for (auto ship : ships)
            addship(ship);
    }

    int addship(pair<Point, Point> ship) {
        if (ship.first.getx() == ship.second.getx())
            for (int y = min(ship.first.gety(), ship.second.gety());
                 y <= max(ship.first.gety(), ship.second.gety()); y++)
                if (checkposocc(ship.first.getx(), y)) return -1;
                else;
        else
            for (int x = min(ship.first.getx(), ship.second.getx());
                 x <= max(ship.first.getx(), ship.second.getx()); x++)
                if (checkposocc(x, ship.first.gety())) return -1;
                else;

        if (ship.first.getx() == ship.second.getx())
            for (int y = min(ship.first.gety(), ship.second.gety());
                 y <= max(ship.first.gety(), ship.second.gety()); y++)
                setpos(ship.first.getx(), y, 1);
        else
            for (int x = min(ship.first.getx(), ship.second.getx());
                 x <= max(ship.first.getx(), ship.second.getx()); x++)
                setpos(x, ship.first.getx(), 1);

        return 1;
    }

    int getpos(int x, int y) {
        return pos.at(x).at(y);
    }

    void setpos(int x, int y, int s) {
        pos.at(x).at(y) = s;
    }

    bool checkposfree(int x, int y) {
        if (pos.at(x).at(y) == 0) return true;
        else return false;
    }

    bool checkposocc(int x, int y) {
        return !checkposfree(x, y);
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
        open.setTextureRect(sf::IntRect(0, 0, 60, 60));
        empty.setTextureRect(sf::IntRect(0, 0, 60, 60));
        ship.setTextureRect(sf::IntRect(0, 0, 60, 60));
        empty.setColor(sf::Color(255, 255, 255, 50));
        ship.setColor(sf::Color(255, 255, 255));
    }

    sf::Sprite getsprite(int type) {
        if (type == 0) return open;
        if (type == -1) return empty;
        else return ship;
    }
};

int main() {
    Board board1, board2;
    sf::RenderWindow window(sf::VideoMode(800, 400), "Battleships");
    SpriteManager sprites;
    sf::Sprite tile;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        //draw board 1
        for (int i = 0; i < SIZE; i++) {
            int xoffset = 20, yoffset = 20;
            for (int j = 0; j < SIZE; j++) {
                tile = sprites.getsprite(board1.getpos(i, j));
                tile.setPosition(sf::Vector2f(xoffset + i * 60, yoffset + j * 60));
                window.draw(tile);
            }
        }
        //draw board 2
        for (int i = 0; i < SIZE; i++) {
            int xoffset = 420, yoffset = 20;
            for (int j = 0; j < SIZE; j++) {
                tile = sprites.getsprite(board2.getpos(i, j));
                tile.setPosition(sf::Vector2f(xoffset + i * 60, yoffset + j * 60));
                window.draw(tile);
            }
        }
        window.display();
    }
    return 0;
}