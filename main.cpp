#include <SFML/Graphics.hpp>
using namespace std;

class Board{
    // 0 = empty
    // 1 = ship
    //-1 = cell used/invalid
    vector< vector<int> > pos;
    Board(){
        for(int i=0;i<6;i++)
            pos.at(i) = vector<int>(6,0);
    }
    
};

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 500), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }

    return 0;
}