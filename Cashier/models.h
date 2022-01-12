#pragma once

#include <string>
#include <vector>

struct Item
{
	std::string id;
	std::string name;
	double price;
};

struct DetailTransaction
{
	Item item;
	double qty;
};