//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : First Project Lab
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <pthread.h>

using namespace std;

int empty_count = 3;
int fill_count = 0;
int MAX_CAR = 0;

int current_direc = 0;

struct Car
{
	int id;
	int direc;
	pthread_mutex_t condition_mutex;
	pthread_cond_t done_cond;
};

Car** c;

int simulation(string& filename);

void* OneVehicle(void* c);

void ArriveBridge(int id, int direc);
void CrossBridge(int id, int direc);
void ExitBridge(int id, int direc);

int main(int argc, char* argv[])
{
	string line = "";

	cout << "Enter input filename > ";
	getline(cin, line);

	while( 1 )
	{
		if ( line == "quit" )
			break;
		else
		{
			simulation(line);
			break;
		}
	}


	return 0;
}

int simulation(string& filename)
{
	ifstream f(filename.c_str());
	string line = "";
	//int car_number = 0;
	int id = 0;
	int direc = 0;
	int count = 0;


	if ( f.is_open() )
	{
		getline(f, line);
		istringstream s(line);
		s >> MAX_CAR;

		pthread_t thread_id[MAX_CAR];
		c = new Car*[MAX_CAR];

		for (int i =0; i < MAX_CAR; i++)
		{
			getline (f,line);
			istringstream l(line);
			l >> id >> direc;

			// for debugging
			//cout << "good so far, id = "<< id << " direc = " << direc << endl;

			c[i] = new Car;
			(*c[i]).id = id;
			(*c[i]).direc = direc;
			(*c[i]).condition_mutex = PTHREAD_MUTEX_INITIALIZER;
			(*c[i]).done_cond = PTHREAD_COND_INITIALIZER;

			if ( pthread_create(&thread_id[i], NULL, OneVehicle, (void*) c[i]) )
			{
				cout << "Fail to create thread" << endl;
				return (-1);
			}
		}

		pthread_create(&thread_id[fill_count], NULL, OneVehicle, (void*) c[fill_count]);

		for (int i = 0; i < MAX_CAR; i++)
		{
			pthread_join(thread_id[i], NULL);
		}

		f.close();
	}
	else
	{
		cout << "Unable to open file" << endl;
		return (0);
	}
}


void* OneVehicle(void* c)
{

	int id = ((Car*)c)->id;
	int direc = ((Car*)c)->direc;

	//cout << "id = " << id << ", direc = " << direc << endl;
	ArriveBridge(id, direc);
	//cout << "  ArriveBridge done" << endl;
	CrossBridge(id, direc);
	//cout << "  CrossBridge done" << endl;
	ExitBridge(id, direc);
	//cout << "  ExitBridge done" << endl;

	return 0;
}

void ArriveBridge(int id, int direc)
{
	pthread_mutex_lock( &c[id-1]->condition_mutex );
	//while ( empty_count < 3 && (empty_count > 0 || current_direc != direc) )
	while ( fill_count > 0 && (fill_count > 2 || current_direc != direc) )
	{
		cout << "Car " << id << " waits to travel in direction " << direc << endl;
		pthread_cond_wait( &c[id-1]->done_cond, &c[id-1]->condition_mutex );
	}

	cout << "  empty_count = " << empty_count << endl;
	cout << "  fill_count = " << fill_count << endl;

	if ( empty_count > 0 && fill_count <= 3)
	{
		empty_count--;
		fill_count++;
		cout << "Car " << id << " arrives traveling direction " << direc << endl;
	}

	if ( empty_count == 0 )
	{
		current_direc = direc;
		cout << "Traffic Direction is being changed to " << direc << endl; 
	}
	pthread_mutex_unlock( &c[id-1]->condition_mutex );
}


void CrossBridge(int id, int direc)
{
	cout << "Car " << id << " crossing bridge in direction " << direc << endl;
	sleep(1);
}


void ExitBridge(int id, int direc)
{
	pthread_mutex_lock( &c[id-1]->condition_mutex );

	cout << "  empty_count = " << empty_count << endl;

	if ( empty_count <= 3)
	{
		empty_count++;
		pthread_cond_signal( &c[id-1]->done_cond );
		cout << "Car " << id << " exits the bridge in direction " << direc << endl;
	}
	pthread_mutex_unlock( &c[id-1]->condition_mutex );
}
