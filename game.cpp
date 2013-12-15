#include "game.h"
#include <QString>

Game::Game()
    :
      width(0),
      height(0),
      map(NULL),
      playable(true)
{
}

Game::Game(unsigned w, unsigned h)
    :
      width(w),
      height(h),
      playable(true)
{

    map = new bool*[height];
    for(unsigned i = 0; i < height; i++)
    {
        map[i] = new bool[width];
        for(unsigned j = 0; j < width; j++)
            map[i][j] = false;
    }

}

Game::~Game()
{
    if(map != NULL)
    {
        for(unsigned i = 0; i < height; i++)
            if(map[i] != NULL)
                delete map[i];
        delete map;
    }
}

unsigned Game::getHeight() const
{
    return height;
}

unsigned Game::getWidth() const
{
    return width;
}

bool Game::getState(unsigned w, unsigned h) const
{
    if(map != NULL && map[h] != NULL)
        return map[h][w];
    else
        throw QString::fromUtf8("Mapa nie istnieje");
}

void Game::setState(unsigned w, unsigned h, bool s)
{
    if(map != NULL && map[h] != NULL)
        map[h][w] = s;
    else
        throw QString::fromUtf8("Mapa nie istnieje");
    playable = true;
}

void Game::tick()
{
/*
    bool bak[height][width];

    for(int i(0); i < this->height; i++)
        {
            for(int j(0); j < this->width; j++)
            {
                //petla dla sasiadow
                int alive = 0;

                for(int x = i - 1; x <= i + 1; x++)
                {
                    for(int y = j - 1; y <= j + 1; y++)
                    {
                        if(x >= 0 && x < this->height && y >= 0 && y < this->width && !(x == i && y == j))
                            if(this->map[x][y] == true)
                                alive++;
                        if(alive > 3)
                            break;
                    }
                    if(alive > 3)
                        break;
                }
                if(alive < 2 || alive > 3)
                    bak[i][j] = false;

                else if(alive == 3)
                    bak[i][j] = true;

                else
                    bak[i][j] = this->map[i][j];
            }
        }
    for(unsigned i = 0; i < height; i++)
        for(unsigned j = 0; i < width; j++)
            map[i][j] = bak[i][j];*/


    bool saZywi = false;
    bool saMartwi = false;
    bool saZmiany = false;

    bool nowa[height][width];

    for(int i = 0; i < int(this->height); i++)
    {
        for(int j = 0; j < int(this->width); j++)
        {
            //petla dla sasiadow
            int iloscZywych = 0;

            for(int x = i - 1; x <= i + 1; x++)
            {
                for(int y = j - 1; y <= j + 1; y++)
                {
                    if(x >= 0 && x < int(this->height) && y >= 0 && y < int(this->width) && !(x == i && y == j))
                        if(this->map[x][y] == true)
                            iloscZywych++;
                    if(iloscZywych > 3)
                        break;
                }
                if(iloscZywych > 3)
                    break;
            }
            if(iloscZywych < 2 || iloscZywych > 3)
            {
                nowa[i][j] = false;
                saMartwi = true;
            }
            else if(iloscZywych == 3)
            {
                nowa[i][j] = true;
                saZywi = true;
            }
            else
            {
                nowa[i][j] = this->map[i][j];

                if(nowa[i][j] == true)
                    saZywi = true;
                else
                    saMartwi = true;
            }
            if(nowa[i][j] != this->map[i][j])
                saZmiany = true;
        }

    }


    for(unsigned i = 0; i < height; i++)
        for(unsigned j = 0; j < width; j++)
            map[i][j] = nowa[i][j];

    if(saZywi && saMartwi && saZmiany)
        playable = true;
    else
        playable = false;


}

bool Game::isPlayable()
{
    return playable;
}
