#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <pthread.h>

using namespace std;

void simulation(string& filename);

void OneVehicle(int id, int direc);

void ArriveBridge(int id, int direc);
void CrossBridge(int id, int direc);
void ExitBridge(int id, int direc);

int main(int argc, char* argv[])
{
	// some variable
	string line = "";
	


	while( 1 )
	{
		cout << "Enter input file name(quit to exit): ";
		getline(cin, line);

		if ( line == "quit" )
			break;

		else
		{	
			simulation(line);
		}
	}


	return 0;
}

void simulation(string& filename)
{
	ifstream f(filename.c_str());
	string line = "";
	int car_number = 0;
	int id = 0;
	int direc = 0;
	int count = 0;

	
	if ( f.is_open() )
  	{
		getline(f, line);
		istringstream s(line);
		s >> car_number;

		while ( getline (f,line) && count <= car_number) 
		{
			istringstream l(line);
			l >> id >> direc;
			OneVehicle(id, direc);
			count++;
		}
		f.close();
	}
	else cout << "Unable to open file" << endl;


}

void OneVehicle(int id, int direc)
{
	ArriveBridge(id, direc);
	CrossBridge(id, direc);
	ExitBridge(id, direc);
}

void ArriveBridge(int id, int direc)
{
	if ( direc < 1 )
		cout << "Car " << id << " arrives at traveling direction " << direc << endl;
	else
		cout << "Car " << id << " waits to travel in direction " << direc << endl;
}

void CrossBridge(int id, int direc)
{
	cout << "Car " << id << " crossing bridge in direction " << direc << endl;
//	cout << "Traffic Direction is being changed to 1" << endl;
}

void ExitBridge(int id, int direc)
{
	cout << "Car " << id << " exits bridge in direction " << direc << endl;
}
