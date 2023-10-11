// PEA1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <string>
#include <conio.h>


class App {
	//menu elements
	std::string title = "MENU GŁÓWNE";
	std::string credits = "Kuba Bigaj 2023";
	std::string[3] options = { "ALgorytmy", "Dane", "Wyjście" };

	//data
	int** matrix=nullptr;

	void load_random_data(int size)
	{
		if (!matrix)
		{
			delete matrix;
		}

		matrix = (int**) new int[size*size];

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				matrix[i][j] = rand() % 100 + 1;
			}
		}
	}

	static int create_sub_menu(std::string top_banner, std::string options[], std::string bot_banner, int number, int def_option) // displays a submenu with options, returns the number chosen
	{
		int chosen_option = def_option;
		while (true)
		{
			system("cls");
			std::cout << top_banner;
			for (int i = 0; i < number; i++)
			{
				if (i == chosen_option)
				{
					std::cout << "==>";
				}
				std::cout << "\t" << options[i] << "\n";
			}
			switch (getch())
			{
			case 72:
				chosen_option--;
				break;
			case 80:
				chosen_option++;
				break;
			case '\r':
				return chosen_option;
			default:
				break;
			}
			chosen_option = (chosen_option < 0 ? 0 : (chosen_option >= number ? number - 1 : chosen_option));
		}
	}

public:
	void run()
	{
		create_sub_menu(title, options, credits, 3, 0);
	}
};


int main()
{

	std::cout << "Hello World!\n";
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
