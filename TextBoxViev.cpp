#include <TextBoxViev.h>

textBoxViev TextBoxViev;

void textBoxViev::Draw(void)
{
	Text.SetSpaces(1);
	Text.GoTo(0,0);
	Text.Write("\x8a              \x81  OK\n");
	
	//Text.SetSpaces(0);
	Text.GoTo(0,1);
	Text.Write("\x82===================\x84\n");
	
	//Text.SetSpaces(1);
	Text.GoTo(0,2);
	Text.Write("\x86=                  \x87\n");
	
	//Text.SetSpaces(0);
	Text.GoTo(0,3);
	Text.Write("\x83===================\x85\n");
	Text.GoTo(0,0);
};

void textBoxViev::BigChars(void)
{
	keycode = 2;
	Text.SetSpaces(4);
	Text.GoToAbs(5,4);
	Text.Write("1234567890()=\n");
	Text.GoToAbs(1,5);
	Text.Write("QWERTYUIOP+-/*\n");
	Text.GoToAbs(5,6);
	Text.Write("ASDFGHJKL.:'\"\n");
	Text.GoToAbs(1,7);
	Text.Write("\x89ZXCVBNM<>?[]|\n");
};

void textBoxViev::SmalChars(void)
{
	keycode = 3;
	Text.SetSpaces(4);
	Text.GoToAbs(5,4);
	Text.Write("1234567890()=");
	Text.GoToAbs(1,5);
	Text.Write("qwertyuiop+-/*");
	Text.GoToAbs(5,6);
	Text.Write("asdfghjkl.:'\"");
	Text.GoToAbs(1,7);
	Text.Write("\x88zxcvbnm<>?[]|");
};

