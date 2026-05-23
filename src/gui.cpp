#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

struct GUI::Impl{
    SDL_Window* win=nullptr;
    SDL_Renderer* ren=nullptr;
    TTF_Font* font=nullptr;
    int selected=-1;
    int activeField=0;
    std::string fields[4];
    double lastPayment=0;
};

static SDL_Texture* renderText(SDL_Renderer* ren, TTF_Font* f, const std::string& s, SDL_Color c, int& w,int& h){
    SDL_Surface* surf = TTF_RenderUTF8_Blended(f, s.c_str(), c);
    if(!surf) return nullptr;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    w = surf->w; h = surf->h;
    SDL_FreeSurface(surf);
    return tex;
}

static std::string trim(const std::string &s){ size_t a=0,b=s.size(); while(a<b && std::isspace((unsigned char)s[a])) ++a; while(b>a && std::isspace((unsigned char)s[b-1])) --b; return s.substr(a,b-a); }

GUI::GUI(): p(new Impl) {}
GUI::~GUI(){ delete p; }

bool GUI::init(){
    if(SDL_Init(SDL_INIT_VIDEO)!=0) return false;
    if(TTF_Init()!=0){ SDL_Quit(); return false; }
    p->win = SDL_CreateWindow("Bank Loan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 520, 0);
    if(!p->win){ TTF_Quit(); SDL_Quit(); return false; }
    p->ren = SDL_CreateRenderer(p->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    p->font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 16);
    if(!p->font) p->font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 16);
    if(!p->font){ SDL_DestroyRenderer(p->ren); SDL_DestroyWindow(p->win); TTF_Quit(); SDL_Quit(); return false; }
    return true;
}

void GUI::run(std::vector<Customer>& customers){
    SDL_StartTextInput();
    bool running = true;
    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT){ running=false; break; }
            if(e.type==SDL_MOUSEBUTTONDOWN){
                int x = e.button.x, y = e.button.y;
                if(y < 40){ int col = x/175; if(col>=0 && col<4) p->activeField = col; }
                else if(y>=50 && y<84){
                    if(x>=10 && x<170){
                        std::string nm = trim(p->fields[0]); if(nm.empty()) nm = std::string("Customer ")+std::to_string(customers.size()); Customer c; c.name = nm; customers.push_back(c); for(int i=0;i<4;i++) p->fields[i].clear();
                    } else if(x>=180 && x<340){
                        if(!customers.empty()){
                            int idx = p->selected>=0? p->selected : (int)customers.size()-1;
                            if(idx>=0){ Customer &c = customers[idx]; std::string nm = trim(p->fields[0]); if(!nm.empty()) c.name = nm; try{ if(!p->fields[1].empty()) c.principal = std::stod(trim(p->fields[1])); if(!p->fields[2].empty()) c.rate = std::stod(trim(p->fields[2])); if(!p->fields[3].empty()) c.years = std::stoi(trim(p->fields[3])); }catch(...){} c.hasLoan = true; }
                        }
                    } else if(x>=350 && x<510){
                        if(!customers.empty()){ int idx = p->selected>=0? p->selected : (int)customers.size()-1; if(idx>=0) p->lastPayment = monthlyPayment(customers[idx].principal, customers[idx].rate, customers[idx].years); }
                    } else if(x>=520 && x<680){ for(int i=0;i<4;i++) p->fields[i].clear(); }
                } else if(y>=110){ int idx = (y-110)/22; if(idx>=0 && (size_t)idx<customers.size()) p->selected = idx; }
            }

            if(e.type==SDL_TEXTINPUT){ std::string t(e.text.text); t.erase(std::remove(t.begin(), t.end(), '\r'), t.end()); t.erase(std::remove(t.begin(), t.end(), '\n'), t.end()); p->fields[p->activeField] += t; }

            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_BACKSPACE){ auto &s = p->fields[p->activeField]; if(!s.empty()) s.pop_back(); }
                else if(e.key.keysym.sym==SDLK_TAB){ p->activeField = (p->activeField+1)%4; }
                else if(e.key.keysym.sym==SDLK_RETURN){ std::string nm = trim(p->fields[0]); if(nm.empty()) nm = std::string("Customer ")+std::to_string(customers.size()); Customer c; c.name = nm; try{ if(!p->fields[1].empty()) c.principal = std::stod(trim(p->fields[1])); if(!p->fields[2].empty()) c.rate = std::stod(trim(p->fields[2])); if(!p->fields[3].empty()) c.years = std::stoi(trim(p->fields[3])); }catch(...){} c.hasLoan = c.principal>0; customers.push_back(c); for(int i=0;i<4;i++) p->fields[i].clear(); }
            }
        }

        SDL_SetRenderDrawColor(p->ren,24,24,24,255); SDL_RenderClear(p->ren);
        SDL_Rect header{0,0,700,40}; SDL_SetRenderDrawColor(p->ren,40,40,40,255); SDL_RenderFillRect(p->ren,&header);
        SDL_Color white={230,230,230,255}; int w,h; SDL_Texture* tx;

        for(int i=0;i<4;i++){
            std::string label = (i==0?"Name:":i==1?"Principal:":i==2?"Rate:":"Years:") + p->fields[i];
            tx = renderText(p->ren,p->font,label,white,w,h);
            if(tx){ SDL_Rect r{10+170*i,10,w,h}; SDL_RenderCopy(p->ren,tx,NULL,&r); SDL_DestroyTexture(tx); }
            if(p->activeField==i){ SDL_Rect box{10+170*i-4,6,160,24}; SDL_SetRenderDrawColor(p->ren,100,100,160,255); SDL_RenderDrawRect(p->ren,&box); }
        }

        SDL_Rect btnAdd{10,50,160,34}; SDL_Rect btnSet{180,50,160,34}; SDL_Rect btnCalc{350,50,160,34}; SDL_Rect btnClear{520,50,160,34}; SDL_SetRenderDrawColor(p->ren,60,60,60,255); SDL_RenderFillRect(p->ren,&btnAdd); SDL_RenderFillRect(p->ren,&btnSet); SDL_RenderFillRect(p->ren,&btnCalc); SDL_RenderFillRect(p->ren,&btnClear);
        tx=renderText(p->ren,p->font,"Add Customer",white,w,h); if(tx){SDL_Rect r{btnAdd.x+10,btnAdd.y+8,w,h};SDL_RenderCopy(p->ren,tx,NULL,&r);SDL_DestroyTexture(tx);} 
        tx=renderText(p->ren,p->font,"Set Loan (selected)",white,w,h); if(tx){SDL_Rect r{btnSet.x+10,btnSet.y+8,w,h};SDL_RenderCopy(p->ren,tx,NULL,&r);SDL_DestroyTexture(tx);} 
        tx=renderText(p->ren,p->font,"Calc Payment",white,w,h); if(tx){SDL_Rect r{btnCalc.x+10,btnCalc.y+8,w,h};SDL_RenderCopy(p->ren,tx,NULL,&r);SDL_DestroyTexture(tx);} 
        tx=renderText(p->ren,p->font,"Clear Fields",white,w,h); if(tx){SDL_Rect r{btnClear.x+10,btnClear.y+8,w,h};SDL_RenderCopy(p->ren,tx,NULL,&r);SDL_DestroyTexture(tx);} 

        tx=renderText(p->ren,p->font,"Customers:",white,w,h); if(tx){SDL_Rect r{10,110,w,h};SDL_RenderCopy(p->ren,tx,NULL,&r);SDL_DestroyTexture(tx);} 
        int y=132;
        for(size_t i=0;i<customers.size();++i){ std::string s = std::to_string(i)+": "+customers[i].name; if(customers[i].hasLoan){ std::ostringstream o; o<<(int)customers[i].principal<<"@"<<(int)customers[i].rate<<"%/"<<customers[i].years<<"years"; s += " Loan:"+o.str(); } SDL_Color col = (int)i==p->selected?SDL_Color{200,200,60,255}:white; tx=renderText(p->ren,p->font,s,col,w,h); if(tx){SDL_Rect r{10,y,w,h};SDL_RenderCopy(p->ren,tx,NULL,&r);SDL_DestroyTexture(tx);} y+=22; if(y>460) break; }

        std::ostringstream o; o<<"Last monthly payment: "; if(p->lastPayment>0) o<<std::fixed<<std::setprecision(2)<<p->lastPayment; else o<<"N/A"; tx=renderText(p->ren,p->font,o.str(),white,w,h); if(tx){SDL_Rect r{10,480,w,h};SDL_RenderCopy(p->ren,tx,NULL,&r);SDL_DestroyTexture(tx);} 

        SDL_RenderPresent(p->ren); SDL_Delay(16);
    }
    SDL_StopTextInput(); TTF_CloseFont(p->font); SDL_DestroyRenderer(p->ren); SDL_DestroyWindow(p->win); TTF_Quit(); SDL_Quit();
}
