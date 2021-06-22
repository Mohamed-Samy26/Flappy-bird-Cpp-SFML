
//  <<<<<<<<<<<<<<<<flappy bird game engine>>>>>>>>>>>>>>>>
//
/*  Team leader : Mohamed samy
        made Game physics and pause function and assets editing and second level
        
        and

        Menus GUI & menu assets
        by : Youssef Abdul-Hamid

        Pipes and background , parallax scrolling and score calculation
        by : Nada Mohamed

        colloision detection and gameover screen
        by : Yara Zedan

        Music and Sound effects
        by : Mai Adel
*/

//       We used Visal Studio 2019 (as an IDE)
//     , SFML graphics libirary (for graphics)
//     , photoshop 2019 (to edit assets and upscale resolution)
//       and audacity (to edit sound effects)


//**************************************************************//
                //to do list: 
   //coins
   //add shop
//*************************************************************//
//

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include"Collision.h"

//<<<<<<<<<<<<<USER DEFINED FUNCTIONS>>>>>>>>>>>>>>>>

/*inline facilitates using function across files
without issues for repetion or declaration*/

inline int updateBrd(int& x)
{
    x++;
    x = x % 4;
    return x;
}

inline bool playGame()
{
    //<<<<<<<<<<<<<<<<<<STRUCT>>>>>>>>>>>>>>>>>>>
    
    struct Textures
    {
        sf::Texture bird[4];
        sf::Texture bg;
        sf::Texture pipe;
        sf::Texture unpipe;
        sf::Texture ground;
        sf::Texture pausedGround;
        sf::Texture cointex;
        sf::Texture GO;

    }textures;

    bool retry = true;
    //intialize window dimenstions 
    const int width = 1366;
    const int windowlength = 786;

    //declaring or initializing variables 

    //before game start & pause

    sf::Text screen;
    sf::Font font;
    font.loadFromFile("./assets/flp.ttf");
    screen.setFont(font);
    screen.setString("Press space to play / continue");
    screen.setCharacterSize(60);
    screen.setFillColor(sf::Color::Yellow);
    screen.setOutlineColor(sf::Color(0, 50, 255, 100));
    screen.setPosition(270, 100);

    //pause control & input correction

    bool isFisrtpress = false;
    sf::Time delayValue;
    sf::Time pauseValue;
    //sf::Time delayHold = sf::seconds(1);
    sf::Time  pauseTime = sf::seconds(1);
    sf::Clock delayCalc;
    sf::Clock waitForPause;

    //declaration of physics variables

    sf::Vector2f gravity(0, 0.3f);  //fall & jump controls 
    sf::Vector2f velocity(0, 0.1f);
    sf::Vector2f velocity1(0, 0.1f);
    sf::Vector2f velocityMax(0, 1.6f);

    float initialRot = 0.0f; //Rotation controls
    float maxRot = 30.f;
    float accRot = 4.5f;
    sf::Clock Vclk;
    sf::Time Vvalue;
    sf::Clock Rclk;
    sf::Time Rvalue;
    sf::Time basicDelay = sf::milliseconds(70);
    sf::Time rapidDelay = sf::milliseconds(20);
    bool jumping = false;

    //sounds
    sf::SoundBuffer flapBuffer;
    sf::SoundBuffer hitBuffer;
    sf::SoundBuffer dieBuffer;
    flapBuffer.loadFromFile("./assets/sfx_wing.wav");
    hitBuffer.loadFromFile("./assets/sfx_hit.wav");
    dieBuffer.loadFromFile("./assets/sfx_die.wav");
    sf::Sound flap;
    sf::Sound hit;
    sf::Sound die;
    flap.setBuffer(flapBuffer);
    hit.setBuffer(hitBuffer);
    die.setBuffer(dieBuffer);
    sf::Music music;
    music.openFromFile("./assets/music.ogg");

    //Window drawing and title
    sf::RenderWindow Game(sf::VideoMode(width, windowlength), "Flappy bird");
    //Game.setVerticalSyncEnabled(true); //too late to use
    //for fullscreen
    //sf::RenderWindow Game(sf::VideoMode::getDesktopMode(), "Flappy bird Fullsceen", sf::Style::Fullscreen);

    //<<<<<<<<<<ARRAY>>>>>>>>>>
    //bird character

    //create array of bird textures to animate
    Collision::CreateTextureAndBitmask(textures.bird[0], "./assets/bird11.png"); //loading bird textures
    Collision::CreateTextureAndBitmask(textures.bird[1], "./assets/bird12.png");
    Collision::CreateTextureAndBitmask(textures.bird[2], "./assets/bird13.png");
    Collision::CreateTextureAndBitmask(textures.bird[3], "./assets/bird14.png");

    int birdState = 0;
    sf::Sprite sprite;
    sprite.setTexture(textures.bird[birdState]);
    float x = width / 3, y = windowlength / 1.5;
    sprite.setPosition(x, y);
    sprite.setOrigin(64, 45);

    // back ground 
    Collision::CreateTextureAndBitmask(textures.bg, "./assets/dayf.png");
    sf::Sprite sky;
    sky.setTexture(textures.bg);
    // pipe

    Collision::CreateTextureAndBitmask(textures.pipe, "./assets/pipe.png");
    sf::Sprite lowerpipe;
    lowerpipe.setTexture(textures.pipe);
    lowerpipe.setScale(1.5, 1.5);
    sf::Sprite upperpipe;
    upperpipe.setTexture(textures.pipe);
    upperpipe.scale(1.5, -1.5);

    // unreal pipe (for score)
    Collision::CreateTextureAndBitmask(textures.unpipe, "./assets/Empty.png");
    sf::Sprite unrealpipe;
    unrealpipe.setTexture(textures.unpipe);
    unrealpipe.scale(0.00001, 1.5);
    std::vector <sf::Sprite > unreal_pipe;
    std::vector <sf::Sprite >::iterator iup = unreal_pipe.begin();
    sf::SoundBuffer unphitbuffer;
    unphitbuffer.loadFromFile("./assets/unphit.ogg");
    sf::Sound unphit;
    unphit.setBuffer(unphitbuffer);
    int r; //verical distance between pipes (bad declaration name)
    std::vector <sf::Sprite> pipes;
    std::vector<sf::Sprite>::iterator itr = pipes.begin();

    //ground

    std::vector <sf::Sprite> sground;
    std::vector <sf::Sprite>::iterator it = sground.begin();
    Collision::CreateTextureAndBitmask(textures.ground, "./assets/ground1.png");
    Collision::CreateTextureAndBitmask(textures.pausedGround, "./assets/ground1.png");
    sf::Sprite GreenGRound;
    sf::Sprite staticGround;
    GreenGRound.setTexture(textures.ground);
    GreenGRound.setPosition(0, 746);
    GreenGRound.scale(1.2, 1);

    staticGround.setTexture(textures.ground);
    staticGround.setPosition(0, 746);
    staticGround.scale(1.2, 1);
    //control speed of pipes
    sf::Time constanttime = sf::milliseconds(7);
    sf::Time time;
    sf::Clock clock;
    int b = 0; //horizontal distance between pipes (bad declaration name)
    int c = 0; //horizontal distance between ground tiles (bad declaration name)
    int f = 0; //horizontal distance between background (bad declaration name)

     // score 
    int score = 0;
    int sc = 0;
    sf::Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(70);
    score_text.setFillColor(sf::Color::White);
    score_text.move(30, 0);

    // high score  
    sf::Text high_score_text;
    high_score_text.setFont(font);
    high_score_text.setCharacterSize(30);
    high_score_text.setFillColor(sf::Color::White);
    high_score_text.move(30, 70);


    int hs = 0;
    sf::Text zero_hs;
    zero_hs.setFont(font);
    zero_hs.setCharacterSize(30);
    zero_hs.setFillColor(sf::Color::White);
    zero_hs.move(60, 70);

    //game over
    bool drawing = false;

    //coins

    sf::Sprite mycoin;
    std::vector <sf::Sprite> coins;
    std::vector<sf::Sprite>::iterator h = coins.begin();
    Collision::CreateTextureAndBitmask(textures.cointex, "./assets/coin_sprite.png");
    mycoin.setTexture(textures.cointex);
    mycoin.setScale(0.3, 0.3);
    mycoin.setTextureRect(sf::IntRect(20, 30, 300, 310));
    //collecting coins
    bool draw = true;

    //GameOver
    Collision::CreateTextureAndBitmask(textures.GO, "./assets/GAMEOVER2.jpg");
    if (!Collision::CreateTextureAndBitmask(textures.GO, "./assets/GAMEOVER2.jpg"))
        std::cout << "GameOver2 Couldnt load" << std::endl;
    sf::Sprite GOO;
    GOO.setTexture(textures.GO);

    music.setVolume(30);
    music.play();
    //Main game loop:

    while (Game.isOpen())
    {

        textures.bird[0].setSmooth(true);
        textures.bird[1].setSmooth(true);
        textures.bird[2].setSmooth(true);
        textures.bird[3].setSmooth(true);
        sprite.setScale(0.8f, 0.8f);
        delayValue = delayCalc.getElapsedTime();


        // build pipe

        if (b % 1000 == 0 && isFisrtpress)
        {
            b = 0;
            r = rand() % 275 + 50;
            upperpipe.setPosition(1400, r);
            lowerpipe.setPosition(1400, r + 300);
            unrealpipe.setPosition(1430, 0);
            pipes.push_back(upperpipe);
            pipes.push_back(lowerpipe);
            unreal_pipe.push_back(unrealpipe);
        }
        b++;

        // build ground
        if (c % 500 == 0 && isFisrtpress)
        {
            c = 0;
            sground.push_back(GreenGRound);
        }
        c++;

        //move coins
        /*for (h = coins.begin(); h != coins.end(); h++)
        {
            (*h).move(-3, 0);
        }*/
        //clock.restart();

        // move ground 
        for (it = sground.begin(); it != sground.end(); it++)
        {
            (*it).move(-0.5f, 0);
        }
        // move pipe
        time = clock.getElapsedTime();
        if (time >= constanttime && isFisrtpress)
        {
            for (itr = pipes.begin(); itr != pipes.end(); itr++)
            {
                if (Collision::PixelPerfectTest(sprite, *itr)) //detect collosion with pipes
                {
                    drawing = true;
                    hit.play();
                }
                else
                {
                    (*itr).move(-3, 0);
                    //(mycoin).move(-3, 0);
                }
            }
            // move unreal pipe 
            for (iup = unreal_pipe.begin(); iup != unreal_pipe.end(); iup++)
            {
                (*iup).move(-3, 0);
                if (sprite.getGlobalBounds().intersects((*iup).getGlobalBounds()))
                {
                    unphit.play();
                    score++;
                }

            }

            //calculate score + Saving highScore
            int highscore = 0;
            std::ifstream scoreData;
            scoreData.open("./assets/scores.txt");
            if (!scoreData.is_open())
                std::cout << "File coulnd't load";
            else
                scoreData >> highscore;

            scoreData.close();
            std::ofstream dataUpdate("./assets/scores.txt", std::fstream::out);

            if (score > 35)
            {
                sc++;
                score = 0;
            }
            std::stringstream as;
            as << sc;
            score_text.setString(as.str());//Casting int 2 string
            if (sc > highscore)
            {
                highscore = sc;
                dataUpdate << highscore;
                dataUpdate.close();
            }
            else
            dataUpdate << highscore;
            std::stringstream ash;
            ash << highscore;
            high_score_text.setString("Highscore : " + ash.str());
            clock.restart();
        }

        //pause
        pauseValue = waitForPause.getElapsedTime();
        if (!isFisrtpress)
        {
            //Game.draw(sky);
            Game.draw(staticGround);
            Game.draw(sprite); //bird
            Game.draw(screen);
            Game.display();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && pauseValue > pauseTime)
        {
            isFisrtpress = true;
        }

        sf::Event event;
        while (Game.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                Game.close();
                break;
            }
        }

        //detect collision with ground
        if (Collision::PixelPerfectTest(sprite, GreenGRound))
        {
            drawing = true;
            die.play();
        }

        while (Game.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Game.close();
        }

        //jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && velocity.y >= 0.1f /*&& delayValue >= delayHold*/ && isFisrtpress)
        {
            //jump (move up)
            velocity.y = -1.25f;
            jumping = true;
            //flap sound
            flap.play();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            isFisrtpress = false;
        }
        //to exit fullscreen
        /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }*/

        //game state update 
        Rvalue = Rclk.getElapsedTime();
        Vvalue = Vclk.getElapsedTime();
        if (isFisrtpress)
        {
            sprite.move(velocity);
            sprite.setRotation(initialRot);
            if (velocity.y < velocityMax.y && Vvalue >= basicDelay)
            {
                velocity += gravity;
                Vclk.restart();
                if (velocity.y >= 0)
                {
                    jumping = false;
                }
            }
            if (!jumping && initialRot < maxRot && Rvalue >= basicDelay)
            {
                initialRot += accRot;
                Rclk.restart();
                //another methed for rotation
                //instantly reflect the bird
                //more buggy and not smooth
                //rest of it is in the "else" statement
                //not currently used due to implementation of better algorithm
                //can be deleted

                /*if (initialRot < 0)
                {
                    initialRot *= -1;
                }*/
            }
            else if (jumping)
            {
                //rest of old method
                /*if (initialRot > 0)
                {
                    initialRot *= -1;
                }*/

                if (Rvalue >= basicDelay && initialRot > maxRot * -1 && Rvalue >= rapidDelay)
                {
                    initialRot -= accRot;
                    Rclk.restart();
                }

            }

            //animate game
            sprite.setTexture(textures.bird[birdState]);//animate bird
            updateBrd(birdState);
            Game.clear();
            Game.draw(sky);
            Game.draw(sprite); //bird

            if (pipes.size() > 0 && isFisrtpress) //move pipes
            {
                for (itr = pipes.begin(); itr != pipes.end(); itr++)
                {
                    Game.draw(*itr);
                    //Game.draw(mycoin);
                }
            }

            if (isFisrtpress)// move ground 
            {
                for (it = sground.begin(); it != sground.end(); it++)
                {
                    Game.draw(*it);
                }
            }
            else
            {
                Game.draw(staticGround);
            }
            // draw unreal pipe 
            if (isFisrtpress)
            {
                for (iup = unreal_pipe.begin(); iup != unreal_pipe.end(); iup++)
                {
                    Game.draw(*iup);
                }
            }
            // draw score 
            Game.draw(score_text);
            Game.draw(high_score_text);
            Game.draw(zero_hs);

            if (drawing) {

                sf::RenderWindow gameover(sf::VideoMode(1366, 786), "game over", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
                sf::Event ev;
                if (sf::Event::Closed)
                gameover.close();
                void update(sf::Text&);
                void degrade(sf::Text&);
                //Counter to determine the indeces 
                int counterForText = 0;

                //back ground
                sf::Texture GameOver;
                Collision::CreateTextureAndBitmask(GameOver, "./assets/GAMEOVER2.jpg");
                sf::Sprite GOO;
                GOO.setTexture(GameOver);
                sf::Text Title;
                sf::Text text[2];
                for (int i = 0; i < 2; i++) {
                    text[i].setFont(font);
                    text[i].setPosition(175, (i * 125) + 400);
                    text[i].setStyle(sf::Text::Bold | sf::Text::Italic);
                    text[i].setCharacterSize(64);
                    switch (i)
                    {
                    case 0:
                        text[i].setString("Play again ");
                        update(text[i]);
                        break;
                    case 1:
                        text[i].setString("Exit");
                        text[i].setFillColor(sf::Color::White);
                        break;
                    default:
                        break;
                    }
                }
                while (gameover.isOpen())
                {
                    music.stop();
                    //events loop to take input :-
                    while (gameover.pollEvent(ev))
                    {
                        //switch case to determine what to do :-
                        switch (ev.type)
                        {
                            //close button
                        case sf::Event::Closed:
                            Game.close();
                            ~playGame();
                            gameover.close();
                            break;
                            //keyboard buttons actions :-
                        case sf::Event::KeyPressed:
                            if (ev.key.code == sf::Keyboard::Escape)
                                gameover.close();
                            // highlighting the text // 
                            /*applying simple animation to the game*/
                            //down event
                            else if (ev.key.code == sf::Keyboard::Down) {
                                counterForText++;
                                counterForText %= 2;
                                update(text[counterForText]);
                                switch (counterForText)
                                {
                                case 0:
                                    degrade(text[1]);
                                    update(text[0]);
                                    break;
                                case 1:
                                    degrade(text[0]);
                                    update(text[1]);
                                    break;
                                }
                        default:break;
                            }
                            //up event
                            else if (ev.key.code == sf::Keyboard::Up) {
                                if (counterForText == 0)
                                    counterForText = 1;
                                else
                                    counterForText--;
                                counterForText %= 2;
                                update(text[counterForText]);
                                switch (counterForText)
                                {
                                case 0:
                                    degrade(text[1]);
                                    break;
                                case 1:
                                    degrade(text[0]);
                                    break;
                                }
                                break;
                            }
                            //choosing from menu event
                            else if (ev.key.code == sf::Keyboard::Enter) {
                                switch (counterForText)
                                {
                                case 0:
                                    Game.close();
                                    gameover.close();
                                    playGame();
                                    return true;
                                    break;
                                case 1:
                                    Game.close();
                                    gameover.close();
                                    retry = false;
                                    return false;
                                    ~playGame();
                                    break;
                                default:
                                    break;

                                }
                            }
                        }
                    }
                    //Render
                    gameover.clear();
                    gameover.draw(GOO);
                    gameover.draw(Title);
                    for (int i = 0; i < 2; i++)
                        gameover.draw(text[i]);
                    gameover.display();
                }
            }
            if (retry == false)
            {
                break;
            }
            //if (isFisrtpress)
            //{
            //    //draw coins
            //    for (h = coins.begin(); h != coins.end(); h++)
            //    {
            //        window.draw(*h);
            //    }
            //}
            /*if (drawing)
            {
                window.draw(GO);
            }*/
            Game.display();
        }
    }
    return retry;
}

inline bool playGameHard()
{

    int sky1 = 0;
    bool retry = true;
    //intialize window dimenstions 
    const int width = 1366;
    const int windowlength = 786;

    //declaring or initializing variables 

    //before game start & pause
    sf::Text screen;
    sf::Font font;
    font.loadFromFile("./assets/flp.ttf");
    screen.setFont(font);
    screen.setString("Press space to play / continue");
    screen.setCharacterSize(60);
    screen.setFillColor(sf::Color::Yellow);
    screen.setOutlineColor(sf::Color(0, 50, 255, 100));
    screen.setPosition(270, 100);

    //pause control & input correction
    bool isFisrtpress = false;
    sf::Time delayValue;
    sf::Time pauseValue;
    //sf::Time delayHold = sf::seconds(1);
    sf::Time  pauseTime = sf::seconds(1);
    sf::Clock delayCalc;
    sf::Clock waitForPause;

    //declaration of physics variables
    sf::Vector2f gravity(0, 0.3f);  //fall & jump controls 
    sf::Vector2f velocity(0, 0.1f);
    sf::Vector2f velocity1(0, 0.1f);
    sf::Vector2f velocityMax(0, 1.6f);

    float initialRot = 5.f; //Rotation controls
    float maxRot = 30.f;
    float accRot = 4.5f;
    sf::Clock Vclk;
    sf::Time Vvalue;
    sf::Clock Rclk;
    sf::Time Rvalue;
    sf::Time basicDelay = sf::milliseconds(70);
    sf::Time rapidDelay = sf::milliseconds(20);
    bool jumping = false;

    //sounds
    sf::SoundBuffer flapBuffer;
    sf::SoundBuffer hitBuffer;
    sf::SoundBuffer dieBuffer;
    flapBuffer.loadFromFile("./assets/sfx_wing.wav");
    hitBuffer.loadFromFile("./assets/sfx_hit.wav");
    dieBuffer.loadFromFile("./assets/sfx_die.wav");
    sf::Sound flap;
    sf::Sound hit;
    sf::Sound die;
    flap.setBuffer(flapBuffer);
    hit.setBuffer(hitBuffer);
    die.setBuffer(dieBuffer);
    sf::Music music;
    music.openFromFile("./assets/hmusic.ogg");

    //Window drawing and title
    sf::RenderWindow Game(sf::VideoMode(width, windowlength), "Flappy bird");
    //for fullscreen
    //window(sf::VideoMode::getDesktopMode(), "Flappy bird Fullsceen", sf::Style::Fullscreen); 

    //loading bird textures
    //<<<<<<<<<<ARRAY>>>>>>>>>>
    sf::Texture bird[4];
    Collision::CreateTextureAndBitmask(bird[0], "./assets/21.png");
    Collision::CreateTextureAndBitmask(bird[1], "./assets/22.png");
    Collision::CreateTextureAndBitmask(bird[2], "./assets/23.png");
    Collision::CreateTextureAndBitmask(bird[3], "./assets/24.png");

    //bird character
    int birdState = 0;
    sf::Sprite sprite;
    sprite.setTexture(bird[birdState]);
    float x = width / 3, y = windowlength / 1.5;
    sprite.setPosition(x, y);
    sprite.setOrigin(64, 45);

    // back ground 
    sf::Texture bg;
    Collision::CreateTextureAndBitmask(bg, "./assets/impossible1.png");
    sf::Sprite sky;
    sky.setTexture(bg);

    // pipe
    sf::Texture pipe;
    Collision::CreateTextureAndBitmask(pipe, "./assets/pipe2.png");
    sf::Sprite lowerpipe;
    lowerpipe.setTexture(pipe);
    lowerpipe.setScale(2.2, 1.5);
    sf::Sprite upperpipe;
    upperpipe.setTexture(pipe);
    upperpipe.scale(2.2, -1.5);

    // unreal pipe (for score)
    sf::Texture unpipe;
    Collision::CreateTextureAndBitmask(unpipe, "./assets/Empty.png");
    sf::Sprite unrealpipe;
    unrealpipe.setTexture(unpipe);
    unrealpipe.scale(0.00001, 1.5);
    std::vector <sf::Sprite > unreal_pipe;
    std::vector <sf::Sprite >::iterator iup = unreal_pipe.begin();
    sf::SoundBuffer unphitbuffer;
    unphitbuffer.loadFromFile("./assets/unphit.ogg");
    sf::Sound unphit;
    unphit.setBuffer(unphitbuffer);
    int r; //verical distance between pipes (bad declaration name)
    std::vector <sf::Sprite> pipes;
    std::vector<sf::Sprite>::iterator itr = pipes.begin();

    //ground

    std::vector <sf::Sprite> sground;
    std::vector <sf::Sprite>::iterator it = sground.begin();
    sf::Texture ground;
    sf::Texture pausedGround;
    Collision::CreateTextureAndBitmask(ground, "./assets/ground1.png");
    Collision::CreateTextureAndBitmask(pausedGround, "./assets/ground1.png");
    sf::Sprite GreenGRound;
    sf::Sprite staticGround;
    GreenGRound.setTexture(ground);
    GreenGRound.setPosition(0, 746);
    GreenGRound.scale(1.2, 1);

    staticGround.setTexture(ground);
    staticGround.setPosition(0, 746);
    staticGround.scale(1.2, 1);

    //control speed of pipes
    sf::Time constanttime = sf::milliseconds(7);
    sf::Time time;
    sf::Clock clock;
    int b = 0; //horizontal distance between pipes (bad declaration name)
    int c = 0; //horizontal distance between ground (bad declaration name)
    int f = 0; //horizontal distance between background (bad declaration name)

     // score 
    int score = 0;
    int sc = 0;
    sf::Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(70);
    score_text.setFillColor(sf::Color::White);
    score_text.move(30, 0);

    // high score  
    sf::Text high_score_text;
    high_score_text.setFont(font);
    high_score_text.setCharacterSize(30);
    high_score_text.setFillColor(sf::Color::White);
    high_score_text.move(30, 70);


    int hs = 0;
    sf::Text zero_hs;
    zero_hs.setFont(font);
    zero_hs.setCharacterSize(30);
    zero_hs.setFillColor(sf::Color::White);
    zero_hs.move(60, 70);

    //game over
    bool drawing = false;

    //coins
    sf::Texture cointex;
    sf::Sprite mycoin;
    std::vector <sf::Sprite> coins;
    std::vector<sf::Sprite>::iterator h = coins.begin();
    Collision::CreateTextureAndBitmask(cointex, "./assets/coin_sprite.png");
    mycoin.setTexture(cointex);
    mycoin.setScale(0.3, 0.3);
    mycoin.setTextureRect(sf::IntRect(20, 30, 300, 310));
    //collecting coins
    bool draw = true;

    //GameOver
    sf::Texture GO;
    Collision::CreateTextureAndBitmask(GO, "./assets/GAMEOVER2.jpg");
    if (!Collision::CreateTextureAndBitmask(GO, "./assets/GAMEOVER2.jpg"))
        std::cout << "GameOver Img Failed to Load" << std::endl;
    sf::Sprite GOO;
    GOO.setTexture(GO);

    //play music
    music.setVolume(30);
    music.play();
    //Main game loop:
    while (Game.isOpen())
    {

        bird[0].setSmooth(true);
        bird[1].setSmooth(true);
        bird[2].setSmooth(true);
        bird[3].setSmooth(true);
        sprite.setScale(0.8f, 0.8f);
        delayValue = delayCalc.getElapsedTime();


        // build pipe

        if (b % 1000 == 0 && isFisrtpress)
        {
            b = 0;
            r = rand() % 275 + 50;
            upperpipe.setPosition(1400, r);
            lowerpipe.setPosition(1400, r + 300);
            unrealpipe.setPosition(1430, 0);
            pipes.push_back(upperpipe);
            pipes.push_back(lowerpipe);
            unreal_pipe.push_back(unrealpipe);
        }
        b++;

        // build ground
        if (c % 500 == 0 && isFisrtpress)
        {
            c = 0;
            sground.push_back(GreenGRound);
        }
        c++;

        //build background

        /* for (it = sground.begin(); it != sground.end(); it++)
         {
             (*it).move(-0.5f, 0);
         }*/



         //move coins
         /*for (h = coins.begin(); h != coins.end(); h++)
         {
             (*h).move(-3, 0);
         }*/
         //clock.restart();

         // move ground 
        for (it = sground.begin(); it != sground.end(); it++)
        {
            (*it).move(-0.5f, 0);
        }
        // move pipe
        //Making/OpeningFile
        int highscore = 0;
        std::ifstream Input;
        Input.open("./assets/text2.txt");
        Input >> highscore;
        Input.close();
        time = clock.getElapsedTime();
        if (time >= constanttime && isFisrtpress)
        {
            for (itr = pipes.begin(); itr != pipes.end(); itr++)
            {
                if (Collision::PixelPerfectTest(sprite, *itr)) //detect collosion with pipes
                {
                    std::ofstream Output("./assets/text2.txt", std::fstream::out);
                    Output << highscore;
                    Output.close();
                    drawing = true;
                    hit.play();
                }
                else
                {
                    (*itr).move(-3.7, 0);
                    //(mycoin).move(-3, 0);
                }
            }
            // move unreal pipe 
            for (iup = unreal_pipe.begin(); iup != unreal_pipe.end(); iup++)
            {
                (*iup).move(-3, 0);
                if (sprite.getGlobalBounds().intersects((*iup).getGlobalBounds()))
                {
                    unphit.play();
                    score++;
                }

            }

           //calculate score + Saving highScore
            int highscore = 0;
            std::ifstream scoreData;
            scoreData.open("./assets/scores1.txt");
            if (!scoreData.is_open())
                std::cout << "File coulnd't load";
            else
                scoreData >> highscore;

            scoreData.close();
            std::ofstream dataUpdate("./assets/scores1.txt", std::fstream::out);

            if (score > 35)
            {
                sc++;
                score = 0;
            }
            std::stringstream as;
            as << sc;
            score_text.setString(as.str());//Casting int 2 string
            if (sc > highscore)
            {
                highscore = sc;
                dataUpdate << highscore;
                dataUpdate.close();

            }
            else
                dataUpdate << highscore;
            std::stringstream ash;
            ash << highscore;
            high_score_text.setString("Highscore : " + ash.str());
            clock.restart();
        }

        //pause
        pauseValue = waitForPause.getElapsedTime();
        /*if (sky1 == 0 && !isFisrtpress)
        {
            Game.draw(sky);
            sky1++;
        }*/
        if (!isFisrtpress )
        {
            Game.draw(staticGround);
            Game.draw(sprite); //bird
            Game.draw(screen);
            Game.display();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && pauseValue > pauseTime)
        {
            isFisrtpress = true;
        }


        sf::Event event;
        while (Game.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                Game.close();
                break;
            }
        }

        //detect collision with ground
        if (Collision::PixelPerfectTest(sprite, GreenGRound))

        {
            drawing = true;
            die.play();
        }

        while (Game.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Game.close();
        }

        //jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && velocity.y >= 0.1f /*&& delayValue >= delayHold*/ && isFisrtpress)
        {
            //jump (move up)
            velocity.y = -1.25f;
            jumping = true;
            //flap sound
            flap.play();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            isFisrtpress = false;
        }
        //to exit fullscreen
        /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }*/

        //game state update 
        Rvalue = Rclk.getElapsedTime();
        Vvalue = Vclk.getElapsedTime();
        if (isFisrtpress)
        {
            sprite.move(velocity);
            sprite.setRotation(initialRot);
            if (velocity.y < velocityMax.y && Vvalue >= basicDelay)
            {
                velocity += gravity;
                Vclk.restart();
                if (velocity.y >= 0)
                {
                    jumping = false;
                }
            }
            if (!jumping && initialRot < maxRot && Rvalue >= basicDelay)
            {
                initialRot += accRot;
                Rclk.restart();
                //another methed for rotation
                //instantly reflect the bird
                //more buggy and not smooth
                //rest of it is in the "else" statement
                //not currently used due to implementation of better algorithm
                //can be deleted

                /*if (initialRot < 0)
                {
                    initialRot *= -1;
                }*/
            }
            else if (jumping)
            {
                //rest of old method
                /*if (initialRot > 0)
                {
                    initialRot *= -1;
                }*/

                if (Rvalue >= basicDelay && initialRot > maxRot * -1 && Rvalue >= rapidDelay)
                {
                    initialRot -= accRot;
                    Rclk.restart();
                }

            }

            //animate game

            sprite.setTexture(bird[birdState]);//animate bird

            updateBrd(birdState);
            Game.clear();

            if (isFisrtpress)
            {
                Game.draw(sky);
            }


            Game.draw(sprite); //bird

            if (pipes.size() > 0 && isFisrtpress) //move pipes
            {
                for (itr = pipes.begin(); itr != pipes.end(); itr++)
                {
                    Game.draw(*itr);
                    //Game.draw(mycoin);
                }
            }

            if (isFisrtpress)// move ground 
            {
                for (it = sground.begin(); it != sground.end(); it++)
                {
                    Game.draw(*it);
                }
            }
            else
            {
                Game.draw(staticGround);
            }
            // draw unreal pipe 
            if (isFisrtpress)
            {
                for (iup = unreal_pipe.begin(); iup != unreal_pipe.end(); iup++)
                {
                    Game.draw(*iup);
                }
            }
            // draw score 

            Game.draw(score_text);
            Game.draw(high_score_text);
            Game.draw(zero_hs);

            if (drawing) {

                sf::RenderWindow gameover(sf::VideoMode(1366, 786), "game over", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
                sf::Event ev;
                if (sf::Event::Closed)
                    gameover.close();
                void update(sf::Text&);
                void degrade(sf::Text&);
                //Counter to determine the indeces 
                int counterForText = 0;

                //back ground
                sf::Sprite GOO;
                GOO.setTexture(GO);
                sf::Text Title;
                sf::Text text[2];
                for (int i = 0; i < 2; i++) {
                    text[i].setFont(font);
                    text[i].setPosition(175, (i * 125) + 400);
                    text[i].setStyle(sf::Text::Bold | sf::Text::Italic);
                    text[i].setCharacterSize(64);
                    switch (i)
                    {
                    case 0:
                        text[i].setString("Play again ");
                        update(text[i]);
                        break;
                    case 1:
                        text[i].setString("Exit");
                        text[i].setFillColor(sf::Color::Black);
                        break;
                    default:
                        break;
                    }
                }
                while (gameover.isOpen())
                {
                    music.stop();
                    //events loop to take input :-
                    while (gameover.pollEvent(ev))
                    {
                        //switch case to determine what to do :-
                        switch (ev.type)
                        {
                            //close button
                        case sf::Event::Closed:
                            Game.close();
                            gameover.close();
                            break;
                            //keyboard buttons actions :-
                        case sf::Event::KeyPressed:
                            if (ev.key.code == sf::Keyboard::Escape)
                            {
                                gameover.close();
                                Game.close();
                            }
                            // highlighting the text // 
                            /*applying simple animation to the game*/
                            //down event

                            else if (ev.key.code == sf::Keyboard::Down)
                            {
                                counterForText++;
                                counterForText %= 2;
                                update(text[counterForText]);
                                switch (counterForText)
                                {
                                case 0:
                                    degrade(text[1]);
                                    update(text[0]);
                                    break;
                                case 1:
                                    degrade(text[0]);
                                    update(text[1]);
                                    break;
                                }
                        default:break;
                            }
                            //up event
                            else if (ev.key.code == sf::Keyboard::Up) {
                                if (counterForText == 0)
                                    counterForText = 1;
                                else
                                    counterForText--;
                                counterForText %= 2;
                                update(text[counterForText]);
                                switch (counterForText)
                                {
                                case 0:
                                    degrade(text[1]);
                                    break;
                                case 1:
                                    degrade(text[0]);
                                    break;
                                }
                                break;
                            }
                            //choosing from menu event
                            else if (ev.key.code == sf::Keyboard::Enter) {
                                switch (counterForText)
                                {
                                case 0:
                                    Game.close();
                                    gameover.close();
                                    playGameHard();
                                    return true;
                                    break;
                                case 1:
                                    Game.close();
                                    gameover.close();
                                    retry = false;
                                    return false;
                                    break;
                                default:
                                    break;

                                }
                            }
                        }
                    }
                    //Render
                    gameover.clear();
                    gameover.draw(GOO);
                    gameover.draw(Title);
                    for (int i = 0; i < 2; i++)
                        gameover.draw(text[i]);
                    gameover.display ();
                }
            }
            if (retry == false)
            {
                break;
            }
            //if (isFisrtpress)
            //{
            //    //draw coins
            //    for (h = coins.begin(); h != coins.end(); h++)
            //    {
            //        window.draw(*h);
            //    }
            //}
            Game.display();
        }
    }
    return retry;
}