#ifndef GAME_H
#define GAME_H

class Game
{
public:
    Game();
    Game(unsigned, unsigned);
    ~Game();
    void tick();
    unsigned getWidth() const;
    unsigned getHeight() const;
    bool getState(unsigned, unsigned) const;
    void setState(unsigned, unsigned, bool);
    bool isPlayable();

private:
    unsigned width;
    unsigned height;
    bool ** map;
    bool playable;
};

#endif // GAME_H
