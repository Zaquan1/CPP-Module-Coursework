#include "Fleet.h"


Fleet::Fleet(string _name)
	: corporationName(_name), money(10000), medic(false)
{
}

int Fleet::getWeight() const
{
	int weight = 0;
	vector<Ship*> list = shipList();

	for (int i = 0; i < list.size(); i++)
	{
		weight += list.at(i)->getWeight();
	}
	if (hasMedic())
		weight++;

	return weight;
}

int Fleet::getEnergyConsumption() const
{
	int consumption = 0;
	vector<Ship*> list = shipList();

	for (int i = 0; i < list.size(); i++)
	{
		consumption += list.at(i)->getEnergyConsumption();
	}
	if (hasMedic())
		consumption++;

	return consumption;
}

int Fleet::getColonistCount() const
{
	ColonyShip * ship;
	int colonistCount = 0;
	for (int i = 0; i < _colonyShips.size(); i++)
	{
		ship = dynamic_cast<ColonyShip*>(_colonyShips.at(i));
		colonistCount += ship->getColonistCount();
	}
	return colonistCount;

}

int Fleet::getCost() const
{
	int cost = 0;
	vector<Ship*> list = shipList();

	for (int i = 0; i < list.size(); i++)
	{
		cost += list.at(i)->getCost();
	}
	if (hasMedic())
		cost += 1000;
	return cost;
}

int Fleet::EnergyProduction() const
{
	SolarSailShip * ship;
	int eProduction = 0;
	for (int i = 0; i < _solarSailShips.size(); i++)
	{
		ship = dynamic_cast<SolarSailShip*>(_solarSailShips.at(i));
		eProduction += ship->getEnergyProduction();
	}
	return eProduction;
}

int Fleet::countProtectedShip() const
{
	ColonyShip * ship;
	int numProtected = 0;
	for (int i = 0; i < _colonyShips.size(); i++)
	{
		ship = dynamic_cast<ColonyShip*>(_colonyShips.at(i));
		if (ship->isProtected())
		{
			numProtected++;
		}
	}
	return numProtected;
	
}

bool Fleet::hasMedic() const
{
	return medic;
}

string Fleet::getCorporationName() const
{
	return corporationName;
}

vector<Ship*> Fleet::protectedShips() const
{
	vector<Ship*> pShip;
	ColonyShip * ship;
	for (int i = 0; i < _colonyShips.size(); i++)
	{
		ship = dynamic_cast<ColonyShip*>(_colonyShips.at(i));
		if (ship->isProtected())
		{
			pShip.push_back(ship);
		}
	}
	return pShip;
}

vector<Ship*> Fleet::unprotectedShips() const
{
	vector<Ship*> unpShip;
	ColonyShip * ship;
	for (int i = 0; i < _colonyShips.size(); i++)
	{
		ship = dynamic_cast<ColonyShip*>(_colonyShips.at(i));
		if (!ship->isProtected())
		{
			unpShip.push_back(ship);
		}
	}
	return unpShip;
}

vector<Ship*> Fleet::colonyShips() const
{
	return _colonyShips;
}

vector<Ship*> Fleet::shipList() const
{
	vector<Ship*> list;
	list.reserve(_solarSailShips.size() + _militaryEscortShips.size() + _colonyShips.size());
	list.insert(list.end(), _colonyShips.begin(), _colonyShips.end());
	list.insert(list.end(), _solarSailShips.begin(), _solarSailShips.end());
	list.insert(list.end(), _militaryEscortShips.begin(), _militaryEscortShips.end());
	
	return list;
}

void Fleet::addShip(Ship* nShip)
{
	int shipType = nShip->getShipType();

	if (money - nShip->getCost() >= 0)
	{
		if (shipType == COLONY)
		{
			_colonyShips.push_back(nShip);
		}
		else if (shipType == SOLAR)
		{
			_solarSailShips.push_back(nShip);
		}
		else if (shipType == MILITARY)
		{
			_militaryEscortShips.push_back(nShip);
		}
		money = money - nShip->getCost();
		cout << endl << nShip->getTypeName() << " successfully purchased\n";
	}
	else
	{
		cout << endl << "Current Money: " << getMoney() << ", not enough to buy " << nShip->getTypeName() << endl;
	}
}

int Fleet::getMoney() const { return money; }

void Fleet::setMoney(int n) { money = n; }

void Fleet::setMedic() { medic = !medic; }

void Fleet::destroyShip(Ship* i) { i->destroyShip(); }

void Fleet::deleteShip(Ship* i)
{
	int type = i->getShipType();

	if (type == COLONY)
	{
		deleteShip(i, _colonyShips);
	}
	else if (type == MILITARY)
	{
		deleteShip(i, _militaryEscortShips);
	}
	else if (type == SOLAR)
	{
		deleteShip(i, _solarSailShips);
	}
}

void Fleet::deleteShip(Ship* i, vector<Ship*>& ships)
{
	int k;
	for (k = 0; k < ships.size(); k++)
	{
		if (ships.at(k) == i)
		{
			ships.erase(ships.begin()+k);
			money += i->getCost();
			delete i;
		}
	}
}


Ship::Ship(int t, string n, int c, int w, int e) 
	:shipType(t), name(n), cost(c), weight(w), energyConsumption(e), destroyed(false)
{}

bool Ship::operator == (const Ship& d) const
{
	cout << "compare" << endl;
	return 0 == compareShip(d);
}

string Ship::getTypeName() const { return name; }

int Ship::getShipType() const { return shipType; }

int Ship::getEnergyConsumption() const { return energyConsumption; }

int Ship::getWeight() const { return weight; }

int Ship::getCost() const { return cost; }

bool Ship::isDestroyed() const { return destroyed; }

void Ship::destroyShip() { destroyed = true; }

int Ship::compareShip(const Ship& d) const
{
	if (name == d.name)
		return 1;
	if (weight == d.weight)
		return 1;
	if (cost == d.cost)
		return 1;
	if (energyConsumption == d.energyConsumption)
		return 1;
	return 0;
}


ColonyShip::ColonyShip(string n, int c, int w, int e, int col)
	: Ship(1, n, c, w, e), colonists(col), infected(false), _protected(false)
{}
ColonyShip::ColonyShip(const ColonyShip& cs)
	: ColonyShip(cs.name, cs.cost, cs.weight, cs.energyConsumption, cs.colonists)
{}

int ColonyShip::getColonistCount() const { return colonists; }

bool ColonyShip::isInfected() const { return infected; }

bool ColonyShip::isProtected() const { return _protected; }

void ColonyShip::underProtected() { _protected = true; }

void ColonyShip::infect() { infected = true; }


SolarSailShip::SolarSailShip(string n, int c, int w, int e, int eg)
	: Ship(2, n, c, w, e), energyGeneration(eg)
{}
SolarSailShip::SolarSailShip(const SolarSailShip& sss)
	: SolarSailShip(sss.name, sss.cost, sss.weight, sss.energyConsumption, sss.energyGeneration)
{}

int SolarSailShip::getEnergyProduction() const { return energyGeneration; }


MilitaryEscortShip::MilitaryEscortShip(string n, int c, int w, int e, int nn)
	: Ship(3, n, c, w, e), nr(nn)
{}
MilitaryEscortShip::MilitaryEscortShip(const MilitaryEscortShip& mes)
	: MilitaryEscortShip(mes.name, mes.cost, mes.weight, mes.energyConsumption, mes.nr)
{}

int MilitaryEscortShip::getNrProtected() const { return nr; }

//default ship
vector<ColonyShip> colony;
vector<SolarSailShip> solaris;
vector<MilitaryEscortShip> fighter;

//default ship option
void createShips()
{
	colony.push_back(ColonyShip("Ferry", 500, 10, 5, 100));
	colony.push_back(ColonyShip("Liner", 1000, 20, 7, 250));
	colony.push_back(ColonyShip("Cloud", 2000, 30, 9, 750));
	solaris.push_back(SolarSailShip("Radiant", 50, 3, 5, 50));
	solaris.push_back(SolarSailShip("Ebulient", 250, 50, 5, 500));
	fighter.push_back(MilitaryEscortShip("Cruiser", 300, 2, 10, 0));
	fighter.push_back(MilitaryEscortShip("Frigate", 1000, 7, 20, 10));
	fighter.push_back(MilitaryEscortShip("Destroy", 2000, 19, 30, 25));
}

string getString(std::istream & is)
{
	std::string input;
	std::getline(is, input);
	return input;
}

//user interface for checking the fleet status
void checkStatus(Fleet* newFleet)
{
	cout << "______________________________" << endl << "\tFleet Status" << endl;
	cout << "______________________________" << endl;
	cout << "Fleet Name: " << newFleet->getCorporationName() << endl;
	cout << "Fleet's cost: " << newFleet->getCost() << endl;
	cout << "Total Money left: " << newFleet->getMoney() << endl;
	cout << "Total Fleet's weight: " << newFleet->getWeight() << endl;
	cout << "Total Fleet's energy consumption: " << newFleet->getEnergyConsumption() << endl;
	cout << "Total Fleet's energy Production: " << newFleet->EnergyProduction() << endl;
	cout << "Total Fleet's Colonist: " << newFleet->getColonistCount() << endl;
	cout << "Has Medic Ship: " << newFleet->hasMedic() << endl;
	cout << "Overall Ship:- " << endl;
	vector<Ship*> list = newFleet->shipList();
	for (int i = 0; i < list.size(); i++)
	{
		cout << i + 1 << ". " << list.at(i)->getTypeName() << endl;
	}
	cout << "______________________________\n\n";
}

//user interface for add ship
void addShip(Fleet* F)
{
	char option;
	int shipO;
	do
	{
		cout << "\nMenu -> Add Ship\n";
		cout << "~~~~~~~~~~~~~~~~~~\n";
		cout << "1. Colony Ship\n";
		cout << "2. Solar Sail Ship\n";
		cout << "3. Military Escort Ship\n";
		cout << "4. Medic Ship\n";
		cout << "5. Back\n:: ";
		cin >> option;

		switch (option)
		{
		case '1':
			int i;
			cout << "\nMenu -> Add Ship -> Colony Ship\n";
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			cout << "\tName\tCost\tColonist\tweight\tEnergyConsumption\n";

			for (i = 0; i < colony.size(); i++)
			{
				cout << i + 1 << ". ";
				colony.at(i).status();
				cout << endl;
			}
			cout << i + 1 << ". Back\nBuy:: ";
			cin >> shipO;
			shipO--;
			if (shipO < colony.size())
			{
				F->addShip(new ColonyShip(colony.at(shipO)));
			}

			break;

		case '2':
			cout << "\nMenu -> Add Ship\ -> Solar Sail Ship\n";
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			cout << "\tName\t\tCost\tEnergyGeneration\tweight\tEnergyConsumption\n";

			for (i = 0; i < solaris.size(); i++)
			{
				cout << i + 1 << ". ";
				solaris.at(i).status();
				cout << endl;
			}
			cout << i + 1 << ". Back\nBuy:: ";
			cin >> shipO;
			shipO--;
			if (shipO < solaris.size())
			{
				F->addShip(new SolarSailShip(solaris.at(shipO)));
			}
			break;

		case '3':
			cout << "\nMenu -> Add Ship -> Military Escort Ship\n";
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			cout << "\tName\tCost\tfighter\tweight\tEnergyConsumption\n";

			for (i = 0; i < fighter.size(); i++)
			{
				cout << i + 1 << ". ";
				fighter.at(i).status();
				cout << endl;
			}
			cout << i + 1 << ". Back\nBuy:: ";
			cin >> shipO;
			shipO--;
			if (shipO < fighter.size())
			{
				F->addShip(new MilitaryEscortShip(fighter.at(shipO)));
			}
			break;
		case '4':
			cout << "\nMenu -> Add Ship -> Medic Ship\n";
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			cout << "\tName\tCost\tweight\tEnergyConsumption\n";
			cout << "1. \tMedic\t1000\t1\t1\n";
			cout << "2. Back\nBuy:: ";
			cin >> shipO;
			if (shipO == 1)
			{
				if (F->hasMedic())
				{
					cout << "\nAlready bought Medic Ship\n";
				}
				else
				{
					if (F->getMoney() - 1000 >= 0)
					{
						F->setMedic();
						F->setMoney(F->getMoney() - 1000);
						cout << "\nMedic successfully purchased\n";
					}
					else
					{
						cout << "\nNot enough money to buy Medic Ship\n";
					}
				}
			}
			break;
		case '5':
			break;
		default:
			cout << "Character input does not recognize, please try again\n";
			break;
		}
	} while (option != '5');
}

//user interface for delete ship 
void DeleteShip(Fleet* F)
{
	vector<Ship*> list = F->shipList();
	int option;
	int i;
	cout << "\nMenu -> Add Ship\n" << "~~~~~~~~~~~~~~~~~~\n";
	cout << "Choose a ship to be deleted:- \n";
	for (i = 0; i < list.size(); i++)
	{
		cout << i + 1 << ". " << list.at(i)->getTypeName() << endl;
	}
	if (F->hasMedic())
	{
		cout << ++i << ". Medic\n";
	}
	cout << ++i << ". Back\n";
	cout << ":: ";

	cin >> option;
	option--;
	if (option < list.size())
	{
		F->deleteShip(list.at(option));
		cout << "\nDelete successful\n";
	}
	else if (option == list.size())
	{
		if (F->hasMedic())
		{
			F->setMedic();
			F->setMoney(F->getMoney() + 1000);
			cout << "\nDelete successful\n";
		}
	}
	

}

//user interface of fleet's menu creation
Fleet* userInterfaceCreateFleet()
{
	string name;
	cout << "Enter the Fleet's name: ";
	cin >> name;
	name += getString(cin);

	Fleet* newFleet = new Fleet(name);
	char exit = 'n';
	char option;
	checkStatus(newFleet);
	do
	{
		cout << "\nMenu\n~~~~\n";
		cout << "1. Check Fleet's Status\n";
		cout << "2. Add ship\n";
		cout << "3. Delete ship\n";
		cout << "4. Done\n:: ";
		cin >> option;

		switch (option)
		{
		case '1':
			checkStatus(newFleet);
			break;
		case '2':
			addShip(newFleet);
			break;
		case '3':
			DeleteShip(newFleet);
			break;
		case '4':
			cout << "Are you sure? (Y to exit, N to cancel) : ";
			cin >> exit;
			break;
		default:
			cout << "Character input not recognize, please try again\n";
			break;

		}

	} while (exit == 'n' || exit == 'N');

	return newFleet;
}


condition_variable cv;
mutex cv_m;

//start the simulation
void run(Fleet* fleet)
{
	long long unsigned int distance = (int)(3.121974372 * pow(10,17));//(0,17)
	long long unsigned int lightSpeed = 299792458;
	long long unsigned int fleetSpeed = ((lightSpeed * 100) / sqrt(fleet->getWeight()));
	long long unsigned int currentDistance = 0;
	
	std::unique_lock<std::mutex> lk(cv_m);
	cout << fleet->getCorporationName() << " prepare for launch\n";
	cv.wait(lk);
	cout << fleet->getCorporationName() << " launch!!\n";
	lk.unlock();
	//cv.notify_all();
	while (true)
	{
		currentDistance += fleetSpeed;
		fleet->setdistance(currentDistance);
		if (distance <= currentDistance)
		{
			break;
		}
		
	}
	cout << "Fleet " << fleet->getCorporationName() << " has reach Gaia!\n";
}

long long unsigned int second = 0;

//managing the time and sync the fleet movement
void timeManager()
{
	std::unique_lock<std::mutex> lk(cv_m);
	cout << "Counter ready" << endl;
	cv.wait(lk);
	cout << "Counter Start!" << endl;
	lk.unlock();
	while (true)
	{
		second++;
	}
}

//starting signal for fleets' launch
void signals()
{
	this_thread::sleep_for(chrono::seconds(2));
	cout << "launch in:\n";
	for (int i = 5; i > 0; i--)
	{
		cout << i << endl;
		this_thread::sleep_for(chrono::seconds(1));
	}
	std::lock_guard<std::mutex> lk(cv_m);
	cv.notify_all();
}

int main()
{
	vector<Fleet*> fleets;
	int fleetNumber;
	createShips();
	cout << "enter the number of fleet competing: ";
	cin >> fleetNumber;

	for (int i = 0; i < fleetNumber; i++)
	{
		fleets.push_back(userInterfaceCreateFleet());
	}
	vector<thread> threads;
	for (int i = 0; i < fleetNumber; i++)
	{
		threads.push_back(thread(run, fleets.at(i)));
	}
	threads.push_back(thread(timeManager));
	threads.push_back(thread(signals));
	
	int test;
	do
	{
		cin >> test;
		if (test == 1)
		{
			for (int i = 0; i < fleets.size(); i++)
			{
				cout << fleets.at(i)->getCorporationName() << ": \t" << fleets.at(i)->getdistance()<< endl;
			}
				cout << "Second: " << second << endl;
				cout << "Month: " << second / 2592000 << endl;
				cout << "Year: " << second / 31104000 << endl;
				cout << "decade: " << second / 311040000 << endl;
		}
		else
		{
			break;
		}
	} while (test == 1);

	for (int i = 0; i < threads.size(); i++)
	{
		threads.at(i).join();
	}
	
}