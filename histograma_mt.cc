#include "include/global.hh"
#include "include/checkArgs.hpp"

std::map<std::string, int> wordHistogram;
std::mutex myMutex;

std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(c)) {
            result += c;
        }
    }
    return result;
}

// Función para dividir una línea en palabras
std::vector<std::string> splitLine(const std::string &line) {
    std::vector<std::string> words;
    std::string word;
    std::istringstream iss(line);
    while (iss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        word = removePunctuation(word);
        words.push_back(word);
    }
    return words;
}

void processLine(const std::string& line) {
    auto words = splitLine(line);
    for (const auto& word : words) {
        {
            std::lock_guard<std::mutex> lock(myMutex);
            wordHistogram[word]++;
        }
    }
}

int main(int argc, char* argv[]) {
    checkArgs argumentos(argc, argv);
    auto args = argumentos.getArgs();

    std::cout << "Número de threads: " << args.numThreads << std::endl;
    std::cout << "Archivo a procesar: " << args.archivo << std::endl;

    std::vector<std::string> textInMemory;
    std::string fileName = args.archivo;

    std::ifstream file(fileName);

    if (!file) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    while (std::getline(file, line)) {
        textInMemory.push_back(line);
    }
    file.close();

    int numThreads = args.numThreads;

    if (numThreads == 1) {
        // Modo secuencial
        for (const auto& line : textInMemory) {
            processLine(line);
        }
    } else {
        // Modo paralelo
        std::vector<std::thread> threads;
        for (const auto& line : textInMemory) {
            threads.push_back(std::thread(processLine, line));
        }

        for (auto& t : threads) {
            t.join();
        }
    }

    // Mostrar el histograma de palabras
    for (const auto& entry : wordHistogram) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }

    return EXIT_SUCCESS;
}

