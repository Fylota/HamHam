#include "graphics.hpp"
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace genv;
using namespace std;

const unsigned int X = 400;
const unsigned int Y = 400;

int ox = X/2;
int oy = Y/2;

void clr(int _x, int _y, int meret)
{
    gout<<move_to(0,0)<<color(0,0,0)<<box(X,Y)<<color(255,255,255);
    // if(_x-meret/2>0 && _x+meret/2 < X && _y-meret/2>0 && _y+meret/2 < Y){
    gout<<move_to(_x-meret/2,_y-meret/2)<<color(255,255,255)<<box(meret,meret); ///Játékos kirajzolása
    //}
}
struct Kocka
{
    bool jo;
    int x,y,r,g,b,a,n;
    Kocka()
    {
        x=rand()%X;
        y=rand()%Y;
        n=rand()%4+1;
        r=255;
        g=0;
        b=0;
        jo = false;
    }
    virtual void mozog()
    {

        y+=n;
        //y%=Y;
    }
    void rajzol()
    {
        if(x>=0 && y>=0 &&x<X &&y<Y)
            gout<<color(r,g,b)<<move_to(x,y)<<box(10,10);
    }
    bool kiesett()
    {
        return y>Y || y<0 || x>X || x<0;
    }
    bool utkozik(int _x, int _y, double m)
    {
        if(x < _x+m/2-1 && _x-m/2+1 < x+10 && y < _y+m/2-1 && _y-m/2+1 < y+10)
        {
            return true;
        }
        else return false;
    }
};
struct Kocka_le : public Kocka
{
    void mozog()
    {
        //int n=rand()%4+1;
        y+=n;
    }
};
struct Kocka_le_jo : public Kocka_le
{
    Kocka_le_jo()
    {
        r=0;
        g=255;
        jo = true;
    }
};
struct Kocka_balrol : public Kocka
{
    void mozog()
    {
        //int n=rand()%4+1;
        x+=n;
    }
};
struct Kocka_balrol_jo : public Kocka_balrol
{
    Kocka_balrol_jo()
    {
        r= 0 ;
        g = 255;
        jo = true;
    }
};
struct Kocka_jobbrol : public Kocka
{
    void mozog()
    {
        //int n=rand()%4+1;
        x-=n;
    }
};
struct Kocka_jobbrol_jo : public Kocka_jobbrol
{
    Kocka_jobbrol_jo()
    {
        r= 0 ;
        g = 255;
        jo = true;
    }
};
struct Kocka_fel : public Kocka
{
    void mozog()
    {
        //int n=rand()%4+1;
        y-=n;
    }
};
struct Kocka_fel_jo : public Kocka_fel
{
    Kocka_fel_jo()
    {
        r= 0 ;
        g = 255;
        jo = true;
    }
};


int main()
{
    gout.open(X,Y);

    vector <Kocka *> v;
    for(int i=0; i<2; i++)
    {
        v.push_back(new Kocka_le);
        v.push_back(new Kocka_le_jo);
        v.push_back(new Kocka_balrol);
        v.push_back(new Kocka_balrol_jo);
        v.push_back(new Kocka_jobbrol);
        v.push_back(new Kocka_jobbrol_jo);
        v.push_back(new Kocka_fel);
        v.push_back(new Kocka_fel_jo);
    }

    event ev;

///#################### MENU #######################///

    bool menu = true;
    while (gin >> ev && menu != false && ev.keycode != key_escape)
    {
        string szoveg = "Play";
        gout << move_to(ox-80,80) << color(255,255,255) << box(160, 60) << move_to(ox-gout.twidth(szoveg)/2,117)
             << color (0,0,0) << text(szoveg);
        szoveg = "Quit";
        gout << move_to(ox-80,160) << color(255,255,255) << box(160, 60) << move_to(ox-gout.twidth(szoveg)/2,197)
             << color (0,0,0) << text(szoveg);
/*
        szoveg = "High Scores";
        gout << move_to(ox-80,240) << color(255,255,255) << box(160, 60) << move_to(ox-gout.twidth(szoveg)/2,277)
             << color (0,0,0) << text(szoveg);
*/

        if(ev.pos_x >=120 && ev.pos_x <= 280 && ev.pos_y >=80 && ev.pos_y<=140)
        {
            gout<<color(255,0,0)<<move_to(120,80)<<line(160,0)<<line(0,60)<<line(-160,0)<<line(0,-60);

            if(ev.button==btn_left && ev.keycode !=key_enter)
            {
                menu = false;

            }
        }
        if(ev.pos_x >=120 && ev.pos_x <= 280 && ev.pos_y >=160 && ev.pos_y<=220)
        {
            gout<<color(255,0,0)<<move_to(120,160)<<line(160,0)<<line(0,60)<<line(-160,0)<<line(0,-60);

            if(ev.button==btn_left && ev.keycode !=key_enter)
            {
                menu = false;
                return 0;
            }
        }
/*
        if(ev.pos_x >=120 && ev.pos_x <= 280 && ev.pos_y >=240 && ev.pos_y<=300)
        {
            gout<<color(255,0,0)<<move_to(120,240)<<line(160,0)<<line(0,60)<<line(-160,0)<<line(0,-60);

            if(ev.button==btn_left && ev.keycode !=key_enter)
            {
                menu = false;

            }
        }
*/
        gout << refresh;
    }

///##################### JÁTÉK ############################///

    gin.timer(14);
    int meret = 15;
    int rekord;

    ifstream highscoref;
    highscoref.open ("highscore.txt");

    highscoref >> rekord;

    highscoref.close();

    gout<<move_to(ev.pos_x-meret/2,ev.pos_y-meret/2)<<color(255,255,255)<<box(meret,meret);

    while(gin >> ev && ev.keycode != key_escape)
    {

        int _x;
        int _y;
        if(ev.type == ev_mouse)
        {

            _x = ev.pos_x;
            _y = ev.pos_y;
        }

        if(ev.type == ev_timer)
        {


            for(Kocka *p : v)
            {
                if (p)
                    p->mozog();
            }

            for(Kocka * &p : v)
            {
                if(p && p->kiesett())
                {
                    delete p;
                    int random = rand() % 4 + 1;
                    if(p->jo)
                    {
                        switch(random)
                        {
                        case 1:
                            p = new Kocka_le_jo;
                            p->y = 0;
                            break;
                        case 2:
                            p = new Kocka_balrol_jo;
                            p->x = 0;
                            break;
                        case 3:
                            p = new Kocka_jobbrol_jo;
                            p->x = X;
                            break;
                        case 4:
                            p = new Kocka_fel_jo;
                            p->y = Y;
                            break;
                        default :
                            cout <<"nem jo 1 " << random <<endl;
                            break;
                        }
                    }
                    else
                    {
                        switch(random)
                        {
                        case 1:
                            p = new Kocka_le;
                            p->y = 0;
                            break;
                        case 2:
                            p = new Kocka_balrol;
                            p->x = 0;
                            break;
                        case 3:
                            p = new Kocka_jobbrol;
                            p->x = X;
                            break;
                        case 4:
                            p = new Kocka_fel;
                            p->y = Y;
                            break;
                        default :
                            cout <<"nem jo 1 " << random <<endl;
                            break;
                        }
                    }
                }
            }

            for(Kocka *&p : v)
            {
                if(p && p->utkozik(_x,_y, meret))
                {
                    if(p && p->jo)
                    {
                        meret ++;
                        delete p;
                        int random = rand()%4 + 1;
                        switch(random)
                        {
                        case 1:
                            p = new Kocka_le_jo;
                            p->y = 0;
                            break;
                        case 2:
                            p = new Kocka_balrol_jo;
                            p->x = 0;
                            break;
                        case 3:
                            p = new Kocka_jobbrol_jo;
                            p->x = X;
                            break;
                        case 4:
                            p = new Kocka_fel_jo;
                            p->y = Y;
                            break;
                        default :

                            cout <<"nem jo2" << endl;
                            break;
                        }

                    }

///######################### GAME OVER ###############################///

                    else
                    {

                        menu = true;
                        while (gin >> ev && menu != false && ev.keycode != key_escape)
                        {
                            gin.timer(0);
                            string szoveg = "Game over";
                            int szoveg_hossza = gout.twidth(szoveg);
                            gout << move_to(ox - szoveg_hossza/2, 40) << color(255,255,0) << text(szoveg);
                            //szoveg = "pontszam: ";
                            stringstream ss;
                            ss << meret-15;
                            string str = ss.str();
                            szoveg = "Score: " + str;
                            gout << move_to(ox - szoveg_hossza/2, 52) << color(255,255,0) << text(szoveg);
                            ///~~~~~~~REKORD~~~~~~~///
                            if(rekord < meret-15)
                            {
                                rekord = meret-15;
                            }
                            ss.clear();
                            stringstream ss2;
                            ss2 << rekord;
                            string str2 = ss2.str();
                            szoveg = "High score: " + str2;

                            ofstream highscoref;
                            highscoref.open ("highscore.txt");
                            highscoref << rekord;
                            highscoref.close();

                            //cout << rekord<<endl;
                            gout << move_to(ox - szoveg_hossza/2, 64) << color(255,255,0) << text(szoveg);
                            gout << move_to(120,80) << color(255,255,255) << box(160, 60) << move_to(142,117)
                                 << color (0,0,0) << text("Try again");
                            gout << move_to(120,160) << color(255,255,255) << box(160, 60) << move_to(142,197)
                                 << color (0,0,0) << text("Quit");
                            if(ev.pos_x >=120 && ev.pos_x <= 280 && ev.pos_y >=80 && ev.pos_y<=140)
                            {
                                gout<<color(255,0,0)<<move_to(120,80)<<line(160,0)<<line(0,60)<<line(-160,0)<<line(0,-60);

                                if(ev.button==btn_left && ev.keycode !=key_enter)
                                {
                                    menu = false;
                                    meret = 15;
                                    for(Kocka *&p : v)
                                    {
                                        p->x=rand()%X;
                                        p->y=rand()%Y;
                                    }
                                    gin.timer(14);
                                }
                            }
                            if(ev.pos_x >=120 && ev.pos_x <= 280 && ev.pos_y >=160 && ev.pos_y<=220)
                            {
                                gout<<color(255,0,0)<<move_to(120,160)<<line(160,0)<<line(0,60)<<line(-160,0)<<line(0,-60);

                                if(ev.button==btn_left && ev.keycode !=key_enter)
                                {
                                    menu = false;
                                    return 0;
                                }
                            }
                            gout << refresh;
                        }
                    }
/// GAME OVER vége ///
                }
            }

            clr(_x,_y,meret);
            for(Kocka *p : v)
            {
                if (p)
                    p->rajzol();
            }
        }
        gout << refresh;
    }
    return 0;
}
