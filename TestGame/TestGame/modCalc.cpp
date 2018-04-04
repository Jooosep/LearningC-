#include "modCalc.h"


modCalc::modCalc(){}

std::map< int, std::vector<int> > modCalc::curveSolutions1(int mod)
{

	//calculations
	//mod
	std::map< int, std::vector<int> > solutionMap;
	std::vector<int> leftVector;

	for (int i = 0; i < mod; i++)
	{
		int y = int(pow(i, 2));
		leftVector.push_back(y % mod);
	}
	for (int i = 0; i < mod; i++)
	{	
		std::vector<int> yVector;
		int x = int(pow(i, 3) - i);
		std::vector<int>::iterator it = std::find(std::begin(leftVector), std::end(leftVector), x % mod);
		if (it != std::end(leftVector))
		{
			yVector.push_back(int(std::distance(leftVector.begin(), it)));
			//std::cout << "x = " << i << " , y = " << (std::distance(leftVector.begin(), it));
		}
		if (it != std::end(leftVector) && (std::distance(leftVector.begin(), it)) < _int64(leftVector.size() - 1) && (std::distance(leftVector.begin(), it)) > 0)
		{
			int st =int( it - leftVector.begin() + 1);
			std::vector<int>::iterator iter = std::next(leftVector.begin(), st);
			std::vector<int>::iterator itt = std::find(iter, std::end(leftVector), x % mod);
			yVector.push_back(int((std::distance(leftVector.begin(), itt))));
			//std::cout << " or " << (std::distance(leftVector.begin(), itt));
		}
		if (it != std::end(leftVector))
		{
			solutionMap[i] = yVector;
			//std::cout << std::endl;
		}
	}
	return solutionMap;
}

std::map< int, std::vector<int> > modCalc::curveSolutions2(int mod)
{
	std::map< int, std::vector<int> > solutionMap;
	std::vector<int> leftVector;

	for (int i = 0; i < mod; i++)
	{
		int y = int(pow(i, 2));
		leftVector.push_back(y % mod);
	}
	for (int i = 0; i < mod; i++)
	{
		std::vector<int> yVector;
		int x = int(pow(i, 3) - i+1);
		std::vector<int>::iterator it = std::find(std::begin(leftVector), std::end(leftVector), x % mod);
		if (it != std::end(leftVector))
		{
			yVector.push_back(int((std::distance(leftVector.begin(), it))));
			//std::cout << "x = " << i << " , y = " << (std::distance(leftVector.begin(), it));
		}
		if (it != std::end(leftVector) && (std::distance(leftVector.begin(), it)) < _int64(leftVector.size() - 1) && (std::distance(leftVector.begin(), it)) > 0)
		{
			int st = int(it - leftVector.begin() + 1);
			std::vector<int>::iterator iter = std::next(leftVector.begin(), st);
			std::vector<int>::iterator itt = std::find(iter, std::end(leftVector), x % mod);
			yVector.push_back(int((std::distance(leftVector.begin(), itt))));
			//std::cout << " or " << (std::distance(leftVector.begin(), itt));
		}
		if (it != std::end(leftVector))
		{
			solutionMap[i] = yVector;
			//std::cout << std::endl;
		}
	}
	return solutionMap;
}
std::string modCalc::soltoString(std::map< int, std::vector<int> > mapped)
{
	std::string sol;
	std::map< int, std::vector<int> >::iterator it;
	for (it = mapped.begin(); it != mapped.end(); it++)
	{
		sol = sol + "x = " + std::to_string(it->first) + ", y = " + std::to_string(it->second[0]);
		if (it->second.size() > 1)
		{
			sol = sol + " or " + std::to_string(it->second[1]);
		}
		sol = sol + "\n";
	}
	return sol;
}