#include "MusicCode.h"

MusicCode::MusicCode(int _pin)
{
    _musicPin=_pin;
}

void MusicCode::begin(void)
{
    pinMode(_musicPin,OUTPUT);
}

void MusicCode::transfor(char *code,int *time)
{
    int i,j;
    int numCode=strlen(code);
    for(i=0;i<numCode;i++)
    {
        switch(code[i])
        {
            case 'd': for(j=0;j<(int)(time[i]/7.648);j++)_do();break;
            case 'r': for(j=0;j<(int)(time[i]/7.814);j++)_re();break;
            case 'm': for(j=0;j<(int)(time[i]/7.070);j++)_mi();break;
            case 'f': for(j=0;j<(int)(time[i]/5.730);j++)_fa();break;
            case 's': for(j=0;j<(int)(time[i]/7.648);j++)_so();break;
            case 'l': for(j=0;j<(int)(time[i]/7.814);j++)_la();break;
            case 'x': for(j=0;j<(int)(time[i]/7.070);j++)_xi();break;

            case 'D': for(j=0;j<(int)(time[i]/5.730);j++)Do();break;
            case 'R': for(j=0;j<(int)(time[i]/7.648);j++)re();break;
            case 'M': for(j=0;j<(int)(time[i]/7.814);j++)mi();break;
            case 'F': for(j=0;j<(int)(time[i]/7.070);j++)fa();break;
            case 'S': for(j=0;j<(int)(time[i]/5.730);j++)so();break;
            case 'L': for(j=0;j<(int)(time[i]/7.648);j++)la();break;
            case 'X': for(j=0;j<(int)(time[i]/7.814);j++)xi();break;

            case '1': for(j=0;j<(int)(time[i]/7.070);j++)do_();break;
            case '2': for(j=0;j<(int)(time[i]/5.730);j++)re_();break;
            case '3': for(j=0;j<(int)(time[i]/7.648);j++)mi_();break;
            case '4': for(j=0;j<(int)(time[i]/7.814);j++)fa_();break;
            case '5': for(j=0;j<(int)(time[i]/7.070);j++)so_();break;
            case '6': for(j=0;j<(int)(time[i]/5.730);j++)la_();break;
            case '7': for(j=0;j<(int)(time[i]/5.730);j++)xi_();break;
            case '0': digitalWrite(_musicPin,0),delay(time[i]);break;
        }
    }
}

void MusicCode::transfor(char *code,int time)
{
    int i,j;
    int numCode=strlen(code);
    for(i=0;i<numCode;i++)
    {
        switch(code[i])
        {
            case 'd': for(j=0;j<(int)(time/7.648);j++)_do();break;
            case 'r': for(j=0;j<(int)(time/7.814);j++)_re();break;
            case 'm': for(j=0;j<(int)(time/7.070);j++)_mi();break;
            case 'f': for(j=0;j<(int)(time/5.730);j++)_fa();break;
            case 's': for(j=0;j<(int)(time/7.648);j++)_so();break;
            case 'l': for(j=0;j<(int)(time/7.814);j++)_la();break;
            case 'x': for(j=0;j<(int)(time/7.070);j++)_xi();break;

            case 'D': for(j=0;j<(int)(time/5.730);j++)Do();break;
            case 'R': for(j=0;j<(int)(time/7.648);j++)re();break;
            case 'M': for(j=0;j<(int)(time/7.814);j++)mi();break;
            case 'F': for(j=0;j<(int)(time/7.070);j++)fa();break;
            case 'S': for(j=0;j<(int)(time/5.730);j++)so();break;
            case 'L': for(j=0;j<(int)(time/7.648);j++)la();break;
            case 'X': for(j=0;j<(int)(time/7.814);j++)xi();break;

            case '1': for(j=0;j<(int)(time/7.070);j++)do_();break;
            case '2': for(j=0;j<(int)(time/5.730);j++)re_();break;
            case '3': for(j=0;j<(int)(time/7.648);j++)mi_();break;
            case '4': for(j=0;j<(int)(time/7.814);j++)fa_();break;
            case '5': for(j=0;j<(int)(time/7.070);j++)so_();break;
            case '6': for(j=0;j<(int)(time/5.730);j++)la_();break;
            case '7': for(j=0;j<(int)(time/5.730);j++)xi_();break;
            case '0': digitalWrite(_musicPin,0),delay(time);break;
        }
    }
}

void MusicCode::_do()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(3824);
    digitalWrite(_musicPin,0);
    delayMicroseconds(3824);
}

void MusicCode::_re()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(3407);
    digitalWrite(_musicPin,0);
    delayMicroseconds(3407);
}

void MusicCode::_mi()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(3035);
    digitalWrite(_musicPin,0);
    delayMicroseconds(3035);
}

void MusicCode::_fa()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(2865);
    digitalWrite(_musicPin,0);
    delayMicroseconds(2865);
}

void MusicCode::_so()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(2556);
    digitalWrite(_musicPin,0);
    delayMicroseconds(2556);
}

void MusicCode::_la()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(2278);
    digitalWrite(_musicPin,0);
    delayMicroseconds(2278);
}

void MusicCode::_xi()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(2024);
    digitalWrite(_musicPin,0);
    delayMicroseconds(2024);
}


void MusicCode::Do()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(1912);
    digitalWrite(_musicPin,0);
    delayMicroseconds(1912);
}

void MusicCode::re()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(1704);
    digitalWrite(_musicPin,0);
    delayMicroseconds(1704);
}

void MusicCode::mi()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(1578);
    digitalWrite(_musicPin,0);
    delayMicroseconds(1578);
}

void MusicCode::fa()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(1433);
    digitalWrite(_musicPin,0);
    delayMicroseconds(1433);
}

void MusicCode::so()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(1276);
    digitalWrite(_musicPin,0);
    delayMicroseconds(1276);
}

void MusicCode::la()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(1137);
    digitalWrite(_musicPin,0);
    delayMicroseconds(1137);
}

void MusicCode::xi()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(1012);
    digitalWrite(_musicPin,0);
    delayMicroseconds(1012);
}


void MusicCode::do_()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(956);
    digitalWrite(_musicPin,0);
    delayMicroseconds(956);
}

void MusicCode::re_()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(852);
    digitalWrite(_musicPin,0);
    delayMicroseconds(852);
}

void MusicCode::mi_()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(759);
    digitalWrite(_musicPin,0);
    delayMicroseconds(759);
}

void MusicCode::fa_()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(717);
    digitalWrite(_musicPin,0);
    delayMicroseconds(717);
}

void MusicCode::so_()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(638);
    digitalWrite(_musicPin,0);
    delayMicroseconds(638);
}

void MusicCode::la_()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(569);
    digitalWrite(_musicPin,0);
    delayMicroseconds(569);
}

void MusicCode::xi_()
{
    digitalWrite(_musicPin,1);
    delayMicroseconds(506);
    digitalWrite(_musicPin,0);
    delayMicroseconds(506);
}
