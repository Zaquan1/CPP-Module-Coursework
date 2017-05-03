#include "Fleet.h"

///////////////////////////////// Fleet Implementation //////////////////////////////////////////////
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
		if (!list.at(i)->isDestroyed())
		{
			weight += list.at(i)->getWeight();
		}
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

//add buyed ship to the fleet
void Fleet::addShip(Ship& nShip)
{
	int shipType = nShip.getShipType();
	Ship* tempship = &nShip;
	if (money - nShip.getCost() >= 0)
	{
		if (shipType == COLONY)
		{
			ColonyShip* temp = new ColonyShip(*dynamic_cast<ColonyShip*>(tempship));
			_colonyShips.push_back(temp);
		}
		else if (shipType == SOLAR)
		{
			SolarSailShip* temp = new SolarSailShip(*dynamic_cast<SolarSailShip*>(tempship));
			_solarSailShips.push_back(temp);
		}
		else if (shipType == MILITARY)
		{
			MilitaryEscortShip* temp = new MilitaryEscortShip(*dynamic_cast<MilitaryEscortShip*>(tempship));
			_militaryEscortShips.push_back(temp);
		}
		//minus the price
		money = money - nShip.getCost();
		cout << endl << nShip.getTypeName() << " successfully purchased\n";
	}
	//if amount of money not enough
	else
	{
		cout << endl << "Current Money: " << getMoney() << ", not enough to buy " << nShip.getTypeName() << endl;
	}
}


int Fleet::getMoney() const { return money; }

void Fleet::setMoney(int n) { money = n; }

void Fleet::setMedic() { medic = !medic; }

void Fleet::destroyShip(Ship* i) { i->destroyShip(); }

//delete buyed ship(cancel buying)
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

//sort ships in descending order based on their weight
void Fleet::sortFleet()
{
	auto compare = [](const Ship* a, const Ship* b) -> bool {return a->getWeight() > b->getWeight(); };
	sort(_colonyShips.begin(), _colonyShips.end(), compare);
	sort(_militaryEscortShips.begin(), _militaryEscortShips.end(), compare);
	sort(_solarSailShips.begin(), _solarSailShips.end(), compare);
}

//sorting ship and set which ship to be protected
void Fleet::organizedFleet()
{
	sortFleet();

	MilitaryEscortShip* mShip;
	ColonyShip* cShip;
	int protector = 0;
	protector = _militaryEscortShips.size();
	for (int i = 0; i < _militaryEscortShips.size(); i++)
	{
		mShip = dynamic_cast<MilitaryEscortShip*>(_militaryEscortShips.at(i));
		protector += mShip->getNrProtected();
	}
	
	for (int i = 0; i < _colonyShips.size(); i++)
	{
		cShip = dynamic_cast<ColonyShip*> (_colonyShips.at(i));
		if (protector > 0)
		{
			cShip->underProtected();
			protector--;
		}
		else
		{
			break;
		}
	}

}
////////////////////////// End Fleet Implementation ///////////////////////////////////////////////

////////////////////////// Ship implementation //////////////////////////////////////////////////////
Ship::Ship(int t, string n, int c, int w, int e) 
	:shipType(t), name(n), cost(c), weight(w), energyConsumption(e), destroyed(false)
{}
Ship::Ship() : name("") {}

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
////////////////////// End Ship Implementation ///////////////////////////////////////////////

///////////////////// ColonyShip Implementation //////////////////////////////////////////////
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
//////////////////////////// End ColonyShip Implementation ///////////////////////////////////////////////

//////////////////////////// SolarSailShip Implementation ///////////////////////////////////////////////
SolarSailShip::SolarSailShip(string n, int c, int w, int e, int eg)
	: Ship(2, n, c, w, e), energyGeneration(eg)
{}
SolarSailShip::SolarSailShip(const SolarSailShip& sss)
	: SolarSailShip(sss.name, sss.cost, sss.weight, sss.energyConsumption, sss.energyGeneration)
{}

int SolarSailShip::getEnergyProduction() const { return energyGeneration; }
//////////////////////////// End SolarSailShip Implementation ///////////////////////////////////////

//////////////////////////// MilitaryEscortShip Implementation ///////////////////////////////////////
MilitaryEscortShip::MilitaryEscortShip(string n, int c, int w, int e, int nn)
	: Ship(3, n, c, w, e), nr(nn)
{}
MilitaryEscortShip::MilitaryEscortShip(const MilitaryEscortShip& mes)
	: MilitaryEscortShip(mes.name, mes.cost, mes.weight, mes.energyConsumption, mes.nr)
{}
int MilitaryEscortShip::getNrProtected() const { return nr; }
//////////////////////////// End MilitaryEscortShip Implementation ///////////////////////////////////////

//////////////////////////// User Interface Creation /////////////////////////////////////////////////////
vector<ColonyShip> colony;
vector<SolarSailShip> solaris;
vector<MilitaryEscortShip> fighter;

//default ship option(template)
void createShips()
{
	colony.push_back(ColonyShip("Ferry", 500, 10, 5, 100));
	colony.push_back(ColonyShip("Liner", 1000, 20, 7, 250));
	colony.push_back(ColonyShip("Cloud", 2000, 30, 9, 750));
	colony.push_back(ColonyShip("CatsOfCat", 10000, 20, 9, 10000));
	solaris.push_back(SolarSailShip("Radiant", 50, 3, 5, 50));
	solaris.push_back(SolarSailShip("Ebulient", 250, 50, 5, 500));
	solaris.push_back(SolarSailShip("CatSun", 1000, 1000, 1, 1000000));
	fighter.push_back(MilitaryEscortShip("Cruiser", 300, 2, 10, 0));
	fighter.push_back(MilitaryEscortShip("Frigate", 1000, 7, 20, 10));
	fighter.push_back(MilitaryEscortShip("Destroyer", 2000, 19, 30, 25));
	fighter.push_back(MilitaryEscortShip("CatDestroy", 10000, 200, 10, 1000));
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
	newFleet->sortFleet();
	vector<Ship*> list = newFleet->shipList();
	for (int i = 0; i < list.size(); i++)
	{
		cout << i + 1 << ". " << list.at(i)->getTypeName() << endl;
	}
	cout << "______________________________\n\n";
}

//calculation for buying medic ship
void buyMedic(Fleet* F)
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
				F->addShip(colony.at(shipO));
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
				F->addShip(solaris.at(shipO));
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
				F->addShip(fighter.at(shipO));
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
				buyMedic(F);
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

//calculation for reading file
void readDatFile(Fleet* F)
{
	ifstream inputFile;
	inputFile.open("015387-Fleet.dat");
	if (inputFile.fail())
	{
		cout << "Error opening the file!\n";
		return;
	}
	string line;
	vector<string> ships;
	vector<int> shipsAmount;

	//get the name and amount of ships stated in the file
	while (getline(inputFile, line))
	{
		istringstream ss(line);

		string name;
		int amount;

		ss >> name >> amount;
		ships.push_back(name);
		shipsAmount.push_back(amount);
	}
	
	inputFile.close();
	std::vector<std::reference_wrapper<Ship>> allList;
	allList.insert(allList.end(), colony.begin(), colony.end());
	allList.insert(allList.end(), fighter.begin(), fighter.end());
	allList.insert(allList.end(), solaris.begin(), solaris.end());
	
	//add the ship into the fleet
	for (int i = 0; i < ships.size(); i++)
	{
		if (ships.at(i) == "Medic")
		{
			buyMedic(F);
			continue;
		}
		for (int j = 0; j < allList.size(); j++)
		{
			if (ships.at(i) == allList.at(j).get().getTypeName())
			{
				for (int k = 0; k < shipsAmount.at(i); k++)
				{
					F->addShip(allList.at(j).get());
				}
				break;
			}
			if (j == allList.size() - 1)
			{
				cout << endl << ships.at(i) << " is not in the list!\n";
			}
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
		cout << "4. Use .dat file\n";
		cout << "5. Done\n:: ";
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
			readDatFile(newFleet);
			break;
		case '5':
			cout << "Are you sure? (Y to exit, N to cancel) : ";
			cin >> exit;
			break;
		default:
			cout << "Character input not recognize, please try again\n";
			break;

		}

	} while (exit == 'n' || exit == 'N');

	newFleet->organizedFleet();
	return newFleet;
}
//////////////////////////////// End User Interface Creation ///////////////////////////////////////////

/////////////////////////////// Threading Implementation //////////////////////////////////////////////
condition_variable cv;
mutex cv_m;
int year = 0;
int waitingFleet = 0;
int completeFlight = 0;
bool completeYear = false;
Fleet* winner;

//calculation for alien attack
void alienAttack(Fleet* fleet)
{
	vector<Ship*> protectedShip = fleet->protectedShips();
	vector<Ship*> unprotectedShip = fleet->unprotectedShips();
	ColonyShip* cShip;
	//check how much ship is unprotected for the calculation to destroy
	if (unprotectedShip.size() > 0)
	{
		int probability = round(0.25 * unprotectedShip.size());
		for (int i = 0; i < probability; i++)
		{
			int random = rand() % unprotectedShip.size();
			cShip = dynamic_cast<ColonyShip*> (unprotectedShip.at(random));
			cShip->destroyShip();
			unprotectedShip.erase(unprotectedShip.begin() + random);
		}
	}
	//calculation for infection if medic is not present
	if (!fleet->hasMedic())
	{
		vector<Ship*> list;
		list.insert(list.end(), protectedShip.begin(), protectedShip.end());
		list.insert(list.end(), unprotectedShip.begin(), unprotectedShip.end());
		if (list.size() > 0)
		{
			int random = rand() % list.size();
			cShip = dynamic_cast<ColonyShip*> (list.at(random));
			cShip->infect();
		}
	}
}

//start the simulation
void run(Fleet* fleet)
{
	int distance = 33;	//lightyear
	double fleetSpeed = ((1 * 10) / sqrt(fleet->getWeight()));	//speed in year/lightyear
	double currentDistance = 0;
	bool meetAlien = false;
	
	std::unique_lock<std::mutex> lk(cv_m);
	cout << fleet->getCorporationName() << " prepare for launch\n";
	//wait for signal to launch at the same time
	cv.wait(lk);
	cout << fleet->getCorporationName() << " launch!!\n";
	lk.unlock();
	//disqualified for fleet with no power
	if (fleet->getEnergyConsumption() > fleet->EnergyProduction())
	{
		cout << "Fleet " << fleet->getCorporationName() << " consume too much energy, fleet " << fleet->getCorporationName() << " disqualified from competing\n";
		completeFlight++;
		return;
	}
	while (true)
	{
		//distance calculation
		currentDistance += fleetSpeed;
		fleet->setdistance(currentDistance);
		//alien attack
		if (!meetAlien && (currentDistance > (distance / 2)))
		{
			lk.lock();
			cout << "Fleet " << fleet->getCorporationName() << " is under attack by space pirate!\n";
			lk.unlock();
			alienAttack(fleet);
			fleetSpeed = ((1 * 10) / sqrt(fleet->getWeight()));
			meetAlien = true;
		}

		//end if reach gaia
		if (distance <= currentDistance)
		{
			break;
		}
		//sleep to sync with other thread
		this_thread::sleep_for(chrono::seconds(1));
		lk.lock();
		waitingFleet++;
		completeYear = false;
		//notify timeManager thread if all ship has completed
		cv.notify_all();
		//wait for other fleet to complete their calculation
		cv.wait(lk, []() {return completeYear; });
		lk.unlock();
	}
	
	lk.lock();
	cout << "Fleet " << fleet->getCorporationName() << " has reach Gaia!\n";
	winner = fleet;
	this_thread::sleep_for(chrono::microseconds(50));
	completeFlight++;
	//notify gaia thread to start calculation
	cv.notify_all();
	lk.unlock();
}

//managing the time and sync the fleet movement
void timeManager(int f)
{
	std::unique_lock<std::mutex> lk(cv_m);
	cout << "Counter ready" << endl;
	//wait to sync with other thread
	cv.wait(lk);
	cout << "Counter Start!" << endl;
	lk.unlock();
	while (true)
	{
		lk.lock();
		//wait for all the fleet to complete
		cv.wait(lk, [&]() {return waitingFleet == (f - completeFlight); });
		completeYear = true;
		waitingFleet = 0;
		//wake all fleet thread
		cv.notify_all();
		cout << "Year: " << year << endl;
		lk.unlock();
		//increase a year
		year++;
		//break if all fleet has reach gaia
		if (completeFlight == f)
		{
			break;
		}
	}
}

//organize the population once fleet reach gaia
void planetGaia(int f)
{
	int gaiaPopulation = 0;
	std::unique_lock<std::mutex> lk(cv_m);
	cout << "Planet Gaia created" << endl;
	//sleep until at least 1 fleet reach gaia
	cv.wait(lk, []() {return completeFlight > 0; });
	Fleet* currentWinner = winner;
	gaiaPopulation = currentWinner->getColonistCount();
	int previousYear = year;
	lk.unlock();
	do
	{
		lk.lock();
		cout << "-------------" << endl;
		cout << "Gaia Status" << endl;
		cout << "-------------" << endl;
		cout << "Current inhabitant: " << currentWinner->getCorporationName() << endl << "Current population: " << gaiaPopulation << endl;
		cout << "-------------------------------------------" << endl;
		lk.unlock();
		while (previousYear == year && (completeFlight != f))
		{
			if (winner->getColonistCount() > gaiaPopulation)
			{
				currentWinner = winner;
				gaiaPopulation = winner->getColonistCount();
			}
		}
		gaiaPopulation += (gaiaPopulation * 0.05);
		previousYear = year;

	} while (completeFlight != f);
	winner = currentWinner;
	lk.lock();
	cout << "Fleet " << winner->getCorporationName() << " win the game with total population of " << gaiaPopulation << endl;
	lk.unlock();

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
/////////////////////////////////// End Threading Implementation ////////////////////////////////////////////////////////

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
	threads.push_back(thread(timeManager, fleets.size()));
	threads.push_back(thread(planetGaia, fleets.size()));
	threads.push_back(thread(signals));
	
	int test = 1;
		this_thread::sleep_for(chrono::seconds(8));
	do
	{
		this_thread::sleep_for(chrono::seconds(1));
		std::unique_lock<std::mutex> lk(cv_m);
		for (int i = 0; i < fleets.size(); i++)
		{
			cout << fleets.at(i)->getCorporationName() << ": \t" << fleets.at(i)->getdistance()<< endl;
		}
		lk.unlock();
	} while (completeFlight != fleets.size());

	for (int i = 0; i < threads.size(); i++)
	{
		if (threads.at(i).joinable())
		{
			threads.at(i).join();
			cout << "Thread " << i << " joint success\n";
		}
		else
		{
			cout << "Thread " << i << " already detatch\n";
		}
	}
	cout << "Game Over!" << endl;
	
}