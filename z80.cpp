#include "z80.h"
#include <iostream>
#include <fstream>
using namespace std;
#define M read_byte(get_hl())
#define GET_BIT(n, val) (((val) >> (n)) & 1)
class z80
{
public:

    private:
    FILE * fp;
    unsigned char interupt_data;
    unsigned char interrupt_vector;
    bool debug= false;
    bool cpm= false;
    long long breakpoint=0;//0x1549420e6;//767742574;
    unsigned char OPCODES_CYCLES[256] = {
//  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
            4,10, 7, 6, 4, 4, 7, 4, 4,11, 7, 6, 4, 4, 7, 4,
            8,10, 7, 6, 4, 4, 7, 4,12,11, 7, 6, 4, 4, 7, 4,
            7,10,16, 6, 4, 4, 7, 4, 7,11,16, 6, 4, 4, 7, 4,
            7,10,13, 6,11,11,10, 4, 7,11,13, 6, 4, 4, 7, 4,
            4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
            4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
            4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
            7, 7, 7, 7, 7, 7, 4, 7, 4, 4, 4, 4, 4, 4, 7, 4,
            4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
            4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
            4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
            4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
            5,10,10,10,10,11, 7,11, 5,10,10, 0,10,17, 7,11, /* cb -> cc_cb */
            5,10,10,11,10,11, 7,11, 5, 4,10,11,10, 0, 7,11, /* dd -> cc_xy */
            5,10,10,19,10,11, 7,11, 5, 4,10, 4,10, 0, 7,11, /* ed -> cc_ed */
            5,10,10, 4,10,11, 7,11, 5, 6,10, 4,10, 0, 7,11  /* fd -> cc_xy */
    };
    //unsigned char OPCODES_CYCLES[256] = {4,10,7,6,5,5,7,4,0,11,7,6,5,5,7,4      ,4,10,7,6,5,5,7,4,4,11,7,6,5,5,7,4      ,4,10,16,6,5,5,7,4,4,11,16,6,5,5,7,4        ,4,10,13,6,10,10,10,4,4,11,13,6,5,5,7,4     ,5,5,5,5,5,5,7,5,5,5,5,5,5,5,7,5        ,5,5,5,5,5,5,7,5,5,5,5,5,5,5,7,5    ,5,5,5,5,5,5,7,5,5,5,5,5,5,5,7,5    ,7,7,7,7,7,7,7,7,5,5,5,5,5,5,7,5    ,4,4,4,4,4,4,7,4,4,4,4,4,4,4,7,4    ,4,4,4,4,4,4,7,4,4,4,4,4,4,4,7,4    ,4,4,4,4,4,4,7,4,4,4,4,4,4,4,7,4    ,4,4,4,4,4,4,7,4,4,4,4,4,4,4,7,4    ,11,10,15,10,18,11,7,11,11,10,15,00,18,17,7,11      ,11,10,15,10,18,11,7,11,11,4,15,10,18,4,7,11    ,11,10,15,4,18,11,7,11,11,4,10,4,18,11,7,11     ,11,10,15,4,18,11,7,11,11,6,15,4,18,4,7,11};
    unsigned char OPCODES_CYCLES_BITS[0x100] = {
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
            8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
            8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
            8, 8, 8, 8, 8, 8,12, 8, 8, 8, 8, 8, 8, 8,12, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8,
            8, 8, 8, 8, 8, 8,15, 8, 8, 8, 8, 8, 8, 8,15, 8
    };

    unsigned char OPCODES_CYCLES_EXTENDED[0x100] = {
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            12,12,15,20,8,14, 8, 9,12,12,15,20, 8,14, 8, 9,
            12,12,15,20,8,14, 8, 9,12,12,15,20, 8,14, 8, 9,
            12,12,15,20,8,14, 8,18,12,12,15,20, 8,14, 8,18,
            12,12,15,20,8,14, 8, 8,12,12,15,20, 8,14, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            16,16,16,16,8, 8, 8, 8,16,16,16,16, 8, 8, 8, 8,
            16,16,16,16,8, 8, 8, 8,16,16,16,16, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
    };
    unsigned char OPCODES_CYCLES_IXIY[0x100] = {
            4+4,10+4, 7+4, 6+4, 4+4, 4+4, 7+4, 4+4, 4+4,11+4, 7+4, 6+4, 4+4, 4+4, 7+4, 4+4,
            8+4,10+4, 7+4, 6+4, 4+4, 4+4, 7+4, 4+4,12+4,11+4, 7+4, 6+4, 4+4, 4+4, 7+4, 4+4,
            7+4,10+4,16+4, 6+4, 4+4, 4+4, 7+4, 4+4, 7+4,11+4,16+4, 6+4, 4+4, 4+4, 7+4, 4+4,
            7+4,10+4,13+4, 6+4,23  ,23  ,19  , 4+4, 7+4,11+4,13+4, 6+4, 4+4, 4+4, 7+4, 4+4,
            4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4, 4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4,
            4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4, 4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4,
            4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4, 4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4,
            19 ,19  ,19  ,19  ,19  ,19  , 4+4,19  , 4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4,
            4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4, 4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4,
            4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4, 4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4,
            4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4, 4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4,
            4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4, 4+4, 4+4, 4+4, 4+4, 4+4, 4+4,19  , 4+4,
            5+4,10+4,10+4,10+4,10+4,11+4, 7+4,11+4, 5+4,10+4,10+4, 0  ,10+4,17+4, 7+4,11+4, /* cb -> cc_xycb */
            5+4,10+4,10+4,11+4,10+4,11+4, 7+4,11+4, 5+4, 4+4,10+4,11+4,10+4, 4  , 7+4,11+4, /* dd -> cc_xy again */
            5+4,10+4,10+4,19+4,10+4,11+4, 7+4,11+4, 5+4, 4+4,10+4, 4+4,10+4, 4  , 7+4,11+4, /* ed -> cc_ed */
            5+4,10+4,10+4, 4+4,10+4,11+4, 7+4,11+4, 5+4, 6+4,10+4, 4+4,10+4, 4  , 7+4,11+4  /* fd -> cc_xy again */
    };
    unsigned char OPCODES_CYCLES_XYBITS[0x100] = {
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
            20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
            20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
            20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,
            23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23
    };
    int par_table[256]={1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1};
    int codes[256];
    unsigned short pc=0;
    
    //int memory[0x1000];
    bool carry=false;//bit0
    bool n=false;//bit1
    bool parity=false;//bit2
    bool f3=false;//bit3
    bool halfcarry=false;//bit4
    bool f5=false;//bit5
    bool zero=false;//bit6
    bool sign=false;//bit7

    bool carry_prime=false;//bit0
    bool n_prime=false;//bit1
    bool parity_prime=false;//bit2
    bool f3_prime=false;//bit3
    bool halfcarry_prime=false;//bit4
    bool f5_prime=false;//bit5
    bool zero_prime=false;//bit6
    bool sign_prime=false;//bit7
    bool interrupts_enabled;
    bool ffIFF1=false;
    bool ffIFF2=false;
    bool halt=false;
    
    long long tc=-1;
    long long last_write;
    unsigned char lbr=0;

    unsigned char B=0;
    unsigned char C=0;
    unsigned char D=0;
    unsigned char E=0;
    unsigned char H=0;
    unsigned char L=0;
    unsigned char A=0;

    unsigned char B_prime=0;
    unsigned char C_prime=0;
    unsigned char D_prime=0;
    unsigned char E_prime=0;
    unsigned char H_prime=0;
    unsigned char L_prime=0;
    unsigned char A_prime=0;


    unsigned char iyl=0;
    unsigned char iyh=0;

    unsigned char ixl=0;
    unsigned char ixh=0;

    unsigned short index;

    unsigned short sp=0;

    unsigned short wz;//internal register

    unsigned char i;
    unsigned char r;
    int interrupt_mode=0;



    bool extra_ship_location=0;//0=1500 1=1000 points
    bool display_coin_in_demo=0;//0=ON
    bool extra_ships=0;//extra starting ships 0-3(3-6 total)
    unsigned char port_2;
    unsigned char port_4lo;
    unsigned char port_4hi;
    // paired registers helpers (setters and getters)
                
    inline void set_af(unsigned short int val) {
        A = val >> 8;
        set_psw (val & 0xFF);
    }
    inline void set_bc(unsigned short int val) {
        B = val >> 8;
        C = val & 0xFF;
    }

    inline void set_de(unsigned short int val) {
        D = val >> 8;
        E = val & 0xFF;
    }

    inline void set_hl(unsigned short int val) {
        H = val >> 8;
        L = val & 0xFF;
    }

    inline void set_af_prime(unsigned short int val) {
        A_prime = val >> 8;
        set_psw_prime (val & 0xFF);
    }
    inline void set_bc_prime(unsigned short int val) {
        B_prime = val >> 8;
        C_prime = val & 0xFF;
    }

    inline void set_de_prime(unsigned short int val) {
        D_prime = val >> 8;
        E_prime = val & 0xFF;
    }

    inline void set_hl_prime(unsigned short int val) {
        H_prime = val >> 8;
        L_prime = val & 0xFF;
    }
    inline void set_ix(unsigned short int val) {
        ixh = val >> 8;
        ixl = val & 0xFF;
    }
    inline void set_iy(unsigned short int val) {
        iyh = val >> 8;
        iyl = val & 0xFF;
    }

    inline unsigned short int get_af() {
        return (A << 8) | get_psw();
    }

    inline unsigned short int get_bc() {
        return (B << 8) | C;
    }

    inline unsigned short int get_de() {
        return (D << 8) | E;
    }

    inline unsigned short int get_hl() {
        return (H << 8) | L;
    }

    inline unsigned short int get_af_prime() {
        return (A_prime << 8) | get_psw_prime();
    }
    inline unsigned short int get_bc_prime() {
        return (B_prime << 8) | C_prime;
    }

    inline unsigned short int get_de_prime() {
        return (D_prime << 8) | E_prime;
    }

    inline unsigned short int get_hl_prime() {
        return (H_prime << 8) | L_prime;
    }
    inline unsigned short int get_ix() {
        return (ixh << 8) | ixl;
    }
    inline unsigned short int get_iy() {
        return (iyh << 8) | iyl;
    }

    inline unsigned char get_psw() {
        return(sign<<7)|(zero<<6)|(f5<<5)|(halfcarry<<4)|(f3<<3)|(parity<<2)|(n<<1)|carry;
    }
    inline unsigned char get_psw_prime() {
        return(sign_prime<<7)|(zero_prime<<6)|(f5_prime<<5)|(halfcarry_prime<<4)|(f3_prime<<3)|(parity_prime<<2)|(n_prime<<1)|carry_prime;
    }

    inline void set_psw(unsigned char x) {
        sign=(x>>7);
        zero=(x>>6)&1;
        f5=(x>>5)&1;
        halfcarry=(x>>4)&1;
        f3=(x>>3)&1;
        parity=(x>>2)&1;
        n=(x>>1)&1;
        carry=x&1;
    }
    inline void set_psw_prime(unsigned char x) {
        sign_prime=(x>>7);
        zero_prime=(x>>6)&1;
        f5_prime=(x>>5)&1;
        halfcarry_prime=(x>>4)&1;
        f3_prime=(x>>3)&1;
        parity_prime=(x>>2)&1;
        n_prime=(x>>1)&1;
        carry_prime=x&1;
    }
    unsigned char read_byte(unsigned short adr){
        if(cpm){
            return memory[adr];
        }
        adr&=0x7fff;
        if(adr>=0x5000&&adr<0x5100){
            //cout<<"ioread at "<<tc<< " "<<pc<<" to "<<adr<<endl;
            switch(adr){
                case 0x5000:
                    return (joystick_up|(joystick_left<<1)|(joystick_right<<2)|(joystick_down<<3)|(rack_advance<<4)|(coin_slot_1<<5)|(coin_slot_2<<6)|(credit_button<<7));//in01
                case 0x5040:
                    return (joystick_up2|(joystick_left2<<1)|(joystick_right2<<2)|(joystick_down2<<3)|(board_test<<4)|(one_player_start<<5)|(two_player_start<<6)|(cabinet_mode<<7));//in02
                case 0x5080:return 0xc9;//dip1
                case 0x50c0:return 0xff;//dip2
                default:return 0;
            }
        }
        else if(adr>=0x5100){
            return 0;
        }
        else{
            return memory[adr];
        }
    }
    void write_byte(unsigned short adr,unsigned char x){
        if(cpm){
            memory[adr]=x;
            lbr=x;

            return;
        }
        adr&=0x7fff;
        if (adr>0x4000&&adr<0x4800){
            //cout<<"write to grapics memory at "<<adr<<endl;
        }
        if(adr<0x4000&&!cpm){
            cout<<"attempted to overwrite rom"<<" to "<<adr<<endl;
        }
        else if(adr>=0x5000&&adr<0x5100){
            //cout<<"iowrite at "<<tc<< " "<<pc<<" to "<<adr<<endl;
            if(adr==0x5000){
                interrupts_enabled=x&1;
            }
        }
        else{
            memory[adr]=x;
        }
        if(adr>=0x5060&&adr<0x5070){
            memory[adr]=x;
        }


    }
    void write_word(unsigned short adr,unsigned short x){
        write_byte(adr,x & 0xFF);
        write_byte(adr + 1, x >> 8);
    }
    unsigned char next_byte() {
        return read_byte(pc++);
    }
    unsigned short next_word(){
        unsigned short result=read_word(pc);
        pc+=2;
        return result;
    }
    unsigned short read_word(unsigned short adr) {
        return read_byte(adr + 1) << 8 |read_byte(adr);
    }
    void inc(unsigned char& x){
        set_psw(carry);
        x++;
        halfcarry = (x & 0xF) == 0;
        check_ZSP(x);
        parity=(x==0x80);
        set_35(x);
    }
    void DCR(unsigned char& x){
        set_psw(carry);
        x--;
        halfcarry = (x & 0xF) == 0xF;
        check_ZSP(x);
        parity=(x==0x7f);
        n=true;
        set_35(x);
    }
    void ld(unsigned char& x,unsigned char y){
        x=y;
    }
    void jump(unsigned short adr){
        pc=adr;
        wz=adr;
    }
    void cond_jump(bool cond){
        unsigned short adr=next_word();
        if(cond){
            jump(adr);
        }
        wz=adr;
        //else{cycles-=5;}
    }
    void relative_jump(signed char adr_ofset){
        pc+=adr_ofset;
        wz=pc;
    }
    void relative_cond_jump(bool cond){
        unsigned char adr_ofset=next_byte();
        if(cond){
            relative_jump(adr_ofset);
        }
        wz=pc;
        //else{cycles-=5;}
    }
    void DJNZ(unsigned char& x){
        //DCR(x);
        if(x-=1!=0){
            relative_jump(next_byte());
        }
        else{
            pc++;
        }

    }
    void call(unsigned short adr){
        PUSH(pc);
        jump(adr);
        wz=adr;
    }
    void cond_call(bool cond){
        unsigned short adr=next_word();
        if(cond){
            call(adr);
            cycles+=7;
        }
        wz=adr;
        //else{cycles-=7;}
    }
    void ret(){
        pc=POP();
        wz=pc;
    }
    void cond_ret(bool cond){
        if(cond){
            ret();
            cycles+=6;
        }
        //else{cycles-=6;}
    }
    static inline bool calc_carry(int bit_no, uint16_t a, uint16_t b, bool cy) {

        int32_t result = a + b + cy;

        int32_t carry = result ^ a ^ b;

        return carry & (1 << bit_no);

    }
    unsigned char add8(unsigned char x,unsigned char y,bool cy){
        unsigned char result=x+y+cy;
        sign=result>>7;
        zero=result==0;
        halfcarry=calc_carry(4,x,y,cy);
        parity=(calc_carry(7, x, y, cy) != calc_carry(8, x, y, cy));
        carry=calc_carry(8,x,y,cy);
        n=false;
        set_35(result);
        x=result;
        return result;
    }
    unsigned char sub8(unsigned char x, unsigned char y,bool cy){
        unsigned char result=add8(x,~y,!cy);
        carry=!carry;
        halfcarry=!halfcarry;
        n=true;
        x=result;
        return result;
    }
    void ANA(unsigned char& x,unsigned char y){
        set_psw(0);
        halfcarry = true;
        x&=y;
        check_ZSP(x);
        set_35(x);
    }
    void XRA(unsigned char& x,unsigned char y){
        set_psw(0);
        x^=y;
        check_ZSP(x);
        set_35(x);
    }
    void ORA(unsigned char& x,unsigned char y){
        set_psw(0);
        halfcarry=false;
        x|=y;
        check_ZSP(x);
        set_35(x);
    }
    void CMP(unsigned char x, unsigned char y){
        carry=y>x;
        unsigned char z=x-y;
        n=true;
        set_35(y);
        halfcarry=((((x & 0x0f) - (y & 0x0f)) & 0x10) != 0);
        check_ZSP(z);
        parity=(((x ^ y) & (x ^ z)) > 0x7f);
    }
    unsigned short adc16(unsigned short x,unsigned short y){
        unsigned short result= x+(y+carry);
        bool cy=((int)x+y-carry)>0xffff;
        halfcarry = (((result ^ x ^ y) & 0x1000) != 0);
        f3=((result & (0x08 << 8)) != 0);
        f5=((result & (0x20 << 8)) != 0);
        sign = (result & 0x8000)!=0;
        zero=result==0;
        carry=cy;
        parity =(((x ^ ~y) & (x ^ result)) > 0x7fff);
        n=false;
        return result;

    }

    unsigned short sbc16(unsigned short x,unsigned short y){
        unsigned short result= x-(y+carry);
        bool cy=((int)x-(y+carry))<0;
        halfcarry = ((((x & 0x0fff) - (y & 0x0fff) - carry) & 0x1000) != 0);
        f3=((result & (0x08 << 8)) != 0);
        f5=((result & (0x20 << 8)) != 0);
        sign = (result & 0x8000)!=0;
        zero=result==0;
        carry=cy;
        parity=(((x ^ y) & (x ^ result)) > 0x7fff);
        n=true;
        wz=x+1;
        return result;
    }
    unsigned short add16(unsigned short x, unsigned short y){
        unsigned short result= x+y;
        carry=((int)x+y>0xffff);
        n=false;
        halfcarry=((result & 0x0fff) < (x & 0x0fff));
        f3=((result & (0x08 << 8)) != 0);
        f5=((result & (0x20 << 8)) != 0);
        wz=x+1;
        return  result;
    }

    void PUSH(unsigned short x){
        sp-=2;
        write_word(sp,x);
        //write_byte(--sp,x >> 8);
        //write_byte(--sp,x & 0xFF);
    }
    unsigned short POP(){
        unsigned short temp=(read_byte(sp+1) << 8) | read_byte(sp);
        sp+=2;
        return temp;
    }
    void rlca(unsigned char& x){
        carry=x>>7;
        x=(x<<1)|carry;
        halfcarry=false;
        n=false;
        set_35(x);

    }
    void rla(unsigned char& x){
        bool cy = carry;
        carry = x >> 7;
        x = (x << 1) | cy;
        halfcarry=false;
        n=false;
        set_35(x);
    }
    void rrca(unsigned char& x){
        carry=x&0b1;
        x=(x>>1)|(carry<<7);
        halfcarry=false;
        n=false;
        set_35(x);
    }
    void rra(unsigned char& x){
        bool cy = carry;
           carry=x&0x1;
        x = (x >> 1) | (cy << 7);
        halfcarry=false;
        n=false;
        set_35(x);
    }
    unsigned char rlc(unsigned char x){
        n=false;
        halfcarry=false;
        carry = x >> 7;
        x = (x << 1) | carry;

        check_ZSP(x);
        set_35(x);
        return x;
    }
    unsigned char rrc(unsigned char x){
        n=false;
        halfcarry=false;
        carry = x&0b1;
        x = (x >> 1) | (carry<<7);

        check_ZSP(x);
        set_35(x);
        return x;
    }
    unsigned char rl(unsigned char x){
        n=false;
        halfcarry=false;
        bool cy=carry;
        carry = x >> 7;
        x = (x << 1) | cy;

        check_ZSP(x);
        set_35(x);
        return x;
    }

    unsigned char rr(unsigned  char x){
        /*n= false;
        halfcarry= false;
        bool cy=carry;
        x = (x << 1) | (cy<<7);
        check_ZSP(x);
        set_35(x);
        return x;*/
        const bool c = carry;
        carry = x & 1;
        x = (x >> 1) | (c << 7);
        sign = x >> 7;
        zero = x == 0;
        n = false;
        halfcarry = false;
        check_parity(x);
        set_35(x);
        return x;
    }


    unsigned char sla(unsigned char x){
        n=false;
        halfcarry=false;
        carry = x >> 7;
        x = (x << 1);

        check_ZSP(x);
        set_35(x);
        return x;
    }
    unsigned char sll(unsigned char x){
        n=false;
        halfcarry=false;
        carry = x >> 7;
        x = (x << 1)|1;

        check_ZSP(x);
        set_35(x);
        return x;
    }
    unsigned char sra(unsigned char x){
        n= false;
        halfcarry= false;
        carry=x&1;
        x=(x>>1)|(x&0x80);
        check_ZSP(x);
        set_35(x);
        return x;
    }
    unsigned char srl(unsigned char x){
        n=false;
        halfcarry=false;
        carry=x&0b1;
        x>>=1;
        check_ZSP(x);
        set_35(x);
        return x;
    }
    void DAA(unsigned char& x){
        unsigned char correction=0;
        if ((A & 0x0F) > 0x09 || halfcarry) {
            correction += 0x06;
        }
        if (A > 0x99 || carry) {
            correction += 0x60;
            carry = true;
        }
        if(n){
            halfcarry = halfcarry && (A & 0x0F) < 0x06;
            A-=correction;
        }
        else{
            halfcarry = (A & 0x0F) > 0x09;
            A += correction;
        }
        sign=A>>7;
        zero=A==0;
        check_parity(A);
        set_35(A);
    }
    inline void check_zero(unsigned char x){
        zero = x==0;
    }
    inline void check_sign(unsigned char x){
        sign = (x & 0x80)!=0;
    }
    inline void check_parity(unsigned char x){
        parity = par_table[x]==1;
    }
    inline void check_ZSP(unsigned char x){
        check_zero(x);
        check_sign(x);
        check_parity(x);
    }
    void set_35(unsigned char x){
        f3=x&0x8;
        f5=x&0x20;
    }
    void testbit(int ofset,unsigned char reg){
        reg=reg&(1<<ofset);
        n=false;
        halfcarry=true;
        //sign unknown
        //p/v unknown
        //sign=false;
        //zero=((reg>>ofset)&1)==0;
        sign=reg>>7;
        zero=reg==0;
        parity=zero;

        set_35(reg);
    }
    unsigned char setbit(int ofset,unsigned char reg){
        return reg|(1<<ofset);
    }
    unsigned char resetbit(int ofset,unsigned char reg){
        return reg& ~(1 << ofset);
    }

    inline void check_carry(unsigned char x){
        carry =(x & 0b100000000) != 0;
    }
    void ldi(){
        write_byte(get_de(),read_byte( get_hl()));
        unsigned short temp=read_byte(get_hl())+A;
        set_hl(get_hl()+1);
        set_de(get_de()+1);
        set_bc(get_bc()-1);
        bool s=sign;
        bool z=zero;
        bool c=carry;
        set_psw(0);
        sign=s;
        zero=z;
        carry=c;
        parity=get_bc()!=0;
        f5=((temp&0x2)!=0);
        f3=((temp&0x8)!=0);
        //fprintf(fp,
        //        "temp: %08X\n",
        //        temp);
    }
    void cpd(){
        cpi();
        set_hl(get_hl()-2);
        wz-=2;
    }
    void cpi(){
        bool cy=carry;
        unsigned char result=sub8(A,read_byte(get_hl()),0);
        set_hl(get_hl()+1);
        set_bc(get_bc()-1);
        parity=get_bc()!=0;
        carry=cy;
        result-=halfcarry;
        f5=((result&0x2)!=0);
        f3=((result&0x8)!=0);
        wz++;
    }
    void ldd(){
        ldi();
        set_hl(get_hl()-2);
        set_de(get_de()-2);

    }

    unsigned char in(unsigned char port){
        cout<<"read ioport "<<(int)port<<endl;
        switch(port){
            case 1:
                //get_key();
                //System.out.println(coin+(two_player_button<<1)+(one_player_button<<2)+(1<<3)+(player_one_fire<<4) +(player_one_left<<5)+(player_one_right<<6));
                //return coin|(two_player_button<<1)|(one_player_button<<2)|(1<<3)|(player_one_fire<<4)|(player_one_left<<5)|(player_one_right<<6);
            case 2:
                //return extra_ships|(extra_ship_location<<3)|(player_two_fire<<4)|(player_two_left<<5)|(player_two_right<<6)|(display_coin_in_demo<<7);

            case 3:
                return (((((int)port_4hi<<8)|(int)port_4lo)<<port_2)>>8);

                default:return 0;
        }
        return 0;
    }
    //takes input from cpu (used for bitshift register input and sound);
    void out(unsigned char a,unsigned char port){
        cout<<(int)port<<" "<<(int)a<<endl;
        switch(port){
            case 0:
                interrupt_vector=a;
                break;
            case 2:
                port_2=a;
                break;
            case 3://play sound
                //since the original sound hardware is analogue we need to check if the sound is already suppost to be
                // playing or else it will run multiple times stack and slow down the entire program
                //special case, ufo sfx needs to loop
                /*if (a%2==1){if(!sounds_playing[0]|x>65){ Main.screen.playAudio(sounds[0]);sounds_playing[0]=true;x=0;} }
                else{sounds_playing[0]=false;}
                x++;
                if ((a>>1)%2==1){if(!sounds_playing[1]){ Main.screen.playAudio(sounds[1]);sounds_playing[1]=true;} }
                else{sounds_playing[1]=false;}
                if ((a>>2)%2==1){if(!sounds_playing[2]){ Main.screen.playAudio(sounds[2]);sounds_playing[2]=true;} }
                else{sounds_playing[2]=false;}
                if ((a>>3)%2==1){if(!sounds_playing[3]){ Main.screen.playAudio(sounds[3]);sounds_playing[3]=true;} }
                else{sounds_playing[3]=false;}
                //if ((a>>4)%2==1){if(!sounds_playing[4]){ Main.screen.playAudio(sounds[4]);sounds_playing[4]=true;} }
                //else{sounds_playing[4]=false;}*/
                break;
            case 4://input for bitshift register
                port_4lo=port_4hi;
                port_4hi=a;
                break;
            case 5://play sound 2(secord set of sounds)
                /*if (a%2==1){if(!sounds_playing[5]){ Main.screen.playAudio(sounds[5]);sounds_playing[5]=true;} }
                else{sounds_pld6aying[5]=false;}
                if ((a>>1)%2==1){if(!sounds_playing[6]){ Main.screen.playAudio(sounds[6]);sounds_playing[6]=true;} }
                else{sounds_playing[6]=false;}
                if ((a>>2)%2==1){if(!sounds_playing[7]){ Main.screen.playAudio(sounds[7]);sounds_playing[7]=true;} }
                else{sounds_playing[7]=false;}
                if ((a>>3)%2==1){if(!sounds_playing[8]){ Main.screen.playAudio(sounds[8]);sounds_playing[8]=true;} }
                else{sounds_playing[8]=false;}
                if ((a>>4)%2==1){if(!sounds_playing[9]){ Main.screen.playAudio(sounds[9]);sounds_playing[9]=true;} }
                else{sounds_playing[9]=false;}*/
                break;
            default:break;
        }
    }
public:
    bool joystick_up=true;
    bool joystick_left=true;
    bool joystick_right=true;
    bool joystick_down=true;
    bool rack_advance=true;
    bool coin_slot_1=false;
    bool coin_slot_2=false;
    bool credit_button=false;

    bool joystick_up2=true;
    bool joystick_left2=true;
    bool joystick_right2=true;
    bool joystick_down2=true;
    bool board_test=true;//1=off 0=on
    bool one_player_start=true;
    bool two_player_start=true;
    bool cabinet_mode=false;
    bool keys[16];
    unsigned char memory[0x10000];
    long long cycles;
    z80(bool c=false){
        cpm=c;
        fp = fopen ("file.txt", "a");

        cycles=0;
        for (int i = 0; i < 16; ++i){
            keys[i]=false;
        }
        for (int i = 0; i < 256; ++i)
        {
            codes[i]=0;
        }
        for(int i=0;i<0x10000;i++){
            memory[i]=0;
        }
        sp=0;
        if(cpm){
            for(int i=0;i<0x10000;i++){
                memory[i]=0;
            }
            if(cpm){
                pc=0x100;
                //memory[5]=0xc9;
                //sp=0x2400;
                // inject "out 1,a" at 0x0000 (signal to stop the test)
                memory[0x0000] = 0xD3;
                memory[0x0001] = 0x00;
                //memory[0x0002] = 0x01;

                // inject "in a,0" at 0x0005 (signal to output some characters)
                //memory[0x0005] = 0xDB;
                //memory[0x0006] = 0x00;
                //memory[0x0007] = 0xC9;
                memory[0x0005] = 0xDB;
                memory[0x0006] = 0x00;
                memory[0x0007] = 0xC9;
                set_af(0xffff);set_bc(0xffff);set_de(0xffff);set_hl(0xffff);set_ix(0xffff);set_iy(0xffff);sp=0xffff;
            }
        }
        cout<<"     "<<"Starting opcode "<<hex<<((memory[pc])<<8) + memory[pc+1]<<endl;
    }
    ~z80(){
        fclose(fp);
    }
    void save_state(const char * file){
        cout<<endl<<"saved state"<<endl;
        ofstream fout;
        unsigned char test[24]={A,get_psw(),B,C,D,E,H,L,
                                A_prime,get_psw_prime(),B_prime,C_prime,D_prime,E_prime,H_prime,L_prime,
                                ixh,ixl,iyh,iyl,static_cast<unsigned char>(sp>>8),static_cast<unsigned char>(sp&0xff),
                                static_cast<unsigned char>(pc>>8),static_cast<unsigned char>(pc&0xff)};
        fout.open(file, ios::binary | ios::out);

        //int a[4] = {100023, 23, 42, 13};
        fout.write((char*) &memory, sizeof(memory));
        fout.write((char*) &test, sizeof(test));

        fout.close();
        //cout<<(int)A;
    }
    void load_state(const char *filename){
        FILE *file = fopen(filename,"rb");
        fseek(file, 0L, SEEK_END);
        int fsize = ftell(file);
        fseek(file, 0L, SEEK_SET);
        unsigned char *buffer = (unsigned char*) malloc(fsize);
        fread(buffer, fsize, 1, file);
        fclose(file);

        cout<<dec<<"     "<<fsize<<" bytes loaded"<<endl;
        //cout<<"     "<<"Header-"<<buffer<<endl; //removed, causes windows alert sound with corrupted headers
        for (int i = 0; i < fsize-24; ++i){
            memory[i] = buffer[i];
        }
        int temp=fsize-24;
        A=buffer[temp];
        set_psw(buffer[temp+1]);
        B=buffer[temp+2];
        C=buffer[temp+3];
        D=buffer[temp+4];
        E=buffer[temp+5];
        H=buffer[temp+6];
        L=buffer[temp+7];

        A_prime=buffer[temp+8];
        set_psw_prime(buffer[temp+9]);
        B_prime=buffer[temp+10];
        C_prime=buffer[temp+11];
        D_prime=buffer[temp+12];
        E_prime=buffer[temp+13];
        H_prime=buffer[temp+14];
        L_prime=buffer[temp+15];

        ixh=buffer[temp+16];
        ixl=buffer[temp+17];
        iyh=buffer[temp+18];
        iyl=buffer[temp+19];
        sp=(buffer[temp+20]<<8)|buffer[temp+21];
        pc=(buffer[temp+22]<<8)|buffer[temp+23];

    }
    void cycle(){
        if (tc==2000){
            //cout<<tc<<" "<<pc<<endl;
        }
        tc++;
        //if(tc==0x90000000){cout<<endl;exit(0);}
        //if(tc==2000000){debug=true;}
        if(debug){
            cout<<hex<<"True counter:"<<tc<<endl;
            cout<<dec<<"cycles:"<<cycles<<endl;
            cout<<hex<<"    Program counter:"<<pc<<endl;
            cout<<"    "<<get_af()<<" "<<get_bc()<<" "<<get_de()<<" "<<get_hl()<<" "<<sp<<" ";
            if(zero){cout<<"z";}
            if(sign){cout<<"s";}
            if(halfcarry){cout<<"h";}
            if(parity){cout<<"p";}
            if(carry){cout<<"c";}
            cout<<endl;
            cout<<"    Opcode:"<<(int)memory[pc]<<"   "<<(int)memory[pc+1]<<" "<<(int)memory[pc+2]<<endl;
            try{
                cout<<"    "<<(int)M<<endl;
            }
            catch (int e)
            {
                cout << "An exception occurred. Exception Nr. " << e << '\n';
            }
        }
        if(breakpoint!=0){
            if(tc==breakpoint){
                cout<<last_write<<endl;
                exit(0);
            }
            if (tc>=0x3000){
                //debug=true;
            }
            if(tc>=0x155ece7b3){
                unsigned char temp=0;
                for (int j = 0x4400; j <0x4800 ; ++j) {
                    //temp+=memory[j];
                }
                fprintf(fp,//                        "TC: %10X, PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, IX: %04X, IY: %04X, M: %02X, OP: %08X CYC: %04X\n",
                        "TC: %016X, PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, IX: %04X, IY: %04X, M: %02X, CYC: %04X, LBR: %04X\n",
                        tc,
                        pc,
                        get_af(),
                        get_bc(),
                        get_de(),
                        get_hl(),
                        sp,
                        get_ix(),
                        get_iy(),
                        read_byte(get_hl()),
                        read_byte(pc)<<24   |read_byte(pc+1)<<16|read_byte(pc+2)<<8|read_byte(pc+3),lbr, 0);
            }
        }


        //cout<<(int)memory[pc];
        if(cpm){
            if(pc==5){
                if(C==9){
                    int z=0;
                    while((char)memory[get_de()+z]!='$'){
                        cout<<((unsigned char)memory[get_de()+z]);
                        z++;
                    }
                    cout<<"("<<tc<<")";
                }
                else if (C==2){
                    cout<<(char)E;
                }
                //cout<<dec<<tc<<hex<<endl;
                //cout<<tc<<endl;
            }
            if(pc==0){
                cout<<endl<<"exiting cpm"<<endl;
                exit(0);
            }
        }
        if(!halt){
            unsigned char opcode=next_byte();
            if(codes[opcode]==0) {
                codes[opcode] = tc;
            }

            run_op(opcode);
            cycles+=OPCODES_CYCLES[opcode];
        }else{
            cycles+=4;
        }

    }
    void run_maskable_innterrupt(unsigned char x){
        //cout<<(int)read_byte(0x5000);
        if (interrupts_enabled&ffIFF1){
            //cout<<"interrupt"<<endl;
            //fprintf(fp,"hello irq %02X\n",interrupt_vector);
            if (halt){
                halt=false;
                pc++;
            }
            if(ffIFF1){
                ffIFF2=ffIFF1=false;
                if(interrupt_mode==0){
                    run_op(x);
                }
                else if(interrupt_mode==2){
                    call(read_word(i<<8|interrupt_vector));
                }
            }
        }

    }
    void run_op(unsigned char opcode){
        //int d16=(op3<<8)+d8;
        cout<<hex;
        switch(opcode){
            case 0x0://nop
                break;
            case 0x1:
                set_bc(next_word());
                break;
            case 0x2://stax b
                write_byte(get_bc(),A);
                wz=(A << 8)|((get_bc()+1)&0xFF);
                break;
            case 0x3://inc bc
                set_bc(get_bc()+1);
                break;
            case 0x4://inc b
                inc(B);
                break;
            case 0x5://dec b
                DCR(B);
                break;
            case 0x6://mvi b d8
                B=next_byte();
                break;
            case 0x7://rlca
                rlca(A);
                break;
            case 0x8://EX AF,AF'
            {
                unsigned short temp=get_af();
                set_af(get_af_prime());
                set_af_prime(temp);
            }

                break;
            case 0x9://dad bc
                set_hl(add16(get_hl(),get_bc()));
                break;
            case 0xa://ldax b
                A=read_byte(get_bc());
                wz=get_bc()+1;
                break;
            case 0xb://dcx b
                set_bc(get_bc()-1);
                break;
            case 0xc://inc c
                inc(C);
                break;
            case 0xd://dcr c
                DCR(C);
                break;
            case 0xe://mvi c d8
                C=next_byte();
                break;
            case 0xf://rrc a
                rrca(A);
                break;
            case 0x10://djnz
                DJNZ(B);
                break;
            case 0x11://lxi d d16
                set_de(next_word());
                break;
            case 0x12://stax d
                write_byte(get_de(),A);
                wz=(A << 8)|((get_de()+1)&0xFF);
                break;
            case 0x13://inc de
                set_de(get_de()+1);
                break;
            case 0x14://inc d
                inc(D);
                break;
            case 0x15://dcr d
                DCR(D);
                break;
            case 0x16://mvi d
                D=next_byte();
                break;
            case 0x17://ral
                rla(A);
                break;
            case 0x18://jr d8
                relative_jump(next_byte());
                break;
            case 0x19://dad d
                set_hl(add16(get_hl(),get_de()));
                break;
            case 0x1a://ldax d
                A=read_byte(get_de());
                wz=get_bc()+1;
                break;
            case 0x1b://dcx d
                set_de(get_de()-1);
                break;
            case 0x1c://inc e
                inc(E);
                break;
            case 0x1d://dcr e
                DCR(E);
                break;
            case 0x1e://mvi e d8
                E=next_byte();
                break;
            case 0x1f://rar
                rra(A);
                break;
            case 0x20://Jr nz d8
                relative_cond_jump(!zero);
                break;
            case 0x21://lxi hl d16
                set_hl(next_word());
                break;
            case 0x22://shld d16
            {
                unsigned short temp = next_word();
                write_word(temp,get_hl());
                wz=temp+1;
                break;
            }

            case 0x23://inc hl
                set_hl(get_hl()+1);
                break;
            case 0x24://inc h
                inc(H);
                break;
            case 0x25://dcr h
                DCR(H);
                break;
            case 0x26://mvi h d8
                H=next_byte();
                break;
            case 0x27://daa Here be dragons
                DAA(A);
                break;
            case 0x28://jr z d8
                relative_cond_jump(zero);
                break;
            case 0x29://dad hl
                set_hl(add16(get_hl(),get_hl()));
                break;
            case 0x2a://lhld d16
            {
                unsigned short temp=next_word();
                set_hl(read_word(temp));
                wz=temp+1;
                break;
            }

            case 0x2b://dcx hl
                set_hl(get_hl()-1);
                break;
            case 0x2c://inc l
                inc(L);
                break;
            case 0x2d://dcr l
                DCR(L);
                break;
            case 0x2e://mvi l d8
                L=next_byte();
                break;
            case 0x2f://cpl
                A^=0xff;
                n=true;
                halfcarry=true;
                set_35(A);
                break;
            case 0x30://jr nc d8
                relative_cond_jump(!carry);
                break;
            case 0x31://lxi sp d16
                sp=next_word();
                break;
            case 0x32://sta d16
            {
                unsigned short temp=next_word();
                write_byte(temp,A);
                wz=(A<<8)|((temp+1)&0xFF);
                break;
            }

            case 0x33://inc sp
                sp++;
                break;
            case 0x34://inc M
                {
                unsigned char tmp= M;
                inc(tmp);
                write_byte(get_hl(),tmp);
                }
                break;
            case 0x35://dcr M
                {
                unsigned char tmp= M;
                DCR(tmp);
                write_byte(get_hl(),tmp);
                }
                break;
            case 0x36:// mci M d8
                write_byte(get_hl(),next_byte());
                break;
            case 0x37://scf
                carry=true;
                halfcarry=false;
                n= false;
                set_35(A);
                break;
            case 0x38://jr C d8
                relative_cond_jump(carry);
                break;
            case 0x39:// Dad sp
                set_hl(add16(get_hl(),sp));
                break;
            case 0x3a://lda d16
            {
                unsigned short temp=next_word();
                A=read_byte(temp);
                wz=temp+1;
                break;
            }
            case 0x3b://dcx sp
                sp--;
                break;
            case 0x3c://inc a
                inc(A);
                break;
            case 0x3d://dcr a
                DCR(A);
                break;
            case 0x3e://mvi a d8
                A=next_byte();
                break;
            case 0x3f://ccf
                halfcarry=carry;
                carry=!carry;
                n=false;
                set_35(A);
                break;
            case 0x40://ld b, b
                break;
            case 0x41://ld b, c
                ld(B,C);
                break;
            case 0x42://ld b, d
                ld(B,D);
                break;
            case 0x43://ld b, e
                ld(B,E);
                break;
            case 0x44:
                ld(B,H);
                break;
            case 0x45:
                ld(B,L);
                break;
            case 0x46:
                ld(B,M);
                break;
            case 0x47:
                ld(B,A);
                break;
            case 0x48:
                ld(C,B);
                break;
            case 0x49:
                break;
            case 0x4a:
                ld(C,D);
                break;
            case 0x4b:
                ld(C,E);
                break;
            case 0x4c:
                ld(C,H);
                break;
            case 0x4d:
                ld(C,L);
                break;
            case 0x4e:
                ld(C,M);
                break;
            case 0x4f:
                ld(C,A);
                break;

            case 0x50:
                ld(D,B);
                break;
            case 0x51:
                ld(D,C);
                break;
            case 0x52:
                break;
            case 0x53:
                ld(D,E);
                break;
            case 0x54:
                ld(D,H);
                break;
            case 0x55:
                ld(D,L);
                break;
            case 0x56:
                ld(D,M);
                break;
            case 0x57:
                ld(D,A);
                break;
            case 0x58:
                ld(E,B);
                break;
            case 0x59:
                ld(E,C);
                break;
            case 0x5a:
                ld(E,D);
                break;
            case 0x5b:
                break;
            case 0x5c:
                ld(E,H);
                break;
            case 0x5d:
                ld(E,L);
                break;
            case 0x5e:
                ld(E,M);
                break;
            case 0x5f:
                ld(E,A);
                break;

            case 0x60:
                ld(H,B);
                break;
            case 0x61:
                ld(H,C);
                break;
            case 0x62:
                ld(H,D);
                break;
            case 0x63:
                ld(H,E);
                break;
            case 0x64:
                break;
            case 0x65:
                ld(H,L);
                break;
            case 0x66:
                ld(H,M);
                break;
            case 0x67:
                ld(H,A);
                break;
            case 0x68:
                ld(L,B);
                break;
            case 0x69:
                ld(L,C);
                break;
            case 0x6a:
                ld(L,D);
                break;
            case 0x6b:
                ld(L,E);
                break;
            case 0x6c:
                ld(L,H);
                break;
            case 0x6d:
                break;
            case 0x6e:
                ld(L,M);
                break;
            case 0x6f:
                ld(L,A);
                break;

            case 0x70:
                write_byte(get_hl(),B);
                break;
            case 0x71:
                write_byte(get_hl(),C);
                break;
            case 0x72:
                write_byte(get_hl(),D);
                break;
            case 0x73:
                write_byte(get_hl(),E);
                break;
            case 0x74:
                write_byte(get_hl(),H);
                break;
            case 0x75:
                write_byte(get_hl(),L);
                break;
            case 0x76://halt
                halt=true;
                pc--;
                break;
            case 0x77:
                write_byte(get_hl(),A);
                break;
            case 0x78:
                ld(A,B);
                break;
            case 0x79:
                ld(A,C);
                break;
            case 0x7a:
                ld(A,D);
                break;
            case 0x7b:
                ld(A,E);
                break;
            case 0x7c:
                ld(A,H);
                break;
            case 0x7d:
                ld(A,L);
                break;
            case 0x7e:
                ld(A,M);
                break;
            case 0x7f:
                break;

            case 0x80:
                A=add8(A,B,0);
                break;
            case 0x81:
                A=add8(A,C,0);
                break;
            case 0x82:
                A=add8(A,D,0);
                break;
            case 0x83:
                A=add8(A,E,0);
                break;
            case 0x84:
                A=add8(A,H,0);
                break;
            case 0x85:
                A=add8(A,L,0);
                break;
            case 0x86:
                A=add8(A,M,0);
                break;
            case 0x87:
                A=add8(A,A,0);
                break;

            case 0x88:
                A=add8(A,B,carry);
                break;
            case 0x89:
                A=add8(A,C,carry);
                break;
            case 0x8a:
                A=add8(A,D,carry);
                break;
            case 0x8b:
                A=add8(A,E,carry);
                break;
            case 0x8c:
                A=add8(A,H,carry);
                break;
            case 0x8d:
                A=add8(A,L,carry);
                break;
            case 0x8e:
                A=add8(A,M,carry);
                break;
            case 0x8f:
                A=add8(A,A,carry);
                break;

            case 0x90:
                A=sub8(A,B,0);
                break;
            case 0x91:
                A=sub8(A,C,0);
                break;
            case 0x92:
                A=sub8(A,D,0);
                break;
            case 0x93:
                A=sub8(A,E,0);
                break;
            case 0x94:
                A=sub8(A,H,0);
                break;
            case 0x95:
                A=sub8(A,L,0);
                break;
            case 0x96:
                A=sub8(A,M,0);
                break;
            case 0x97:
                A=sub8(A,A,0);
                break;

            case 0x98:
                A=sub8(A,B,carry);
                break;
            case 0x99:
                A=sub8(A,C,carry);
                break;
            case 0x9a:
                A=sub8(A,D,carry);
                break;
            case 0x9b:
                A=sub8(A,E,carry);
                break;
            case 0x9c:
                A=sub8(A,H,carry);
                break;
            case 0x9d:
                A=sub8(A,L,carry);
                break;
            case 0x9e:
                A=sub8(A,M,carry);
                break;
            case 0x9f:
                A=sub8(A,A,carry);
                break;

            case 0xa0:
                ANA(A,B);
                break;
            case 0xa1:
                ANA(A,C);
                break;
            case 0xa2:
                ANA(A,D);
                break;
            case 0xa3:
                ANA(A,E);
                break;
            case 0xa4:
                ANA(A,H);
                break;
            case 0xa5:
                ANA(A,L);
                break;
            case 0xa6:
                ANA(A,M);
                break;
            case 0xa7:
                ANA(A,A);
                break;

            case 0xa8:
                XRA(A,B);
                break;
            case 0xa9:
                XRA(A,C);
                break;
            case 0xaa:
                XRA(A,D);
                break;
            case 0xab:
                XRA(A,E);
                break;
            case 0xac:
                XRA(A,H);
                break;
            case 0xad:
                XRA(A,L);
                break;
            case 0xae:
                XRA(A,M);
                break;
            case 0xaf:
                XRA(A,A);
                break;

            case 0xb0:
                ORA(A,B);
                break;
            case 0xb1:
                ORA(A,C);
                break;
            case 0xb2:
                ORA(A,D);
                break;
            case 0xb3:
                ORA(A,E);
                break;
            case 0xb4:
                ORA(A,H);
                break;
            case 0xb5:
                ORA(A,L);
                break;
            case 0xb6:
                ORA(A,M);
                break;
            case 0xb7:
                ORA(A,A);
                break;

            case 0xb8:
                CMP(A,B);
                break;
            case 0xb9:
                CMP(A,C);
                break;
            case 0xba:
                CMP(A,D);
                break;
            case 0xbb:
                CMP(A,E);
                break;
            case 0xbc:
                CMP(A,H);
                break;
            case 0xbd:
                CMP(A,L);
                break;
            case 0xbe:
                CMP(A,M);
                break;
            case 0xbf:
                CMP(A,A);
                break;


            case 0xc0:
                cond_ret(!zero);
                break;
            case 0xc1:
                set_bc(POP());
                break;
            case 0xc2:
                cond_jump(!zero);
                break;
            case 0xc3:
                jump(next_word());
                break;
            case 0xc4:
                cond_call(!zero);
                break;
            case 0xc5:
                PUSH(get_bc());
                break;
            case 0xc6://adi a
                A=add8(A,next_byte(),0);
                break;
            case 0xc7://rst 0
                call(0x0);
                break;
            case 0xc8://rz
                cond_ret(zero);
                break;
            case 0xc9://ret
                ret();
                break;
            case 0xca://jz d16
                cond_jump(zero);
                break;
            case 0xcb://Bit instructions
                run_op_cb();
                break;
            case 0xcc://cz d16
                cond_call(zero);
                break;
            case 0xcd://call d16
                call(next_word());
                break;
            case 0xce:// aci a
                A=add8(A,next_byte(),carry);
                break;
            case 0xcf://rst 8
                call(0x8);
                break;
            case 0xd0://rnc
                cond_ret(!carry);
                break;
            case 0xd1://pop d
                set_de(POP());
                break;
            case 0xd2://jnc d16
                cond_jump(!carry);
                break;
            case 0xd3://out d8
            {
                unsigned short port =next_byte();
                out(A,port);
                wz=(A<<8)|(port+1);
                break;
            }

            case 0xd4://cnc d16
                cond_call(!carry);
                break;
            case 0xd5://push d
                PUSH(get_de());
                break;
            case 0xd6://sui a d8
                A=sub8(A,next_byte(),0);
                break;
            case 0xd7://rst 10
                call(0x10);
                break;
            case 0xd8://rc
                cond_ret(carry);
                break;
            case 0xd9://exx
                {
                unsigned short temp=get_bc();
                set_bc(get_bc_prime());
                set_bc_prime(temp);

                temp=get_de();
                set_de(get_de_prime());
                set_de_prime(temp);

                temp=get_hl();
                set_hl(get_hl_prime());
                set_hl_prime(temp);
            }
            break;

            case 0xda://jc d16
                cond_jump(carry);
                break;
            case 0xdb://in d8
            {
                unsigned char a=A;
                if(!cpm){
                    A=in(next_byte());
                }
                else{
                    A=0xff;
                    pc++;
                }
                wz=(a<<8)|(A+1);
                break;
            }
            case 0xdc://cc d16
                cond_call(carry);
                break;
            case 0xdd:// ix instructions
                run_op_dd();
                break;
            case 0xde://sbi A d8
                A=sub8(A,next_byte(),carry);
                break;
            case 0xdf://rst 18
                call(0x18);
                break;
            case 0xe0://rpo
                cond_ret(!parity);
                break;
            case 0xe1://pop hl
                set_hl(POP());
                break;
            case 0xe2://jpo d16
                cond_jump(!parity);
                break;
            case 0xe3://xthl
            {
                unsigned short temp=POP();
                PUSH(get_hl());
                set_hl(temp);
                wz=temp;
            }
                break;
            case 0xe4://cpo d16
                cond_call(!parity);
                break;
            case 0xe5://push hl
                PUSH(get_hl());
                break;
            case 0xe6://ani A d8
                ANA(A,next_byte());
                break;
            case 0xe7://rst 20
                call(0x20);
                break;
            case 0xe8://rpe
                cond_ret(parity);
                break;
            case 0xe9://pchl
                pc=(get_hl());
                break;
            case 0xea://jpe d16
                cond_jump(parity);
                break;
            case 0xeb://xchg
            {
                unsigned short temp=get_de();
                set_de(get_hl());
                set_hl(temp);}
                break;
            case 0xec://cpe d16
                cond_call(parity);
                break;
            case 0xed://Extended instructions
                run_op_ed();
                break;
            case 0xee://xri A d8
                XRA(A,next_byte());
                break;
            case 0xef://rst 28
                call(0x28);
                break;
            case 0xf0://rnz
                cond_ret(!sign);
                break;
            case 0xf1://pop psw
                set_af(POP());
                break;
            case 0xf2://jp d16
                cond_jump(!sign);
                break;
            case 0xf3://di
                ffIFF1=false;
                ffIFF2=false;
                break;
            case 0xf4://cp d16
                cond_call(!sign);
                break;
            case 0xf5://push psw
                PUSH(get_af());
                break;
            case 0xf6://ora a
                ORA(A,next_byte());
                break;
            case 0xf7://rst 30
                call(0x30);
                break;
            case 0xf8://rnz
                cond_ret(sign);
                break;
            case 0xf9://sphl
                sp=get_hl();
                break;
            case 0xfa://jm d16
                cond_jump(sign);
                break;
            case 0xfb://ei
                ffIFF1=true;
                //ffIFF2=true;
                break;
            case 0xfc://cm d16
                cond_call(sign);
                break;
            case 0xfd://iy instructions
                run_op_fd();
                break;
            case 0xfe://cpi d8
                CMP(A,next_byte());
                break;
            case 0xff://rst 38
                call(0x38);
                break;
            default:cout<<"opcode not implemented "<<(int)opcode<<endl;cout<<tc<<endl<<pc; exit(0x15);
        }
    }
    void run_op_ed(){
        unsigned char op=next_byte();
        cycles+=OPCODES_CYCLES_EXTENDED[op];
        switch(op){
            case 0x40:if(debug){cout<<"in b,(c)"<<endl; }
                B=in(C);
                break;
            case 0x41:if(debug){cout<<"out (c),b"<<endl; }
                out(B,C);
                break;
            case 0x42:if(debug){cout<<"sbc hl,bc"<<endl; }
                set_hl(sbc16(get_hl(),get_bc()));
                break;
            case 0x43:if(debug){cout<<"ld adr bc"<<endl; }{
                unsigned short temp = next_word();
                write_word(temp,get_bc());
                wz=temp+1;
                break;
            }

            case 0x44:if(debug){cout<<"neg a"<<endl; }{
                int tmp=A;
                A=0;
                A=sub8(A,tmp,0);
                }
                break;
            case 0x45:if(debug){cout<<"ret n"<<endl; }
                ffIFF1=ffIFF2;
                pc=POP();
                break;
            case 0x46:if(debug){cout<<"im 0"<<endl; }
                interrupt_mode=0;
                break;
            case 0x47:if(debug){cout<<"ld i,a"<<endl; }
                i=A;
                break;
            case 0x4a:if(debug){cout<<"adc hl,bc"<<endl; }
                set_hl(adc16(get_hl(),get_bc()));
                break;
            case 0x4b:if(debug){cout<<"ld bc, adr"<<endl; }{
                unsigned short temp=next_word();
                set_bc(read_word(temp));
                wz=temp+1;
                break;
            }
            case 0x52:if(debug){cout<<"sbc hl,de"<<endl; }
                set_hl(sbc16(get_hl(),get_de()));
                break;
            case 0x53:if(debug){cout<<"ld adr de"<<endl; }{
                unsigned short temp = next_word();
                write_word(temp, get_de());
                wz=temp+1;
                break;
            }
            case 0x5a:if(debug){cout<<"adc hl,de"<<endl; }
                set_hl(adc16(get_hl(),get_de()));
                break;
            case 0x5b:if(debug){cout<<"ld de, adr"<<endl; }{
                unsigned short temp=next_word();
                set_de(read_word(temp));
                wz=temp+1;
                break;
            }

            case 0x5e:if(debug){cout<<"im 2"<<endl; }
                interrupt_mode=2;
                break;
            case 0x62:if(debug){cout<<"sbc hl,hl"<<endl; }
                set_hl(sbc16(get_hl(),get_hl()));
                break;
            case 0x67:if(debug){cout<<"rrd"<<endl; }{
                uint8_t a = A;
                uint8_t val = read_byte(get_hl());
                A = (a & 0xF0) | (val & 0xF);
                write_byte(get_hl(), (val >> 4) | (a << 4));
                n = 0;
                halfcarry = 0;
                f3 = GET_BIT(3, A);
                f5 = GET_BIT(5, A);
                zero = A == 0;
                sign = A >> 7;
                check_parity(A);
                wz=get_hl()+1;
                break;
            }



            case 0x6a:if(debug){cout<<"adc hl,hl"<<endl; }
                set_hl(adc16(get_hl(),get_hl()));
                break;
            case 0x6f:if(debug){cout<<"rld"<<endl; }{
                uint8_t a = A;
                uint8_t val = read_byte(get_hl());
                A = (a & 0xF0) | (val >> 4);
                write_byte(get_hl(), (val << 4) | (a & 0xF));

                n = 0;
                halfcarry = 0;
                f3 = GET_BIT(3, A);
                f5 = GET_BIT(5, A);
                zero = A == 0;
                sign = A >> 7;
                check_parity(A);
                wz=get_hl()+1;
                break;
            }
            case 0x72:if(debug){cout<<"sbc hl,sp"<<endl; }
                set_hl(sbc16(get_hl(),sp));
                break;
            case 0x73:if(debug){cout<<"ld adr sp"<<endl; }{
                unsigned short temp=next_word();
                write_word(temp,sp);
                wz=temp+1;
                break;
            }

            case 0x7a:if(debug){cout<<"adc hl,sp"<<endl; }
                    set_hl(adc16(get_hl(),sp));
                break;
            case 0x7b:if(debug){cout<<"ld sp adr"<<endl; }{
                unsigned short temp=next_word();
                sp=read_word(temp);
                wz=temp+1;
                break;
            }

            case 0xa0:if(debug){cout<<"ldi"<<endl; }
                ldi();
                break;
            case 0xa1:if(debug){cout<<"cpi"<<endl; }
                cpi();
                break;
            case 0xa8:if(debug){cout<<"ldd"<<endl; }
                ldd();
                break;
            case 0xa9:if(debug){cout<<"cpd"<<endl; }
                cpd();
                break;
            case 0xb0:if(debug){cout<<"ldir"<<endl; }
                ldi();
                if(get_bc()>0){
                    pc-=2;
                    wz=pc+1;
                }
                break;
            case 0xb1:if(debug){cout<<"cpir"<<endl; }
                cpi();
                if(!zero&&get_bc()!=0){
                    pc-=2;
                    wz=pc+1;
                }
                else{
                    wz++;
                }
                break;

            case 0xb8:if(debug){cout<<"lldr"<<endl; }
                ldd();
                if (get_bc() != 0) {
                    pc -= 2;
                }
                break;
            case 0xb9:if(debug){cout<<"cpdr"<<endl; }
                cpd();
                if(!zero&&get_bc()!=0){
                    pc-=2;
                }
                else{
                    wz++;
                }
                break;



            default:cout<<"extended opcode not implemented "<<(int)op<<endl;cout<<tc<<endl<<pc; exit(0x15);

        }
    }
    void run_op_cb(){//Bit
        unsigned char op=next_byte();
        cycles+=OPCODES_CYCLES_BITS[op];
        switch(op){
            case 0x00:if(debug){cout<<"rlc b"<<endl; }
                B=rlc(B);
                break;
            case 0x01:if(debug){cout<<"rlc c"<<endl; }
                C=rlc(C);
                break;
            case 0x02:if(debug){cout<<"rlc d"<<endl; }
                D=rlc(D);
                break;
            case 0x03:if(debug){cout<<"rlc e"<<endl; }
                E=rlc(E);
                break;
            case 0x04:if(debug){cout<<"rlc h"<<endl; }
                H=rlc(H);
                break;
            case 0x05:if(debug){cout<<"rlc l"<<endl; }
                L=rlc(L);
                break;
            case 0x06:if(debug){cout<<"rlc (hl)"<<endl; }
                write_byte(get_hl(),rlc(read_byte(get_hl())));
                break;
            case 0x07:if(debug){cout<<"rlc A"<<endl; }
                A=rlc(A);
                break;
            case 0x08:if(debug){cout<<"rrc b"<<endl; }
                B=rrc(B);
                break;
            case 0x09:if(debug){cout<<"rrc c"<<endl; }
                C=rrc(C);
                break;
            case 0x0a:if(debug){cout<<"rrc d"<<endl; }
                D=rrc(D);
                break;
            case 0x0b:if(debug){cout<<"rrc e"<<endl; }
                E=rrc(E);
                break;
            case 0x0c:if(debug){cout<<"rrc h"<<endl; }
                H=rrc(H);
                break;
            case 0x0d:if(debug){cout<<"rrc l"<<endl; }
                L=rrc(L);
                break;
            case 0x0e:if(debug){cout<<"rrc (hl)"<<endl; }
                write_byte(get_hl(),rrc(read_byte(get_hl())));
                break;
            case 0x0f:if(debug){cout<<"rrc A"<<endl; }
                A=rrc(A);
                break;
            case 0x10:if(debug){cout<<"rl b"<<endl; }
                B=rl(B);
                break;
            case 0x11:if(debug){cout<<"rl c"<<endl; }
                C=rl(C);
                break;
            case 0x12:if(debug){cout<<"rl d"<<endl; }
                D=rl(D);
                break;
            case 0x13:if(debug){cout<<"rl e"<<endl; }
                E=rl(E);
                break;
            case 0x14:if(debug){cout<<"rl h"<<endl; }
                H=rl(H);
                break;
            case 0x15:if(debug){cout<<"rl l"<<endl; }
                L=rl(L);
                break;
            case 0x16:if(debug){cout<<"rl (hl)"<<endl; }
                write_byte(get_hl(),rl(read_byte(get_hl())));
                break;
            case 0x17:if(debug){cout<<"rl a"<<endl; }
                A=rl(A);
                break;
            case 0x18:if(debug){cout<<"rr b"<<endl; }
                B=rr(B);
                break;
            case 0x19:if(debug){cout<<"rr c"<<endl; }
                C=rr(C);
                break;
            case 0x1a:if(debug){cout<<"rr d"<<endl; }
                D=rr(D);
                break;
            case 0x1b:if(debug){cout<<"rr e"<<endl; }
                E=rr(E);
                break;
            case 0x1c:if(debug){cout<<"rr h"<<endl; }
                H=rr(H);
                break;
            case 0x1d:if(debug){cout<<"rr l"<<endl; }
                L=rr(L);
                break;
            case 0x1e:if(debug){cout<<"rr (hl)"<<endl; }
                write_byte(get_hl(),rr(read_byte(get_hl())));
                break;
            case 0x1f:if(debug){cout<<"rr A"<<endl; }
                A=rr(A);
                break;
            case 0x20:if(debug){cout<<"sla b"<<endl; }
                B=sla(B);
                break;
            case 0x21:if(debug){cout<<"sla c"<<endl; }
                C=sla(C);
                break;
            case 0x22:if(debug){cout<<"sla d"<<endl; }
                D=sla(D);
                break;
            case 0x23:if(debug){cout<<"sla e"<<endl; }
                E=sla(E);
                break;
            case 0x24:if(debug){cout<<"sla h"<<endl; }
                H=sla(H);
                break;
            case 0x25:if(debug){cout<<"sla l"<<endl; }
                L=sla(L);
                break;
            case 0x26:if(debug){cout<<"sla (hl)"<<endl; }
                write_byte(get_hl(),sla(read_byte(get_hl())));
                break;
            case 0x27:if(debug){cout<<"sla a"<<endl; }
                A=sla(A);
                break;
            case 0x28:if(debug){cout<<"sra b"<<endl; }
                B=sra(B);
                break;
            case 0x29:if(debug){cout<<"sra c"<<endl; }
                C=sra(C);
                break;
            case 0x2a:if(debug){cout<<"sra d"<<endl; }
                D=sra(D);
                break;
            case 0x2b:if(debug){cout<<"sra e"<<endl; }
                E=sra(E);
                break;
            case 0x2c:if(debug){cout<<"sra h"<<endl; }
                H=sra(H);
                break;
            case 0x2d:if(debug){cout<<"sra l"<<endl; }
                L=sra(L);
                break;
            case 0x2e:if(debug){cout<<"sra (hl)"<<endl; }
                write_byte(get_hl(),sra(read_byte(get_hl())));
                break;
            case 0x2f:if(debug){cout<<"sra A"<<endl; }
                A=sra(A);
                break;
            case 0x30:if(debug){cout<<"sll b"<<endl; }
                B=sll(B);
                break;
            case 0x31:if(debug){cout<<"sll c"<<endl; }
                C=sll(C);
                break;
            case 0x32:if(debug){cout<<"sll d"<<endl; }
                D=sll(D);
                break;
            case 0x33:if(debug){cout<<"sll e"<<endl; }
                E=sll(E);
                break;
            case 0x34:if(debug){cout<<"sll h"<<endl; }
                H=sll(H);
                break;
            case 0x35:if(debug){cout<<"sll l"<<endl; }
                L=sll(L);
                break;
            case 0x36:if(debug){cout<<"sll (hl)"<<endl; }
                write_byte(get_hl(),sll(read_byte(get_hl())));
                break;
            case 0x37:if(debug){cout<<"sll a"<<endl; }
                A=sll(A);
                break;
            case 0x38:if(debug){cout<<"srl b"<<endl; }
                B=srl(B);
                break;
            case 0x39:if(debug){cout<<"srl c"<<endl; }
                C=srl(C);
                break;
            case 0x3a:if(debug){cout<<"srl d"<<endl; }
                D=srl(D);
                break;
            case 0x3b:if(debug){cout<<"srl e"<<endl; }
                E=srl(E);
                break;
            case 0x3c:if(debug){cout<<"srl h"<<endl; }
                H=srl(H);
                break;
            case 0x3d:if(debug){cout<<"srl l"<<endl; }
                L=srl(L);
                break;
            case 0x3e:if(debug){cout<<"srl (hl)"<<endl; }
                write_byte(get_hl(),srl(read_byte(get_hl())));
                break;
            case 0x3f:if(debug){cout<<"srl A"<<endl; }
                A=srl(A);
                break;
            case 0x40:if(debug){cout<<"bit 0,b"<<endl; }
                testbit(0,B);
                break;
            case 0x41:if(debug){cout<<"bit 0,c"<<endl; }
                testbit(0,C);
                break;
            case 0x42:if(debug){cout<<"bit 0,d"<<endl; }
                testbit(0,D);
                break;
            case 0x43:if(debug){cout<<"bit 0,e"<<endl; }
                testbit(0,E);
                break;
            case 0x44:if(debug){cout<<"bit 0,h"<<endl; }
                testbit(0,H);
                break;
            case 0x45:if(debug){cout<<"bit 0,l"<<endl; }
                testbit(0,L);
                break;
            case 0x46:if(debug){cout<<"bit 0,(hl)"<<endl; }
                testbit(0,read_byte(get_hl()));
                set_35(wz>>8);
                break;
            case 0x47:if(debug){cout<<"bit 0,a"<<endl; }
                testbit(0,A);
                break;
            case 0x48:if(debug){cout<<"bit 1,b"<<endl; }
                testbit(1,B);
                break;
            case 0x49:if(debug){cout<<"bit 1,c"<<endl; }
                testbit(1,C);
                break;
            case 0x4a:if(debug){cout<<"bit 1,d"<<endl; }
                testbit(1,D);
                break;
            case 0x4b:if(debug){cout<<"bit 1,e"<<endl; }
                testbit(1,E);
                break;
            case 0x4c:if(debug){cout<<"bit 1,h"<<endl; }
                testbit(1,H);
                break;
            case 0x4d:if(debug){cout<<"bit 1,l"<<endl; }
                testbit(1,L);
                break;
            case 0x4e:if(debug){cout<<"bit 1,(hl)"<<endl; }
                testbit(1,read_byte(get_hl()));
                set_35(wz>>8);
                break;
            case 0x4f:if(debug){cout<<"bit 1,a"<<endl; }
                testbit(1,A);
                break;

            case 0x50:if(debug){cout<<"bit 2,b"<<endl; }
                testbit(2,B);
                break;
            case 0x51:if(debug){cout<<"bit 2,c"<<endl; }
                testbit(2,C);
                break;
            case 0x52:if(debug){cout<<"bit 2,d"<<endl; }
                testbit(2,D);
                break;
            case 0x53:if(debug){cout<<"bit 2,e"<<endl; }
                testbit(2,E);
                break;
            case 0x54:if(debug){cout<<"bit 2,h"<<endl; }
                testbit(2,H);
                break;
            case 0x55:if(debug){cout<<"bit 2,l"<<endl; }
                testbit(2,L);
                break;
            case 0x56:if(debug){cout<<"bit 2,(hl)"<<endl; }
                testbit(2,read_byte(get_hl()));
                set_35(wz>>8);
                break;
            case 0x57:if(debug){cout<<"bit 2,a"<<endl; }
                testbit(2,A);
                break;
            case 0x58:if(debug){cout<<"bit 3,b"<<endl; }
                testbit(3,B);
                break;
            case 0x59:if(debug){cout<<"bit 3,c"<<endl; }
                testbit(3,C);
                break;
            case 0x5a:if(debug){cout<<"bit 3,d"<<endl; }
                testbit(3,D);
                break;
            case 0x5b:if(debug){cout<<"bit 3,e"<<endl; }
                testbit(3,E);
                break;
            case 0x5c:if(debug){cout<<"bit 3,h"<<endl; }
                testbit(3,H);
                break;
            case 0x5d:if(debug){cout<<"bit 3,l"<<endl; }
                testbit(3,L);
                break;
            case 0x5e:if(debug){cout<<"bit 3,(hl)"<<endl; }
                testbit(3,read_byte(get_hl()));
                set_35(wz>>8);
                break;
            case 0x5f:if(debug){cout<<"bit 3,a"<<endl; }
                testbit(3,A);
                break;

            case 0x60:if(debug){cout<<"bit 4,b"<<endl; }
                testbit(4,B);
                break;
            case 0x61:if(debug){cout<<"bit 4,c"<<endl; }
                testbit(4,C);
                break;
            case 0x62:if(debug){cout<<"bit 4,d"<<endl; }
                testbit(4,D);
                break;
            case 0x63:if(debug){cout<<"bit 4,e"<<endl; }
                testbit(4,E);
                break;
            case 0x64:if(debug){cout<<"bit 4,h"<<endl; }
                testbit(4,H);
                break;
            case 0x65:if(debug){cout<<"bit 4,l"<<endl; }
                testbit(4,L);
                break;
            case 0x66:if(debug){cout<<"bit 4,(hl)"<<endl; }
                testbit(4,read_byte(get_hl()));
                set_35(wz>>8);
                break;
            case 0x67:if(debug){cout<<"bit 4,a"<<endl; }
                testbit(4,A);
                break;
            case 0x68:if(debug){cout<<"bit 5,b"<<endl; }
                testbit(5,B);
                break;
            case 0x69:if(debug){cout<<"bit 5,c"<<endl; }
                testbit(5,C);
                break;
            case 0x6a:if(debug){cout<<"bit 5,d"<<endl; }
                testbit(5,D);
                break;
            case 0x6b:if(debug){cout<<"bit 5,e"<<endl; }
                testbit(5,E);
                break;
            case 0x6c:if(debug){cout<<"bit 5,h"<<endl; }
                testbit(5,H);
                break;
            case 0x6d:if(debug){cout<<"bit 5,l"<<endl; }
                testbit(5,L);
                break;
            case 0x6e:if(debug){cout<<"bit 5,(hl)"<<endl; }
                testbit(5,read_byte(get_hl()));
                set_35(wz>>8);
                break;
            case 0x6f:if(debug){cout<<"bit 5,a"<<endl; }
                testbit(5,A);
                break;

            case 0x70:if(debug){cout<<"bit 6,b"<<endl; }
                testbit(6,B);
                break;
            case 0x71:if(debug){cout<<"bit 6,c"<<endl; }
                testbit(6,C);
                break;
            case 0x72:if(debug){cout<<"bit 6,d"<<endl; }
                testbit(6,D);
                break;
            case 0x73:if(debug){cout<<"bit 6,e"<<endl; }
                testbit(6,E);
                break;
            case 0x74:if(debug){cout<<"bit 6,h"<<endl; }
                testbit(6,H);
                break;
            case 0x75:if(debug){cout<<"bit 6,l"<<endl; }
                testbit(6,L);
                break;
            case 0x76:if(debug){cout<<"bit 6,(hl)"<<endl; }
                testbit(6,read_byte(get_hl()));
                set_35(wz>>8);
                break;
            case 0x77:if(debug){cout<<"bit 6,a"<<endl; }
                testbit(6,A);
                break;
            case 0x78:if(debug){cout<<"bit 7,b"<<endl; }
                testbit(7,B);
                break;
            case 0x79:if(debug){cout<<"bit 7,c"<<endl; }
                testbit(7,C);
                break;
            case 0x7a:if(debug){cout<<"bit 7,d"<<endl; }
                testbit(7,D);
                break;
            case 0x7b:if(debug){cout<<"bit 7,e"<<endl; }
                testbit(7,E);
                break;
            case 0x7c:if(debug){cout<<"bit 7,h"<<endl; }
                testbit(7,H);
                break;
            case 0x7d:if(debug){cout<<"bit 7,l"<<endl; }
                testbit(7,L);
                break;
            case 0x7e:if(debug){cout<<"bit 7,(hl)"<<endl; }
                testbit(7,read_byte(get_hl()));
                set_35(wz>>8);
                break;
            case 0x7f:if(debug){cout<<"bit 7,a"<<endl; }
                testbit(7,A);
                break;

            case 0x80:if(debug){cout<<"res 0,b"<<endl; }
                B=resetbit(0,B);
                break;
            case 0x81:if(debug){cout<<"res 0,c"<<endl; }
                C=resetbit(0,C);
                break;
            case 0x82:if(debug){cout<<"res 0,d"<<endl; }
                D=resetbit(0,D);
                break;
            case 0x83:if(debug){cout<<"res 0,e"<<endl; }
                E=resetbit(0,E);
                break;
            case 0x84:if(debug){cout<<"res 0,h"<<endl; }
                H=resetbit(0,H);
                break;
            case 0x85:if(debug){cout<<"res 0,l"<<endl; }
                L=resetbit(0,L);
                break;
            case 0x86:if(debug){cout<<"res 0,(hl)"<<endl; }
                write_byte(get_hl(),resetbit(0,read_byte(get_hl())));
                break;
            case 0x87:if(debug){cout<<"res 0,a"<<endl; }
                A=resetbit(0,A);
                break;
            case 0x88:if(debug){cout<<"res 1,b"<<endl; }
                B=resetbit(1,B);
                break;
            case 0x89:if(debug){cout<<"res 1,c"<<endl; }
                C=resetbit(1,C);
                break;
            case 0x8a:if(debug){cout<<"res 1,d"<<endl; }
                D=resetbit(1,D);
                break;
            case 0x8b:if(debug){cout<<"res 1,e"<<endl; }
                E=resetbit(1,E);
                break;
            case 0x8c:if(debug){cout<<"res 1,h"<<endl; }
                H=resetbit(1,H);
                break;
            case 0x8d:if(debug){cout<<"res 1,l"<<endl; }
                L=resetbit(1,L);
                break;
            case 0x8e:if(debug){cout<<"res 1,(hl)"<<endl; }
                write_byte(get_hl(),resetbit(1,read_byte(get_hl())));
                break;
            case 0x8f:if(debug){cout<<"res 1,a"<<endl; }
                A=resetbit(1,A);
                break;

            case 0x90:if(debug){cout<<"res 2,b"<<endl; }
                B=resetbit(2,B);
                break;
            case 0x91:if(debug){cout<<"res 2,c"<<endl; }
                C=resetbit(2,C);
                break;
            case 0x92:if(debug){cout<<"res 2,d"<<endl; }
                D=resetbit(2,D);
                break;
            case 0x93:if(debug){cout<<"res 2,e"<<endl; }
                E=resetbit(2,E);
                break;
            case 0x94:if(debug){cout<<"res 2,h"<<endl; }
                H=resetbit(2,H);
                break;
            case 0x95:if(debug){cout<<"res 2,l"<<endl; }
                L=resetbit(2,L);
                break;
            case 0x96:if(debug){cout<<"res 2,(hl)"<<endl; }
                write_byte(get_hl(),resetbit(2,read_byte(get_hl())));
                break;
            case 0x97:if(debug){cout<<"res 2,a"<<endl; }
                A=resetbit(2,A);
                break;
            case 0x98:if(debug){cout<<"res 3,b"<<endl; }
                B=resetbit(3,B);
                break;
            case 0x99:if(debug){cout<<"res 3,c"<<endl; }
                C=resetbit(3,C);
                break;
            case 0x9a:if(debug){cout<<"res 3,d"<<endl; }
                D=resetbit(3,D);
                break;
            case 0x9b:if(debug){cout<<"res 3,e"<<endl; }
                E=resetbit(3,E);
                break;
            case 0x9c:if(debug){cout<<"res 3,h"<<endl; }
                H=resetbit(3,H);
                break;
            case 0x9d:if(debug){cout<<"res 3,l"<<endl; }
                L=resetbit(3,L);
                break;
            case 0x9e:if(debug){cout<<"res 3,(hl)"<<endl; }
                write_byte(get_hl(),resetbit(3,read_byte(get_hl())));
                break;
            case 0x9f:if(debug){cout<<"res 3,a"<<endl; }
                A=resetbit(3,A);
                break;

            case 0xa0:if(debug){cout<<"res 4,b"<<endl; }
                B=resetbit(4,B);
                break;
            case 0xa1:if(debug){cout<<"res 4,c"<<endl; }
                C=resetbit(4,C);
                break;
            case 0xa2:if(debug){cout<<"res 4,d"<<endl; }
                D=resetbit(4,D);
                break;
            case 0xa3:if(debug){cout<<"res 4,e"<<endl; }
                E=resetbit(4,E);
                break;
            case 0xa4:if(debug){cout<<"res 4,h"<<endl; }
                H=resetbit(4,H);
                break;
            case 0xa5:if(debug){cout<<"res 4,l"<<endl; }
                L=resetbit(4,L);
                break;
            case 0xa6:if(debug){cout<<"res 4,(hl)"<<endl; }
                write_byte(get_hl(),resetbit(4,read_byte(get_hl())));
                break;
            case 0xa7:if(debug){cout<<"res 4,a"<<endl; }
                A=resetbit(4,A);
                break;
            case 0xa8:if(debug){cout<<"res 5,b"<<endl; }
                B=resetbit(5,B);
                break;
            case 0xa9:if(debug){cout<<"res 5,c"<<endl; }
                C=resetbit(5,C);
                break;
            case 0xaa:if(debug){cout<<"res 5,d"<<endl; }
                D=resetbit(5,D);
                break;
            case 0xab:if(debug){cout<<"res 5,e"<<endl; }
                E=resetbit(5,E);
                break;
            case 0xac:if(debug){cout<<"res 5,h"<<endl; }
                H=resetbit(5,H);
                break;
            case 0xad:if(debug){cout<<"res 5,l"<<endl; }
                L=resetbit(5,L);
                break;
            case 0xae:if(debug){cout<<"res 5,(hl)"<<endl; }
                write_byte(get_hl(),resetbit(5,read_byte(get_hl())));
                break;
            case 0xaf:if(debug){cout<<"res 5,a"<<endl; }
                A=resetbit(5,A);
                break;

            case 0xb0:if(debug){cout<<"res 6,b"<<endl; }
                B=resetbit(6,B);
                break;
            case 0xb1:if(debug){cout<<"res 6,c"<<endl; }
                C=resetbit(6,C);
                break;
            case 0xb2:if(debug){cout<<"res 6,d"<<endl; }
                D=resetbit(6,D);
                break;
            case 0xb3:if(debug){cout<<"res 6,e"<<endl; }
                E=resetbit(6,E);
                break;
            case 0xb4:if(debug){cout<<"res 6,h"<<endl; }
                H=resetbit(6,H);
                break;
            case 0xb5:if(debug){cout<<"res 6,l"<<endl; }
                L=resetbit(6,L);
                break;
            case 0xb6:if(debug){cout<<"res 6,(hl)"<<endl; }
                write_byte(get_hl(),resetbit(6,read_byte(get_hl())));
                break;
            case 0xb7:if(debug){cout<<"res 6,a"<<endl; }
                A=resetbit(6,A);
                break;
            case 0xb8:if(debug){cout<<"res 7,b"<<endl; }
                B=resetbit(7,B);
                break;
            case 0xb9:if(debug){cout<<"res 7,c"<<endl; }
                C=resetbit(7,C);
                break;
            case 0xba:if(debug){cout<<"res 7,d"<<endl; }
                D=resetbit(7,D);
                break;
            case 0xbb:if(debug){cout<<"res 7,e"<<endl; }
                E=resetbit(7,E);
                break;
            case 0xbc:if(debug){cout<<"res 7,h"<<endl; }
                H=resetbit(7,H);
                break;
            case 0xbd:if(debug){cout<<"res 7,l"<<endl; }
                L=resetbit(7,L);
                break;
            case 0xbe:if(debug){cout<<"res 7,(hl)"<<endl; }
                write_byte(get_hl(),resetbit(7,read_byte(get_hl())));
                break;
            case 0xbf:if(debug){cout<<"res 7,a"<<endl; }
                A=resetbit(7,A);
                break;

            case 0xc0:if(debug){cout<<"set 0,B"<<endl; }
                B=setbit(0,B);
                break;
            case 0xc1:if(debug){cout<<"set 0,C"<<endl; }
                C=setbit(0,C);
                break;
            case 0xc2:if(debug){cout<<"set 0,D"<<endl; }
                D=setbit(0,D);
                break;
            case 0xc3:if(debug){cout<<"set 0,E"<<endl; }
                E=setbit(0,E);
                break;
            case 0xc4:if(debug){cout<<"set 0,H"<<endl; }
                H=setbit(0,H);
                break;
            case 0xc5:if(debug){cout<<"set 0,L"<<endl; }
                L=setbit(0,L);
                break;
            case 0xc6:if(debug){cout<<"set 0,(hl)"<<endl; }
                write_byte(get_hl(),setbit(0,read_byte(get_hl())));
                break;
            case 0xc7:if(debug){cout<<"set 0,A"<<endl; }
                A=setbit(0,A);
                break;
            case 0xc8:if(debug){cout<<"set 1,B"<<endl; }
                B=setbit(1,B);
                break;
            case 0xc9:if(debug){cout<<"set 1,C"<<endl; }
                C=setbit(1,C);
                break;
            case 0xca:if(debug){cout<<"set 1,D"<<endl; }
                D=setbit(1,D);
                break;
            case 0xcb:if(debug){cout<<"set 1,E"<<endl; }
                E=setbit(1,E);
                break;
            case 0xcc:if(debug){cout<<"set 1,H"<<endl; }
                H=setbit(1,H);
                break;
            case 0xcd:if(debug){cout<<"set 1,L"<<endl; }
                L=setbit(1,L);
                break;
            case 0xce:if(debug){cout<<"set 1,(hl)"<<endl; }
                write_byte(get_hl(),setbit(1,read_byte(get_hl())));
                break;
            case 0xcf:if(debug){cout<<"set 1,A"<<endl; }
                A=setbit(1,A);
                break;

            case 0xd0:if(debug){cout<<"set 2,B"<<endl; }
                B=setbit(2,B);
                break;
            case 0xd1:if(debug){cout<<"set 2,C"<<endl; }
                C=setbit(2,C);
                break;
            case 0xd2:if(debug){cout<<"set 2,D"<<endl; }
                D=setbit(2,D);
                break;
            case 0xd3:if(debug){cout<<"set 2,E"<<endl; }
                E=setbit(2,E);
                break;
            case 0xd4:if(debug){cout<<"set 2,H"<<endl; }
                H=setbit(2,H);
                break;
            case 0xd5:if(debug){cout<<"set 2,L"<<endl; }
                L=setbit(2,L);
                break;
            case 0xd6:if(debug){cout<<"set 2,(hl)"<<endl; }
                write_byte(get_hl(),setbit(2,read_byte(get_hl())));
                break;
            case 0xd7:if(debug){cout<<"set 2,A"<<endl; }
                A=setbit(2,A);
                break;
            case 0xd8:if(debug){cout<<"set 3,B"<<endl; }
                B=setbit(3,B);
                break;
            case 0xd9:if(debug){cout<<"set 3,c"<<endl; }
                C=setbit(3,C);
                break;
            case 0xda:if(debug){cout<<"set 3,D"<<endl; }
                D=setbit(3,D);
                break;
            case 0xdb:if(debug){cout<<"set 3,E"<<endl; }
                E=setbit(3,E);
                break;
            case 0xdc:if(debug){cout<<"set 3,H"<<endl; }
                H=setbit(3,H);
                break;
            case 0xdd:if(debug){cout<<"set 3,L"<<endl; }
                L=setbit(3,L);
                break;
            case 0xde:if(debug){cout<<"set 3,(hl)"<<endl; }
                write_byte(get_hl(),setbit(3,read_byte(get_hl())));
                break;
            case 0xdf:if(debug){cout<<"set 3,A"<<endl; }
                A=setbit(3,A);
                break;
            case 0xe0:if(debug){cout<<"set 4,B"<<endl; }
                B=setbit(4,B);
                break;
            case 0xe1:if(debug){cout<<"set 4,C"<<endl; }
                C=setbit(4,C);
                break;
            case 0xe2:if(debug){cout<<"set 4,D"<<endl; }
                D=setbit(4,D);
                break;
            case 0xe3:if(debug){cout<<"set 4,E"<<endl; }
                E=setbit(4,E);
                break;
            case 0xe4:if(debug){cout<<"set 4,H"<<endl; }
                H=setbit(4,H);
                break;
            case 0xe5:if(debug){cout<<"set 4,L"<<endl; }
                L=setbit(4,L);
                break;
            case 0xe6:if(debug){cout<<"set 4,(hl)"<<endl; }
                write_byte(get_hl(),setbit(4,read_byte(get_hl())));
                break;
            case 0xe7:if(debug){cout<<"set 4,A"<<endl; }
                A=setbit(4,A);
                break;
            case 0xe8:if(debug){cout<<"set 5,B"<<endl; }
                B=setbit(5,B);
                break;
            case 0xe9:if(debug){cout<<"set 5,c"<<endl; }
                C=setbit(5,C);
                break;
            case 0xea:if(debug){cout<<"set 5,D"<<endl; }
                D=setbit(5,D);
                break;
            case 0xeb:if(debug){cout<<"set 5,E"<<endl; }
                E=setbit(5,E);
                break;
            case 0xec:if(debug){cout<<"set 5,H"<<endl; }
                H=setbit(5,H);
                break;
            case 0xed:if(debug){cout<<"set 5,L"<<endl; }
                L=setbit(5,L);
                break;
            case 0xee:if(debug){cout<<"set 5,(hl)"<<endl; }
                write_byte(get_hl(),setbit(5,read_byte(get_hl())));
                break;
            case 0xef:if(debug){cout<<"set 5,A"<<endl; }
                A=setbit(5,A);
                break;

            case 0xf0:if(debug){cout<<"set 6,b"<<endl; }
                B=setbit(6,B);
                break;
            case 0xf1:if(debug){cout<<"set 6,c"<<endl; }
                C=setbit(6,C);
                break;
            case 0xf2:if(debug){cout<<"set 6,d"<<endl; }
                D=setbit(6,D);
                break;
            case 0xf3:if(debug){cout<<"set 6,e"<<endl; }
                E=setbit(6,E);
                break;
            case 0xf4:if(debug){cout<<"set 6,h"<<endl; }
                H=setbit(6,H);
                break;
            case 0xf5:if(debug){cout<<"set 6,l"<<endl; }
                L=setbit(6,L);
                break;
            case 0xf6:if(debug){cout<<"set 6,(hl)"<<endl; }
                write_byte(get_hl(),setbit(6,read_byte(get_hl())));
                break;
            case 0xf7:if(debug){cout<<"set 6,l"<<endl; }
                A=setbit(6,A);
                break;
            case 0xf8:if(debug){cout<<"set 7,b"<<endl; }
                B=setbit(7,B);
                break;
            case 0xf9:if(debug){cout<<"set 7,c"<<endl; }
                C=setbit(7,C);
                break;
            case 0xfa:if(debug){cout<<"set 7,d"<<endl; }
                D=setbit(7,D);
                break;
            case 0xfb:if(debug){cout<<"set 7,e"<<endl; }
                E=setbit(7,E);
                break;
            case 0xfc:if(debug){cout<<"set 7,h"<<endl; }
                H=setbit(7,H);
                break;
            case 0xfd:if(debug){cout<<"set 7,l"<<endl; }
                L=setbit(7,L);
                break;
            case 0xfe:if(debug){cout<<"set 7,(hl)"<<endl; }
                write_byte(get_hl(),setbit(7,read_byte(get_hl())));
                break;
            case 0xff:if(debug){cout<<"set 7,l"<<endl; }
                A=setbit(7,A);
                break;
            default:cout<<"bit opcode not implemented "<<(int)op<<endl;cout<<tc<<endl<<pc; exit(0x15);

        }
    }
    void run_op_dd(){
        index=get_ix();
        run_op_ddfd();
        set_ix(index);
    }
    void run_op_fd(){
        index=get_iy();
        run_op_ddfd();
        set_iy(index);
    }
    void run_op_ddfd(){//IX IY
        unsigned char op=next_byte();
        cycles+=OPCODES_CYCLES_IXIY[op];
        switch(op){
            case 0x09:if(debug){cout<<"add ix,bc"<<endl; }
                index=(add16(index,get_bc()));
                break;
            case 0x19:if(debug){cout<<"add ix,de"<<endl; }
                index=(add16(index,get_de()));
                break;
            case 0x21:if(debug){cout<<"ld ix,d16"<<endl; }
                index=next_word();
                break;
            case 0x22:if(debug){cout<<"ld (**),ix"<<endl; }
                write_word(next_word(),index);
                break;
            case 0x23:if(debug){cout<<"inc ix"<<endl; }
                index++;
                break;
            case 0x24:if(debug){cout<<"inc ixh"<<endl; }{
                unsigned char ih=(index>>8);
                inc(ih);
                index=(ih<<8)|(index&0xff);
                break;
                }
            case 0x25:if(debug){cout<<"dec ixh"<<endl; }{
                unsigned char ih=(index>>8);
                DCR(ih);
                index=(ih<<8)|(index&0xff);
                break;
                }
            case 0x26:if(debug){cout<<"ld ixh,*"<<endl; }
                index=(next_byte()<<8)|(index&0xff);
                break;
            case 0x29:if(debug){cout<<"add ix,ix"<<endl; }
                index=(add16(index,index));
                break;
            case 0x2a:if(debug){cout<<"add ix,ix"<<endl; }
                index=read_word(next_word());
                break;
            case 0x2b:if(debug){cout<<"dec ix"<<endl; }
                index--;
                break;
            case 0x2c:if(debug){cout<<"inc ixl"<<endl; }{
                unsigned char il=(index&0xff);
                inc(il);
                index=(il)|(index&0xff00);
                break;
            }
            case 0x2d:if(debug){cout<<"dec ixl"<<endl; }{
                unsigned char il=(index&0xff);
                DCR(il);
                index=(il)|(index&0xff00);
                break;
            }
            case 0x2e:if(debug){cout<<"ld ixl,*"<<endl; }
                index=(next_byte())|(index&0xff00);
                break;
            case 0x34:if(debug){cout<<"inc (ix+d8) "<<endl;}{
                unsigned short adr=index+next_byte();
                unsigned char tmp= read_byte(adr);
                inc(tmp);
                write_byte(adr,tmp);
            }
                break;
            case 0x35:if(debug){cout<<"dec (ix+d8) "<<endl;}{
                unsigned short adr=index+next_byte();
                unsigned char tmp= read_byte(adr);
                DCR(tmp);
                write_byte(adr,tmp);
            }
                break;
            case 0x36:if(debug){cout<<"ld (ix+d8),a"<<endl; }{
                unsigned short adr=index+next_byte();
                write_byte(adr,next_byte());
                break;
            }
            case 0x39:if(debug){cout<<"add ix,sp"<<endl; }
                index=(add16(index,sp));
                break;

            case 0x40 ... 0x43:
                run_op(op);
                break;
            case 0x44:if(debug){cout<<"ld b,ixh"<<endl; }
                B=index>>8;
                break;
            case 0x45:if(debug){cout<<"ld b,ixl"<<endl; }
                B=index&0xff;
                break;

            case 0x46:if(debug){cout<<"ld b,(ix+d8)"<<endl; }
                B=read_byte(index+next_byte());
                break;
            case 0x47 ... 0x4b:
                run_op(op);
                break;

            case 0x4c:if(debug){cout<<"ld c,ixh"<<endl; }
                C=index>>8;
                break;
            case 0x4d:if(debug){cout<<"ld c,ixl"<<endl; }
                C=index&0xff;
                break;

            case 0x4e:if(debug){cout<<"ld b,(ix+d8)"<<endl; }
                C=read_byte(index+next_byte());
                break;
            case 0x4f:
                run_op(0x4f);
                break;

            case 0x50 ... 0x53:
                run_op(op);
                break;
            case 0x54:if(debug){cout<<"ld d,ixh"<<endl; }
                D=index>>8;
                break;
            case 0x55:if(debug){cout<<"ld d,ixl"<<endl; }
                D=index&0xff;
                break;

            case 0x56:if(debug){cout<<"ld d,(ix+d8)"<<endl; }
                D=read_byte(index+next_byte());
                break;

            case 0x57 ... 0x5b:
                run_op(op);
                break;
            case 0x5c:if(debug){cout<<"ld e,ixh"<<endl; }
                E=index>>8;
                break;
            case 0x5d:if(debug){cout<<"ld e,ixl"<<endl; }
                E=index&0xff;
                break;

            case 0x5e:if(debug){cout<<"ld e,(ix+d8)"<<endl; }
                E=read_byte(index+next_byte());
                break;
            case 0x5f:
                run_op(0x5f);
                break;
            case 0x60:if(debug){cout<<"ld ixh, b"<<endl; }{
                unsigned char ih=(index>>8);
                ih=B;
                index=(ih<<8)|(index&0xff);
                break;
            }
            case 0x61:if(debug){cout<<"ld ixh, c"<<endl; }{
                unsigned char ih=(index>>8);
                ih=C;
                index=(ih<<8)|(index&0xff);
                break;
            }
            case 0x62:if(debug){cout<<"ld ixh, d"<<endl; }{
                unsigned char ih=(index>>8);
                ih=D;
                index=(ih<<8)|(index&0xff);
                break;
            }
            case 0x63:if(debug){cout<<"ld ixh, e"<<endl; }{
                unsigned char ih=(index>>8);
                ih=E;
                index=(ih<<8)|(index&0xff);
                break;
            }
            case 0x64:if(debug){cout<<"ld ixh, ixh"<<endl; }{
                unsigned char ih=(index>>8);
                ih=ih;
                index=(ih<<8)|(index&0xff);
                break;
            }
            case 0x65:if(debug){cout<<"ld ixh, ixl"<<endl; }{
                unsigned char ih=(index>>8);
                ih=index&0xff;
                index=(ih<<8)|(index&0xff);
                break;
            }

            case 0x66:if(debug){cout<<"ld h,(ix+d8)"<<endl; }
                H=read_byte(index+next_byte());
                break;
            case 0x67:if(debug){cout<<"ld ixh, a"<<endl; }{
                unsigned char ih=(index>>8);
                ih=A;
                index=(ih<<8)|(index&0xff);
                break;
            }
            case 0x68:if(debug){cout<<"ld ixl, b"<<endl; }{
                unsigned char il=(index&0xff);
                il=B;
                index=(il)|(index&0xff00);
                break;
            }
            case 0x69:if(debug){cout<<"ld ixl, c"<<endl; }{
                unsigned char il=(index&0xff);
                il=C;
                index=(il)|(index&0xff00);
                break;
            }
            case 0x6a:if(debug){cout<<"ld ixl, d"<<endl; }{
                unsigned char il=(index&0xff);
                il=D;
                index=(il)|(index&0xff00);
                break;
            }
            case 0x6b:if(debug){cout<<"ld ixl, e"<<endl; }{
                unsigned char il=(index&0xff);
                il=E;
                index=(il)|(index&0xff00);
                break;
            }
            case 0x6c:if(debug){cout<<"ld ixl, ixh"<<endl; }{
                unsigned char il=(index&0xff);
                il=index>>8;
                index=(il)|(index&0xff00);
                break;
            }
            case 0x6d:if(debug){cout<<"ld ixl, ixl"<<endl; }{
                unsigned char il=(index&0xff);
                il=il;
                index=(il)|(index&0xff00);
                break;
            }
            case 0x6e:if(debug){cout<<"ld l,(ix+d8)"<<endl; }
                L=read_byte(index+next_byte());
                break;
            case 0x6f:if(debug){cout<<"ld ixl, a"<<endl; }{
                unsigned char il=(index&0xff);
                il=A;
                index=(il)|(index&0xff00);
                break;
            }
            case 0x70:if(debug){cout<<"ld (ix+d8),b"<<endl; }
                write_byte(index+next_byte(),B);
                break;
            case 0x71:if(debug){cout<<"ld (ix+d8),c"<<endl; }
                write_byte(index+next_byte(),C);
                break;
            case 0x72:if(debug){cout<<"ld (ix+d8),d"<<endl; }
                write_byte(index+next_byte(),D);
                break;
            case 0x73:if(debug){cout<<"ld (ix+d8),e"<<endl; }
                write_byte(index+next_byte(),E);
                break;
            case 0x74:if(debug){cout<<"ld (ix+d8),h"<<endl; }
                write_byte(index+next_byte(),H);
                break;
            case 0x75:if(debug){cout<<"ld (ix+d8),l"<<endl; }
                write_byte(index+next_byte(),L);
                break;
            case 0x77:if(debug){cout<<"ld (ix+d8),a"<<endl; }{
                unsigned short adr=index+next_byte();
                write_byte(adr,A);
                break;
            }
            case 0x78 ... 0x7b:
                run_op(op);
                break;
            case 0x7c:if(debug){cout<<"ld a,ixh"<<endl; }
                A=index>>8;
                break;
            case 0x7d:if(debug){cout<<"ld a,ixl"<<endl; }
                A=index&0xff;
                break;
            case 0x7e:if(debug){cout<<"ld a,(ix+d8)"<<endl; }
                A=read_byte(index+next_byte());
                break;
            case 0x7f:
                run_op(0x7f);
                break;
            case 0x84:if(debug){cout<<"add a,ixh"<<endl; }
                A=add8(A,index>>8,0);
                break;
            case 0x85:if(debug){cout<<"add a,ixl"<<endl; }
                A=add8(A,index&0xff,0);
                break;
            case 0x86:if(debug){cout<<"add a,(ix+d8)"<<endl; }
                A=add8(A,read_byte(index+next_byte()),0);
                break;
            case 0x8c:if(debug){cout<<"adc a,ixh"<<endl; }
                A=add8(A,index>>8,carry);
                break;
            case 0x8d:if(debug){cout<<"adc a,ixl"<<endl; }
                A=add8(A,index&0xff,carry);
                break;
            case 0x8e:if(debug){cout<<"adc a,(ix+d8)"<<endl; }
                A=add8(A,read_byte(index+next_byte()),carry);
                break;
            case 0x94:if(debug){cout<<"sub a,ixh"<<endl; }
                A=sub8(A,index>>8,0);
                break;
            case 0x95:if(debug){cout<<"sub a,ixl"<<endl; }
                A=sub8(A,index&0xff,0);
                break;
            case 0x96:if(debug){cout<<"sub a,(ix+d8)"<<endl; }
                A=sub8(A,read_byte(index+next_byte()),0);
                break;
            case 0x9c:if(debug){cout<<"sbc a,ixh"<<endl; }
                A=sub8(A,index>>8,carry);
                break;
            case 0x9d:if(debug){cout<<"sbc a,ixl"<<endl; }
                A=sub8(A,index&0xff,carry);
                break;
            case 0x9e:if(debug){cout<<"sbc a,(ix+d8)"<<endl; }
                A=sub8(A,read_byte(index+next_byte()),carry);
                break;
            case 0xa4:if(debug){cout<<"and a,ixh"<<endl; }
                ANA(A,index>>8);
                break;
            case 0xa5:if(debug){cout<<"and a,ixl"<<endl; }
                ANA(A,index&0xff);
                break;
            case 0xa6:if(debug){cout<<"and a,(ix+d8)"<<endl; }
                ANA(A,read_byte(index+next_byte()));
                break;
            case 0xac:if(debug){cout<<"xor a,ixh"<<endl; }
                XRA(A,index>>8);
                break;
            case 0xad:if(debug){cout<<"xor a,ixl"<<endl; }
                XRA(A,index&0xff);
                break;
            case 0xae:if(debug){cout<<"xor a,(ix+d8)"<<endl; }
                XRA(A,read_byte(index+next_byte()));
                break;
            case 0xb4:if(debug){cout<<"or a,ixh"<<endl; }
                ORA(A,index>>8);
                break;
            case 0xb5:if(debug){cout<<"or a,ixl"<<endl; }
                ORA(A,index&0xff);
                break;
            case 0xb6:if(debug){cout<<"or a,(ix+d8)"<<endl; }
                ORA(A,read_byte(index+next_byte()));
                break;
            case 0xbc:if(debug){cout<<"cp a,ixh"<<endl; }
                CMP(A,index>>8);
                break;
            case 0xbd:if(debug){cout<<"cp a,ixl"<<endl; }
                CMP(A,index&0xff);
                break;
            case 0xbe:if(debug){cout<<"cp a,(ix+d8)"<<endl; }
                CMP(A,read_byte(index+next_byte()));
                break;
            case 0xcb:if(debug){cout<<"IX Bit instruction ";}
                run_op_xycb(index+next_byte());
                break;
            case 0xe1:if(debug){cout<<"pop ix"<<endl; }
                index=POP();
                break;
            case 0xe5:if(debug){cout<<"push ix"<<endl; }
                PUSH(index);
                break;
            default:save_state("crash.bin");cout<<"index IX/IY opcode not implemented "<<(int)op<<endl;cout<<tc<<endl<<pc; exit(0x15);

        }
    }

    void run_op_xycb(unsigned short adr){// IX/IY bits
        unsigned char op=next_byte();
        cycles+=OPCODES_CYCLES_XYBITS[op];
        switch(op){
            case 0x06:if(debug){cout<<"rlc (ix+*)"<<endl; }
                write_byte(adr,rlc(read_byte(adr)));
                break;
            case 0x0e:if(debug){cout<<"rrc (ix+*)"<<endl; }
                write_byte(adr,rrc(read_byte(adr)));
                break;
            case 0x16:if(debug){cout<<"rl (ix+*)"<<endl; }
                write_byte(adr,rl(read_byte(adr)));
                break;
            case 0x1e:if(debug){cout<<"rr (ix+*)"<<endl; }
                write_byte(adr,rr(read_byte(adr)));
                break;
            case 0x26:if(debug){cout<<"sla (ix+*)"<<endl; }
                write_byte(adr,sla(read_byte(adr)));
                break;
            case 0x2e:if(debug){cout<<"sra (ix+*)"<<endl; }
                write_byte(adr,sra(read_byte(adr)));
                break;
            case 0x36:if(debug){cout<<"sll (ix+*)"<<endl; }
                write_byte(adr,sll(read_byte(adr)));
                break;
            case 0x3e:if(debug){cout<<"srl (ix+*)"<<endl; }
                write_byte(adr,srl(read_byte(adr)));
                break;

            case 0x40 ... 0x47:if(debug){cout<<"bit 0, (ix+*)"<<endl; }
                testbit(0,read_byte(adr));
                set_35(adr>>8);
                break;
            case 0x48 ... 0x4f:if(debug){cout<<"bit 1, (ix+*)"<<endl; }
                testbit(1,read_byte(adr));
                set_35(adr>>8);
                break;
            case 0x50 ... 0x57:if(debug){cout<<"bit 2, (ix+*)"<<endl; }
                testbit(2,read_byte(adr));
                set_35(adr>>8);
                break;
            case 0x58 ... 0x5f:if(debug){cout<<"bit 3, (ix+*)"<<endl; }
                testbit(3,read_byte(adr));
                set_35(adr>>8);
                break;
            case 0x60 ... 0x67:if(debug){cout<<"bit 4, (ix+*)"<<endl; }
                testbit(4,read_byte(adr));
                set_35(adr>>8);
                break;
            case 0x68 ... 0x6f:if(debug){cout<<"bit 5, (ix+*)"<<endl; }
                testbit(5,read_byte(adr));
                set_35(adr>>8);
                break;
            case 0x70 ... 0x77:if(debug){cout<<"bit 6, (ix+*)"<<endl; }
                testbit(6,read_byte(adr));
                set_35(adr>>8);
                break;
            case 0x78 ... 0x7f:if(debug){cout<<"bit 7, (ix+*)"<<endl; }
                testbit(7,read_byte(adr));
                set_35(adr>>8);
                break;

            case 0x80 ... 0x87:if(debug){cout<<"res 0,(ix+*)"<<endl; }
                write_byte(adr,resetbit(0,read_byte(adr)));
                break;
            case 0x88 ... 0x8f:if(debug){cout<<"res 1,(ix+*)"<<endl; }
                write_byte(adr,resetbit(1,read_byte(adr)));
                break;
            case 0x90 ... 0x97:if(debug){cout<<"res 2,(ix+*)"<<endl; }
                write_byte(adr,resetbit(2,read_byte(adr)));
                break;
            case 0x98 ... 0x9f:if(debug){cout<<"res 3,(ix+*)"<<endl; }
                write_byte(adr,resetbit(3,read_byte(adr)));
                break;
            case 0xa0 ... 0xa7:if(debug){cout<<"res 4,(ix+*)"<<endl; }
                write_byte(adr,resetbit(4,read_byte(adr)));
                break;
            case 0xa8 ... 0xaf:if(debug){cout<<"res 5,(ix+*)"<<endl; }
                write_byte(adr,resetbit(5,read_byte(adr)));
                break;
            case 0xb0 ... 0xb7:if(debug){cout<<"res 6,(ix+*)"<<endl; }
                write_byte(adr,resetbit(6,read_byte(adr)));
                break;
            case 0xb8 ... 0xbf:if(debug){cout<<"res 7,(ix+*)"<<endl; }
                write_byte(adr,resetbit(7,read_byte(adr)));
                break;


            case 0xc0 ... 0xc7:if(debug){cout<<"set 0,(ix+*)"<<endl; }
                write_byte(adr,setbit(0,read_byte(adr)));
                break;
            case 0xc8 ... 0xcf:if(debug){cout<<"set 1,(ix+*)"<<endl; }
                write_byte(adr,setbit(1,read_byte(adr)));
                break;
            case 0xd0 ... 0xd7:if(debug){cout<<"set 2,(ix+*)"<<endl; }
                write_byte(adr,setbit(2,read_byte(adr)));
                break;
            case 0xd8 ... 0xdf:if(debug){cout<<"set 3,(ix+*)"<<endl; }
                write_byte(adr,setbit(3,read_byte(adr)));
                break;
            case 0xe0 ... 0xe7:if(debug){cout<<"set 4,(ix+*)"<<endl; }
                write_byte(adr,setbit(4,read_byte(adr)));
                break;
            case 0xe8 ... 0xef:if(debug){cout<<"set 5,(ix+*)"<<endl; }
                write_byte(adr,setbit(5,read_byte(adr)));
                break;
            case 0xf0 ... 0xf7:if(debug){cout<<"set 6,(ix+*)"<<endl; }
                write_byte(adr,setbit(6,read_byte(adr)));
                break;
            case 0xf8 ... 0xff:if(debug){cout<<"set 7,(ix+*)"<<endl; }
                write_byte(adr,setbit(7,read_byte(adr)));
                break;







            default:cout<<"IX/IY bits opcode not implemented "<<(int)op<<endl;cout<<tc<<endl<<pc; exit(0x15);
        }
    }
};
