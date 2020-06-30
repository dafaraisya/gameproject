#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>

using namespace sf;
using namespace std;

//gameplay
class Game{
public:
    Texture objek_img;
    Sprite objek;
    IntRect objek_koordinat;
    Clock clock;

    //fungsi set
    void setdraw(char* loadimg, int koordinatleft, int koordinattop,int koordinatwidth,
                 int koordinatheight,float scaleX,float scaleY, int positionX, int positionY){
        objek_img.loadFromFile(loadimg);
        objek.setTexture(objek_img);
        objek_koordinat.left = koordinatleft;
        objek_koordinat.top = koordinattop;
        objek_koordinat.width = koordinatwidth;
        objek_koordinat.height = koordinatheight;
        objek.setTextureRect(objek_koordinat);
        objek.setScale(scaleX,scaleY);
        objek.setPosition(positionX,positionY);
    }
    //input keyboard
    void keyboard(){
        int posisi_x = objek.getPosition().x;
        //kanan
        if(Keyboard::isKeyPressed(Keyboard::Right)){
            if(posisi_x>=750) {
                posisi_x = 750;
            }else{
                objek.setPosition(posisi_x + 20  ,600);
                if(posisi_x>=750) {
                posisi_x = 750;
                }
            }
        }
        //kiri
        if(Keyboard::isKeyPressed(Keyboard::Left)){
            if(posisi_x<=450){
                posisi_x = 450;
            } else {
                objek.setPosition(posisi_x - 20 ,600);
                if(posisi_x>=450) {
                posisi_x = 450;
                }
            }
        }
    }

    //animasi player
    void animasiPlayer(){
        if(clock.getElapsedTime().asSeconds() > 0.1f){
            //player
            if(objek_koordinat.left== 350 ){
                objek_koordinat.left = 700;
            }
            else if(objek_koordinat.left == 700 ){
                objek_koordinat.left = 0;
            }
            else{
                objek_koordinat.left += 350;
            }

            objek.setTextureRect(objek_koordinat);

            clock.restart();
        }
    }
    //animasi background
    void animasiBackground(){
        if(clock.getElapsedTime().asSeconds() > 0.1f){
            //player
            if(objek_koordinat.top== 16 ){
                objek_koordinat.top = 1336;
            }else{
                objek_koordinat.top -= 20;
            }

            objek.setTextureRect(objek_koordinat);

            clock.restart();
        }
    }

};

string stringscore =" ";
string stringhighscore = " ";
int score= 0;
int highscore = 0;
int gameover();
int main();
RenderWindow window;

//gameover
class GameOver{
public:
    Text text;
    Font myfont;

    void set(string tulisan, int fontsize, int textY){
        myfont.loadFromFile("font/GROBOLD.ttf");
        text.setString(tulisan);
        text.setFont(myfont);
        text.setCharacterSize(fontsize);
        int textX = window.getSize().x/2 - text.getGlobalBounds().width / 2;
        text.setPosition(textX,textY);
    }

    void clickmenu(){
        int posisimouseX = Mouse::getPosition().x;
        int posisimouseY = Mouse::getPosition().y;
        if( posisimouseX < text.getPosition().x + text.getGlobalBounds().width &&
           posisimouseX > text.getPosition().x &&
           posisimouseY < text.getPosition().y + text.getGlobalBounds().height &&
           posisimouseY > text.getPosition().y ){
            if(Mouse::isButtonPressed(Mouse::Left)){
                score = 0;
                main();//kembali ke main menu
            }
        }
    }

    void clickquit(){
        int posisimouseX = Mouse::getPosition().x;
        int posisimouseY = Mouse::getPosition().y;
        if( posisimouseX < text.getPosition().x + text.getGlobalBounds().width &&
           posisimouseX > text.getPosition().x &&
           posisimouseY < text.getPosition().y + text.getGlobalBounds().height &&
           posisimouseY > text.getPosition().y ){
            if(Mouse::isButtonPressed(Mouse::Left)){
                exit(2);//keluar
            }
        }
    }

};

Clock myClock;
void MainMenu(Clock clock);

Sound sound;

int main()
{
    SoundBuffer buffer;
    if (!buffer.loadFromFile("backsound.wav "))
    return -1;
    SoundBuffer buffergameover;
    if (!buffergameover.loadFromFile("gameover.wav "))
    return -1;

    Sound gameoversound;

    sound.setBuffer(buffer);
    sound.play();
    sound.setLoop(10);

    gameoversound.setBuffer(buffergameover);

    srand(time(NULL));
    window.create(VideoMode(), "PASS AND GET IT!",Style::Fullscreen);
    //logo screen
    MainMenu(myClock);
    //draw koin
    Texture koin_img;
    koin_img.loadFromFile("img/coin.png");
    IntRect koin_koordinat(0,0,84,84);
    Sprite koin(koin_img, koin_koordinat);
    koin.setScale(0.8,0.8);
    koin.setPosition(0,-100);

    vector<Sprite> coins;
    coins.push_back(Sprite(koin));

    int koinkeluar = 0;

    //draw batu
    Texture batu_img;
    batu_img.loadFromFile("img/batu.png");
    IntRect batu_koordinat(0,0,150,150);
    Sprite batu(batu_img, batu_koordinat);
    batu.setPosition(0,-150);

    vector<Sprite> batuu;
    batuu.push_back(Sprite(batu));

    int batukeluar = 0;

    //draw peluru
    CircleShape peluru;
    peluru.setFillColor(Color::Red);
    peluru.setRadius(7.f);

    vector<CircleShape> vecpeluru;

    int kecpeluru = 0;

    //font
    Font myfont;
    myfont.loadFromFile("font/xirod.ttf");

    //inisialisasi objek
    Game player;
    Game background;
    //draw
    player.setdraw("img/player5.png", 0, 0, 350,279,0.5,0.5,600,600);
    background.setdraw("img/background.png", 0, 1336, 1000, 1002, 1.5,1.5, -70,0);

    //buat file highscore
    ofstream bukahighscore;
    bukahighscore.open("highscore.txt", ios::out | ios::in | ios::app);
    bukahighscore.close();

    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            //handle event
            if (event.type == Event::Closed)
                window.close();
            //input keyboard
            player.keyboard();
        }

        //score
        stringscore="SCORE:"+ to_string(score);
        Text text(stringscore, myfont, 35);
        text.setPosition(20,20);

        int posisi_playerx = player.objek.getPosition().x;
        int posisi_playery = player.objek.getPosition().y;

        //peluru
        if(Keyboard::isKeyPressed(Keyboard::Space) && kecpeluru >= 100){
            peluru.setPosition(posisi_playerx + (player.objek.getGlobalBounds().width /2) - peluru.getGlobalBounds().width, posisi_playery);
            vecpeluru.push_back(CircleShape(peluru));
            kecpeluru=0;
        }
        if(kecpeluru<100){
            kecpeluru++;
        }
        for(int i=0;i<vecpeluru.size();i++){
            vecpeluru[i].move(0.0f, -0.5f);

            if(vecpeluru[i].getPosition().y > window.getSize().y){
                vecpeluru.erase(vecpeluru.begin() + i);
            }
        }


        //batu
        int randbatukeluar = (rand()%100+1)*1000;
        if(batukeluar<randbatukeluar)
            batukeluar++;
        if(batukeluar>=randbatukeluar){
            int random = rand()%3+1;
            if(random==1){
                batu.setPosition(460, -120);
            }else if(random==2){
                batu.setPosition(610, -120);
            }else if(random==3){
                batu.setPosition(760,-120);
            }

            batuu.push_back(Sprite(batu));
            batukeluar = 0;
        }

        for(int i=1;i<batuu.size();i++){
            batuu[i].move(0.f,0.5f);

            if(batuu[i].getPosition().y > window.getSize().y){
                batuu.erase(batuu.begin() + i);
            }

            //cek batu kena player
            if(batuu[i].getPosition().y >= 600 && batuu[i].getPosition().y <= 700 ){
                if(posisi_playerx >= batuu[i].getPosition().x-50 &&
                   posisi_playerx <= batuu[i].getPosition().x+70){
                    sound.stop();
                    gameoversound.play();
                    gameover();
                }
            }
        }

        //koin
        if(koinkeluar<800)
            koinkeluar++;
        if(koinkeluar>=800){
            int random = rand()%3+1;
            if(random==1){
                koin.setPosition(500, -100);
            }else if(random==2){
                koin.setPosition(650, -100);
            }else if(random==3){
                koin.setPosition(800,-100);
            }

            coins.push_back(Sprite(koin));
            koinkeluar = 0;
        }

        for(int i=1;i<coins.size();i++){
            coins[i].move(0.f,0.5f);

            if(coins[i].getPosition().y > window.getSize().y){
                coins.erase(coins.begin() + i);
            }

            //cek koin kena player
            if(coins[i].getPosition().y >= 600){
                if(posisi_playerx >= coins[i].getPosition().x-90 &&
                   posisi_playerx <= coins[i].getPosition().x+34 ){
                    coins.erase(coins.begin() + i);
                    score++;
                }
            }
        }

        window.clear();

        background.animasiBackground();
        player.animasiPlayer();
        window.draw(background.objek);
        window.draw(player.objek);

        for(int i=0;i<coins.size();i++){
            window.draw(coins[i]);
        }

        for(int i=0;i<batuu.size();i++){
            for(int j=0;j<vecpeluru.size();j++){
                if(batuu[i].getGlobalBounds().intersects(vecpeluru[j].getGlobalBounds())){
                    batuu.erase(batuu.begin() + i);
                }
            }
            window.draw(batuu[i]);
        }

        for(int i=0;i<vecpeluru.size();i++){
            window.draw(vecpeluru[i]);
        }


        window.draw(text);

        window.display();
    }

    return 0;
}

//gameover
int gameover(){
    GameOver scoreakhir;
    GameOver scoretertinggi;
    GameOver mainmenu;
    GameOver quit;
    Texture gameover_img;
    Sprite gameover;
    gameover_img.loadFromFile("img/gameover.png");
    gameover.setTexture(gameover_img);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        stringscore="YOUR SCORE:"+to_string(score);
        scoreakhir.set(stringscore, 35 ,300);

        //cek highscore
        string line;
        ifstream bacahighscore;
        bacahighscore.open("highscore.txt");
        getline(bacahighscore, line );
        int datahighscore = atoi(line.c_str());
        if(score >= datahighscore){
            highscore = score;
            ofstream inputhighscore;
            inputhighscore.open("highscore.txt");
            inputhighscore << highscore;
            inputhighscore.close();
        } else {
            highscore = datahighscore;
        }

        bacahighscore.close();

        stringhighscore="HIGH SCORE:"+to_string(highscore);
        scoretertinggi.set(stringhighscore, 35 ,350);

        string stringmainmenu="MAIN MENU";
        mainmenu.set(stringmainmenu, 50, 480);
        mainmenu.clickmenu();

        string stringquit ="QUIT";
        quit.set(stringquit, 50, 550);
        quit.clickquit();

        window.clear();
        window.draw(gameover);
        window.draw(scoreakhir.text);
        window.draw(scoretertinggi.text);
        window.draw(mainmenu.text);
        window.draw(quit.text);
        window.display();

    }
}

//Mainmenu

void MainMenu(Clock clock)
{
    bool running = true;
    bool FadeIn = true;

    Texture background_menu_img;
    Sprite  background_menu;
    Texture play_img;
    Sprite  play;
    Texture quit_img;
    Sprite quit;
    Time logoTime = seconds(2.0f);

    background_menu_img.loadFromFile("img/mainmenu.png");
    background_menu.setTexture(background_menu_img);

    play_img.loadFromFile("img/play.png");
    play.setTexture(play_img);
    play.setPosition( 372 , 507);

    quit_img.loadFromFile("img/quit.png");
    quit.setTexture(quit_img);
    quit.setPosition( 858 , 507);

    // main opening loop
    clock.restart();
    while (running)
    {
        Event event;
        while (window.pollEvent(event))
        {
            // "close window" event
            if (event.type == Event::Closed) {
                //window.close();
                running = false;
            }

        }

        window.clear(Color::Black);

        // draw
        window.draw(background_menu);
        window.draw(play);
        window.draw(quit);

        window.display();

        // ---- update animation
        Time elapsed1 = clock.getElapsedTime();
        if (FadeIn) {
            background_menu.setColor(Color(255, 255, 255, elapsed1.asSeconds()/logoTime.asSeconds()*255));
            if (elapsed1 >= logoTime) {
                FadeIn = false;
                clock.restart();
            }
        }

        if (FadeIn) {
            play.setColor(Color(255, 255, 255, elapsed1.asSeconds()/logoTime.asSeconds()*255));
            if (elapsed1 >= logoTime) {
                FadeIn = false;
                clock.restart();
            }
        }

        if (FadeIn) {
            quit.setColor(Color(255, 255, 255, elapsed1.asSeconds()/logoTime.asSeconds()*255));
            if (elapsed1 >= logoTime) {
                FadeIn = false;
                clock.restart();
            }
        }

        int posisimouseX = Mouse::getPosition().x;
        int posisimouseY = Mouse::getPosition().y;

        if(posisimouseX < play.getPosition().x + play.getGlobalBounds().width &&
           posisimouseX > play.getPosition().x &&
           posisimouseY < play.getPosition().y + play.getGlobalBounds().height &&
           posisimouseY > play.getPosition().y){
            if(Mouse::isButtonPressed(Mouse::Left)){
                running = false;
            }
        }

        if(posisimouseX < quit.getPosition().x + quit.getGlobalBounds().width &&
           posisimouseX > quit.getPosition().x &&
           posisimouseY < quit.getPosition().y + quit.getGlobalBounds().height &&
           posisimouseY > quit.getPosition().y){
            if(Mouse::isButtonPressed(Mouse::Left)){
                sound.stop();
                window.close();
            }
        }


    }
}
