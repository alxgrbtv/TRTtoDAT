#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <iomanip>

std::map<float, float> getMapFromFile(std::string filePath, std::string fileName, std::string fileType, float initLenght, float finitLenght)
{
    std::ifstream inputFile;
    inputFile.open(filePath + fileName + fileType);

    std::string line;
    std::map<float, float> spectre;

    while (inputFile) {
        getline(inputFile, line);

        std::regex lineExThreeDigitType = std::regex("^[0-9][0-9][0-9](,||.)[0-9][0-9];.*");
        std::regex lineExFourDigitType = std::regex("^[0-9][0-9][0-9][0-9](,||.)[0-9][0-9];.*");

        float nm;
        float intensity;

        if (regex_match(line, lineExThreeDigitType))
        {
            replace(line.begin(), line.end(), ',', '.');
            nm = stof(line.substr(0, 6));
            intensity = stof(line.substr(7, 9));

            if (nm > initLenght && nm < finitLenght)
            {
                spectre.insert(std::make_pair(nm, intensity));
            }            
        }

        if (regex_match(line, lineExFourDigitType))
        {
            replace(line.begin(), line.end(), ',', '.');
            nm = stof(line.substr(0, 7));
            intensity = stof(line.substr(8, 9));

            if (nm > initLenght && nm < finitLenght)
            {
                spectre.insert(std::make_pair(nm, intensity));
            }
        }
    }

    return spectre;
}

void createOutputFileFromMap(std::string filePath, std::string fileName, std::string fileType, std::map<float, float> float_data_map)
{
    std::ofstream outputFile(filePath + fileName + fileType);

    for (auto& float_data : float_data_map) {
        outputFile << std::fixed << std::setprecision(2) << float_data.first << "	"
                   << std::fixed << std::setprecision(4) << float_data.second << "\n";
    }
}

int main()
{
    std::cout << "The console doesn't understand Russian! Underscores only, no spaces! \n"
         << "For more information check out the documentation. \n"
         << "If you ready... \n"
         << "Enter path to data (example: C:\\experiment\\trt_data): \n";
    std::string inputFilePath;
    std::cin >> inputFilePath;
    inputFilePath = inputFilePath + "\\";

    std::cout << "\n" << "Enter initial wavelength without [nm] (example: 365): ";
    float initData;
    std::cin >> initData;
    initData = initData - 0.01f;

    std::cout << "Enter finite wavelength without [nm] (example: 385): ";
    float finitData;
    std::cin >> finitData;
    finitData = finitData + 0.01f;

    std::string outputFilePath;
    outputFilePath = "C:\\trt_to_dat\\converted\\";
    std::filesystem::create_directory(outputFilePath);
    
    std::string inputFileType = ".trt";
    std::string outputFileType = ".dat";

    for (auto& file : std::filesystem::directory_iterator(inputFilePath))
    {
        std::string fileName;
        fileName = file.path().filename().string();
        fileName = fileName.erase(fileName.length() - 4);
        std::map<float, float> spectre;

        for (auto& float_data : spectre) {
            std::cout << std::fixed << std::setprecision(2) << float_data.first << "	"
                << std::fixed << std::setprecision(4) << float_data.second << "\n";
        }

        spectre = getMapFromFile(inputFilePath, fileName, inputFileType, initData, finitData);               
        createOutputFileFromMap(outputFilePath, fileName, outputFileType, spectre);
    }
       
    std::cout << "\n" << "Converting data can be found along the path: \n"
         << outputFilePath << "\n"
         << "Maybe..." << "\n\n";

    system("pause");
    return 0;
}
