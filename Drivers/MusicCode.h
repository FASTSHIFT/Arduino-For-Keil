#ifndef MusicCode_h
#define MusicCode_h

extern"C"
{
#include "FileGroup.h"
}
#include "string.h"

class MusicCode
{
	public:
		MusicCode(int _pin);
		void begin();
		void transfor(char *code,int *time);
		void transfor(char *code,int time);

	private:
		unsigned char   _musicPin;
		void _do();
		void _re();
		void _mi();
		void _fa();
		void _so();
		void _la();
		void _xi();

		void Do();
		void re();
		void mi();
		void fa();
		void so();
		void la();
		void xi();

		void do_();
		void re_();
		void mi_();
		void fa_();
		void so_();
		void la_();
		void xi_();
};

#endif
