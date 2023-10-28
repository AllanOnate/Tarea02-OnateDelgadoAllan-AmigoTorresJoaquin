#ifndef _CHECKARGS_H_
#define _CHECKARGS_H_

#include <unistd.h>
#include "global.hh"
extern char *optarg;
extern int optind, opterr, optopt;

class checkArgs {
private:
	// Modificar esta sección
    const std::string opciones = "--threads N --file FILENAME [--help]"; 

	const std::string descripcion = "Modo de uso: ./histograma_mt\n"
								  " --threads: cantidad de threads a utilizar. Si es 1,\n"
								  " entonces ejecuta la versión secuencial.\n"
								  " --file : archivo a procesar.\n"
								  " --help : muestra este mensaje y termina.";

	typedef struct args_t {
        std::string archivo;
		uint32_t numThreads;
	} args_t;

	args_t parametros;

	int argc;
	char **argv;

public:
	checkArgs(int _argc, char **_argv);
	~checkArgs();
	args_t getArgs();

private:
	void printUsage();
};

checkArgs::checkArgs(int _argc, char **_argv) {
	parametros.archivo = "";
	parametros.numThreads = 0;

	argc = _argc;
	argv = _argv;
}

checkArgs::~checkArgs() {
}

checkArgs::args_t checkArgs::getArgs() {
    int opcion;
    int option_index = 0;
    
    static struct option long_options[] = {
        {"threads", required_argument, 0, 't'},
        {"file",    required_argument, 0, 'f'},
        {"help",    no_argument,       0, 'h'},
        {0,         0,                 0,  0 }
    };

    while ((opcion = getopt_long(argc, argv, "t:f:h", long_options, &option_index)) != -1){
        switch (opcion) {
            case 't':
                parametros.numThreads = std::stoi(optarg);
                break;
            case 'f':
                parametros.archivo = optarg;
                break;
            case 'h':
            default:
                printUsage();
                exit(EXIT_SUCCESS);
        }
    }

    if (parametros.numThreads == 0 || parametros.archivo.empty()){
        printUsage();
        exit(EXIT_FAILURE);
    }

    return parametros;
}


void checkArgs::printUsage() {
	std::cout << "Uso: " <<
		argv[0] << " " << opciones << " " << descripcion << std::endl;
}

#endif
