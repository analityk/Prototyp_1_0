#include <TextBoxViev.h>

textBoxViev TextBoxViev;

void textBoxViev::Draw(void)
{
	Text.SetSpaces(1);
	Text.GoTo(0,0);
	Text.Write(" \x8a              \x81  OK");
	
	Text.SetSpaces(0);
	Text.GoTo(0,1);
	Text.Write("\x82=======================\x84");
	
	Text.SetSpaces(1);
	Text.GoTo(0,2);
	Text.Write("\x86=                  \x87");
	
	Text.SetSpaces(0);
	Text.GoTo(0,3);
	Text.Write("\x83=======================\x85");
	Text.GoTo(0,0);
};

void textBoxViev::BigChars(void)
{

	Text.SetSpaces(4);
	Text.GoToAbs(5,4);
	Text.Write("1234567890()=");
	Text.GoToAbs(1,5);
	Text.Write("QWERTYUIOP+-/*");
	Text.GoToAbs(5,6);
	Text.Write("ASDFGHJKL.:'\"");
	Text.GoToAbs(1,7);
	Text.Write("\x89ZXCVBNM<>?[]|");
};

void textBoxViev::SmalChars(void)
{
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
