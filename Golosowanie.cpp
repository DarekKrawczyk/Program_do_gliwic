#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include <MMSystem.h>

namespace fs = std::filesystem;

void Wyjazd() {
    system("/Praca_Inżynierska/Golosowanie/exec/wyjazd.bat");
}

void Golosowanie_Audio() {
    system("/Praca_Inżynierska/Golosowanie/exec/GachiMuchi.bat");
    PlaySound(TEXT("/Praca_Inżynierska/Golosowanie/exec/audio.wav"), nullptr, SND_SYNC);
}

struct Baza_Danych {
    std::string nazwa;
    int id;
    bool soocka;
    bool lektor;
    int probability;
    Baza_Danych* next;
};

void Co_Ogladamy() {
    std::cout << "Na co chcesz glosowac? W tym koku wybierasz na jakie filmy glosujesz, a w nastepnym co z nimi zrobisz" << std::endl;
    std::cout << "1. Kazdy dostepny film" << std::endl;
    std::cout << "2. Tylko Soocki" << std::endl;
    std::cout << "3. Tylko Lektor" << std::endl;
    std::cout << "4. Tylko Lektor i Soocki" << std::endl;
    std::cout << "==>  ";
};

Baza_Danych* Nowy_Rekord(std::string nazwa, int ID, bool soocka,bool lektor, int probablity) {
    return new Baza_Danych{nazwa, ID, soocka,lektor, probablity};
}

bool sprawdzenie(std::string path);

int Co_Golosujemy(bool kategoria, bool lektor) {
    if (kategoria == false and lektor ==false) {
        return 1;
    } 
    if (kategoria == true and lektor == false) {
        return 2;
    }
    if (kategoria == false and lektor == true) {
        return 3;
    }
    return 4;
}

void losowanie_tablicy_bez_powt(int tablica[], int rozmiar ){

    for (int i = 0; i < rozmiar; i++)
    {
        tablica[i] = (rand() % rozmiar) + 1;
        for (int j = 0; j < i; j++)
        {
            if (tablica[j] == tablica[i])
            {
                tablica[i] = (rand() % rozmiar) + 1;
                j = -1; 
            }
        }
    }
}

/*void Dodaj_Rekord(Baza_Danych* rekord, std::string nazwa, int ID, bool soocka, int probablity) {

    while (rekord) {
        if (rekord->next == nullptr) {
            rekord->next == Nowy_Rekord(nazwa, ID, soocka, probablity);
        }
        rekord = rekord->next;
    }
}*/

int Zliczanie(std::string file_path) {

    int iterator = 0;
    
    for (const auto& entry : fs::directory_iterator(file_path)) {

        fs::path path = fs::path(entry.path());
        std::string path_string = path.string();

        if (sprawdzenie(path_string) == true) {
            iterator++;
        }
    }
    return iterator;
}

int Golosowanie(int tablica[], int ilosc, int prop, int ile, int ID) {

    int wartosc=0;

    for (int i = 0; i < ilosc; i++) {
        if (ID == i) {
            wartosc = tablica[i];
        }
    }

    // PROP - co bedzie ogladane 
    // 1. Wszystko
    // 2. Tylko soock
    // 3 - tylko lektor
    // 4 - tylko lektor i soocki

    switch (prop){
        case 2:
            wartosc = wartosc * ile;
            break;
        case 3:
            wartosc = wartosc * ile;
            break;
        case 4:
            wartosc = wartosc * ile *ile;
            break;
    }
    return wartosc;
}

void Dodaj_Rekord(Baza_Danych* rekord, std::string nazwa, int ID, bool soocka,bool lektor, int probablity) {
    Baza_Danych* nowyRekord = Nowy_Rekord(nazwa, ID, soocka,lektor, probablity);
    if (rekord == nullptr) {
        rekord = nowyRekord;
    }
    else {
        Baza_Danych* nastepnyRekord = rekord;
        while (nastepnyRekord) {
            if (nastepnyRekord->next == nullptr) {
                nastepnyRekord->next = nowyRekord;
                break;
            }
            nastepnyRekord = nastepnyRekord->next;
        }
    }
}

void Wyswietl(const Baza_Danych* rekord) {
    while (rekord != nullptr) {
        std::cout <<"Tytul==> "<< rekord->nazwa << std::endl;
        std::cout <<"ID==> "<< rekord->id+1 << std::endl;
        if (rekord->soocka == true) {
            std::cout << "Socki==> Tak"<< std::endl;
        }
        else {
            std::cout << "Socki==> Nie" << std::endl;
        }
        if (rekord->lektor == true) {
            std::cout << "Lektor==> Tak" << std::endl;
        }
        else {
            std::cout << "Lektor==> Nie" << std::endl;
        }
        std::cout <<"Prawdopodobienstwo==> " << rekord->probability << std::endl;
        rekord = rekord->next;
        std::cout << std::endl;
    }
}

void Zamiana_Prawdopodobienstwa(Baza_Danych* rekord,int Opcja, int mnoznik) {

    // PROP - co bedzie ogladane 
    // 1. Wszystko
    // 2. Tylko soock
    // 3 - tylko lektor
    // 4 - tylko lektor i soocki

    switch (Opcja) {
    case 1:
        while (rekord) {
            rekord->probability = rekord->probability * mnoznik;
            rekord = rekord->next;
    }
        break;
    case 2:
        while (rekord) {
            if (rekord->soocka == true) {
                rekord->probability = rekord->probability * mnoznik;
            }
            rekord = rekord->next;
        }
        break;
    case 3:
        while (rekord) {
            if (rekord->lektor == true) {
                rekord->probability = rekord->probability * mnoznik;
            }
            rekord = rekord->next;
        }
        break;
    case 4:
        while (rekord) {
            if (rekord->soocka == true and rekord->lektor==true) {
                rekord->probability = rekord->probability * mnoznik;
            }
            rekord = rekord->next;
        }
        break;
    }
}

bool Jaka_Kategoria(std::string kategoria) {
    if (kategoria == "Soocka") {
        return true;
    }
    return false;
}

bool Jaki_Lektor(std::string lektor) {
    if (lektor == "Lektor") {
        return true;
    }
    return false;
}

int winner(Baza_Danych* rekord) {
    int maximum = 0;
    int identyfikator = 0;

    while (rekord) {
        if (rekord->probability > maximum) {
            maximum = rekord->probability;
            identyfikator = rekord->id;
        }
        rekord = rekord->next;
    }
    return identyfikator;
};

void winner_text(Baza_Danych* rekord, int identyfikator) {
    while (rekord) {
        if (rekord->id == identyfikator) {
            std::cout << "Zwyciezca: " << rekord->nazwa;
        }
        rekord = rekord->next;
    }
    std::cout<<""<<std::endl;
    std::cout<<""<<std::endl;
    std::cout<<""<<std::endl;
};

std::string Tytul(std::string nazwa) {

    std::size_t begin = nazwa.find_last_of(int(92));
    std::string bez_begin = nazwa.erase(0, begin + 1);

    std::size_t end = bez_begin.find_last_of(".");
    std::string nazwa_koncowa = bez_begin.erase(end, 4);

    std::size_t nawias_pos = bez_begin.find_first_of("(");
    std::string tytul = bez_begin.erase(nawias_pos, bez_begin.length());

    //std::cout << tytul << std::endl;

    return tytul;
}

std::string KategoriaXD(std::string nazwa) {

    std::size_t kategoriaP_pos = nazwa.find_first_of("(");
    std::string kategoria_pomoc = nazwa.erase(0, kategoriaP_pos + 1);
    std::size_t kategoriaK_pos = kategoria_pomoc.find_first_of(")");
    std::string kategoria = kategoria_pomoc.erase(kategoriaK_pos, kategoria_pomoc.length());

    //std::cout <<kategoria << std::endl; // KATEGORIA

    return kategoria;
}

std::string Lektor(std::string nazwa) {

    std::size_t lektor_pos = nazwa.find_last_of("(");
    std::string lektor_pomoc = nazwa.erase(0, lektor_pos + 1);
    std::size_t nawias_pos = lektor_pomoc.find_last_of(")");
    std::string lektor = lektor_pomoc.erase(nawias_pos, lektor_pomoc.length());

    //std::cout << lektor << std::endl; // LEKTOR

    return lektor;
}

bool sprawdzenie(std::string path) {

    int length = path.length();
    std::size_t dot = path.find_last_of(".");
    std::string end = path.erase(0, dot+1);
    if (end == "mp4") {
        return true;
    }
    return false;
}

void Menu1() {
    std::cout << "Witaj w programie Golosowanie" << std::endl;
    std::cout << "1. Dawaj tego szczocha" << std::endl;
    std::cout << "2. Cos tu dodam xD" << std::endl;
    std::cout << "3. Golosowanie_Audio" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "==> ";
};

void Menu2() {
    std::cout << "Teraz podaj przelicznik danego filmu. Podajac przelicznik rowny 0 kompletnie usuwasz mozliwosc wylosowania filmow z tych kategori" << std::endl;
    std::cout << "Przelicznik: ";
}

int main(){

    //Pewnie ogladasz ten kod po kilku miesiacach i nie wiesz co tu sie odwala
    //Spokojnie ja miałem tak samo jak to pisałem  ;D

    srand(time(nullptr));

    std::fstream plik_asci;
    int ID = 0, pierwsza_opcja =0;
    int Iterator = 0;
    int* tablica;
    int ile =1;
    int co_ogladamy = 0;
    char znak;
    int wartosc = 0;
    int wykonanie;

    Baza_Danych* rekord = nullptr;

    std::string pathx = "/CPP/Golosowanie";

    //std::cout << Zliczanie(pathx) << std::endl;

    tablica = new int[Zliczanie(pathx)];

    losowanie_tablicy_bez_powt(tablica, Zliczanie(pathx));

    // TU SIE ZACZYNA MENU

    Menu1();
    std::cin >> pierwsza_opcja;
    system("CLS");
    switch (pierwsza_opcja){
    case 1:
        system("cls");
        Co_Ogladamy();
        std::cin >> co_ogladamy;
        system("CLS");
        Menu2();
        std::cin >> wartosc;
        system("cls");
        break;
    case 2:
        std::cout << "Jeszcze nie dodane";
        znak = getchar();
        exit(0);
        break;
    case 3:
        Golosowanie_Audio();
        break;
    case 4:
        exit(0);
        break;
    }

    // TU SIE KONCZY MENU

    for (auto& entry : fs::directory_iterator(pathx)) {

        fs::path path = fs::path(entry.path());
        std::string path_string = path.string();

        if (sprawdzenie(path_string) == true) {

            std::string tytul=Tytul(path_string);
            std::string kategoria=KategoriaXD(path_string);
            std::string lektor = Lektor(path_string);
            
            //std::cout << kategoria << std::endl;
            //std::cout << tytul << kategoria << std::endl;

            if (rekord==nullptr) {
                rekord = Nowy_Rekord(tytul, ID, Jaka_Kategoria(kategoria), Jaki_Lektor(lektor), Golosowanie(tablica,Zliczanie(pathx),Co_Golosujemy(Jaka_Kategoria(kategoria), Jaki_Lektor(lektor)),ile,ID));
            }
            else {
                Dodaj_Rekord(rekord, tytul, ID, Jaka_Kategoria(kategoria), Jaki_Lektor(lektor), Golosowanie(tablica, Zliczanie(pathx), Co_Golosujemy(Jaka_Kategoria(kategoria), Jaki_Lektor(lektor)), ile,ID));
            }
           
            ID++;
            Iterator++;
        }
    }

    Zamiana_Prawdopodobienstwa(rekord, co_ogladamy, wartosc);
    Golosowanie_Audio();
    winner_text(rekord, winner(rekord));
    //Wyswietl(rekord);
    delete[]tablica;

    std::cout << "Czy powinnosc zostala wykonana?" << std::endl;
    std::cout << "1. Tak" << std::endl;
    std::cout << "2. Nie" << std::endl;
    std::cout << "==>";
    std::cin >> wykonanie;

    if (wykonanie == 2) {
        Wyjazd();
    }
}