#pragma once

#include <iostream>
#include <iomanip>
#include <string>

class ProgressBar
{
	public:
		void update(double newProgress)
		{
			currentProgress += newProgress;
			amountOfFiller = (int)((currentProgress / neededProgress) * (double)pBarLength);
		}
		void print()
		{
			currUpdateVal %= pBarUpdater.length();
			std::cout << "\rProgress: " << firstPartOfpBar;
			for(int a = 0; a < amountOfFiller; a++)
			{
				std::cout << pBarFiller;
			}
			std::cout << pBarUpdater[currUpdateVal];
			for(int b = 0; b < pBarLength - amountOfFiller; b++)
			{
				std::cout << " ";
			}
			std::cout << lastPartOfpBar
				 << " (" << std::setw(3) << (int)(100 * (currentProgress/neededProgress)) << "%)"
				 << std::flush;
			currUpdateVal += 1;
		}
		void printNoUpdate()
		{
			currUpdateVal %= pBarUpdater.length();
			std::cout << " " << firstPartOfpBar;
			for(int a = 0; a < amountOfFiller; a++)
			{
				std::cout << pBarFiller;
			}
			std::cout << pBarUpdater[currUpdateVal];
			for(int b = 0; b < pBarLength - amountOfFiller; b++)
			{
				std::cout << " ";
			}
			std::cout << lastPartOfpBar
				 << " (" << std::setw(3) << (int)(100 * (currentProgress/neededProgress)) << "%)"
				 << std::flush;
			currUpdateVal += 1;
		}
		double getProgressionRate()
		{
			return currentProgress / neededProgress;
		}
		std::string firstPartOfpBar = "[",
					lastPartOfpBar = "]",
					pBarFiller = "|",
					pBarUpdater = "/-\\|";
	private:
		int amountOfFiller     = 0;
		int pBarLength         = 20;
		int currUpdateVal      = 0;
		double currentProgress = 0;
		double neededProgress  = 100;
};
