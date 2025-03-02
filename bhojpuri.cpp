#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <regex>
#include <codecvt>
#include <locale>

// Function to check if a character is a Hindi character
bool isHindiChar(wchar_t c)
{
    return (c >= 0x0900 && c <= 0x097F);
}

// Function to clean and keep only Hindi characters
std::wstring cleanHindiWord(const std::wstring &word)
{
    std::wstring cleaned;
    for (wchar_t c : word)
    {
        if (isHindiChar(c))
        {
            cleaned += c;
        }
    }
    return cleaned;
}

int main()
{
    std::string inputFileName, outputFileName;

    // Get file names from user
    std::cout << "Enter input file name: ";
    std::cin >> inputFileName;
    std::cout << "Enter output file name: ";
    std::cin >> outputFileName;

    // Open input and output files with UTF-8 encoding
    std::wifstream inputFile(inputFileName, std::ios::binary);
    std::wofstream outputFile(outputFileName, std::ios::binary);

    if (!inputFile.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return 1;
    }

    if (!outputFile.is_open())
    {
        std::cerr << "Error opening output file!" << std::endl;
        return 1;
    }

    // Set the locale for UTF-8
    inputFile.imbue(std::locale(inputFile.getloc(), new std::codecvt_utf8<wchar_t>));
    outputFile.imbue(std::locale(outputFile.getloc(), new std::codecvt_utf8<wchar_t>));

    std::unordered_set<std::wstring> uniqueWords;
    std::wstring word;

    // Process the input file
    while (inputFile >> word)
    {
        std::wstring cleanedWord = cleanHindiWord(word);
        if (!cleanedWord.empty())
        {
            uniqueWords.insert(cleanedWord);
        }
    }

    // Write unique Hindi words to the output file
    for (const auto &uniqueWord : uniqueWords)
    {
        outputFile << uniqueWord << L'\n';
    }

    // Close files
    inputFile.close();
    outputFile.close();

    std::cout << "Processing complete. Output saved to " << outputFileName << std::endl;

    return 0;
}