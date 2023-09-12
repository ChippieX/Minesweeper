#include <iostream>
#include <time.h>
#include <string.h>
#include <SFML/Graphics.hpp>

void textStringUpdate(sf::Text&, std::string);
void textStringUpdate(sf::Text&, std::string, int);
void resetGrid(int[][30]);
void resetGrid(bool[][30]);
void randGrid(int[][30], int);
void labelGrid(int[][30], int);
void revealTiles(int[][30], bool[][30], int, int);
bool buttonPressed(sf::RectangleShape, sf::RenderWindow&);
bool notOutOfBounds(int[][30], int, int, int, int);
bool gameWin(int[][30], bool[][30], int);

int main()
{
    bool showTile[16][30], hideFlag[16][30], leftMouseHeld = false,
        rightMouseHeld = false, firstClick = true, showGame = false,
        gameOver = false, startSetup = false;
    int minefield[16][30], difficulty = 3;
    float fade = 0;
    const float THE_NUM = 505 / 16.0;
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Minesweeper");
    sf::RectangleShape button, menuButton[3];
    sf::Font heavitas;
    sf::Text mineNumber, endText, restartText, menuText, menuButtonText[3];
    sf::Texture flag_image, mine_image;
    sf::Sprite flag, mine;
    srand(time(0));

    window.setFramerateLimit(30);

    if (!heavitas.loadFromFile("Heavitas.ttf")) {}

    menuText.setFont(heavitas);
    textStringUpdate(menuText, "Minesweeper", 75);
    menuText.setPosition(window.getSize().x / 2 - 3, 100);
    
    for (int i = 0; i < 3; i++)
    {
        menuButton[i].setSize(sf::Vector2f(500, 100));
        menuButton[i].setOrigin(menuButton[i].getSize().x / 2,
            menuButton[i].getSize().y / 2);
        menuButton[i].setFillColor(sf::Color(255, 255, 255, 75));

        menuButtonText[i].setFont(heavitas);
        menuButtonText[i].setCharacterSize(50);
    }

    menuButton[0].setPosition(window.getSize().x / 2, window.getSize().y / 2 - 100);
    textStringUpdate(menuButtonText[0], "beginner");
    menuButton[1].setPosition(window.getSize().x / 2, window.getSize().y / 2 + 50);
    textStringUpdate(menuButtonText[1], "intermediate");
    menuButton[2].setPosition(window.getSize().x / 2, window.getSize().y / 2 + 200);
    textStringUpdate(menuButtonText[2], "advanced");

    for (int i = 0; i < 3; i++)
    {
        menuButtonText[i].setPosition(menuButton[i].getPosition().x - 3,
            menuButton[i].getPosition().y - 10);
    }

    endText.setFont(heavitas);
    endText.setCharacterSize(50);
    endText.setFillColor(sf::Color::White);

    restartText.setFont(heavitas);
    textStringUpdate(restartText, "Press \"R\" to play again.", 50);
    restartText.setPosition(window.getSize().x / 2, window.getSize().y - 50);
    restartText.setFillColor(sf::Color::White);

    resetGrid(minefield);
    resetGrid(showTile);
    resetGrid(hideFlag);

    //game loop begins
    while (window.isOpen())
    {
        //closing the window
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();       
        }
        
        if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            leftMouseHeld = false;

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
            rightMouseHeld = false;

        //main menu before game
        if (!showGame && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for (int i = 0; i < 3; i++)
            {
                if (buttonPressed(menuButton[i], window))
                {
                    difficulty = i + 1;
                    showGame = true;
                    startSetup = true;
                }
            }
        }

        //finishing setup after difficulty has been decided
        if(startSetup)
        {
            button.setFillColor(sf::Color::Cyan);
            switch (difficulty)
            {
            case 1:
                button.setSize(sf::Vector2f(60, 60));
                button.setOrigin(30, 30);
                break;
            case 2:
            case 3:
                button.setSize(sf::Vector2f(THE_NUM, THE_NUM));
                button.setOrigin(THE_NUM / 2, THE_NUM / 2);
                break;
            default:
                break;
            }

            mineNumber.setFont(heavitas);
            switch (difficulty)
            {
            case 1:
                mineNumber.setCharacterSize(50);
                break;
            case 2:
            case 3:
                mineNumber.setCharacterSize(30);
                break;
            default:
                break;
            }
            mineNumber.setFillColor(sf::Color::White);

            if (!flag_image.loadFromFile("flag.png")) {};
            flag_image.setSmooth(true);
            flag.setTexture(flag_image);
            switch (difficulty)
            {
            case 1:
                flag.setScale(0.07, 0.07);
                break;
            case 2:
            case 3:
                flag.setScale(0.04, 0.04);
                break;
            default:
                break;
            }
            flag.setOrigin(flag.getLocalBounds().width / 2, flag.getLocalBounds().height / 2);

            if (!mine_image.loadFromFile("mine.png")) {};
            mine_image.setSmooth(true);
            mine.setTexture(mine_image);
            switch (difficulty)
            {
            case 1:
                mine.setScale(0.1, 0.1);
                break;
            case 2:
            case 3:
                mine.setScale(0.06, 0.06);
                break;
            default:
                break;
            }
            mine.setOrigin(mine.getLocalBounds().width / 2, mine.getLocalBounds().height);

            startSetup = false;
        }

        //interacting with tiles
        if (showGame && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !leftMouseHeld && !gameOver)
        {
            switch (difficulty)
            {
            case 1:
                for (int i = 0; i < 81; i++)
                {
                    if (showTile[i / 9][i % 9])
                    {
                        button.setPosition(window.getSize().x / 2.0 + (i / 9 - 4) * 65,
                            window.getSize().y / 2.0 + (i % 9 - 4) * 65);
                        if (buttonPressed(button, window) && hideFlag[i / 9][i % 9])
                        {
                            if (firstClick)
                            {
                                do
                                {
                                    resetGrid(minefield);
                                    randGrid(minefield, difficulty);
                                    labelGrid(minefield, difficulty);
                                } while (minefield[i / 9][i % 9] != 0);

                                firstClick = false;
                            }

                            showTile[i / 9][i % 9] = false;

                            if (minefield[i / 9][i % 9] == 0)
                            {
                                revealTiles(minefield, showTile, difficulty, i);

                                //fixing flags after 0 reveal
                                for (int i = 0; i < 81; i++)
                                {
                                    if (!showTile[i / 9][i % 9] && !hideFlag[i / 9][i % 9])
                                    {
                                        hideFlag[i / 9][i % 9] = true;
                                    }
                                }
                            }
                            else if (minefield[i / 9][i % 9] == 9)
                            {
                                mineNumber.setFillColor(sf::Color::Red);
                                button.setFillColor(sf::Color::Red);
                                endText.setFillColor(sf::Color::Red);
                                restartText.setFillColor(sf::Color::Red);
                                textStringUpdate(endText, "You lose...");
                                endText.setPosition(window.getSize().x / 2, 25);
                                gameOver = true;
                            }
                        }
                    }
                }
                break;
            case 2:
                for (int i = 0; i < 256; i++)
                {
                    if (showTile[i / 16][i % 16])
                    {
                        button.setPosition(window.getSize().x / 2 - 290 + THE_NUM / 2 + (i / 16 * (THE_NUM + 5)),
                            window.getSize().y / 2 - 290 + THE_NUM / 2 + (i % 16 * (THE_NUM + 5)));
                        if (buttonPressed(button, window) && hideFlag[i / 16][i % 16])
                        {
                            if (firstClick)
                            {
                                do
                                {
                                    resetGrid(minefield);
                                    randGrid(minefield, difficulty);
                                    labelGrid(minefield, difficulty);
                                } while (minefield[i / 16][i % 16] != 0);

                                firstClick = false;
                            }

                            showTile[i / 16][i % 16] = false;

                            if (minefield[i / 16][i % 16] == 0)
                            {
                                revealTiles(minefield, showTile, difficulty, i);

                                //fixing flags after 0 reveal
                                for (int i = 0; i < 256; i++)
                                {
                                    if (!showTile[i / 16][i % 16] && !hideFlag[i / 16][i % 16])
                                    {
                                        hideFlag[i / 16][i % 16] = true;
                                    }
                                }
                            }
                            else if (minefield[i / 16][i % 16] == 9)
                            {
                                mineNumber.setFillColor(sf::Color::Red);
                                button.setFillColor(sf::Color::Red);
                                endText.setFillColor(sf::Color::Red);
                                restartText.setFillColor(sf::Color::Red);
                                textStringUpdate(endText, "You lose...");
                                endText.setPosition(window.getSize().x / 2, 25);
                                gameOver = true;
                            }
                        }
                    }
                }
                break;
            case 3:
                for (int i = 0; i < 480; i++)
                {
                    if (showTile[i / 30][i % 30])
                    {
                        button.setPosition(window.getSize().x / 2 - 546 + THE_NUM / 2 + (i % 30 * (THE_NUM + 5)),
                            window.getSize().y / 2 - 290 + THE_NUM / 2 + (i / 30 * (THE_NUM + 5)));
                        if (buttonPressed(button, window) && hideFlag[i / 30][i % 30])
                        {
                            if (firstClick)
                            {
                                do
                                {
                                    resetGrid(minefield);
                                    randGrid(minefield, difficulty);
                                    labelGrid(minefield, difficulty);
                                } while (minefield[i / 30][i % 30] != 0);

                                firstClick = false;
                            }

                            showTile[i / 30][i % 30] = false;

                            if (minefield[i / 30][i % 30] == 0)
                            {
                                revealTiles(minefield, showTile, difficulty, i);

                                //fixing flags after 0 reveal
                                for (int i = 0; i < 256; i++)
                                {
                                    if (!showTile[i / 30][i % 30] && !hideFlag[i / 30][i % 30])
                                    {
                                        hideFlag[i / 30][i % 30] = true;
                                    }
                                }
                            }
                            else if (minefield[i / 30][i % 30] == 9)
                            {
                                mineNumber.setFillColor(sf::Color::Red);
                                button.setFillColor(sf::Color::Red);
                                endText.setFillColor(sf::Color::Red);
                                restartText.setFillColor(sf::Color::Red);
                                textStringUpdate(endText, "You lose...");
                                endText.setPosition(window.getSize().x / 2, 25);
                                gameOver = true;
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }

            leftMouseHeld = true;
        }

        //placing flags on tiles
        if (showGame && sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rightMouseHeld && !gameOver)
        {
            switch (difficulty)
            {
            case 1:
                for (int i = 0; i < 81; i++)
                {
                    if (showTile[i / 9][i % 9])
                    {
                        button.setPosition(window.getSize().x / 2.0 + (i / 9 - 4) * 65,
                            window.getSize().y / 2.0 + (i % 9 - 4) * 65);
                        if (buttonPressed(button, window))
                        {
                            if (hideFlag[i / 9][i % 9])
                            {
                                hideFlag[i / 9][i % 9] = false;
                            }
                            else
                            {
                                hideFlag[i / 9][i % 9] = true;
                            }
                        }
                    }
                }
                break;
            case 2:
                for (int i = 0; i < 256; i++)
                {
                    if (showTile[i / 16][i % 16])
                    {
                        button.setPosition(window.getSize().x / 2 - 290 + THE_NUM / 2 + (i / 16 * (THE_NUM + 5)),
                            window.getSize().y / 2 - 290 + THE_NUM / 2 + (i % 16 * (THE_NUM + 5)));
                        if (buttonPressed(button, window))
                        {
                            if (hideFlag[i / 16][i % 16])
                            {
                                hideFlag[i / 16][i % 16] = false;
                            }
                            else
                            {
                                hideFlag[i / 16][i % 16] = true;
                            }
                        }
                    }
                }
                break;
            case 3:
                for (int i = 0; i < 480; i++)
                {
                    if (showTile[i / 30][i % 30])
                    {
                        button.setPosition(window.getSize().x / 2 - 546 + THE_NUM / 2 + (i % 30 * (THE_NUM + 5)),
                            window.getSize().y / 2 - 290 + THE_NUM / 2 + (i / 30 * (THE_NUM + 5)));
                        if (buttonPressed(button, window))
                        {
                            if (hideFlag[i / 30][i % 30])
                            {
                                hideFlag[i / 30][i % 30] = false;
                            }
                            else
                            {
                                hideFlag[i / 30][i % 30] = true;
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
            
            rightMouseHeld = true;
        }

        //making sure you actually enjoy your victory
        if (showGame && gameWin(minefield, showTile, difficulty))
        {
            mineNumber.setFillColor(sf::Color::Green);
            button.setFillColor(sf::Color::Green);
            endText.setFillColor(sf::Color::Green);
            restartText.setFillColor(sf::Color::Green);
            textStringUpdate(endText, "You win!");
            endText.setPosition(window.getSize().x / 2, 25);
            gameOver = true;
        }

        //reseting after a game
        if (showGame && gameOver && !leftMouseHeld && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            button.setFillColor(sf::Color::Cyan);
            mineNumber.setFillColor(sf::Color::White);
            endText.setFillColor(sf::Color::White);
            restartText.setFillColor(sf::Color::White);
            resetGrid(showTile);
            resetGrid(hideFlag);
            resetGrid(minefield);
            fade = 0;
            firstClick = true;
            gameOver = false;
        }

        //displaying to the window
        window.clear();
        if (!showGame)
        {
            window.draw(menuText);
            for (int i = 0; i < 3; i++)
            {
                window.draw(menuButton[i]);
                window.draw(menuButtonText[i]);
            }
        }
        switch (difficulty)
        {
        case 1:
            for (int i = 0; i < 81; i++)
            {
                button.setPosition(window.getSize().x / 2.0 + (i / 9 - 4) * 65,
                    window.getSize().y / 2.0 + (i % 9 - 4) * 65);
                
                textStringUpdate(mineNumber, std::to_string(minefield[i / 9][i % 9]));
                mineNumber.setPosition(button.getPosition().x - 1, button.getPosition().y - 13);
                
                if (minefield[i / 9][i % 9] != 9)
                {
                    window.draw(mineNumber);
                }
                
                if (showTile[i / 9][i % 9])
                {
                    window.draw(button);
                }

                flag.setPosition(window.getSize().x / 2.0 + (i / 9 - 4) * 65,
                    window.getSize().y / 2.0 + (i % 9 - 4) * 65);

                if (!hideFlag[i / 9][i % 9])
                {
                    window.draw(flag);
                }
                
                if (minefield[i / 9][i % 9] == 9)
                {
                    mine.setPosition(button.getPosition().x + 3, button.getPosition().y + 28);

                    if (button.getFillColor() == sf::Color::Green)
                    {
                        if (fade < 255)
                        {
                            fade += 2.5;
                            mine.setColor(sf::Color(255, 255, 255, fade));
                        }

                        window.draw(mine);
                    }
                    else if (button.getFillColor() == sf::Color::Red)
                    {
                        window.draw(mine);
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; i < 256; i++)
            {
                button.setPosition(window.getSize().x / 2 - 290 + THE_NUM / 2 + (i / 16 * (THE_NUM + 5)),
                    window.getSize().y / 2 - 290 + THE_NUM / 2 + (i % 16 * (THE_NUM + 5)));

                textStringUpdate(mineNumber, std::to_string(minefield[i / 16][i % 16]));
                mineNumber.setPosition(button.getPosition().x - 1, button.getPosition().y - 8);
                
                if (minefield[i / 16][i % 16] != 9)
                {
                    window.draw(mineNumber);
                }

                if (showTile[i / 16][i % 16])
                {
                    window.draw(button);
                }

                flag.setPosition(button.getPosition().x, button.getPosition().y);

                if (!hideFlag[i / 16][i % 16])
                {
                    window.draw(flag);
                }

                if (minefield[i / 16][i % 16] == 9)
                {
                    mine.setPosition(button.getPosition().x + 1, button.getPosition().y + 17);

                    if (button.getFillColor() == sf::Color::Green)
                    {
                        if (fade < 255)
                        {
                            fade += 2.5;
                            mine.setColor(sf::Color(255, 255, 255, fade));
                        }

                        window.draw(mine);
                    }
                    else if (button.getFillColor() == sf::Color::Red)
                    {
                        window.draw(mine);
                    }
                }
            }
            break;
        case 3:
            for (int i = 0; i < 480; i++)
            {
                button.setPosition(window.getSize().x / 2 - 546 + THE_NUM / 2 + (i % 30 * (THE_NUM + 5)),
                    window.getSize().y / 2 - 290 + THE_NUM / 2 + (i / 30 * (THE_NUM + 5)));

                textStringUpdate(mineNumber, std::to_string(minefield[i / 30][i % 30]));
                mineNumber.setPosition(button.getPosition().x - 1, button.getPosition().y - 8);

                if (minefield[i / 30][i % 30] != 9)
                {
                    window.draw(mineNumber);
                }

                if (showTile[i / 30][i % 30])
                {
                    window.draw(button);
                }

                flag.setPosition(button.getPosition().x, button.getPosition().y);

                if (!hideFlag[i / 30][i % 30])
                {
                    window.draw(flag);
                }

                if (minefield[i / 30][i % 30] == 9)
                {
                    mine.setPosition(button.getPosition().x + 1, button.getPosition().y + 17);

                    if (button.getFillColor() == sf::Color::Green)
                    {
                        if (fade < 255)
                        {
                            fade += 2.5;
                            mine.setColor(sf::Color(255, 255, 255, fade));
                        }

                        window.draw(mine);
                    }
                    else if (button.getFillColor() == sf::Color::Red)
                    {
                        window.draw(mine);
                    }
                }
            }
            break;
        default:
            break;
        }
        if (gameOver)
        {
            window.draw(endText);
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}


//automatic readjustment for string changes in text
void textStringUpdate(sf::Text& text, std::string string)
{
    text.setString(string);
    text.setOrigin(text.getLocalBounds().width / 2.0,
        text.getLocalBounds().height / 2.0);
}
void textStringUpdate(sf::Text& text, std::string string, int size)
{
    text.setCharacterSize(size);
    text.setString(string);
    text.setOrigin(text.getLocalBounds().width / 2.0,
        text.getLocalBounds().height / 2.0);
}

//resets grids to their default values
void resetGrid(int minefield[][30])
{
    for (int i = 0; i < 480; i++)
        minefield[i / 30][i % 30] = 0;
}
void resetGrid(bool showTile[][30])
{
    for (int i = 0; i < 480; i++)
        showTile[i / 30][i % 30] = true;
}

//places random mines on grid based on difficulty
void randGrid(int minefield[][30], int difficulty)
{
    int coords[2];

    //mines are nines since 9 cant show up on a minesweeper grid
    switch (difficulty)
    {
    case 1:
        for (int i = 0; i < 10; i++)
        {
            do
            {
                coords[0] = rand() % 9;
                coords[1] = rand() % 9;
            } while (minefield[coords[0]][coords[1]] == 9);

            minefield[coords[0]][coords[1]] = 9;
        }
        break;
    case 2:
        for (int i = 0; i < 40; i++)
        {
            do
            {
                coords[0] = rand() % 16;
                coords[1] = rand() % 16;
            } while (minefield[coords[0]][coords[1]] == 9);

            minefield[coords[0]][coords[1]] = 9;
        }
        break;
    case 3:
        for (int i = 0; i < 99; i++)
        {
            do
            {
                coords[0] = rand() % 16;
                coords[1] = rand() % 30;
            } while (minefield[coords[0]][coords[1]] == 9);

            minefield[coords[0]][coords[1]] = 9;
        }
        break;
    default:
        break;
    }
}

//gives grid spaces numbers depending on the number of mines surrounding the space
void labelGrid(int minefield[][30], int difficulty)
{
    //mines are nines since 9 cant show up on a minesweeper grid
    switch (difficulty)
    {
    case 1:
        for (int i = 0; i < 81; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                for (int k = -1; k <= 1; k++)
                {
                    if ((j != 0 || k != 0) && minefield[i / 9][i % 9] != 9
                        && notOutOfBounds(minefield, difficulty, i, j , k)
                        && minefield[i / 9 + j][i % 9 + k] == 9)
                    {
                        minefield[i / 9][i % 9]++;
                    }
                }
            }
        }
        break;
    case 2:
        for (int i = 0; i < 256; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                for (int k = -1; k <= 1; k++)
                {
                    if ((j != 0 || k != 0) && minefield[i / 16][i % 16] != 9
                        && notOutOfBounds(minefield, difficulty, i, j, k)
                        && minefield[i / 16 + j][i % 16 + k] == 9)
                    {
                        minefield[i / 16][i % 16]++;
                    }
                }
            }
        }
        break;
    case 3:
        for (int i = 0; i < 480; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                for (int k = -1; k <= 1; k++)
                {
                    if ((j != 0 || k != 0) && minefield[i / 30][i % 30] != 9
                        && notOutOfBounds(minefield, difficulty, i, j, k)
                        && minefield[i / 30 + j][i % 30 + k] == 9)
                    {
                        minefield[i / 30][i % 30]++;
                    }
                }
            }
        }
        break;
    default:
        break;
    }
}

//reveals surrounding tiles when a 0 tile is revealed
void revealTiles(int minefield[][30], bool showTile[][30], int difficulty, int i)
{
    switch (difficulty)
    {
    case 1:
        for (int j = -1; j <= 1; j++)
        {
            for (int k = -1; k <= 1; k++)
            {
                if ((j != 0 || k != 0) && notOutOfBounds(minefield, difficulty, i, j, k)
                    && showTile[i / 9 + j][i % 9 + k])
                {
                    showTile[i / 9 + j][i % 9 + k] = false;
                    if (minefield[i / 9 + j][i % 9 + k] == 0)
                    {
                        //RECURSION :O
                        revealTiles(minefield, showTile, difficulty, i + k + (j * 9));
                    }
                }
            }
        }
        break;
    case 2:
        for (int j = -1; j <= 1; j++)
        {
            for (int k = -1; k <= 1; k++)
            {
                if ((j != 0 || k != 0) && notOutOfBounds(minefield, difficulty, i, j, k)
                    && showTile[i / 16 + j][i % 16 + k])
                {
                    showTile[i / 16 + j][i % 16 + k] = false;
                    if (minefield[i / 16 + j][i % 16 + k] == 0)
                    {
                        //RECURSION :O
                        revealTiles(minefield, showTile, difficulty, i + k + (j * 16));
                    }
                }
            }
        }
        break;
    case 3:
        for (int j = -1; j <= 1; j++)
        {
            for (int k = -1; k <= 1; k++)
            {
                if ((j != 0 || k != 0) && notOutOfBounds(minefield, difficulty, i, j, k)
                    && showTile[i / 30 + j][i % 30 + k])
                {
                    showTile[i / 30 + j][i % 30 + k] = false;
                    if (minefield[i / 30 + j][i % 30 + k] == 0)
                    {
                        //RECURSION :O
                        revealTiles(minefield, showTile, difficulty, i + k + (j * 30));
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    
}

//detects if a button is clicked based on the bounderies
//of the button and the cursors position on the window
bool buttonPressed(sf::RectangleShape button, sf::RenderWindow& window)
{
    if (sf::Mouse::getPosition(window).x >= button.getGlobalBounds().left
        && sf::Mouse::getPosition(window).x <= button.getGlobalBounds().left 
        + button.getSize().x 
        && sf::Mouse::getPosition(window).y >= button.getGlobalBounds().top
        && sf::Mouse::getPosition(window).y <= button.getGlobalBounds().top
        + button.getSize().y)
    {
        return true;
    }
    return false;
}

//prevents an off-by-one error from taking place
bool notOutOfBounds(int minefield[][30], int difficulty, int i, int j, int k)
{
    switch (difficulty)
    {
    case 1:
        if (i / 9 + j >= 0 && i % 9 + k >= 0
            && i / 9 + j < 9 && i % 9 + k < 9)
        {
            return true;
        }
        break;
    case 2:
        if (i / 16 + j >= 0 && i % 16 + k >= 0
            && i / 16 + j < 16 && i % 16 + k < 16)
        {
            return true;
        }
        break;
    case 3:
        if (i / 30 + j >= 0 && i % 30 + k >= 0
            && i / 30 + j < 16 && i % 30 + k < 30)
        {
            return true;
        }
        break;
    }
    return false;
}

//checks if the game has been won by the player
bool gameWin(int minefield[][30], bool showTile[][30], int difficulty)
{
    switch (difficulty)
    {
    case 1:
        for (int i = 0; i < 81; i++)
        {
            if (showTile[i / 9][i % 9] && minefield[i / 9][i % 9] != 9)
                return false;
        }
        break;
    case 2:
        for (int i = 0; i < 256; i++)
        {
            if (showTile[i / 16][i % 16] && minefield[i / 16][i % 16] != 9)
                return false;
        }
        break;
    case 3:
        for (int i = 0; i < 480; i++)
        {
            if (showTile[i / 30][i % 30] && minefield[i / 30][i % 30] != 9)
                return false;
        }
        break;
    default:
        break;
    }

    return true;
}
