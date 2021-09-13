#include <iostream>
//headers required by SFML
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

//base game code and function
#include"engine.h"

//Declaration of user defined functions
void update(sf::Text&);
void degrade(sf::Text&);
void createHTPwindow();
void createPwindow();
void AboutUs();
bool again = false;

int main()
{
    //Music
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("./assets/hamada.ogg"))
        std::cout << "Sound buffer couldnt load";
    buffer.loadFromFile("./assets/hamada.ogg");
    sf::Sound keypressed;
    keypressed.setBuffer(buffer);
    keypressed.setVolume(100);
    keypressed.play();
        //font
    sf::Font font;
    font.loadFromFile("./assets/flp.ttf");
    if (!font.loadFromFile("./assets/flp.ttf"))
    {
        std::cout << "Font Load Failed!" << std::endl;
        return 0;
    }

    //Window for GUI:-
    sf::RenderWindow window(sf::VideoMode(1366, 786), "Flappy bird", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    sf::Event ev;
    window.setVerticalSyncEnabled(true);
   
    //Counter to determine the indeces 
    int counterForText = 0;
    bool exit = 0;
    int size = 4;
    //text as GUI options
    sf::Text* text= new sf::Text[size];
    
    for (int i = 0; i < 4; i++)
    {

        text[i].setFont(font);
        text[i].setPosition(160, (i * 115) + 400);
        text[i].setStyle(sf::Text::Bold | sf::Text::Italic);
        text[i].setCharacterSize(64);

        switch (i)
        {
        case 0:
            text[i].setString("Play");
            update(text[i]);
            break;
        case 1:
            text[i].setString("How To Play");
            text[i].setFillColor(sf::Color::White);
            break;
        case 2:
            text[i].setString("About Us");
            text[i].setFillColor(sf::Color::White);
            break;
        case 3 :
            text[i].setString("Exit");
            text[i].setFillColor(sf::Color::White);
            text[i].setPosition(1100, 550);
            text[i].setCharacterSize(86);
        default:
            break;
        }
    }

    //back ground
    sf::Texture backgroundT;
    Collision::CreateTextureAndBitmask(backgroundT,"./assets/back.jpg");
    if (!Collision::CreateTextureAndBitmask(backgroundT, "./assets/back.jpg"))
    {
        std::cout << "BackGroundT img Load Failed!" << std::endl;
        return 0;
    }

    sf::Sprite backG;
    backG.setTexture(backgroundT);

    //GUI Body  :-
    while (window.isOpen())
    {
        
        //events loop to take input :-
        while (window.pollEvent(ev))
        {
            //switch case to determine what to do :-
            switch (ev.type)
            {
                //close button
            case sf::Event::Closed:
                //delete &text;
                text = NULL;
                window.close();
                return 0;
                break;
                //keyboard buttons actions :-
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape) {
                    text = NULL;
                    window.close();
                    return 0;
                }
                // highlighting the text // 
                /*applying simple animation to the game*/
                //down event
                else if (ev.key.code == sf::Keyboard::Down) {
                    exit = 0;
                    text[3].setCharacterSize(86);
                    text[3].setFillColor(sf::Color::White);
                    keypressed.play();
                    counterForText++;
                    counterForText %= 3;
                    update(text[counterForText]);
                    //Animating Text using simple math trick of Remainder 
                    switch (3 - counterForText)
                    {
                    case 3:
                        degrade(text[1]);
                        degrade(text[2]);
                        break;
                    case 2:
                        degrade(text[0]);
                        degrade(text[2]);
                        break;
                    case 1:
                        degrade(text[0]);
                        degrade(text[1]);
                        break;

                    }
                    break;
                }
                //up event
                else if (ev.key.code == sf::Keyboard::Up) {
                    exit = 0;
                    text[3].setCharacterSize(86);
                    text[3].setFillColor(sf::Color::White);
                    keypressed.play();
                    if (counterForText == 0)
                        counterForText = 2;
                    else
                        counterForText--;
                    counterForText %= 3;
                    update(text[counterForText]);
                    switch (3 - counterForText)
                    {
                    case 3:
                        degrade(text[1]);
                        degrade(text[2]);
                        break;
                    case 2:
                        degrade(text[0]);
                        degrade(text[2]);
                        break;
                    case 1:
                        degrade(text[0]);
                        degrade(text[1]);
                        break;
                    }
                    break;
                }
                else if (ev.key.code == sf::Keyboard::Right && !exit)
                {
                    keypressed.play();
                    degrade(text[0]);
                    degrade(text[2]);
                    degrade(text[1]);
                    text[3].setCharacterSize(108);
                    text[3].setFillColor(sf::Color::Red);
                    exit = 1;
                }
                else if (ev.key.code == sf::Keyboard::Left && exit)
                {
                    keypressed.play();
                    counterForText = 0;
                    update(text[counterForText]);
                    text[3].setCharacterSize(86);
                    text[3].setFillColor(sf::Color::White);
                    exit = 0;
                }
                //choosing from menu event
                else if (ev.key.code == sf::Keyboard::Enter|| ev.key.code == sf::Keyboard::Space) {
                    if (exit)
                    {
                        text = NULL;
                        window.close();
                        return 0;
                    }
                    else {
                        switch (counterForText)
                        {
                        case 0://Play

                            window.setActive(0);
                            window.setVisible(0);
                            createPwindow();
                            window.setVisible(1);
                            window.setActive(1);

                            break;
                        case 1://how to play
                            window.setActive(0);
                            window.setVisible(0);
                            createHTPwindow();
                            window.setVisible(1);
                            window.setActive(1);
                            break;
                        case 2://About Us
                            window.setActive(0);
                            window.setVisible(0);
                            AboutUs();
                            window.setVisible(1);
                            window.setActive(1);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

        }

        //Render
        window.clear();
        window.draw(backG);
        for (int i = 0; i < 4; i++)
            window.draw(text[i]);
        window.display();
    }
}
//functions to change in options 

/*Updating and Degrading*/
void update(sf::Text& text)
{
    text.setFillColor(sf::Color::Yellow);
    text.setCharacterSize(80);

}
void degrade(sf::Text& text) {
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(64);
}

//Creating a new window for How To Play
void createHTPwindow() {
   
    
    //Window for How to Play :-
    sf::RenderWindow howToPlayW(sf::VideoMode(1366, 786), "How To Play", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    sf::Event ev;
    //background img :-
    sf::Texture background2;
    Collision::CreateTextureAndBitmask(background2,"./assets/backgroundhtp.png");
    if (!Collision::CreateTextureAndBitmask(background2, "./assets/backgroundhtp.png")) {
        std::cout << "background2 Img Load Failed\n";}

    sf::Sprite background2G;
    background2G.setTexture(background2);
    
    //what's written ************************************************* 
        //font
    sf::Font font;
    font.loadFromFile("./assets/arial.ttf");
    if (!font.loadFromFile("./assets/arial.ttf"))
        std::cout << "Font(flp.ttf) Load Failed!" << std::endl;
    //text :-
    sf::Text howToPlay;
    howToPlay.setFont(font);
    howToPlay.setFillColor(sf::Color::Black);
    howToPlay.setCharacterSize(44);
    howToPlay.setStyle(sf::Text::Bold | sf::Text::Italic);
    howToPlay.setLineSpacing(1.5);
    howToPlay.setString("\t--FlappyBird is a simple retro / pixel art game which has a \n simple goal \n\t\t DO NOT HIT THE PIPES \n\t Controls :\n\t\t Press Space to flap/jump or continue game if paused  \n\t\t Press Escape to pause game .\n\n\t--collect coins to get bonus.\n\n\t--Difficulty can be set before starting. \n\n\n\n\n\t\t\t\t\t\t");
    sf::Text back;
    back.setFont(font);
    back.setFillColor(sf::Color::White);
    back.setCharacterSize(64);
    back.setStyle(sf::Text::Bold | sf::Text::Italic);
    back.setString("Press Enter To Go Back");
    back.setPosition(500, 450);
    //Function execution :-

    while (howToPlayW.isOpen())
    {
        while (howToPlayW.pollEvent(ev)) {
            switch (ev.type)
            {
            case sf::Event::Closed:
                howToPlayW.close();
                break;

            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Enter || ev.key.code == sf::Keyboard::Space)
                    howToPlayW.close();
                break;
            }
        }
        howToPlayW.clear();
        howToPlayW.draw(background2G);
        howToPlayW.draw(back);
        howToPlayW.draw(howToPlay);
        howToPlayW.display();
    }
}
void createPwindow() {
    
    //font
    sf::Font font;
    font.loadFromFile("./assets/flp.ttf");
    if (!font.loadFromFile("./assets/flp.ttf"))
        std::cout << "Font Load Failed!" << std::endl;

    //Window for Play:-
    sf::RenderWindow play(sf::VideoMode(1366, 786), "Flappy bird", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    sf::Event ev;

    //Counter to determine the indeces 
    int counterForText = 0;

    //music
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("hamada.ogg"))
        std::cout << "Sound buffer couldnt load";
    buffer.loadFromFile("./assets/hamada.ogg");
    sf::Sound keypressed;
    keypressed.setBuffer(buffer);
    keypressed.setVolume(100);
    keypressed.play();


    //text as Play options

    sf::Text text[3];
    sf::Text Title;
    Title.setFont(font);
    Title.setPosition(100, 50);
    Title.setStyle(sf::Text::Underlined | sf::Text::Bold );
    Title.setCharacterSize(64);
    Title.setFillColor(sf::Color::Black);
    Title.setString("Difficulty :-");
    for (int i = 0; i < 3; i++) {

        text[i].setFont(font);
        text[i].setPosition(175, (i * 150) + 150);
        text[i].setStyle(sf::Text::Bold | sf::Text::Italic);
        text[i].setCharacterSize(64);

        switch (i)
        {
        case 0:
            text[i].setString("Level 1 <Easy>");
            update(text[i]);
            break;
        case 1:
            text[i].setString("Level 2 <Hard>");
            text[i].setFillColor(sf::Color::White);
            break;
        case 2:
            text[i].setString("<= Back");
            text[i].setFillColor(sf::Color::White);
            break;
        default:
            break;
        }
    }

    //back ground
    sf::Texture backgroundT;
    Collision::CreateTextureAndBitmask(backgroundT,"./assets/backgroundhtp.png");
    if (!Collision::CreateTextureAndBitmask(backgroundT, "./assets/backgroundhtp.png"))
        std::cout << "Play img Load Failed!" << std::endl;

    sf::Sprite playG;

    playG.setTexture(backgroundT);

    //GUI Body  :-
    while (play.isOpen())
    {
        //events loop to take input :-
        while (play.pollEvent(ev))
        {
            //switch case to determine what to do :-
            switch (ev.type)
            {
                //close button
                case sf::Event::Closed:
                play.close();
                break;
                //keyboard buttons actions :-
                case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape)
                    play.close();
                // highlighting the text // 
                                /*applying simple animation to the game*/
                                //down event
                else if (ev.key.code == sf::Keyboard::Down) {
                    keypressed.play();
                    counterForText++;
                    counterForText %= 3;
                    update(text[counterForText]);
                    //Animating Text using simple math trick of Remainder 
                    switch (3 - counterForText)
                    {
                    case 3:
                        degrade(text[1]);
                        degrade(text[2]);
                        break;
                    case 2:
                        degrade(text[0]);
                        degrade(text[2]);
                        break;
                    case 1:
                        degrade(text[0]);
                        degrade(text[1]);
                        break;

                    }
                    break;
                }
                //up event
                else if (ev.key.code == sf::Keyboard::Up) {
                    keypressed.play();
                    if (counterForText == 0)
                        counterForText = 2;
                    else
                        counterForText--;
                    counterForText %= 3;
                    update(text[counterForText]);
                    switch (3 - counterForText)
                    {
                    case 3:
                        degrade(text[1]);
                        degrade(text[2]);
                        break;
                    case 2:
                        degrade(text[0]);
                        degrade(text[2]);
                        break;
                    case 1:
                        degrade(text[0]);
                        degrade(text[1]);
                        break;
                    }
                    break;
                }
                else if (ev.key.code == sf::Keyboard::Enter || ev.key.code == sf::Keyboard::Space) {
                    switch (counterForText)
                    {
                    case 0:
                        play.setVisible(0);
                        play.setActive(0);
                        playGame();
                        play.setVisible(1);
                        play.setActive(1);                     
                        break;
                    case 1:
                        play.setVisible(0);
                        play.setActive(0);
                        playGameHard();
                        play.setVisible(1);
                        play.setActive(1);
                        break;
                    case 2:
                        play.close();
                        break;
                    default:
                        std::cout << "An unexpected error occured\n";
                    }
                    play.close();
                }
            }

        }

        //Render
        play.clear();
        play.draw(playG);
        play.draw(Title);
        for (int i = 0; i < 3; i++)
        play.draw(text[i]);
        play.display();
    }
}
void AboutUs() {
    //Window for How to Play :-
    sf::RenderWindow AboutUs(sf::VideoMode(1366, 786), "About Us", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    sf::Event ev;
    //background img :-
    sf::Texture background2;
    Collision::CreateTextureAndBitmask(background2, "./assets/backgroundhtp.png");
    if (!Collision::CreateTextureAndBitmask(background2, "./assets/backgroundhtp.png"))
        std::cout << "background2 Img Load Failed\n";

    sf::Sprite background2G;
    background2G.setTexture(background2);

    //what's written ************************************************* 
        //font
    sf::Font font;
    font.loadFromFile("./assets/arial.ttf");
    if (!font.loadFromFile("./assets/arial.ttf"))
        std::cout << "Font(flp.ttf) Load Failed!" << std::endl;
    //text :-
    sf::Text aboutUs;
    aboutUs.setFont(font);
    aboutUs.setFillColor(sf::Color::Black);
    aboutUs.setCharacterSize(36);
    aboutUs.setStyle(sf::Text::Bold | sf::Text::Italic);
    aboutUs.setLineSpacing(1.5);
    /*
    This game was developed only and for only academic purposes by 6 students of first year of FCIS ASU as a project for SP module.\n-Any usage for this source code without the permision of makers IS NOT ALLOWED by any mean-\n
    */
    aboutUs.setLetterSpacing(1);
    aboutUs.setString("Makers :-\n~Mohamed Samy(Team Leader):\n   -Game physics, pause function, assets editing and second level.\n~Yousef AbdulHamid:\n   -Menus GUI & assets ,Optimization of collision and textures' loading\n -Saving highscore.\n~Nada Mohammed :\n   -Pipes and background , parallax scrolling and score calculation.\n~Yara Zidan :\n   -collision detection and gameover screen.\n~Mai Adel :\n   -Music and Sound effects and coins.\n ");
    sf::Text back;
    back.setFont(font);
    back.setFillColor(sf::Color::Color(255,87,51));
    back.setCharacterSize(54);
    back.setStyle(sf::Text::Bold);
    back.setString("Press Enter To Go Back");
    back.setPosition(700, 610);
    //Function execution :-

    while (AboutUs.isOpen())
    {
        while (AboutUs.pollEvent(ev)) {
            switch (ev.type)
            {
            case sf::Event::Closed:
                AboutUs.close();
                break;

            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Enter || ev.key.code == sf::Keyboard::Space)
                    AboutUs.close();
                break;
            }
        }
        AboutUs.clear();
        AboutUs.draw(background2G);
        AboutUs.draw(back);
        AboutUs.draw(aboutUs);
        AboutUs.display();
    }
}