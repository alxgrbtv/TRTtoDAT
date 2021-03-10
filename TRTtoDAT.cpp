#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <iomanip>

using std::cout;
using std::cin;
using std::getline;
using std::map;
using std::string;
using std::ifstream;
using std::ofstream;
using std::regex;
using std::make_pair;
using std::fixed;
using std::setprecision;
using std::filesystem::create_directories;
using std::filesystem::directory_iterator;
using std::filesystem::path;

map<float, float> getMapFromFile(string filePath, string fileName, string fileType, float startWavelenght, float endWavelenght);
string getInputFilePath();
string getRootOfPath(string sPath);
float getWavelength(string startOrEnd);
void createOutputFileFromMap(string filePath, string fileName, string fileType, map<float, float> float_data_map);
void convert(string inputFilePath, string outputFilePath, float startWavelength, float endWavelength, string inputFileType, string outputFileType);
void showOutputFilePath(string outputFilePath);
void closeConsole();

int main()
{   
    string inputFilePath, outputFilePath;
    inputFilePath = getInputFilePath();
    outputFilePath = getRootOfPath(inputFilePath) + "TRTtoDAT\\converted\\";
            
    float startWavelength, endWavelength;
    startWavelength = getWavelength("start");
    endWavelength = getWavelength("end");
  
    string inputFileType = ".trt";
    string outputFileType = ".dat";

    create_directories(outputFilePath);
    convert(inputFilePath, outputFilePath, startWavelength, endWavelength, inputFileType, outputFileType);       
    showOutputFilePath(outputFilePath);

    closeConsole();
}

string getRootOfPath(string sPath)
{
    path pPath(sPath);
    return pPath.root_path().string();
}

map<float, float> getMapFromFile(string filePath, string fileName, string fileType, float startWavelenght, float endWavelenght)
{
    ifstream inputFile;
    inputFile.open(filePath + fileName + fileType);

    string line;
    map<float, float> spectre;

    while (inputFile) {
        getline(inputFile, line);

        regex lineExThreeDigitType = regex("^[0-9][0-9][0-9](,||.)[0-9][0-9];.*");
        regex lineExFourDigitType = regex("^[0-9][0-9][0-9][0-9](,||.)[0-9][0-9];.*");

        float nm;
        float intensity;

        // needs to do something with these "if" statements
        if (regex_match(line, lineExThreeDigitType))
        {
            replace(line.begin(), line.end(), ',', '.');
            nm = stof(line.substr(0, 6));
            intensity = stof(line.substr(7, 9));

            if (nm > startWavelenght && nm < endWavelenght)
            {
                spectre.insert(make_pair(nm, intensity));
            }
        }

        if (regex_match(line, lineExFourDigitType))
        {
            replace(line.begin(), line.end(), ',', '.');
            nm = stof(line.substr(0, 7));
            intensity = stof(line.substr(8, 9));

            if (nm > startWavelenght && nm < endWavelenght)
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
        outputFile << fixed << setprecision(2) << float_data.first << "	"
                   << fixed << setprecision(4) << float_data.second << "\n";
    }
}

string getInputFilePath()
{
    cout << "The console doesn't understand Russian! Underscores only, no spaces! \n"
         << "For more information check out the documentation. \n\n"
         << "If you ready... \n"
         << "Enter path to data (example: C:\\experiment\\trt_data): \n";
    string inputFilePath;
    getline(cin, inputFilePath);
    cout << "\n";
    return inputFilePath + "\\";
}

float getWavelength(string startOrEnd)
{
    if (startOrEnd == "start") { cout << "Enter start wavelength (example: 365): "; }
    if (startOrEnd == "end")   { cout << "Enter end wavelength (example: 385): "; }
    float wavelength;
    cin >> wavelength;
    return wavelength - 0.01f;
}

void convert(string inputFilePath, string outputFilePath, float startWavelength, float endWavelength, string inputFileType, string outputFileType)
{
    for (auto& file : directory_iterator(inputFilePath))
    {
        string fileName;
        fileName = file.path().filename().string();
        fileName = fileName.erase(fileName.length() - 4);
        map<float, float> spectre;
        spectre = getMapFromFile(inputFilePath, fileName, inputFileType, startWavelength, endWavelength);
        createOutputFileFromMap(outputFilePath, fileName, outputFileType, spectre);
    }
}

void showOutputFilePath(string outputFilePath) 
{
    cout << "\n" << "Converting data can be found along the path: \n"
         << outputFilePath << "\n"
         << "Maybe..." << "\n\n";
}

void closeConsole() 
{
    cout << "Press any button to close console... \n";
    cin.ignore(256, '\n');
    cin.get();
}