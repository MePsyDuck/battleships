#include <SFML/Graphics.hpp>

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
    // 0 = empty
    // 1 = ship
    //-1 = cell used/invalid
    vector< vector<int> > pos;
public:
    Board(){
        for(int i=0;i<6;i++)
            pos.at(i) = vector<int>(SIZE, 0);
    }

    Board(vector<pair<Point, Point> > ships) {
        Board();
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

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 400), "Battleships");
    sf::Texture water;
    water.loadFromFile("texture/water.jpg");
    sf::Sprite opensea(water), emptysea(water), ship(water);
    opensea.setTextureRect(sf::IntRect(0, 0, 60, 60));
    emptysea.setTextureRect(sf::IntRect(0, 0, 60, 60));
    ship.setTextureRect(sf::IntRect(0, 0, 60, 60));
    emptysea.setColor(sf::Color(255, 255, 255, 50));
    ship.setColor(sf::Color(255, 255, 255));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(ship);
        window.display();
    }
    return 0;
}