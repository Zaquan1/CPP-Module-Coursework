#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <fstream>

#include <thread>
#include <chrono>
#include <condition_variable>

using namespace std;

class Ship
{

public:
	Ship(int t, string n, int c, int w, int e);
	Ship();
	virtual ~Ship() {};
	bool operator == (const Ship& d) const;
	
	int getEnergyConsumption() const;
	int getWeight() const;
	int getCost() const;
	
	string getTypeName() const;
	int getShipType() const;
	bool isDestroyed() const;
	void destroyShip();

	virtual void status() const { cout << name << "\t" << cost << "\t" << weight << "\t" << energyConsumption << endl; };

	int compareShip(const Ship& d) const;


protected:

	string name;
	int cost;
	int weight;
	int energyConsumption;
	bool destroyed;
	int shipType;
};

class Fleet 
{

public:
	static const int  COLONY = 1;
	static const int SOLAR = 2;
	static const int MILITARY = 3;

	Fleet(string _name);

	int getWeight() const;
	int getEnergyConsumption() const;
	int getColonistCount() const;
	int getCost() const;

	int EnergyProduction() const;
	int countProtectedShip() const;
	bool hasMedic() const;
	string getCorporationName() const;
	
	vector<Ship*> protectedShips() const;
	vector<Ship*> unprotectedShips() const;
	vector<Ship*> colonyShips() const;
	vector<Ship*> shipList() const;

	void addShip(Ship& nShip);
	int getMoney() const;
	void setMoney(int n);
	void setMedic();
	void deleteShip(Ship* i);
	void deleteShip(Ship* i, vector<Ship*>& ships);
	
	void destroyShip(Ship* i);
	void sortFleet();
	void organizedFleet();

	void setdistance(double c) { currentDistance = c; }
	double getdistance() { return currentDistance; };

private:

	double currentDistance;
	string corporationName;
	int money;
	bool medic;

	vector<Ship*> _colonyShips;
	vector<Ship*> _solarSailShips;
	vector<Ship*> _militaryEscortShips;

};



class ColonyShip : public Ship
{

public:
	ColonyShip(string n, int c, int w, int e, int col);
	ColonyShip(const ColonyShip& cs);
	~ColonyShip() {};

	int getColonistCount() const;
	void infect();
	void underProtected();
	bool isInfected() const;
	bool isProtected() const;

	virtual void status() const 
	{ 
		cout << "\t" << name << "\t" << cost << "\t" << colonists << "\t\t" << weight << "\t" << energyConsumption << endl;
	};

protected:
	int colonists;
	bool infected;
	bool _protected;
};

class SolarSailShip : public Ship
{

public:
	SolarSailShip(string n, int c, int w, int e, int ep);
	SolarSailShip(const SolarSailShip& sss);
	~SolarSailShip() {};

	int getEnergyProduction() const;

	virtual void status() const 
	{ 
		cout << "\t" << name << (name.length() == 7 ? "\t\t" : "\t") << cost << "\t" << energyGeneration << "\t\t\t" << weight << "\t" << energyConsumption << endl;
	};

protected:
	int energyGeneration;
};

class MilitaryEscortShip : public Ship
{

public:
	MilitaryEscortShip(string n, int c, int w, int e, int nn);
	MilitaryEscortShip(const MilitaryEscortShip& mes);
	~MilitaryEscortShip() {};

	int getNrProtected() const;

	virtual void status() const 
	{ 
		cout << "\t" << name << "\t" << cost << "\t" << nr << "\t" << weight << "\t" << energyConsumption << endl; 
	};

protected:

	int nr;
};