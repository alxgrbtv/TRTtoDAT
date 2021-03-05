#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <iomanip>

using namespace std;

map<float, float> getMapFromFile(string filePath, string fileName, string fileType, float initLenght, float finitLenght)
{
    ifstream inputFile;
    inputFile.open(filePath + fileName + fileType);

    string line;
    map<float, float> spectre;

    while (inputFile) {
        getline(inputFile, line);

        regex lineEx = regex("^3[0-9][0-9],[0-9][0-9];.*");

        float nm;
        float intensity;

        if (regex_match(line, lineEx))
        {
            replace(line.begin(), line.end(), ',', '.');
            nm = stof(line.substr(0, 6));
            intensity = stof(line.substr(7, 9));

            if (nm > initLenght && nm < finitLenght)
            {
                spectre.insert(make_pair(nm, intensity));
            }            
        }
    }

    return spectre;
}

void createOutputFileFromMap(string filePath, string fileName, string fileType, map<float, float> float_data_map)
{
    ofstream outputFile(filePath + fileName + fileType);

    for (auto& float_data : float_data_map) {
        outputFile << fixed << setprecision(2) << float_data.first << "   "
                   << fixed << setprecision(4) << float_data.second << endl;
    }
}

int main()
{
    cout << "The console doesn't understand Russian! Underscores only, no spaces! \n"
         << "For more information check out the documentation. \n"
         << "If you ready... \n"
         << "Enter path to data (example: C:\\experiment\\trt_data): \n";
    string inputFilePath;
    cin >> inputFilePath;
    inputFilePath = inputFilePath + "\\";

    cout << "Enter initial wavelength without [nm] (example: 365): ";
    float initData;
    cin >> initData;
    initData = initData - 0.05f;

    cout << "Enter finite wavelength without [nm] (example: 385): ";
    float finitData;
    cin >> finitData;
    finitData = finitData + 0.05f;

    string outputFilePath;
    outputFilePath = "C:\\trt_to_dat\\converted\\";
    std::filesystem::create_directory(outputFilePath);
    
    string inputFileType = ".trt";
    string outputFileType = ".dat";

    for (auto& file : std::filesystem::directory_iterator(inputFilePath))
    {
        string fileName;
        fileName = file.path().filename().string();
        fileName = fileName.erase(fileName.length() - 4);
        map<float, float> spectre;
        spectre = getMapFromFile(inputFilePath, fileName, inputFileType, initData, finitData);
        createOutputFileFromMap(outputFilePath, fileName, outputFileType, spectre);
    }

    cout << "\n" << "Converting data can be found along the path: \n"
         << outputFilePath << "\n"
         << "Maybe..." << "\n\n";

    system("pause");
    return 0;
}
