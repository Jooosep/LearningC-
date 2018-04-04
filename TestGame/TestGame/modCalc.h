#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

class modCalc
{
public:
	modCalc();     //Contructor
	std::map< int, std::vector<int> > curveSolutions1(int mod);
	std::map< int, std::vector<int> > curveSolutions2(int mod);
	std::string soltoString(std::map< int, std::vector<int> > mapped);
private:

};