#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <chrono>
#include "dynamic_array.h"

struct strint
{
	std::string str;
	int num;
};

class App {
	//data storage
	int** matrix = nullptr;
	int size = 0;

	//static UI helper functions
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

	//data manipulation functions
	void dealloc_matrix() //deallocates the matrix, sets size to 0
	{
		if (!matrix)
		{
			size = 0;
			return;
		}
		for (int i = 0; i < size; i++)
		{
			delete matrix[i];
		}
		delete matrix;
		matrix = nullptr;
		size = 0;
	}

	void alloc_matrix(int s) //allocates the matrix
	{
		size = s;
		matrix = new int*[s];
		for (int i = 0; i < s; i++)
		{
			matrix[i] = new int[s];
		}
	}

	void generate_random_data(int in_size) //allocates and fills the matrix with random numbers in range <1, 100>
	{
		dealloc_matrix();
		alloc_matrix(in_size);

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				matrix[i][j] = i!=j ? rand() % 100 + 1 : -1;
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

	void read_data_from_file(std::string filename)
	{
		std::ifstream file(filename);

		if (!file.is_open())
		{
			std::cout << "File open error!\n";
			system("pause");
			return;
		}

		int f_size;
		file >> f_size;

		if (file.fail())
		{
			std::cout << "Size read error!\n";
			file.close();
			system("pause");
			return;
		}

		dealloc_matrix();
		alloc_matrix(f_size);

		int val;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				file >> val;
				if (file.fail())
				{
					std::cout << "Data read error!\n";
					break;
				}
				else
					matrix[i][j] = val;
			}
		}
		file.close();
		return;
	}

	//algorithms
	void brute_force()
	{
		auto start = std::chrono::high_resolution_clock::now();
		int min_path = INT_MAX;
		dynamic_array cities;

		for (int i = 1; i < size; i++)
		{
			cities.push(i - 1, i);
		}

		strint results = brute_check(0, 0, &cities, "");
		auto stop = std::chrono::high_resolution_clock::now();


		std::cout << "Min path = " << results.str << "\n";
		std::cout << "Value = " << results.num << "\n";
		std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count() << "us\n";

		system("pause");
	}

	strint brute_check(int who, int acc_value, dynamic_array* vert_to_check, std::string path) //returns the shortest complete path found from this city
	{
		int checked;
		strint current_best, child_result;
		current_best.num = INT_MAX;
		current_best.str = "";

		path += " -> " + std::to_string(who);

		if (vert_to_check->get_size() == 0)
		{
			acc_value += matrix[who][0];
			current_best.str = path;
			current_best.num = acc_value;
			return current_best;
		}

		for (int i = 0; i < vert_to_check->get_size(); i++)
		{
			checked = *vert_to_check->get(i);
			vert_to_check->pop(i);
			acc_value += matrix[who][checked];
			child_result = brute_check(checked, acc_value, vert_to_check, path);
			vert_to_check->push(i, checked);
			if (child_result.num < current_best.num)
			{
				current_best.num = child_result.num;
				current_best.str = child_result.str;
			}
		}
		return current_best;
	}

	//UI logic functions
	void handle_algorithms()
	{
		std::string options[3] = { "Brute force", "Branch and bound", "Back" };
		int chosen_option = 0;
		while (true)
		{
			chosen_option = create_sub_menu("", options, "", 3, chosen_option);
			switch (chosen_option)
			{
			case 0:
				brute_force();
				break;
			case 1:
				break;
			case 2:
				return;
			default:
				break;
			}
		}


		return;
	}

	void handle_data()
	{
		std::string options[4] = { "Load data", "Generate data", "Show current data", "Back" };
		int chosen_option = 0;
		std::string filename;
		int rozmiar;

		while (true) {
			chosen_option = create_sub_menu("", options, "", 4, chosen_option);
			switch (chosen_option)
			{
			case 0:
				std::cout << "Enter filename:\n";
				std::cin >> filename;
				if (test_input_validity("unsupported filename!\n"))
				{
					read_data_from_file(filename);
					std::cout << "Loaded data: \n";
					show_data();
				}
				break;
			case 1:
				std::cout << "Enter data size:\n";
				std::cin >> rozmiar;
				if (test_input_validity("Incorrect data size!\n"))
				{
					generate_random_data(rozmiar);
					std::cout << "Generated data: \n";
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
		std::string title = "MAIN MENU\n";
		std::string credits = "Kuba Bigaj 2023\n";
		std::string options[3] = { "Algorithms", "Data", "Exit" };
		int chosen_option = 0;

		while (true)
		{
			chosen_option = create_sub_menu(title, options, credits, 3, chosen_option);

			switch (chosen_option)
			{
			case 0:
				handle_algorithms();
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
	void debug()
	{
		read_data_from_file("data4.txt");
		show_data();
		brute_force();
	}
};


int main()
{
	App a;
	a.run();
}