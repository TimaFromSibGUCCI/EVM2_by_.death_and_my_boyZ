#include <fstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>

#define ErrorMemoryOut 0b01
#define ErrorInvalidReg 0b10
#define ErrorWrongCom 0b11

int massive[100];//, value;

int flag; //registr = registr & (~(1 << (k вЂ“ 1)));

int sc_memoryInit(void);

int sc_memorySet(int adress, int value);

int sc_memoryGet(int adress, int *value);

int sc_memorySave(char *filename);

int sc_memoryLoad(char *filename);

int sc_regInit(void);

int sc_regSet(int register, int value);

int sc_regGet(int register, int *value);

int sc_commandEncode (int command, int operand, int *value);

int sc_commandDecode(int value, int *command, int *operand);

int main(void)
{
	system("clear");
	int adress, registr, operand, command, value;
	char* File = "file.bin"; // warning
	sc_memoryInit();
	std::cout << "Input ur adress: ";
	std::cin >> adress;
	std::cin >> value;
	std::cout << std::endl;
	int memorySet = sc_memorySet(adress, value); // = value
	int memoryGet = sc_memoryGet(adress, &value); // = *value
	if(adress > 99)
	{
		std::cout << "Array[" << adress << "] = " << memorySet << "\n";
		std::cout << "Array[" << adress << "] has cell of memory: " << &value << "\n";
	}
	sc_memorySave(&*File);
	sc_memoryLoad(&*File);
	sc_regInit();
	sc_commandEncode(0x11, 0x51, &value);
	sc_commandDecode(value, &command, &operand);
	sc_regGet(registr, &value);
	std::cout.unsetf(std::ios::dec);
	std::cout.setf(std::ios::hex);
	std::cout /*; PERENESITE VERHNIE 2 KOMANDI SUDA std::cout*/ << " Command - " << command << "\tOperand - " << operand << "\n";
	std::cout.unsetf(std::ios::hex);
	std::cout.setf(std::ios::dec);
	system("pause");
	return 0;
}

int sc_memoryInit(void)
{
	for(auto i = 0; i < 100; i++)
		massive[i] = 0;
}

int sc_memorySet(int adress, int value)
{
	if(adress > 99 || adress < 0)
	{
		std::cout << "Wrong number of memeory!(sc_memorySet)\n";
		sc_regSet(ErrorMemoryOut, 1);
	}
	else
	{
		massive[adress] = value;
	}
	return value;
}

int sc_memoryGet(int adress, int *value)
{
	if(adress > 99 || adress < 0)
	{
		std::cout << "Wrong number of memeory!(sc_memoryGet)\n";
		sc_regSet(ErrorMemoryOut, 1);
	}
	else
	{
		*value = massive[adress];
		return *value;
	}
}

int sc_memorySave(char *filename)
{
	std::ofstream file_open(filename, std::ios::binary);
	if(file_open.is_open())
	{
		file_open.write((char*) massive, sizeof(int));
    		{
			std::cout.unsetf(std::ios::dec);
			std::cout.setf(std::ios::hex);
			for(auto i = 0; i < 100; i++)
				file_open << massive[i];
		}
	}
	file_open.close();
}

int sc_memoryLoad(char *filename)
{
	int new_massive[100];
    std::ifstream file_open(filename, std::ios::binary);
    if(file_open.is_open())
    {
    	while(file_open.read((char *) massive, sizeof(int)))
    	{
		std::cin.unsetf(std::ios::dec);
		std::cin.setf(std::ios::hex);
    		for(auto i = 0; i < 100; i++)
        		file_open >> new_massive[i];
    	}
    }
    file_open.close();
    std::cout << "\n\nBinary vision of massive memory: \n";
    for(auto i = 0; i < 100; i++)
    {
	std::cout.unsetf(std::ios::hex); std::cout.setf(std::ios::dec); std::cout << "Array[" << i << "] = "; std::cout.unsetf(std::ios::dec); std::cout.setf(std::ios::hex); std::cout << new_massive[i] << "\t\t";
    }
    std::cout << "\n";
    return 1;
}

int sc_regInit(void)
{
	flag = 0;
	return 0;
}

int sc_regSet(int registr, int value)
{
	if (registr == ErrorMemoryOut || registr == ErrorWrongCom || registr == ErrorInvalidReg)
	{
		if (value == 0)
		{
			flag = flag & ~registr;
		}
		else if (value == 1)
		{
			flag = flag | registr;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

int sc_regGet(int registr, int *value)
{
	if (registr == ErrorMemoryOut || registr == ErrorWrongCom || registr == ErrorInvalidReg)
	{
		if ((flag & registr) != 0)
		{
			*value = 1;
		}
		else
		{
			*value = 0;
		}
	}
	return 0;
}

int sc_commandEncode(int command, int operand, int *value)
{
	if (command < 0x10
		|| (command > 0x11 && command < 0x20)
		|| (command > 0x21 && command < 0x30)
		|| (command > 0x33 && command < 0x40)
		|| (command > 0x43 && command < 0x51)
		|| command > 0x76)
	{
		sc_regSet(ErrorWrongCom, 1);
		return 1;
	}

	if (operand > 0x127 || operand < 0)
	{
		sc_regSet(ErrorInvalidReg, 1);
		return 1;
	}

	*value = (command << 7) | operand;

	return 0;
}

int sc_commandDecode(int value, int *command, int *operand)
{
	if ((value >> 14) != 0)
	{
		return 1;
	}
	*command = value >> 7;
	*operand = value & 0x7F;
	return 0;
}
