/*

G³ówne okno programu wygl¹da tak:

	Text.ClrScr();
	Text.GoTo(0,0);
	Text.SetSpaces(1);
	Text.Write("\x88 \x81   A         B  \x80 ");
	
	Text.GoTo(0,2);
	Text.Write("1");
	
	Text.GoTo(0,5);
	Text.Write("2");

	Text.GoTo(0,7);
	Text.Write("\x89");
	
	Text.SetSpaces(1);
	Text.GoToAbs(10, 2);
	Text.Write("-2.31E31");
	
	// wykropkowanie poziome
	for( uint8_t k=0; k<8; k+=3	){
		for( uint8_t i=6; i<126; i+=6 ){
			Lcd_KS0108.SetPixel( i, 7+(8*k));
		};
	};
	
	// wykropkowanie pionowe
	
	for( uint8_t k=0; k<2; k++ ){
		for( uint8_t i=0; i<60; i+=4 ){
			Lcd_KS0108.SetPixel(6+(60*k), 3+i);
		};
	};
	
	for( uint8_t i=0; i<60; i+=4 ){
		Lcd_KS0108.SetPixel(126, 3+i);
	};

Dostêpne s¹ komórki od A do H (8). Prezenrtowane s¹ zawsze dwa wiersze i dwie kolumny. Strza³kami
przewuwamy zawartoœæ ekranu na po¿¹dane miejsce. 

Program steruj¹cy przechowuje adres komórki znajduj¹cej siê w lewym górnym rogu, pocz¹tkowo jest 
to @A1. 

*************************
Program przechowuje adres lewej górnej komórki. Dotkniêcie strza³ki powoduje jego zmianê zgodnie z
wybranym wariantem oraz aktualizacje widoku na ekranie, czyli ustawienie w³aœciwych liter kolumn oraz
w³aœciwych numerów wierszy, nastêpnie na ich podstawie pobierane s¹ dane z pamiêci ram o wynikach
obliczeñ zawartych w tych komórkach. 
*************************

Dotkniêcie pola komórki wywo³uje okno textbox. 




KOMÓRKA

pole robocze to 8 x 72 = 576 komórek
ka¿da komórka przechowuje wy³¹cznie tekst, maksymalnie 40 znaków
znak '=' wprowadzony przed innymi znakami powoduje obliczenie wyra¿enia o ile to mozliwe
dotkniêcie komórki powoduje wyœwietlenie textboxa. 
jeœli komórka jest pusta, textbox jest pusty
jeœli komórka cokolwiek zawiera, prezentowana jest zawartoœæ tekstowa
odwo³anie do komórki realizowane jest poprzez wpisanie do nowej komórki @ oraz adresu 
z³o¿onego z litery kolumny A-H oraz numeru wiersza 1-72,
elementem docelowym komórki, do której adres podaliœmy jest wynik obliczony w tej komórce,
nie ca³a zawartoœæ tekstowa. 
Dla wartoœci liczbowej prezentowane s¹ 3 cyfry znacz¹ce i maksymalnie 6 znaków. Wyk³adnik E oznacza 10^E,
wyk³adnik e oznacza 10^(-e). Przyk³ady:
123456	- 123E6
123		- 123
-123	- -123
-123123 - -123E6
0.13	- 0.13
0.000123- 123e6
-0.000123 - -123e6
Dotykaj¹c komórki z wartoœci¹ liczbow¹ pojawi siê textbox z wprowadzonym tekstem, wybranie opcji E
wyœwietli dok³adn¹ wartoœæ obliczonego wyra¿enia. Dok³adna wartoœæ oznacza 7 cyfr znacz¹cych. Pozosta³e 
opcje prezentowania wyników pozostaj¹ takie jak wy¿ej. 
W widoku g³ównym prezentowane s¹ maksymalnie 4 pierwsze znaki z wprowadzonego tekstu. 




TEXTBOX
pole textowe prezentuje tekst wprowadzony do komórki. 
textbox zawiera dwie strza³ki kierunku: w lewo i w prawo. S³u¿¹ one do przesuwania kursora edycji tekstu. 
w textboxie widocznych jest maksymalnie 19 znaków, w³¹cznie ze znakiem '='.
Gdy kursor dochodzi do koñca widocznego tekstu, tekst jest przesuwany w odpowiednim kierunku, 
zgodnie z iloœci¹ naciœniêæ odpowiedniej strza³ki lewo-prawo. 
Dotkniêcie pola tekstowego w textboxie przeniesie kursor we wskazane miejsce. Jednoczeœnie jeœli tekst
jest d³u¿szy ni¿ 15 znaków wyra¿enie zostanie tak przesuniête w lewo, by kursor by³ na pozycji 8. 
Nad textboxem prezentowane s¹ znaki opcji edycji. Reprezentuj¹ one dostêpne funkcje wspomagaj¹ce 
edycje tekstu. Ich iloœæ mo¿e byæ zmienna oraz mo¿e byæ zmieniana. 
Pod oknem edycji tekstu znajduje siê standardowa klawiatura. 
Widok textboxu zas³ania ca³y obszar widoku g³ównego.
Wybranie opcji "adres" chwilowo przywraca g³ówny widok programu. Wybranie adresu realizowane jest przez 
dotkniêcie komórki, której adres chcemy pobraæ. Po wybraniu adresu znika widok g³ówny, powraca widok 
edycji textboxa a w tekscie znajduje siê adres wybranej przez dotkniêcie komórki. 
Dotkniêcie opcji OK zatwierdza wporowadzone zmiany.
Text przeznaczony do obliczenia ('=') obliczany jest natychmiast. Warunkiem wyswietlenia wyniku jest
logiczna poprawnoœæ wprowadzonego wyra¿enia. B³¹d w procedurze obliczania sygnalizowny jest stosownym
komunikatem. 
Odwo³anie do pustej komórki traktowane jest jako b³¹d. 
Kursor jest automatycznie ustawiany za ostatnim znakiem znajduj¹cym siê w polu tekstowym. 
Wybranie opcji "O" pozwoli wprowadziæ krótki opis tekstu (20 znaków). PodpowiedŸ nie jest normalnie 
widoczna. 
Textbox domyslnie zawiera pierwszy znak "=". Aby wprowadzaæ tekst nale¿y siê go pozbyæ dotykaj¹c "<-"

Automat - baza wzorów
Baza zawiera gotowe fomu³y pozwalaj¹ce na szybkie obliczenie bardziej z³o¿onych wyra¿eñ 
czêsto spotykanych w codziennej praktyce, na przyk³ad na obliczenie entalpii reakcji lub
czegoœ innego co mi do g³owy nie przychodzi. WeŸmy na przyk³ad równanie stanu gazu idealnego
"pv = nRT".
Aby wybraæ taki wzór nale¿y dotkn¹æ komórki po³o¿nej ni¿ej ni¿ najni¿sza aktualnie zajêta ( o maksymalnym
numerze adresu ), wpisaæ "=+++" oraz dotkn¹æ "OK". 
Program wyœwietli menu z wyborem dziedziny ("MATH", "CHEM", "PHIS", "STAT");
Dotkniêcie pola spowoduje wyœwietlenie nazw dostêpnych równañ lub dokona dalszego zawê¿ania obszaru 
poszukiwania formu³y. Ostateczie wybranie nazwy formu³y spowoduje pokazanie okna ze wszystkimi parametrami
formu³y - nale¿y wybraæ paramter, który chcemy obliczyæ. 
Nastêpnie pojawi siê okno do wprowadzenia wartoœci pozosta³ych parametrów. 
W pierwszej linijce znajduje siê symbol parametru oraz jego nazwa w jêzyku angielskim. 
Poni¿ej znajduje siê pasek do wprowadzenia liczby. 
Poni¿ej paska znajduje siê czêœæ standardowej klawiatury ale zawieraj¹ca jedynie znaki numeryczne 0-9 oraz
kropkê (nie u¿ywamy przecinków). 
Po wprowadzeniu wartoœci wybieramy "OK", pojawia siê nastêpny parametr. 
Gdy skoñczymy wprowadzaæ wartoœci parametrów pojawi siê normalny textbox w wynikiem operacji. Po 
wybraniu "OK" pojawi siê g³ówne okno programu. 
W oknie wybranym do wstawienia wzoru z bazy pojawi siê wynik w formie normalnie przyjêtej (3 cyfry 
znacz¹ce, maksymalnie 6 znaków)
Poni¿ej znajd¹ sie symbole, które wystêpowa³y w równaniu a poni¿ej nich ich wartoœci. Zmiana wartoœci
tych parametrów spowoduje ponowne obliczenie wyniku i jego aktualizacjê. 


Trochê o dzia³aniu i pamiêci. 
Pamiêæ mikroprocesora nie zawiera ¿adnych danych na temat tego, co jest na ekranie. Wszystko znajduje siê 
w zewnêtrzenj pamiêci ram. Pamiêæ ram uC przechowuje tymczasowe kopie obiektów za³adowane z pamiêci ram 
oraz dostarcza zasobów niezbêdnych ale ograniczonych do potrzeb przetwarzania danych. 

Pamiêæ komórki musi zmieœciæ nastêpuj¹ce dane:
pamiêæ wyra¿enia:
	odwo³ania s¹ traktowane jako zwyk³y tekst
	znak równoœci, jesli wyra¿enia podlega obliczaniu
	przyk³adowe wyra¿enie:
		=sin(30)-log(123)
		=-atan(0.1)
		=2+2*2/123.123
 (#@A3)*=@A1/@A2
		(znak @ jest opcj¹ zawart¹ normalnie w oknie textbox)
wynik wyra¿enia, który jest wartoœci¹ typu double (zajmuje 4 bajty). Do tej komórki pamiêci odnosiæ bêdzie
siê adres wprowadzony z innej komórki.
* - kolejnoœæ obliczania - jeœli zmieni siê A1 trzeba obliczyæ A3.
Dlatego: do wyra¿enia, które jest nadrzêdne dla innych dodawana jest informacja, które wyra¿enia nale¿y 
obliczyæ. Dodanie takiej informacji polega na dopisaniu do g³ównego wyra¿enia znaku "#" oraz adresów
komórek, które nale¿y obliczyæ. W przypadku, kiedy komórek jest wiêcej ni¿ jedna, oblicza siê kolejno
komórki. Powstanie doœæ skomplikowna struktura. Trzeba to narysowaæ.
		
	b1 = 1
	b2 = 2
	a1 = b1/b2
	a2 = a1/b1
	a3 = a1/3
		 
W momencie wciœniêcia ok dodaæ do b1, ¿e ma zostaæ obliczone a1 po obliczeniu b1.
Informacja jest dodawana równie¿ do b2, bo nie musi siê zmieniaæ ale przecie¿ te¿ mo¿e.
Zabronione s¹ odwo³ania do w³asnego adresu (wywo³ania cykliczne).

W obec tego komórka ma nastêpuj¹c¹ strukturê:
40 bajtów dla danych znakowych (na ca³y zapamiêtany tekst)
4 bajty wyniku
8 bajtów podpowiedzi
Jedna komórka w pamiêci ram zajmuje dok³adnie 52 bajty. Rozmiar jest sta³y. 

jeszcze:

a1 = 1;	b1 = 3;	"a1" =1#a3b3a5	"b1" =3#b3
a2 = 2;	b2 = 4; "a2" =2#a3		"b2" =4

a3 = a1 + a2;	"a3" =a1+a2#a4
b3 = b1 + a1;	"b3" =b1+a1#a4

a4 = a3 + b3;	"a4" =a3+b3#a5
a5 = a4 + a1;	"a5" =a4+a1


a1=1;	a2=a1+1;	"a1" =1#a2a3	"a2" = a1+1#a3		"a3" =a1+a2
a3 = a1 + a2;
a3 = a1 + a1+1;

Standardowa klawiatura:

		Text.SetSpaces(4);
		
		Text.GoToAbs(5,4);
		Text.Write("1234567890()=");
		Text.GoToAbs(1,5);
		Text.Write("qwertyuiop+-/*");
		Text.GoToAbs(5,6);
		Text.Write("asdfghjkl.:'\"");
		Text.GoToAbs(1,7);
		Text.Write("\x88zxcvbnm<>?[]|");
		
		Text.GoToAbs(5,4);
		Text.Write("1234567890()=");
		Text.GoToAbs(1,5);
		Text.Write("QWERTYUIOP+-/*");
		Text.GoToAbs(5,6);
		Text.Write("ASDFGHJKL.:'\"");
		Text.GoToAbs(1,7);
		Text.Write("\x89ZXCVBNM<>?[]|");
Ramka TextBox

		Text.SetSpaces(0);
		Text.GoTo(0,1);
		Text.Write("\x82=======================\x84");
		
		Text.SetSpaces(1);
		Text.GoTo(0,2);
		Text.Write("\x86=                  \x87");
		
		Text.SetSpaces(0);
		Text.GoTo(0,3);
		Text.Write("\x83=======================\x85");
	
	dane wprowadzamy do TextBoxBuffer, kopiujemy do TextBoxWindow 19 znaków wraz ze znakiem null
	i wyœwietlamy st¹d na ekranie. 	
	
		uint8_t TextBoxBuffer[30];
		uint8_t TextBoxWindow[19];
		
		const char* tt = "qwertyuiopasdfghjkasd"; // wprowadzany tekst l¹duje w TextBoxBuffer, 
		st¹d trafia do pamiêci ram jako parametr komórki o wybranym adresie. 
		
		uint8_t len = strlen(tt);
		uint8_t start = 0;
		if( len > 18 ){
			start = len - 18;
		};
		
		strncpy( (char*)TextBoxWindow, tt+start, 19);
		
		
*************************
zmiana wielkoœci liter na ekranie
uwaga, do wartoœci znaków specjalnych z tablicy lut dodaæ 32 
		if( read_key == 231 ){
			if( TextBoxViev.charsize == 2 ){
				TextBoxViev.SmalChars();
				}else{
				TextBoxViev.BigChars();
			};
			delay(0xFFFF);
		};
				
*/