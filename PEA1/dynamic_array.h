#pragma once
class dynamic_array
{
	int size = 0;
	int *data = nullptr;

	/**
	 * Function: generating the string represantaton of this object
	 * Returns: the string representation of this object
	 * Author: Kuba Bigaj
	 */
	std::string gen_string_rep()
	{
		std::string output = "[";
		for (int i = 0; i < size; i++)
		{
			output.append(std::to_string(data[i]) + " ");
		}
		output.append("]\n");
		return output;
	}

public:
	~dynamic_array()
	{
		delete data;
	}

	/**
	 * Function: convinience function for printing the memory to std out
	 * Returns: nothing
	 * Author: Kuba Bigaj
	 */
	void print()
	{
		std::cout << gen_string_rep();
	}

	/**
	 * Function: accessing an element on the given position
	 * Returns: a pointer to the value held at a given position, or a nullptr if there is no element there
	 * Author: Kuba Bigaj
	 */
	int *get(int index)
	{
		if (index < 0 || index >= size)
		{
			return nullptr;
		}
		return &data[index];
	}


	/**
	 * Function: inserting a value at a given index. The index has to be valid (0 <= index <= size)
	 * Returns: nothing
	 * Author: Kuba Bigaj
	 */
	void push(int index, int input)
	{
		if (index < 0 || index > size)
		{
			return;
		}

		int* new_data = new int[size + 1];


		for (int i = 0; i < index; i++)
		{
			new_data[i] = data[i];
		}

		new_data[index] = input;

		for (int i = index; i < size; i++)
		{
			new_data[i + 1] = data[i];
		}

		if (data)
		{
			delete data;
		}
		data = new_data;
		size++;
	}

	/**
	 * Function: removing a value from a given index. The index has to be valid (0 <= index < size)
	 * Returns: nothing
	 * Author: Kuba Bigaj
	 */
	void pop(int index)
	{
		if (index < 0 || index >= size)
		{
			return;
		}

		if (size == 1)
		{
			delete data;
			data = nullptr;
			size = 0;
			return;
		}

		int* new_data = new int[size - 1];

		for (int i = 0; i < index; i++)
		{
			new_data[i] = data[i];
		}

		for (int i = index; i < size - 1; i++)
		{
			new_data[i] = data[i + 1];
		}

		delete data;
		data = new_data;
		size--;
	}

	/**
	 * Function: getting the size of array
	 * Returns: size
	 * Author: Kuba Bigaj
	 */
	int get_size()
	{
		return this->size;
	}
};
