#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <ctime>

#include "models.h"
#include "xcurse.h"

std::vector<Item> items;

void load_data()
{
	std::ifstream is("items.csv");
	
	std::string item;
	while(std::getline(is, item))
	{
		int begin = 0;
		int pos = item.find(",");
		std::string id = item.substr(begin, pos);
		
		begin = pos + 1;
		pos = item.find(",", begin);
		std::string name = item.substr(begin, pos - begin);
		
		begin = pos + 1;
		std::string price = item.substr(begin);
		
		items.push_back({id, name, stod(price)});
	}
	
	is.close();
}

void show_data(int col, int row)
{
	xc::color(0, 7);
	xc::xy(col, row);
		
	for(auto i : items)
	{
		xc::xy(col, row++);
		std::cout << std::setw(5) << i.id;
		std::cout << std::setw(17) << i.name;
		std::cout << std::setw(8) << i.price;
		std::cout << ' ' << std::endl;
	}
	
	xc::color(7, 0);
	xc::xy(col, row);
	
	std::cin.get();
}

Item find_data(std::string id)
{
	for(auto i : items)
		if(i.id == id)
			return i;
		
	return Item{"", "", 0};
}

void print_bill(std::vector<DetailTransaction> details)
{
	auto now = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(now);
	
	std::ofstream os("bill.txt");
	
	os << "            >> Fresh Fruitisia <<" << std::endl;
	os << "#" << tt << "          " << ctime(&tt);
	os << "---------------------------------------------" << std::endl;
	
	double total = 0;
	for(DetailTransaction d : details)
	{
		double sub = d.qty * d.item.price;
		total += sub;
		os << std::setw(20) << d.item.name;
		os << std::setw(4) << d.qty;
		os << std::setw(3) << "x";
		os << std::setw(9) << d.item.price;
		os << std::setw(9) << sub;
		os << std::endl;
	}
	os << "---------------------------------------------" << std::endl;
	os << std::setw(35) << "Total";
	os << std::setw(10) << total << std::endl;
	os << "               -- thanks --";
	
	os.close();
	
	system("start /min notepad /p bill.txt");
}

// ----- User Interface -----

int cedges[] = {2, 15, 46, 58, 65, 77};
int redges[] = {4, 6, 26, 28};

bool repeat = true;

void refresh(int x1, int y1, int x2, int y2)
{
	for(auto r = y1; r <= y2; ++r)
	{
		xc::xy(x1, r);
		for(auto c = x1; c <= x2; ++c)			
			std::cout << ' ';
	}
}

void draw_form()
{
	xc::color(14, 0);
	xc::xy(2,1); std::cout << "CASHIER v0.1";
	
	auto now = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(now);
	
	xc::color(7, 0);
	xc::xy(cedges[0], 3); std::cout << "Batch No.: #";
	xc::color(10, 0);
	std::cout << tt;
	xc::xy(cedges[2], 3); std::cout << std::setw(cedges[5] - cedges[2]) << ctime(&tt);
	
	xc::color(7, 0);
	
	int rheader  = redges[0] + 1;
	
	for(auto e:redges)
	{
		xc::xy(cedges[0] + 1, e);
		for(auto i = cedges[0] + 1; i < cedges[5]; ++i)
		{
			std::cout << char(196);
		}
	}
	
	for(auto i = redges[1] + 1; i < redges[3]; ++i)
	{
		xc::xy(cedges[0], i); std::cout << char(179);
		xc::xy(cedges[5], i); std::cout << char(179);
	}
	
	xc::xy(cedges[0], redges[0]); std::cout << char(218);
	xc::xy(cedges[5], redges[0]); std::cout << char(191);
	xc::xy(cedges[0], redges[1]); std::cout << char(195);
	xc::xy(cedges[5], redges[1]); std::cout << char(180);
	xc::xy(cedges[0], redges[2]); std::cout << char(195);
	xc::xy(cedges[5], redges[2]); std::cout << char(180);
	xc::xy(cedges[0], redges[3]); std::cout << char(192);
	xc::xy(cedges[5], redges[3]); std::cout << char(217);
	
	for(auto e:cedges)
	{
		if(e != cedges[0] && e != cedges[5])
		{
			xc::xy(e, redges[0]); std::cout << char(194);
			xc::xy(e, redges[1]); std::cout << char(193);
		}
		xc::xy(e, rheader); std::cout << char(179);
	}
	
	std::string header[] = {"ID", "Name", "Price", "QTY", "Sub"};
	for(auto i = 0; i < 5; ++i)
	{
		xc::xy(cedges[i] + 2, rheader);
		std::cout << header[i];
	}
	
	xc::xy(cedges[0] + 1, redges[3] - 1);
	std::cout << std::setw(cedges[3] - cedges[0]) << "Total";
	
	xc::xy(cedges[0] + 1, redges[3] + 1);
	std::cout << std::setw(cedges[3] - cedges[0]) << "Pay";
	
	xc::xy(cedges[0] + 1, redges[3] + 2);
	std::cout << std::setw(cedges[3] - cedges[0]) << "Change";
}

void input_form()
{
	system("cls");
	draw_form();
	
	double sub = 0;
	double total = 0;
	std::vector<DetailTransaction> details;
	
	while(true)
	{
		auto row = redges[1] + 1;
		
		for(auto d:details)
		{
			xc::color(11, 0);
			xc::xy(cedges[0] + 2, row); std::cout << d.item.id;
			xc::xy(cedges[1] + 2, row); std::cout << d.item.name;
			xc::xy(cedges[2] + 2, row); std::cout << std::setw(9) << d.item.price;
			xc::xy(cedges[3] + 2, row); std::cout << std::setw(4) << d.qty;
			sub = d.item.price * d.qty;
			xc::color(14, 0);
			xc::xy(cedges[4] + 2, row); std::cout << std::setw(9) << sub;
			++row;
		}
		
		total += sub;
		xc::color(14, 0);
		xc::xy(cedges[4] + 2, redges[3] - 1); std::cout << std::setw(8) << total;
		
		xc::color(7, 0);
		xc::xy(cedges[0] + 2, row);
		std::string id;
		std::getline(std::cin, id);
		
		if(id == "*")
		{
			show_data(cedges[0] + 2, redges[1] + 1);
			refresh(cedges[0] + 1, redges[1] + 1, cedges[5] - 1, redges[2] - 1);
			continue;
		}
		
		if(id == "!")
			return;
		
		if(id == "#")
		{
			repeat = false;
			return;
		}
		
		if(id == "")
			break;
			
		Item i = find_data(id);
			
		if(i.id == "")
			continue;
		
		double qty = 1;
		
		xc::xy(cedges[1] + 2, row); std::cout << i.name;
		xc::xy(cedges[2] + 2, row); std::cout << i.price;
		xc::xy(cedges[3] + 2, row); std::cout << qty;
		
		std::string in_qty;	
		xc::xy(cedges[3] + 2, row);
		getline(std::cin, in_qty);
		if(in_qty != "")
			qty = stod(in_qty);
			
		details.push_back({{i.id, i.name, i.price}, qty});
	}
	
	xc::color(10, 0);
	xc::xy(cedges[4] + 2, redges[3] + 1);
	
	std::string in_pay;
	std::getline(std::cin, in_pay);
	
	if(in_pay == "!")
		return;
	
	auto pay = stod(in_pay);
	xc::xy(cedges[4], redges[3] + 1); std::cout << std::setw(cedges[5] - cedges[4]) << pay;
	
	xc::color(14, 0);
	xc::xy(cedges[4], redges[3] + 2); std::cout << std::setw(cedges[5] - cedges[4]) << pay - total;
	
	//print_bill(details);
	
	xc::color(7, 0);
	xc::xy(cedges[0], redges[3] + 2); std::cout << "Press ";
	xc::color(0, 7); std::cout << " ENTER ";
	xc::color(7, 0); std::cout << " to continue.";
	
	std::cin.get();
}

void do_transaction()
{
	while(repeat)
		input_form();
	
	system("cls");
}
