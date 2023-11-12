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
		if (!matrix)
			return;

		bool* vis = new bool[size];

		for (int i = 0; i < size; i++)
		{
			vis[i] = false;
		}

		auto start = std::chrono::high_resolution_clock::now();

		strint res = brute_step(0, vis, "", 0, strint("", INT_MAX));

		auto stop = std::chrono::high_resolution_clock::now();

		delete vis;

		res.str.erase(0, 4);

		std::cout << "Min path: " << res.str << "\n";
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
		if (!matrix)
			return;

		bool* vis = new bool[size];
		for (int i = 0; i < size; i++)
		{
			vis[i] = false;
		}

		auto start = std::chrono::high_resolution_clock::now();

		strint res = branch_step(0, vis, "", 0, strint("", INT_MAX));

		auto stop = std::chrono::high_resolution_clock::now();

		delete vis;

		res.str.erase(0, 4);

		std::cout << "Min path: " << res.str << "\n";
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

	void dynamic_programming()
	{
		if (!matrix)
			return;

		int** dp_tab = new int*[size];
		unsigned char** dp_traceback_tab = new unsigned char*[size];

		strint result;

		int num_of_masks = 1 << size;

		for (int i = 0; i < size; i++)
		{
			dp_tab[i] = new int[num_of_masks];
			dp_traceback_tab[i] = new unsigned char[num_of_masks];

			for (int j = 0; j < num_of_masks; j++)
			{
				dp_tab[i][j] = -1;
				dp_traceback_tab[i][j] = UCHAR_MAX;
			}
		}

		auto start = std::chrono::high_resolution_clock::now();

		result.num = dp_step(0x1, 0, dp_tab, dp_traceback_tab);

		auto stop = std::chrono::high_resolution_clock::now();

		dp_trace_path(result.str, dp_traceback_tab);

		for (int i = 0; i < size; i++)
		{
			delete dp_tab[i];
			delete dp_traceback_tab[i];
		}
		delete dp_tab;
		delete dp_traceback_tab;

		std::cout << "Min path: " << result.str << "\n";
		std::cout << "Value = " << result.num << "\n";
		std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n\n";

		system("pause");
	}

	int dp_step(unsigned int cur_mask, int cur_vert, int** dp_tab, unsigned char** dp_traceback_tab)
	{
		if (cur_mask == (1 << size) - 1) //if all the cities were visited...
		{
			return matrix[cur_vert][0]; //...return the distance to the end (0th city) 
		}

		if (dp_tab[cur_vert][cur_mask] != -1) //if we already know the answer...
		{
			return dp_tab[cur_vert][cur_mask]; //...return it
		}

		int result = INT_MAX, prev_vertex;

		for (int i = 0; i < size; i++)
		{
			if ((cur_mask & (1 << i)) == 0) //if the city has not been visited...
			{
				int val = matrix[cur_vert][i] + dp_step(cur_mask | (1 << i), i, dp_tab, dp_traceback_tab); //...calculate the shortest distance to it...
				if (result > val)
				{
					result = val; //...and check wether it is the shortest path to the current city
					prev_vertex = i;
				}
			}
		}

		dp_tab[cur_vert][cur_mask] = result;
		dp_traceback_tab[cur_vert][cur_mask] = prev_vertex;


		return result;
	}

	void dp_trace_path(std::string&  path_so_far, unsigned char** dp_traceback_tab)
	{
		unsigned int mask = 0x1, final_mask = (1 << size) - 1;
		int current_vertex = 0;
		path_so_far += "0";
		while (mask != final_mask)
		{
			current_vertex = dp_traceback_tab[current_vertex][mask];
			mask = mask | (1 << current_vertex);
			path_so_far += " => " + std::to_string(current_vertex);
		}

	}

	auto brute_force_test()
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

		return stop - start;
	}

	auto branch_and_bound_test()
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

		return stop - start;
	}

	auto dynamic_programming_test()
	{
		int** dp_tab = new int*[size];
		unsigned char** dp_traceback_tab = new unsigned char*[size];


		int num_of_masks = 1 << size;

		for (int i = 0; i < size; i++)
		{
			dp_tab[i] = new int[num_of_masks];
			dp_traceback_tab[i] = new unsigned char[num_of_masks];

			for (int j = 0; j < num_of_masks; j++)
			{
				dp_tab[i][j] = -1;
				dp_traceback_tab[i][j] = UCHAR_MAX;
			}
		}

		auto start = std::chrono::high_resolution_clock::now();

		dp_step(0x1, 0, dp_tab, dp_traceback_tab);

		auto stop = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < size; i++)
		{
			delete dp_tab[i];
			delete dp_traceback_tab[i];
		}
		delete dp_tab;
		delete dp_traceback_tab;

		return stop - start;
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
				dynamic_programming();
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
		read_data_from_file("data4.txt");
		branch_and_bound();
		dynamic_programming();
	}

	void run_tests()
	{
		std::chrono::duration<double, std::milli> results[100];

		std::ofstream brute_res("brute_force.csv");
		brute_res.imbue(std::locale("pl_PL.UTF8"));
		brute_res.close();


		for (int i = 3; i <= 12; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				generate_random_data(i);
				results[j] = brute_force_test();
			}

			brute_res.open("brute_force.csv", std::ios::app);
			brute_res << i << "; ";

			for (int j = 0; j < 99; j++)
			{
				brute_res << results[j].count() << "; ";
			}

			brute_res << results[99].count() << "\n";
			brute_res.close();
		}

		std::ofstream branch_res("branch_and_bound.csv");
		branch_res.imbue(std::locale("pl_PL.UTF8"));
		branch_res.close();


		for (int i = 3; i <= 17; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				generate_random_data(i);
				results[j] = branch_and_bound_test();
			}

			branch_res.open("branch_and_bound.csv", std::ios::app);
			branch_res << i << "; ";

			for (int j = 0; j < 99; j++)
			{
				branch_res << results[j].count() << "; ";
			}

			branch_res << results[99].count() << "\n";
			branch_res.close();
		}

		std::ofstream dynamic_res("dynamic_programming.csv");
		dynamic_res.imbue(std::locale("pl_PL.UTF8"));
		dynamic_res.close();


		for (int i = 3; i <= 23; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				generate_random_data(i);
				results[j] = dynamic_programming_test();
			}

			dynamic_res.open("dynamic_programming.csv", std::ios::app);
			dynamic_res << i << "; ";

			for (int j = 0; j < 99; j++)
			{
				dynamic_res << results[j].count() << "; ";
			}

			dynamic_res << results[99].count() << "\n";
			dynamic_res.close();
		}


	}
};


int main(int argc, char *argv[])
{
	App a;

	if (argc > 1)
	{
		if (strcmp(argv[1], "-t") == 0)
		{
			a.run_tests();
			return 0;
		}
	}
	a.run();
	return 0;
}