#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <chrono>

struct strint
{
	std::string str;
	int num;

	strint(std::string ins, int inn)
	{
		str = ins;
		num = inn;
	}

	strint()
	{
		strint("", 0);
	}
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
				matrix[i][j] = i != j ? rand() % 100 + 1 : -1;
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

	void load_data()
	{
		std::string filename;
		std::cout << "Enter filename:\n";
		std::cin >> filename;
		if (test_input_validity("unsupported filename!\n"))
		{
			read_data_from_file(filename);
			std::cout << "Loaded data: \n";
			show_data();
		}
	}

	void generate_data()
	{
		int rozmiar;
		std::cout << "Enter data size:\n";
		std::cin >> rozmiar;
		if (test_input_validity("Incorrect data size!\n"))
		{
			generate_random_data(rozmiar);
			std::cout << "Generated data: \n";
			show_data();
		}
	}

	//algorithms
	void brute_force()
	{
		bool* vis = new bool[size];
		for (int i = 0; i < size; i++)
		{
			vis[i] = false;
		}
		auto start = std::chrono::high_resolution_clock::now();
		strint res = brute_step(0, vis, "", 0, strint("", INT_MAX));
		auto stop = std::chrono::high_resolution_clock::now();
		delete vis;

		std::cout << "Min path = " << res.str << "\n";
		std::cout << "Value = " << res.num << "\n";
		std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";

		system("pause");
	}

	strint brute_step(int cur_vert, bool* &vis, std::string path, int cur_distance, strint cur_best)
	{
		bool is_last = true;
		vis[cur_vert] = true;

		path += " => " + std::to_string(cur_vert);

		for (int i = 0; i < size; i++)
		{
			if (vis[i])
				continue;
			is_last = false;
			strint res = brute_step(i, vis, path, cur_distance + matrix[cur_vert][i], cur_best);
			if (res.num < cur_best.num)
			{
				cur_best = res;
			}
		}

		vis[cur_vert] = false;

		if (is_last)
		{
			return strint(path, cur_distance + matrix[cur_vert][0]);
		}

		return cur_best;
	}

	void branch_and_bound()
	{
		bool* vis = new bool[size];
		for (int i = 0; i < size; i++)
		{
			vis[i] = false;
		}
		auto start = std::chrono::high_resolution_clock::now();
		strint res = branch_step(0, vis, "", 0, strint("", INT_MAX));
		auto stop = std::chrono::high_resolution_clock::now();
		delete vis;

		std::cout << "Min path = " << res.str << "\n";
		std::cout << "Value = " << res.num << "\n";
		std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";

		system("pause");
	}

	strint branch_step(int cur_vert, bool* &vis, std::string path, int cur_distance, strint cur_best)
	{
		bool is_last = true;
		vis[cur_vert] = true;

		path += " => " + std::to_string(cur_vert);

		for (int i = 0; i < size; i++)
		{
			if (vis[i])
				continue;
			is_last = false;

			if (bind_branch(vis) + cur_distance + matrix[cur_vert][i] > cur_best.num)
				continue;

			strint res = branch_step(i, vis, path, cur_distance + matrix[cur_vert][i], cur_best);
			if (res.num < cur_best.num)
			{
				cur_best = res;
			}
		}

		vis[cur_vert] = false;

		if (is_last)
		{
			return strint(path, cur_distance + matrix[cur_vert][0]);
		}

		return cur_best;
	}

	int bind_branch(bool* vis)
	{
		int res = 0;
		for (int i = 0; i < size; i++)
		{
			if (vis[i])
				continue;
			int min = matrix[i][0];
			for (int j = 1; j < size; j++)
			{
				if (vis[j])
					continue;
				min = min > matrix[i][j] ? matrix[i][j] : min;
			}

			res += min;
		}
		return res;
	}

public:
	void run()
	{
		std::string title = "MAIN MENU\n";
		std::string credits = "Kuba Bigaj 2023\n";
		std::string options[7] = { "Load data", "Generate data", "Show current data", "Brute force", "Branch and bound","Dynamic programming", "Exit" };
		int chosen_option = 0;

		while (true)
		{
			chosen_option = create_sub_menu(title, options, credits, 7, chosen_option);

			switch (chosen_option)
			{
			case 0:
				load_data();
				break;
			case 1:
				generate_data();
				break;
			case 2:
				show_data();
				break;
			case 3:
				brute_force();
				break;
			case 4:
				branch_and_bound();
				break;
			case 5:
				std::cout << "Unsupported operation!\n";
				system("pause");
				break;
			case 6:
				return;
			default:
				break;
			}
		}
	}
	void debug()
	{
		generate_random_data(12);
		show_data();
		brute_force();
		branch_and_bound();
	}
};


int main()
{
	App a;
	a.run();
}