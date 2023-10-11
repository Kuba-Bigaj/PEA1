// PEA1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <string>
#include <conio.h>


class App {
	//menu elements
	std::string title = "MENU GŁÓWNE\n";
	std::string credits = "Kuba Bigaj 2023\n";
	std::string options[3] = { "Algorytmy", "Dane", "Wyjście" };
	int chosen_option = 0;



	//data
	int** matrix = nullptr;
	int size = 0;

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
			std::cout << bot_banner;
			switch (_getch())
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

	static bool test_input_validity(std::string err_message)
	{
		if (std::cin.fail())
		{
			std::cout << err_message << "\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			system("pause");
			return false;
		}
		return true;
	}


	void generate_random_data(int in_size)
	{
		if (!matrix)
		{
			for (int i = 0; i < size; i++)
			{
				delete matrix[i];
			}
			delete matrix;
		}

		this->size = in_size;
		matrix = new int*[size];

		for (int i = 0; i < size; i++)
		{
			matrix[i] = new int[size];
		}


		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				matrix[i][j] = rand() % 100 + 1;
			}
		}
	}

	void show_data()
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				std::cout << matrix[i][j] << "\t";
			}
			std::cout << "\n";
		}
		system("pause");
	}

	void read_data_from_file(std::string filename);

	void handle_algorithms()
	{
		return;
	}

	void handle_data()
	{
		std::string o[4] = { "Wczytaj dane", "Wygeneruj dane", "Wyświetl dane", "Wróć" };
		int ch_o = 0;

		while (true) {
			ch_o = create_sub_menu("", o, "", 4, ch_o);
			switch (ch_o)
			{
			case 0:
				//TODO file read
				break;
			case 1:
				int rozmiar;
				std::cout << "Podaj rozmiar danych do wygenerowania:\n";
				std::cin >> rozmiar;
				if (test_input_validity("Błędne dane!\n"))
				{
					generate_random_data(rozmiar);
					std::cout << "Wygenerowane dane: \n";
					show_data();
				}
				break;
			case 2:
				show_data();
				break;
			case 3:
				return;
			default:
				break;
			}
		}
	}


public:
	void run()
	{
		while (true)
		{
			chosen_option = create_sub_menu(title, options, credits, 3, chosen_option);

			switch (chosen_option)
			{
			case 0:
				break;
			case 1:
				handle_data();
				break;
			case 2:
				return;
			default:
				break;
			}
		}
	}
};


int main()
{
	App a;
	a.run();
	std::cout << "Owari da.\n";
}