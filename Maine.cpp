//# Punch
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "tinyxml.h"
#include "level.h"
#include <vector>
#include "Menu.h"
using namespace sf;


class TextureAndAnimations
{
private:
    float x, y,w,h;
    Image heroimage;
    String FileName;
    Texture herotexture;
    Sprite herosprite;
    Clock clock;
    float CurrentFrame = 0;
public:
    std::vector<Object> obj;//вектор объектов карты
    TextureAndAnimations(String fileName, float x,float y,float w,float h) {
        this->x = x; this->y - y; this->w = w; this->h = h;
        FileName = fileName;
        heroimage.loadFromFile("images/" + fileName);
        herotexture.loadFromImage(heroimage);
        herosprite.setTexture(herotexture);
        herosprite.setTextureRect(IntRect(5,15,w-10,h+3));

        herosprite.setPosition(150, 150);
        herosprite.setOrigin(w/2 , h/2);
    }
    Sprite getSprite() {
        return herosprite;
    }
    void setPosition(float X, float Y) {
        herosprite.setPosition(X,Y);
    };
    void Move(float X, float Y,float CurrentFrame, float w, float h) {
        
            if (X < 0)
            {
                herosprite.setTextureRect(IntRect(53 + (w * int(CurrentFrame)), 90, -w, h));
            }
            else
            {
                if (X > 0)
                {
                    herosprite.setTextureRect(IntRect(5 + (w * int(CurrentFrame)), 90, w, h));
                }
            }
    }
    void AttackAnimG(float X, float Y, float CurrentFrame, float w, float h) {
        int constanta = 0;
        switch (int(CurrentFrame))
        {
        case 1:
             constanta = 10;
            break;
        case 2:
            constanta = 20;
            break;
        case 3:
            constanta = 30;
            break;
        case 4:
            constanta = 30;
            break;
        }
        if (X < 0)
        {
            herosprite.setTextureRect(IntRect((39+constanta) + (w * int(CurrentFrame)), 155, -(w-10+constanta), h+5));
        }
        else
        {
            if (X > 0)
            {
                herosprite.setTextureRect(IntRect(0 + (w * int(CurrentFrame)), 155, w-10+ constanta, h+5));
            }
        }
    }
    void AttackAnimH(float X, float Y, float CurrentFrame, float w, float h) {

        if (X < 0)
        {
            herosprite.setTextureRect(IntRect(40 + ((w-8) * int(CurrentFrame)), 220, -(w-10), h+10));
        }
        else
        {
            if (X > 0)
            {
                herosprite.setTextureRect(IntRect(0 + ((w-8) * int(CurrentFrame)), 220, w-10, h+10));
            }
        }
    }
    void AttackAnimJ(float X, float Y, float CurrentFrame, float w, float h) {

        
            herosprite.setTextureRect(IntRect(315 + (w* int(CurrentFrame)), 150, -(w-2), h + 10));
        
    }
    void AttackAnimY(float X, float Y, float CurrentFrame, float w, float h) {
        if (X < 0)
        {
            herosprite.setTextureRect(IntRect(550, 90, -(w - 20), h + 5));
        }
        else
        {
            if (X > 0)
            {
                herosprite.setTextureRect(IntRect(520, 90, (w - 20), h + 5));
            }
        }
    }
    void Stand(float X) {
        if (X < 0)
        {
            herosprite.setTextureRect(IntRect(5+w-10, 15, -(w - 10), h + 3));
        }
        else
        {
            if (X > 0)
            {
                herosprite.setTextureRect(IntRect(5, 15, w - 10, h + 3));
            }
        }
    }
    void AnimationDeath(float X, float CurrentFrame, float w, float h)
    {
        int constanta=0;
        if (int(CurrentFrame) == 3)constanta = 30;
        if (X < 0)
        {
            herosprite.setTextureRect(IntRect(195 + (w  * int(CurrentFrame))-constanta, 760, -(w), h));
        }
        else
        {
            if (X > 0)
            {
                herosprite.setTextureRect(IntRect(145+(w*int(CurrentFrame))-constanta, 760, w, h));
            }
        }
        
    }
};


class Czlowiek: public TextureAndAnimations 
{
private:

    enum stateObj { left, right, up, down, jump, stay };
    float life ;
    bool isDead;
    stateObj state;
    enum {punch1,punch2,punch3,shield, nothing}attack;
    String Name;
    bool onGround,stand;
    bool check = false, checkShield = false;
    float time = 0;
    float x, y, w, h, dx, dy, speed = 0; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
    int dir = 0;
    float CurrentFrame = 0;
public:
    Czlowiek(String File, String Name, Level& lvl, float X, float Y, float W, float H):TextureAndAnimations(File,X,Y,W,H){
        w = W; h = H; x = X; y = Y; this->Name = Name; state = stay; 
        onGround = false; dx = 0; dy = 0; stand = true; attack = nothing;
        obj = lvl.GetAllObjects(); life = 280; isDead = false;
    }
    void control() {
        CurrentFrame += 0.005 * time;
        if (CurrentFrame > 4) { CurrentFrame -= 4; stand = true; }
        if (Keyboard::isKeyPressed) {
            if (Keyboard::isKeyPressed(Keyboard::A)&&attack!=shield) {
                state = left; speed = 0.1; stand = false; checkShield = false;
                Move(-0.1, 0, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::D) && attack != shield) {
                state = right; speed = 0.1; stand = false; checkShield = false;
                Move(0.1, 0, CurrentFrame, w, h);
            }

            if ((Keyboard::isKeyPressed(Keyboard::W)) && (onGround) && attack != shield) {
                state = up; dy = -0.7; onGround = false; stand = false; checkShield = false;
                Move(0, -0.1, CurrentFrame, w, h);
            }

            if (Keyboard::isKeyPressed(Keyboard::S) && attack != shield) {
                state = down; stand = false; checkShield = false;
                Move(0, 0.1, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::G))
            {
                attack = punch1; stand = false; checkShield = false;
                if(state==right)AttackAnimG(0.1, 0.1, CurrentFrame, w, h);
                if (state == left)AttackAnimG(-0.1, 0.1, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::H))
            {
                attack = punch2; stand = false; checkShield = false;
                if (state == right)AttackAnimH(0.1, 0.1, CurrentFrame, w, h);
                if (state == left)AttackAnimH(-0.1, 0.1, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::J))
            {
                attack = punch3; stand = false; checkShield = false;
                if (state == right)AttackAnimJ(0.1, 0.1, CurrentFrame, w, h);
                if (state == left)AttackAnimJ(-0.1, 0.1, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::Y))
            {
                attack = shield; stand = false; checkShield = true;
                if (state == right)AttackAnimY(0.1, 0.1, CurrentFrame, w, h);
                if (state == left)AttackAnimY(-0.1, 0.1, CurrentFrame, w, h);
            }
        }
        else { stand = true; }
    }
    void control2() {
        CurrentFrame += 0.005 * time;
        if (CurrentFrame > 4) { CurrentFrame -= 4; stand = true; }
        if (Keyboard::isKeyPressed) {
            if (Keyboard::isKeyPressed(Keyboard::Left) && attack != shield) {
                state = left; speed = 0.1; stand = false; checkShield = false;
                Move(-0.1, 0, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::Right) && attack != shield) {
                state = right; speed = 0.1; stand = false; checkShield = false;
                Move(0.1, 0, CurrentFrame, w, h);
            }

            if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround) && attack != shield) {
                state = up; dy = -0.7; onGround = false; stand = false; checkShield = false;
                Move(0, -0.1, CurrentFrame, w, h);
            }

            if (Keyboard::isKeyPressed(Keyboard::Down) && attack != shield) {
                state = down; stand = false; checkShield = false;
                Move(0, 0.1, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::Numpad4))
            {
                attack = punch1; stand = false; checkShield = false;
                if (state == right)AttackAnimG(0.1, 0.1, CurrentFrame, w, h);
                if (state == left)AttackAnimG(-0.1, 0.1, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::Numpad5))
            {
                attack = punch2; stand = false; checkShield = false;
                if (state == right)AttackAnimH(0.1, 0.1, CurrentFrame, w, h);
                if (state == left)AttackAnimH(-0.1, 0.1, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::Numpad6))
            {
                attack = punch3; stand = false; checkShield = false;
                if (state == right)AttackAnimJ(0.1, 0.1, CurrentFrame, w, h);
                if (state == left)AttackAnimJ(-0.1, 0.1, CurrentFrame, w, h);
            }
            if (Keyboard::isKeyPressed(Keyboard::Numpad8))
            {
                attack = shield; stand = false; checkShield = true;
                if (state == right)AttackAnimY(0.1, 0.1, CurrentFrame, w, h);
                if (state == left)AttackAnimY(-0.1, 0.1, CurrentFrame, w, h);
            }
        }
        else { stand = true; }
    }
    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = 0; i < obj.size(); i++)
        {
            if (getRect().intersects(obj[i].rect))
            {
                if (obj[i].name == "solid")
                {
                    if (Dy > 0) { y = obj[i].rect.top - h; dy = 0; onGround = true; }
                    if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height; dy = 0; }
                    if (Dx > 0) { x = obj[i].rect.left - w; }
                    if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
                }
            }
        }
    }
    void checkCollisionWithPlayer(Czlowiek &czel, float DX, float DY) {
        if (getRect().intersects(czel.getRect()))
        {
            if(attack!=nothing&&attack!=shield)
            {
                if (czel.getAttack() != 3) { czel.setLife(czel.getLife() - 1.4);}
                attack = nothing;
            }
            if (DY > 0) { if (state == left) { x = czel.getX() - w; } else { if (state == right) { x = czel.getX() + czel.getW(); } } }
            if (DY < 0) { y = czel.getY() + czel.getH(); dy = 0; }
            
                if (DX > 0) { x = czel.getX() - w; }
                if (DX < 0) { x = czel.getX() + czel.getW(); }
                if ((DX != 0 && DX > 0) && (czel.getDX() < 0 && czel.getDX() != 0)){
                    x = czel.getX() -w-10; czel.setX(czel.getX() +12);
                }
                if ((DX != 0 && DX < 0) && (czel.getDX() > 0 && czel.getDX() != 0)) {
                    x = x -2; czel.setX(czel.getX() +czel.getW()+10);
                }
        }
    }
    void update(float time) //функция "оживления" объекта класса.
    {
        if (!isDead)
        {
            this->time = time;
            if (Name == "Player1")
            {
                control();
            }
            else { control2(); }
            if (stand) {
                if (state == left) { Stand(-0.1); }
                if (state == right) { Stand(0.01); }
            }
            switch (state)
            {
            case right:dx = speed; break;
            case left:dx = -speed; break;
            case up:  break;
            case down: dx = 0; break;
            case stay: break;
            }
            if (life < 0) { life = 0; isDead = true; }

            x += dx * time;//то движение. наше ускорение на время получаем смещение координат и как следствие движение
            checkCollisionWithMap(dx, 0);
            y += dy * time;//аналогично по игреку
            checkCollisionWithMap(0, dy);
            speed = 0;//зануляем скорость, чтобы персонаж остановился.
            setPosition(x + w / 2, y + h / 2); //выводим спрайт в позицию x y 
            if (!onGround) { dy = dy + 0.0015 * time; }
            check = false;
        }
        else {
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) { CurrentFrame = 3; stand = true; } 
            if(state==left)
            AnimationDeath(-0.01, CurrentFrame, w, h);
            if (state == right)
                AnimationDeath(0.01, CurrentFrame, w, h);
        }

    }
    bool getCheckShield() {
        return checkShield;
    }
    int getAttack() {
        return attack;
    }
    void setAttack() {
        attack = nothing;
    }
    bool SAttack() {
        if (attack!=nothing&&attack!=shield) { attack=nothing; return true; }
        else { return false; }
    };
    void setLife(float health) {
        life = health;
    }
    float getLife() {
        return life;
    }
    FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
        return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
    }
    float getDX() {
        return dx; 
    }
    float getDY() { 
        return dy; 
    }
    void setDX(float dx) {
        this->dx = dx;
    }
    void setDY(float dy) {
        this->dy = dy;
    }
    float getX() {
        return x;
    }
    float getY() {
        return y;
    }
    void setX(float x) { 
        this->x = x;
    }
    void setY(float y) { 
        this->y = y;
    }
    float getW() {
        return w;
    }
    float getH() {
        return h;
    }
    bool getDead() { return isDead; }
};


class Life {
private:
    Image health;
    Texture healthtexture;
    Sprite healthsprite;
    Sprite healthsprite2;
    Image healthfull;
    Texture healthfulltexture;
    Sprite healthfullsprite;
    Sprite healthfullsprite2;
public:
    Life() {
        health.loadFromFile("images/health.png");
        healthtexture.loadFromImage(health);
        healthsprite.setTexture(healthtexture);
        healthsprite.setTextureRect(IntRect(20, 50, 455, 115));
        healthsprite2.setTexture(healthtexture);
        healthsprite2.setTextureRect(IntRect(455, 50, -435, 115));

        healthfull.loadFromFile("images/fullhealth.png");
        healthfulltexture.loadFromImage(healthfull);
        healthfullsprite.setTexture(healthfulltexture);
        healthfullsprite.setTextureRect(IntRect(45, 40, 415, 105));
        healthfullsprite2.setTexture(healthfulltexture);
        healthfullsprite2.setTextureRect(IntRect(460, 40, -415, 105));

        healthsprite.setPosition(50, 50);
        healthsprite2.setPosition(1012.5, 50);

        healthsprite.setScale(0.5, 0.5);
        healthsprite2.setScale(0.5, 0.5);

        healthfullsprite.setPosition(50, 50);
        healthfullsprite2.setPosition(1012.5, 50);

        healthfullsprite.setScale(0.53, 0.54762);
        healthfullsprite2.setScale(0.53, 0.54762);
    }
    Sprite getSpriteHealth() {
        return healthsprite;
    }
    Sprite getSpriteHealth2() {
        return healthsprite2;
    }
    Sprite getSpriteFull() {
        return healthfullsprite;
    }
    Sprite getSpriteFull2() {
        return healthfullsprite2;
    }
    void updateHealth(Czlowiek& czel1, Czlowiek& czel2) {
        healthfullsprite.setTextureRect(IntRect(45, 40, 130 + czel1.getLife(), 105));
        healthfullsprite2.setTextureRect(IntRect(740 - czel2.getLife() , 40, -360, 105));
    }
};

bool startGame(RenderWindow &window) {
    menu(window);

    Life l;
    bool check = false;
    float KO=0;
    Level lvl;
    lvl.LoadFromFile("Map.tmx");

    Music music;//создаем объект музыки
    music.openFromFile("music/theme.ogg");
    music.play();//воспроизводим музыку
    music.setLoop(true);

    SoundBuffer bufferPunch;
    bufferPunch.loadFromFile("music/punch.ogg");
    Sound soundPunch;
    soundPunch.setBuffer(bufferPunch);

    SoundBuffer bufferKO;
    bufferKO.loadFromFile("music/ko.ogg");
    Sound soundKO;
    soundKO.setBuffer(bufferKO);

    Object Player1 = lvl.GetObject("Player1");
    Object Player2 = lvl.GetObject("Player2");

    Czlowiek Sanji("sanji.png", "Player1", lvl, Player1.rect.left, Player1.rect.top, 50, 50);
    Czlowiek Sanji2("sanji.png", "Player2", lvl, Player2.rect.left, Player2.rect.top, 50, 50);
    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                if (Sanji2.SAttack())
                {
                    soundPunch.play();
                }
                if (Sanji.SAttack())
                {
                    soundPunch.play();
                }
            }
        }
        Sanji.checkCollisionWithPlayer(Sanji2, Sanji.getDX(), 0);
        Sanji.checkCollisionWithPlayer(Sanji2, 0, Sanji.getDY());
        Sanji2.checkCollisionWithPlayer(Sanji, Sanji2.getDX(), 0);
        Sanji2.checkCollisionWithPlayer(Sanji, 0, Sanji2.getDY());

        if (Sanji.getDead() || Sanji2.getDead())
        {
            KO += time;
            if (!check)
            {
                soundKO.play();
                check = true;
            }
            if (KO > 3500)
            {
                return true;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }

        if (!Keyboard::isKeyPressed(Keyboard::Y))
        {
            Sanji.setAttack();
        }
        if (!Keyboard::isKeyPressed(Keyboard::Numpad8))
        {
            Sanji2.setAttack();
        }

        Sanji.update(time);
        Sanji2.update(time);
        l.updateHealth(Sanji, Sanji2);

        window.clear();
        lvl.Draw(window);
        window.draw(Sanji.getSprite());

        window.draw(l.getSpriteFull());
        window.draw(l.getSpriteHealth());
        window.draw(Sanji2.getSprite());

        window.draw(l.getSpriteFull2());
        window.draw(l.getSpriteHealth2());
        window.display();
    }
}


void gameRunning(RenderWindow &window) {
    if (startGame(window)||!isExite) { gameRunning(window);}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 640), "Death Punch");
    gameRunning(window);
    
    return 0;
}
