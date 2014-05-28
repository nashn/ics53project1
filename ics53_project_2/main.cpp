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

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;

std::mutex my_mute;

struct Car
{
	int id;
	int direc;
};

void simulation(string& filename);

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

void simulation(string& filename)
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
		Car** c = new Car*[MAX_CAR];

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

			pthread_create(&thread_id[i], NULL, OneVehicle, (void*) c[i]);
		}

		for (int i = 0; i < MAX_CAR; i++)
		{
			pthread_join(thread_id[i], NULL);
		}

		f.close();
	}
	else cout << "Unable to open file" << endl;

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
	while ( 1 )
	{
		pthread_mutex_lock( &condition_mutex );
		//cout << "good so far, id = " << id << ", direc = " << direc << endl;

		while (  empty_count < 0 || empty_count > 3) {
			//cout << "  get into loop " << endl;
			pthread_cond_wait( &condition_cond, &condition_mutex );
			my_mute.lock();
			cout << "Car " << id << " waits to travel in direction " << direc << endl;
			my_mute.unlock();
		}

		pthread_mutex_unlock( &condition_mutex );
		
		pthread_mutex_lock( &count_mutex );
		empty_count--;
		my_mute.lock();
		cout << "Car " << id << " arrives at traveling direction " << direc << endl;
		my_mute.unlock();
		//cout << "   empty_count = " << empty_count << endl;
		//cout << "   fill_count = " << fill_count << endl;

		pthread_mutex_unlock( &count_mutex );

		if ( empty_count > 3 || empty_count < 0)
			break;

		//if ( fill_count >= MAX_CAR )
		//	break;
	}
}


void CrossBridge(int id, int direc)
{
	my_mute.lock();
	cout << "Car " << id << " crossing bridge in direction " << direc << endl;
	my_mute.unlock();
}


void ExitBridge(int id, int direc)
{
	while ( 1 )
	{
		pthread_mutex_lock( &condition_mutex );
		if ( empty_count >= 0 && empty_count <= 3 )
		{
			pthread_cond_signal( &condition_cond );
		}
		pthread_mutex_unlock( &condition_mutex );

		
		pthread_mutex_lock( &count_mutex );
		empty_count++;
		fill_count++;
		my_mute.lock();
		cout << "Car " << id << " exits at traveling direction " << direc << endl;
		my_mute.unlock();
		//cout << "   empty_count = " << empty_count << endl;
		//cout << "   fill_count = " << fill_count << endl;

		pthread_mutex_unlock( &count_mutex );


		if ( empty_count > 3 || empty_count < 0)
			break;
/*
		if ( fill_count >= MAX_CAR )
			break;
*/
	}
}

/*
void ArriveBridge(int id, int direc)
{
	while ( 1 )
	{
		empty_count--;
		pthread_mutex_lock(&condition_mutex);
		
		// for debugging
		cout << "empty_count = " << empty_count << endl;
		cout << "fill_count = " << fill_count << endl;

		if ( empty_count <= 0 )
		{
			empty_count = 0;
			cout << "Car " << id << " waits to travel in direction " << direc << endl;
			break;
		}
		else
		{
			cout << "Car " << id << " arrives at traveling direction " << direc << endl;
		}
		pthread_mutex_unlock(&condition_mutex);
		fill_count++;
	}

	if ( empty_count || exit_flag )
		arrive_flag = false;
	fill_count += 1;
	arrive_flag = true;

	if ( direc < 1 )
		cout << "Car " << id << " arrives at traveling direction " << direc << endl;
	else
		cout << "Car " << id << " waits to travel in direction " << direc << endl;

}

void CrossBridge(int id, int direc)
{

	fill_count -= 1;
	if ( fill_count == 0 )
		exit_flag = true;

	cout << "Car " << id << " crossing bridge in direction " << direc << endl;
	//cout << "Traffic Direction is being changed to 1" << endl;
}

void ExitBridge(int id, int direc)
{
	while ( 1 )
	{
		fill_count--;
		pthread_mutex_lock(&condition_mutex);

		// for debugging
		cout << "empty_count = " << empty_count << endl;
		cout << "fill_count = " << fill_count << endl;

		if ( fill_count <= MAX_CAR )
		{
			cout << "Car " << id << " exits bridge in direction " << direc << endl;
		}
		else
		{
			fill_count = MAX_CAR;
			break;
			cout << "good so far" << endl;
		}
		pthread_mutex_unlock(&condition_mutex);
		empty_count++;
	}
}
*/
