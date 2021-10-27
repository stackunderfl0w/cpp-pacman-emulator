//
//  Screen.cpp
//  cpp test
//
//  Created by Patrick Iacob on 6/21/19.
//  Copyright © 2019 Patrick Iacob. All rights reserved.
//
#include <iostream>
#include "Screen.h"
#include <SDL2/SDL_ttf.h>
using namespace std;

class Screen{

private:
    SDL_Surface* screenSurface=NULL;
    SDL_Renderer *renderer;
    SDL_Rect screen_rect;
    int timer;
    /*TTF_Font * Sans = TTF_OpenFont("PressStart2P.ttf", 25);
    if (Sans == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }*/
    SDL_Color White = {255, 255, 255};
    SDL_Color palettes[0x100];
    int render_scale=2;

public:
    SDL_Window *window=NULL;
    unsigned char tilerom[0x1000];
    unsigned char spriterom[0x1000];
    unsigned char colorrom[0x20];
    unsigned char paletterom[0x100];
    SDL_Color colors[0x20];
    Screen(){

        window = SDL_CreateWindow("Stackunderfl0w 8080", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 224*render_scale, 288*render_scale, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

        screen_rect.x = 0;
        screen_rect.y = 0;
        //cout<<timer<<endl;
        update();
    }
    ~Screen(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

    }
    void compile_palettes(){
        for (int i = 0; i < 0x100; i++) {
            palettes[i]=getcolor(i);
        }
    }

    void update(){
        //Get window surface
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

        SDL_RenderClear( renderer );

        // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
        SDL_Rect r={timer%640,50,50,50};

        timer++;

        // Set render color to blue ( rect will be rendered in this color )
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );

        // Render rect
        SDL_RenderFillRect( renderer, &r );
        for(int x=0; x<64;x++){
            for(int y=0; y<32;y++){
                r={x*10,y*10,10,10};
                if((x+y)%2==0){
                    SDL_RenderFillRect( renderer, &r );
                }
            }
        }

        // Render the rect to the screen
        SDL_RenderPresent(renderer);
    }
    void render(const unsigned char *disp){
        int width;
        int height;
        unsigned char charac=0;
        SDL_GetWindowSize(window, &width, &height);
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        render_tiles(disp);
        render_sprites(disp);


        SDL_RenderPresent(renderer);
    }


    void render_tiles(const unsigned char *disp){
        for (int x = 0; x < 28; x++) {
            for (int y = 0; y < 32; y++) {
                int temp=0x4040+x*0x20+y;
                int tile=disp[0x4040+x*0x20+y];
                int palette=disp[0x4440+x*0x20+y];
                //tile=charac++;
                int startx=224-x*8-8;
                int starty=24+8*y;
                render_tile(tile,startx,starty,palette);
                //SDL_SetRenderDrawColor(renderer,8*x+y,0,0,0);
                //SDL_Rect r={224-x*8-8, 16+8*y, 8, 8};
                //SDL_RenderFillRect( renderer, &r );


            }
        }
        for (int x = 0; x < 28; x++) {
            for (int y = 0; y < 2; y++) {
                int tile=disp[0x4002+y*0x20+x];
                int palette=disp[0x4402+y*0x20+x];
                //tile=charac++;
                int startx=224-x*8-8;
                int starty=288-8+8*y;
                render_tile(tile,startx,starty,palette);
                //SDL_SetRenderDrawColor(renderer,8*x+y,0,0,0);
                //SDL_Rect r={224-x*8-8, 16+8*y, 8, 8};
                //SDL_RenderFillRect( renderer, &r );
                tile=disp[0x43c2+y*0x20+x];
                palette=disp[0x47c2+y*0x20+x];
                //tile=charac++;
                startx=224-x*8-8;
                starty=8+8*y;
                render_tile(tile,startx,starty,palette);


            }
        }
    }
    void render_tile(int tile,int startx,int starty,int palette){
        render_strip(tile*16,startx,starty, palette);
        render_strip(tile*16+8,startx,starty-4, palette);
        /*std::string str=to_string(palette);
        const char * c = str.c_str();
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, c, White);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
        SDL_Rect Message_rect; //create a rect
        Message_rect.x = startx*2+4;  //controls the rect's x coordinate
        Message_rect.y = starty*2; // controls the rect's y coordinte
        Message_rect.w = 8; // controls the width of the rect
        Message_rect.h = 8;
        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);*/


        /*for (int i = 0; i < 4; i++) {
            SDL_Color temp=getcolor(paletterom[palette*4+i]);
            SDL_SetRenderDrawColor(renderer,temp.r,temp.g,temp.b,temp.a);
            SDL_Rect r={(startx+i)*2, (starty-1)*2, 1*2, 1*2};
            SDL_RenderFillRect( renderer, &r );
        }*/

        /*
        for(int i=0; i<8;i++){
            //int byte=tilerom[(x*16*32+ y*16+(7-i))%0x1000];
            int byte=tilerom[(tile*16)+(7-i)];
            for(int z=0; z<4;z++){
                int pix=((byte>>z)&0x01)|((byte>>(3+z))&0x02);
                if(pix!=0){
                    SDL_SetRenderDrawColor(renderer,47*pix,0,0,0);
                    SDL_Rect r={(startx+i)*2, (starty-z-1)*2, 1*2, 1*2};
                    SDL_RenderFillRect( renderer, &r );
                }
                //cout<<startx+i<<starty-z<<endl;
            }
        }
        for(int i=0; i<8;i++){
            //int byte=tilerom[(x*16*32+ y*16+(7-i))%0x1000];
            int byte=tilerom[(tile*16)+(7-i)+8];
            for(int z=0; z<4;z++){
                int pix=((byte>>z)&0x01)|((byte>>(3+z))&0x02);
                if(pix!=0){
                    SDL_SetRenderDrawColor(renderer,47*pix,0,0,0);
                    SDL_Rect r={(startx+i)*2, ((starty-z-1)-4)*2, 1*2, 1*2};
                    SDL_RenderFillRect( renderer, &r );
                }
                //cout<<startx+i<<starty-z<<endl;
            }
        }
         */
    }

    void render_sprites(const unsigned char *disp){
        for(int i=0; i<8;i++){
            int spritex=disp[0x5060+(2*i)];
            int spritey=disp[0x5061+(2*i)];
            int spritenum=disp[0x4ff0+(i*2)]>>2;
            bool sprite_flip_x=(disp[0x4ff0+(i*2)]>>1)%2;
            bool sprite_flip_y=(disp[0x4ff0+(i*2)])%2;
            int sprite_palette=disp[0x4ff1+(i*2)];
            int sprite_adr=spritenum*64;
            //cout<<spritex<<spritey;
            render_sprite(sprite_adr,spritex,spritey,sprite_flip_x,sprite_flip_y,sprite_palette);
        }
    }
    void render_sprite(int sprite, int startx, int starty,bool flip_x, bool flip_y,int palette){
        startx=224-startx+15;
        starty=288-starty-12;
        //flip_x=false;
        //right side of sprite
        render_sprte_strip(sprite+8,startx+8-8*flip_x,starty+0+12*flip_y,flip_x,flip_y,palette);
        render_sprte_strip(sprite+16,startx+8-8*flip_x,starty+4+4*flip_y,flip_x,flip_y,palette);
        render_sprte_strip(sprite+24,startx+8-8*flip_x,starty+8-4*flip_y,flip_x,flip_y,palette);
        render_sprte_strip(sprite+0,startx+8-8*flip_x,starty+12-12*flip_y,flip_x,flip_y,palette);


        //left side of sprite
        render_sprte_strip(sprite+40,startx+8*flip_x,starty+0+12*flip_y,flip_x,flip_y,palette);
        render_sprte_strip(sprite+48,startx+8*flip_x,starty+4+4*flip_y,flip_x,flip_y,palette);
        render_sprte_strip(sprite+56,startx+8*flip_x,starty+8-4*flip_y,flip_x,flip_y,palette);
        render_sprte_strip(sprite+32,startx+8*flip_x,starty+12-12*flip_y,flip_x,flip_y,palette);



        //char* pixels=new char[16*16*3];
        //SDL_Surface *surface= SDL_CreateRGBSurfaceFrom((void*)pixels,16,16,24,48,0x0000FF,0x00FF00,0xFF0000,0);

    }
    void render_strip(int tile,int startx,int starty,int palette){
        for(int i=0; i<8;i++){
            //int byte=tilerom[(x*16*32+ y*16+(7-i))%0x1000];
            int byte=tilerom[tile+(7-i)];
            for(int z=0; z<4;z++){
                int pix=((byte>>z)&0x01)|((byte>>(3+z))&0x02);
                SDL_Color temp=getcolor(paletterom[(palette*4+pix)&0xff]);
                if(temp.a!=55){
                    SDL_SetRenderDrawColor(renderer,temp.r,temp.g,temp.b,temp.a);
                    SDL_Rect r={(startx+i)*render_scale, (starty-z-1)*render_scale, 1*render_scale, 1*render_scale};
                    SDL_RenderFillRect( renderer, &r );

                }
                //cout<<startx+i<<starty-z<<endl;
            }
        }
    }
    void render_sprte_strip(int sprite,int startx,int starty,bool flip_x, bool flip_y,int palette){
        for(int i=0; i<8;i++){
            //int byte=tilerom[(x*16*32+ y*16+(7-i))%0x1000];
            int byte=spriterom[sprite+(7-i)];
            for(int z=0; z<4;z++){
                int pix=((byte>>z)&0x01)|((byte>>(3+z))&0x02);
                SDL_Color temp=getcolor(paletterom[(palette*4+pix)&0xff]);
                if(temp.a!=55){
                    SDL_SetRenderDrawColor(renderer,temp.r,temp.g,temp.b,0);
                    SDL_Rect r;
                    if(!flip_x){
                        if(!flip_y){
                            r={(startx+i+1)*render_scale, (starty-z-1)*render_scale, 1*render_scale, 1*render_scale};
                        }
                        else{
                            r={(startx+i+1)*render_scale, (starty+z-5)*render_scale, 1*render_scale, 1*render_scale};
                        }
                    }
                    else{
                        if(!flip_y){
                            r={(startx-i+7)*render_scale, (starty-z-1)*render_scale, 1*render_scale, 1*render_scale};
                        }
                        else{
                            r={(startx-i+7)*render_scale, (starty+z-5)*render_scale, 1*render_scale, 1*render_scale};
                        }
                    }


                    SDL_RenderFillRect( renderer, &r );

                }
                //cout<<startx+i<<starty-z<<endl;
            }

        }
    }
    SDL_Color getcolor(int adr){
        int colour=colorrom[adr&0x3f];
        SDL_Color temp;
        temp.r= ((colour>>0)&0x01)*0x21 + ((colour>>1)&0x01)*0x47 + ((colour>>2)&0x01)*0x97;
        temp.g = ((colour>>3)&0x01)*0x21 + ((colour>>4)&0x01)*0x47 + ((colour>>5)&0x01)*0x97;
        temp.b = ((colour>>6)&0x01)*0x51 + ((colour>>7)&0x01)*0xae;
        if (colour==0){
            temp.a=55;
        }
        else{
            temp.a=0;
        }
        return temp;
    }


};
