//
//  main.cpp
//  cpp test
//
//  Created by Patrick Iacob on 6/21/19.
//  Copyright © 2019 Patrick Iacob. All rights reserved.
//

#include <iostream>
#include<SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include "Screen.cpp"
#include "i8080.cpp"
#include "z80.cpp"
#include <chrono>
#include <cmath>
#include <string>
#include <thread>
#include "zip/src/zip.c"
#define systime chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now()-starttime).count()
using namespace std;
int on_extract_entry(const char *filename, void *arg) {
    static int i = 0;
    int n = *(int *)arg;
    printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

    return 0;
}
double round(double value, int places) {
    if (places < 0) {return 0;};

    long factor = (long) pow(10, places);
    value = value * factor;
    long tmp = (long)(value);
    return (double)tmp / factor;
}
void load_file(const char *filename,unsigned char *mem, int adr){
    FILE *file = fopen(filename,"rb");
    if (file != NULL)
    {
        cout<<endl<<"File opened "<<filename<<endl;
    }
    fseek(file, 0L, SEEK_END);
    int fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    unsigned char *buffer = (unsigned char*) malloc(fsize);
    fread(buffer, fsize, 1, file);
    fclose(file);

    cout<<dec<<"     "<<fsize<<" bytes loaded"<<endl;
    //cout<<"     "<<"Header-"<<buffer<<endl; //removed, causes windows alert sound with corrupted headers
    for (int i = 0; i < fsize; ++i){
        mem[i+adr] = buffer[i];
        // Print for debug purposes
        //printf("%02X", memory[0x200 + i]);
        //cout<<endl;
    }
}
//void render(unsigned char*) 
int main(int argv, char** args) {
    cout<<args[0]<<endl;
    auto starttime = chrono::high_resolution_clock::now();
    long long last_fps_update = systime;
    long long last_frame_render = systime;
    long long next_frame=0;
    long long last_frame[60];
    int max_fps=60;
    int clock_speed=3072000;
    int clocks_per_frame=clock_speed/max_fps;

    // insert code here...
    cout << "Hello, World!\n";
    cout<<strstr(args[1],".com")<<endl;
    bool cpm = args[1]!= nullptr && strstr(args[1],".com")!= nullptr;
    cout<<(int)cpm<<endl;
    z80 cpu=z80(cpm);
    SDL_Init(SDL_INIT_VIDEO);
    Screen screen;

    SDL_Event e;
    bool running = true;

    int arg = 2;
    if(args[1]!=NULL){
        string filename=args[1];
        cout<<filename<<endl;
        cout<<filename.find(".zip")<<endl;
        const char * c = filename.c_str();
        if (filename.find(".zip")<0x1000){
            zip_extract(args[1], "tmp", on_extract_entry, &arg);
            load_file("tmp/pacman.6e",cpu.memory,0x0000);
            load_file("tmp/pacman.6f",cpu.memory,0x1000);
            load_file("tmp/pacman.6h",cpu.memory,0x2000);
            load_file("tmp/pacman.6j",cpu.memory,0x3000);
            load_file("tmp/pacman.5e",screen.tilerom,0x0000);
            load_file("tmp/pacman.5f",screen.spriterom,0x0000);
            load_file("tmp/82s123.7f",screen.colorrom,0x0000);
            load_file("tmp/82s126.4a",screen.paletterom,0x0000);
            screen.compile_palettes();
        }
        if (filename.find(".com")<0x1000){
            load_file(c,cpu.memory,0x0100);
            clocks_per_frame*=1000;
        }
        //SDL_Delay(1000);
        cout<<endl<<endl;

    }

    unsigned short po;
    unsigned char* f=(unsigned char*)&po;

    last_frame[0]=5;
     //cpu.load_state("sav.bin");

    while (running){
        if(true){
            //next_frame=systime+1000000/(max_fps);
            next_frame=systime+1000000/(60);

            double fps =(1000000.0/(systime-last_frame[59])*60);
            if(systime - last_fps_update>500000){
                last_fps_update=systime;
                SDL_SetWindowTitle(screen.window,to_string(fps*max_fps/60).c_str());
                //cout<<to_string(fps).c_str()<<endl;
            }


            //cout<<dec<<systime<<endl;
            for (int i = 59; i > 0; --i)
            {
                last_frame[i]=last_frame[i-1];
            }
            last_frame[0]=systime;
            //for (int i = 0; i < 60; i++){cout<<last_frame[i]<<" ";}

            //next_frame=last_frame[0]+1000000000/(int)max_fps+1;
            while (SDL_PollEvent(&e)){
                //cout<<"hello "<<e.type<<endl;
                if (e.type == SDL_QUIT){
                    running = false;
                }
                if (e.type == SDL_KEYDOWN){
                    //running = false;
                    switch(e.key.keysym.sym){
                        //case SDLK_1:chip8.keys[0]=true;break;
                        case SDLK_w:cpu.joystick_up=false;break;
                        case SDLK_a:cpu.joystick_left=false;break;
                        case SDLK_s:cpu.joystick_down=false;break;
                        case SDLK_d:cpu.joystick_right=false;break;
                        case SDLK_o:cpu.one_player_start=false;break;
                        case SDLK_p:cpu.two_player_start=false;break;

                        case SDLK_c:cpu.coin_slot_1=true;break;
                        case SDLK_x:cpu.credit_button=false;break;

                        case SDLK_f:running=false;break;
                        case SDLK_l:cpu.save_state("sav.bin"); break;
                        case SDLK_m:cpu.load_state("sav.bin"); break;
                        case SDLK_q:max_fps=300; break;
                        case SDLK_u:max_fps=3000; break;
                        case SDLK_i:max_fps=30000; break;
                        case SDLK_7:max_fps=30; break;
                    }

                }
                if (e.type == SDL_KEYUP){
                    switch(e.key.keysym.sym){
                        //case SDLK_1:chip8.keys[0]=false;break;
                        case SDLK_w:cpu.joystick_up=true;break;
                        case SDLK_a:cpu.joystick_left=true;break;
                        case SDLK_s:cpu.joystick_down=true;break;
                        case SDLK_d:cpu.joystick_right=true;break;
                        case SDLK_o:cpu.one_player_start=true;break;
                        case SDLK_p:cpu.two_player_start=true;break;

                        case SDLK_c:cpu.coin_slot_1=false;break;
                        case SDLK_x:cpu.credit_button=true;break;
                        case SDLK_q:max_fps=60; break;
                        case SDLK_u:max_fps=60; break;
                        case SDLK_i:max_fps=60; break;
                    }

                }
                if (e.type == SDL_MOUSEBUTTONDOWN){
                    //running = false;
                }
            }
            auto start = chrono::high_resolution_clock::now();

            //while(cpu.cycles<clocks_per_frame/2){
            //    cpu.cycle();
            //}
            //cpu.cycles-=clocks_per_frame/2;
            //cpu.run_innterrupt(0xcf);
            for (int i = 0; i < max_fps/60; ++i) {
                while(cpu.cycles<=clocks_per_frame){//51200
                    cpu.cycle();
                    //cout<<cpu.cycles<<endl;
                }
                cpu.cycles=0;
                cpu.run_maskable_innterrupt(0);
            }



            //for (int i = 0; i < 50000; ++i){cpu.cycle();}
            if ((systime-last_frame_render)/1000>15){
                last_frame_render=systime;
                screen.render(cpu.memory);
            }
            //screen.render(cpu.memory);

            //thread thread_obj(screen.render,cpu.memory);

            auto elapsed = chrono::high_resolution_clock::now() - start;
            long long microseconds = chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

            //cout<<dec<<microseconds<<endl;
            //cout<<dec<<microseconds<<endl;
        }
        unsigned int time_left=(next_frame-systime)/1000;
        if (time_left<1000){
            //cout<<time_left<<endl;
            SDL_Delay(time_left);
        }
    }
    #if defined _WIN32 || defined _WIN64
        system( "rmdir /s /q tmp");
    #else
        system( "rm -rf ./tmp");
    #endif
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("We compiled against SDL version %d.%d.%d ...\n",
           compiled.major, compiled.minor, compiled.patch);
    printf("But we are linking against SDL version %d.%d.%d.\n",
           linked.major, linked.minor, linked.patch);
    return 0;
}

