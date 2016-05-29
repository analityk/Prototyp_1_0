#include <menu.h>

void enterScreen(void){
	kb.Init();
	
	Text.GoTo(7,0);
	Text.WriteString("Siema");
	Text.GoTo(6,7);
	Text.WriteString("zapraszam");
	
	kb.LoadGroup(kb.KeysNavi);
	
	
	Text.Refresh();
	kb.DrawVisible();
};
