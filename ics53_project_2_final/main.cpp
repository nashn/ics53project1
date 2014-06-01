//============================================================================
// Name        : main.cpp
// Author      : She Nie (Nash)
//				 Qixiang Zhang (Kevin)
// 				 Yuhong Li (Stanley)
// Version     : Ver 1.51
// Copyright   : Team project for ICS53. All rights reserved
// Description : Second Project Lab - Multi-threading
//				 This project is relatively small, so we keep all the functions
//				 and variables in the same file in order to reduce compiling comlexity
//
// History     : 05/13/2014: Start working on project 2
// 				 05/13/2014: Finished code base, start hand-simulating
//				 05/14/2014: Logic bugs, hand-simulating again
//				 05/19/2014: Ver 1.1 finished, degbugging
//				 05/21/2014: Ver 1.2 finished, degbugging
//				 05/24/2014: Ver 1.23 finished, degbugging
//				 05/27/2014: Ver 1.3 finished, degbugging
//				 05/28/2014: Ver 1.32 finished, degbugging
//				 05/29/2014: Ver 1.4 finished, degbugging
//				 05/30/2014: Ver 1.41 finished, degbugging
//				 05/30/2014: Ver 1.5 finished, degbugging
// 				 05/31/2014: Ver 1.51, finished debugging, clean up and add comments
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <pthread.h>
#include <unistd.h>

using namespace std;

// create structure for the bridge, with a mutex and condition semaphore
struct Bridge {
	int cars;
	int direction;
	pthread_mutex_t condition_mutex;
	pthread_cond_t empty_cond;
};

// creat structure for car, to store related information
struct Car
{
	int id;
	int direc;
	int car_status;
};

// global variables and pointers
static Bridge* single_bridge;
static Car** car_queue;

int current_direc = 0;
int MAX_CAR = 0;


// function declarations
int simulation(char*& filename);

void* OneVehicle(void* c);

void ArriveBridge(int id, int direc);
void CrossBridge(int id, int direc);
void ExitBridge(int id, int direc);


// simple command line
int main(int argc, char* argv[])
{
	string line = "";

	if ( argc != 2 )
		cout << "Usage: ./run <filename>" << endl;
	else
	{
		simulation(argv[1]);
	}

	return 0;
}

// function for doing input parsing and thread creating
int simulation(char*& filename)
{
	ifstream f(filename);
	
	string line = "";

	// local variables
	int id = 0;
	int direc = 0;
	
	// check the input file
	if ( f.is_open() )
	{
		getline(f, line);
		istringstream s(line);
		s >> MAX_CAR;

		// thread array
		pthread_t thread_id[MAX_CAR];

		// store information about whether a car already pass;
		car_queue = new Car*[MAX_CAR];

		// initialize bridge structure
		single_bridge = new Bridge;
		single_bridge->cars = 0;
		single_bridge->direction = 0;
		single_bridge->condition_mutex = PTHREAD_MUTEX_INITIALIZER;
		single_bridge->empty_cond = PTHREAD_COND_INITIALIZER;

		for (int i = 0; i < MAX_CAR; i++)
		{
			getline (f,line);
			istringstream l(line);
			l >> id >> direc;

			// for debugging
			//cout << "good so far, id = "<< id << " direc = " << direc << endl;

			// initialize car structure array
			car_queue[i] = new Car;
			car_queue[i]->id = id;
			car_queue[i]->direc = direc;
			car_queue[i]->car_status = 0;
	
			//cout << "car[" << i << "] = { id = " << (*c[i]).id << ", direc = " << (*c[i]).direc << " } "<< endl;
			if ( pthread_create(&thread_id[i], NULL, OneVehicle, (void*) car_queue[i]) )
			{
				cout << "Thread creation failed @ id = " << id << endl;
				return (-1);
			}
		}

		// synchronize threads if its car representation is marked passed
		int i = 0;
		while ( i < MAX_CAR )
		{
			// join the thread if the car_status = 0
			if ( thread_id[i] && !car_queue[i]->car_status )
			{
				pthread_join(thread_id[i], NULL);
				i++;
			}
		}

		f.close();
	}
	else 
		cout << "Unable to open file" << endl;

	return (0);
}

void* OneVehicle(void* c)
{
	int id = ((Car*)c)->id;
	int direc = ((Car*)c)->direc;
	int car_status = ((Car*)c)->car_status;

	// for debugging
	//cout << "id = " << id << ", direc = " << direc << endl;
	ArriveBridge(id, direc);
	//cout << "  ArriveBridge done" << endl;
	CrossBridge(id, direc);
	//cout << "  CrossBridge done" << endl;
	ExitBridge(id, direc);
	//cout << "  ExitBridge done" << endl;

	// mark the car is passed
	car_status = 1;

	return 0;
}

void ArriveBridge(int id, int direc)
{
	pthread_mutex_lock( &single_bridge->condition_mutex );
	//cout << "            id = " << id << ", direc = " << direc << endl;

	while ( single_bridge->cars > 0 && (single_bridge->cars > 2 || single_bridge->direction != direc) )
	{
		//cout << "Car " << id << " waits to travel in direction " << direc << endl;
		pthread_cond_wait( &single_bridge->empty_cond, &single_bridge->condition_mutex );
		cout << "Car " << id << " waits to travel in direction " << direc << endl;
	}

	if ( single_bridge->cars == 0 )
	{
		single_bridge->direction = direc;
		cout << "Traffic Direction is being changed to " << direc << endl;
	}
	single_bridge->cars++;
	cout << "Car " << id << " arrives traveling direction " << direc << endl;

	pthread_mutex_unlock( &single_bridge->condition_mutex );	
}

// since the CPU is too fast,
// using system call sleep() to "slow" down the whole process
void CrossBridge(int id, int direc)
{
	cout << "Car " << id << " crossing bridge in direction " << direc << endl;
	sleep(1);
}

void ExitBridge(int id, int direc)
{
	pthread_mutex_lock( &single_bridge->condition_mutex );
	single_bridge->cars--;
	cout << "Car " << id << " exits in direction " << direc << endl;
	pthread_cond_signal( &single_bridge->empty_cond );
	pthread_mutex_unlock( &single_bridge->condition_mutex );	
}

