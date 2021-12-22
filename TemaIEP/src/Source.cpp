#include <iostream>
#include <memory>

class Teams {


public:
	int data = 0;


	Teams(){}
	Teams(const Teams& rhs) = delete;
private:
	Teams& operator = (const Teams& rs);


};

class Leader {

private:
	int number;
	bool isLocked = false;

public:
	Leader() :number(0) {}

	int getNumber()
	{
		return number;
	}

	void setNumber(int num)
	{
		number = num;
	}

	void setisLocked(bool val)
	{
		this->isLocked = val;
	}

	void askLeader()
	{
		if (isLocked)
		{
			std::cout << "Lider blocat" << std::endl;
		}
		else
		{
			std::cout << "Lider nu e blocat" << std::endl;
		}
	}

};

void lock(Leader& l)
{
	l.setisLocked(true);
	std::cout << "Lider blocat"<<std::endl;
}

void unlock(Leader& l)
{
	l.setisLocked(false);
	std::cout << "Lider deblocat"<<std::endl;
}


class LockLeader {
	
private:
	Leader& lockPtr;
public:
	LockLeader(Leader& ptr) :
		lockPtr(ptr) {
		lock(lockPtr);
	}
	~LockLeader()
	{
		unlock(lockPtr);
	}
};


class Player {

private:
	int age;
	float points;
	std::string name;
	std::string team;
	Leader *leader;

public:

	Player(int age, float points, std::string name, std::string team, Leader* leader) // constructor
		:age(age),
		points(points),
		name(name),
		team(team),
		leader(leader)
	{
		std::cout << "constructor called\n";
	}

	Player():age(0),points(0),name("Unknown"),team("NoTeam"),leader(new Leader){}


	Player(const Player& rhs) //copy constructor
	{
		age = rhs.age;
		points = rhs.points;
		name = rhs.name;
		team = rhs.team;

		//Don't forget new added parts when copying

		leader = rhs.leader;

		//
		std::cout << "copy constructor called\n";
	}
	~Player() //destructor
	{

	}

	Player& operator = (const Player& rhs) //copy assignment operator
	{
		std::cout << "copy assignment operator called\n";

		//if (this == &rhs) return *this; // Varianta copy assignment-safe, DAR nu este exception-safe, in caz de delete la pointer.


		//exception-safe and self assignment-safe

		Leader* original = leader;
		leader = new Leader(*rhs.leader);
		delete original;

		//

		this->age = rhs.age;
		this->points = rhs.points;
		this->name = rhs.name;  // Oops, am uitat de fieldul name.
		this->team = rhs.team;

		return *this;
	}

	std::string getName()
	{
		return this->name;
	}

	std::string getTeam()
	{
		return this->team;
	}

	int getAge()
	{
		return this->age;
	}
	float getPoints()
	{
		return this->points;
	}

	void rename(std::string newName)
	{
		name = newName;
	}


};


void print_info(Player &player)
{
	std::cout << player.getName() << ", aged "<<player.getAge()<<" plays for team " << player.getTeam() <<" and scored "<<player.getPoints()<<" points"<< std::endl;
}

int main()
{
	Player player1(20, 45.6, "John", "Blue",new Leader); // constructor example

	print_info(player1);

	Player player2 = player1; // copy-constructor example

	print_info(player2);

	player2.rename("Mike");

	print_info(player2);

	Player player3;

	print_info(player3);

	player3 = player2; // copy assignment operator

	print_info(player3);

	Teams team1;

	//Teams team2 = team1; // example of inaccessible function (deleted copy constructor) 
	Teams team3;

	//team3 = team1; // example of inaccessible function (copy assignment operator declared private)

	std::unique_ptr<Leader> point1 ( new Leader);

	point1->setNumber(22);
	std::cout << point1->getNumber()<<std::endl;

//	std::unique_ptr<Leader> point2(point1); Nu functioneaza copierea 
	std::unique_ptr<Leader> point2;
	point2 = move(point1); // transfer de ownership, nu copiere !!

	std::cout << point2->getNumber() << std::endl;
	//std::cout << point1->getNumber() << std::endl; // segmentation fault, point1 este null

	std::shared_ptr<Leader> shrpoint1(new Leader);
	std::shared_ptr<Leader> shrpoint2(shrpoint1);
	std::shared_ptr<Leader> shrpoint3(shrpoint1);
	std::shared_ptr<Leader> shrpoint4(shrpoint1);

	std::cout << "Numar de copii " << shrpoint1.use_count()<<std::endl;

	std::shared_ptr<Leader> new_shrpoint;
	new_shrpoint = move(shrpoint1);

	std::cout << "Numar de copii pointer vechi " << shrpoint1.use_count() << std::endl;
	std::cout << "Numar de copii pointer nou " << new_shrpoint.use_count() << std::endl;

	std::cout << "\n\nLocking test:\n";

	Leader lead;
	lead.setNumber(123);
	lead.askLeader();

	LockLeader* lockedLead = new LockLeader(lead);
	lead.askLeader();
	delete lockedLead;
	lead.askLeader();


	
	return 0;
}