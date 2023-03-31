#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<complex>
#include<chrono>

class ChronoTimer {
public:
	ChronoTimer() {};
	void startTimer() {
		start = std::chrono::system_clock::now();
	}
	void stopTimer() {
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		cumulativeTimeInSeconds += elapsed_seconds.count();
		std::cout << "Elapsed Time: " << elapsed_seconds.count() << " seconds\n";
	}
	double getTotalTimeInSeconds() {
		return cumulativeTimeInSeconds;
	}
	void resetTimer() {
		cumulativeTimeInSeconds = 0;
	}

private:
	std::chrono::time_point<std::chrono::system_clock> start, end;
	double cumulativeTimeInSeconds;
};

class PartialFileWriter {
public:
	PartialFileWriter() = default;
	~PartialFileWriter() {
		closeFile();
	}

	template<typename T>
	void writeBinData(const std::vector<T>& input, const int len) {
		file.write(reinterpret_cast<const char*>(input.data()), len * sizeof(T));
	};

	void openExistingFile() {
		file.open(fileAddress, std::ios::app | std::ios::binary);
		//file.open(fileAddress, std::ios::binary);
		if (!file.is_open())
		{
			//std::cout << " Error, Couldn't find the input file: " << fileAddress << "\n";
			exit(-1);
		}
	}

	void openFile() {
		//file.open(fileAddress, std::ios::app | std::ios::binary);
		file.open(fileAddress, std::ios::binary);
		if (!file.is_open())
		{
			std::cout << " Error, Couldn't find the input file: " << fileAddress << "\n";
			exit(-1);
		}
	}

	void closeFile() {
		if (file.is_open())	file.close();
	};

	void setFileName(const std::string& fileAddress) {
		this->fileAddress = fileAddress;
	}

private:
	std::ofstream file;
	std::string fileAddress;
};

class PartialFileReader {
public:
	PartialFileReader() = default;
	~PartialFileReader() {
		closeFile();
	}

	template<typename T>
	void readBinData(std::vector<T>& input, const int len) {
		file.read(reinterpret_cast<char*>(input.data()), len * sizeof(T));
	};

	template<typename T>
	bool readBinDataAndCheckEOF(std::vector<T>& input, const int len) {
		file.read(reinterpret_cast<char*>(input.data()), len * sizeof(T));
		return file.eof();
	};

	void seekBOF(const int& n_seek_bytes) {
		file.seekg(n_seek_bytes, std::ios::beg);
	}

	template<typename T>
	bool readBinDataAndCheckEOF(std::vector<T>& input, int skipFromCurrent, const int len) {
		file.seekg(skipFromCurrent * sizeof(T), std::ios_base::cur);
		file.read(reinterpret_cast<char*>(input.data()), len * sizeof(T));
		return file.eof();
	};

	void openFile() {
		//file.open(fileAddress, std::ios::app | std::ios::binary);
		file.open(fileAddress, std::ios::binary);
		if (!file.is_open())
		{
			std::cout << " Error, Couldn't find the input file: " << fileAddress << "\n";
			exit(-1);
		}
	}

	void closeFile() {
		if (file.is_open())	file.close();
	};

	void setFileName(const std::string& fileAddress) {
		this->fileAddress = fileAddress;
	}

	int getTotalFileSizeInBytes() {
		int num_elements{};
		file.seekg(0, std::ios::end);
		num_elements = file.tellg();
		file.seekg(0, std::ios::beg);
		return num_elements;
	}

private:
	std::ifstream file;
	std::string fileAddress;
};

//-----------------------
template<typename T>
void readBinData(std::vector<T>& input, std::string fileName, const int dataLen = 0)
{
	//std::cout << "FileName: " << fileName << std::endl;
	std::ifstream signalFile;
	signalFile.open(fileName, std::ios::binary);

	if (!signalFile)
	{
		std::cout << " Error, Couldn't find the input file: " << fileName << "\n";
		exit(-1);
	}
	int num_elements{};
	signalFile.seekg(0, std::ios::end);
	num_elements = signalFile.tellg() / sizeof(T);
	signalFile.seekg(0, std::ios::beg);
	//std::cout << "number of samples in the file: " << num_elements << std::endl;
	if (dataLen != 0 && dataLen <= num_elements)
		num_elements = dataLen;

	input.resize(num_elements);
	std::string signalLine;

	signalFile.read(reinterpret_cast<char*>(input.data()), num_elements * sizeof(T));
	//std::cout << "First 10 inputs:\n";
	//for (int i{}; i < 10; ++i) std::cout << (int)input[i] << std::endl;
	signalFile.close();
}

template<typename T>
int readBinData(T* input, std::string fileName, const int dataLen = 0)
{
	//std::cout << "FileName: " << fileName << std::endl;
	std::ifstream signalFile;
	signalFile.open(fileName, std::ios::binary);

	if (!signalFile)
	{
		std::cout << " Error, Couldn't find the input file: " << fileName << "\n";
		exit(-1);
	}
	int num_elements{};
	signalFile.seekg(0, std::ios::end);
	num_elements = signalFile.tellg() / sizeof(T);
	signalFile.seekg(0, std::ios::beg);
	//std::cout << "number of samples in the file: " << num_elements << std::endl;
	if (dataLen != 0 && dataLen <= num_elements)
		num_elements = dataLen;

	std::string signalLine;

	signalFile.read(reinterpret_cast<char*>(input), num_elements * sizeof(T));
	signalFile.close();
	return num_elements;
}

template<typename T>
void recordData(T* input, int sizeOfWrite, std::string fileName)
{
	std::ofstream outFile;
	outFile.open(fileName, std::ios::binary);
	if (!outFile.is_open()) {
		std::cout << "Error: Output file is not open" << std::endl;
		exit(-1);
	}

	//std::cout << "isOpen, writing " << sizeOfWrite << " numbers to .bin file" << std::endl;
	outFile.write(reinterpret_cast<const char*>(input), sizeOfWrite * sizeof(T));
	outFile.close();
}