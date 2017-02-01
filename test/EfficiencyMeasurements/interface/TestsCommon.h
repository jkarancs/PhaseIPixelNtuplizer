#ifndef TESTS_COMMON_H
#define TESTS_COMMON_H

void printUsage(int& argc, char** argv, bool killProcess, int argsType = 2)
{
	switch(argsType)
	{
		case 2:
			std::cout << "Usage: " << argv[0] << " <Ntuple path> <optional: --savePlots>" << std::endl;
			break;
		default:
			std::cout << "Undefined usage use-case." << std::endl;
	}
	if(killProcess) exit(-1);
}

void processArgs(int& argc, char** argv, std::string& inputFileName, int& savePlots)
{
	if(argc != 2 && argc != 3)
	{
		printUsage(argc, argv, true);
	}
	inputFileName = argv[1];
	if(argc == 3)
	{
		if(std::string(argv[2]) == std::string("--savePlots"))
		{
			savePlots = 1;
		}
		else
		{
			printUsage(argc, argv, true);
		}
	}
	else
	{
		savePlots = 0;
	}
}

void printClusterFieldInfo(const Cluster& clusterField)
{
	std::cout << "--- Begin cluster field info ---" << std::endl;

	std::cout << "det: "        << clusterField.mod_on.det    << "." << std::endl;
	std::cout << "layer: "      << clusterField.mod_on.layer  << "." << std::endl;
	std::cout << "module: "     << clusterField.mod_on.module << "." << std::endl;
	std::cout << "ladder: "     << clusterField.mod_on.ladder << "." << std::endl;
	std::cout << "x: "          << clusterField.x             << "." << std::endl;
	std::cout << "y: "          << clusterField.y             << "." << std::endl;
	std::cout << "clustSizeX: " << clusterField.sizeX         << "." << std::endl;
	std::cout << "clustSizeY: " << clusterField.sizeY         << "." << std::endl;
	std::cout << "clustIndex: " << clusterField.i  << "." << std::endl;
	std::cout << "charge: "     << clusterField.charge        << "." << std::endl;
	
	std::cout << "--- End cluster field info ---" << std::endl;
}

#endif // TESTS_COMMON_H
