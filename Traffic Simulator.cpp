
#include<iostream>
#include<iomanip> //for setw() manipulator
#include<exception>
#include <unistd.h>
#include<sstream>
#include<string>
#include<time.h>
#ifdef _WIN32
#define clear_screen "clrscr"
#else
#define clear_screen "clear"
#endif
#define SIZE 5
#define TOP 0
#define LEFT 1
#define BOTTOM 2
#define RIGHT 3
#define MAX_CARS 200
int current_signal;
int SPEED = 1; //1=slowest 
using namespace std;

string colors[] = { "\033[33m", "\033[34m", "\033[35m", "\033[36m", "\033[37m", "\033[38m" };

//===================================================
class QueueEmpty : public exception
{
public:
	virtual const char* what() const throw()
	{
		return "Queue is Empty";
	}
};
//=================================================
class QueueFull : public exception
{
public:
	virtual const char* what() const throw()
	{
		return "Queue is Full";
	}
};
//=================================================

template <typename T>
class Queue
{
private:
	T* array;
	int N;	//Array Size (Capacity)
	int r;	//index where a new element will be added
	int f; // first element in queue
	int n;// number of items in queue
public:
	Queue(int N);
	~Queue();
	void enqueue(T);
	T dequeue();
	bool isEmpty();
	bool isFull();
	friend void display();
};
//===========================================
class Car
{
private:
	time_t arrival_time;
	string color;
public:
	Car()
	{
		this->arrival_time = time(NULL);
		this->color = colors[random() % 6];
	}
	time_t get_arrival_time()
	{
		return arrival_time;
	}
	string getColor()
	{
		return color;
	}
};
//===========================================
void display();
//===========================================
Queue<Car> left_queue(SIZE);
Queue<Car> right_queue(SIZE);
Queue<Car> top_queue(SIZE);
Queue<Car> bottom_queue(SIZE);
//==========================================
int main()
{

	srand(time(NULL));
	int cars = 0;
	time_t signal_timer = time(NULL);
	int waiting_time = 0;
	current_signal = rand() % 4;
	// Creating an array for holding the queues
	Queue<Car>* queue[] = { &top_queue, &left_queue, &bottom_queue, &right_queue };
	// Loop runs until the number of cars that has passed the junction is equal to Max cars for the simulation
	while (cars < MAX_CARS)
	{

		//complete the main function
		// condition for when the green light or signal is on top queue
		if (current_signal == 0) {
			if (!top_queue.isEmpty()) {
				// Removing a car from the queue and returning it into a "dummy" car
				Car C = top_queue.dequeue();
				// Getting the arrival time of the car to be dequeued and Calculation the waiting time
				waiting_time += time(NULL) - C.get_arrival_time();
				// Increasing number of cars that has passed a junction
				++cars;
			}
		}
		// condition for when the green light or signal is on left queue
		else if (current_signal == 1) {
			if (!left_queue.isEmpty()) {
				// Removing a car from the queue and returning it into a "dummy" car
				Car C = left_queue.dequeue();
				// Getting the arrival time of the car to be dequeued and Calculation the waiting time
				waiting_time += time(NULL) - C.get_arrival_time();
				// Increasing number of cars that has passed a junction
				++cars;
			}
		}
		// condition for when the green light or signal is on bottom queue
		else if (current_signal == 2) {
			if (!bottom_queue.isEmpty()) {
				// Removing a car from the queue and returning it into a "dummy" car
				Car C = bottom_queue.dequeue();
				// Getting the arrival time of the car to be dequeued and Calculation the waiting time
				waiting_time += time(NULL) - C.get_arrival_time();
				// Increasing number of cars that has passed a junction
				++cars;
			}
		}
		// condition for when the green light or signal is on right queue
		else {
			if (!right_queue.isEmpty()) {
				// Removing a car from the queue and returning it into a "dummy" car
				Car C = right_queue.dequeue();
				// Getting the arrival time of the car to be dequeued and Calculation the waiting time
				waiting_time += time(NULL) - C.get_arrival_time();
				// Increasing number of cars that has passed a junction
				++cars;
			}
		}
		// Moving green light to the next queue
		current_signal = (current_signal + 1) % 4;
		int x;
		// creating a temporary car 
		Car car;
		// A loop for adding a car randomly to a queue that is not full
		while (true) {
			x = rand() % 4;
			if (!queue[x]->isFull()) {
				// Adding a car to the queue
				queue[x]->enqueue(car);
				break;
			}
		}


		//.......................
		system(clear_screen);
		display();
		cout << "Total cars passed through the junction:" << cars << endl;
		cout << "Avg. waiting time for a car: " << setprecision(3) << (float(waiting_time) / cars) * SPEED << " seconds" << endl;

		usleep(200000 / SPEED);
	}
	return EXIT_SUCCESS;
}
//===================================================
void display()
{
	string spaces;
	for (int j = 0; j <= SIZE; j++)
		spaces += "     ";
	cout << spaces << "╔════╗" << endl;
	for (int i = SIZE - 1; i >= 0; i--)
	{
		cout << spaces << "║";
		if (i < top_queue.r)
			cout << top_queue.array[i].getColor() << setw(4) << " ▒▒ " << "\033[0m";
		//cout<<setw(4)<<" ▒▒ ";
		else
			cout << "    ";
		cout << "║" << endl;
		cout << spaces << "╠════╣" << endl;;
	}

	cout << spaces << "║";
	if (current_signal == TOP)
		cout << "\033[1;32m" << " ■■ " << "\033[0m";
	else
		cout << "\033[1;31m" << " ■■ " << "\033[0m";
	cout << "║" << endl;

	cout << "╔════";
	for (int i = 0; i < SIZE; i++)
		cout << "╦════";
	cout << "╝    ╚";
	for (int i = 0; i < SIZE; i++)
		cout << "════╦";

	cout << "════╗" << endl;


	for (int i = SIZE - 1; i >= 0; i--)
	{
		cout << "║";
		if (i < left_queue.r)
			cout << left_queue.array[i].getColor() << setw(4) << " ▒▒ " << "\033[0m";
		else
			cout << "    ";
	}
	cout << "║";

	if (current_signal == LEFT)
		cout << "\033[1;32m" << " █ " << "\033[0m";
	else
		cout << "\033[1;31m" << " █ " << "\033[0m";
	cout << "      ";

	if (current_signal == RIGHT)
		cout << "\033[1;32m" << "   █ " << "\033[0m";
	else
		cout << "\033[1;31m" << "   █ " << "\033[0m";
	//cout<<"      ";

	for (int i = 0; i < SIZE; i++)
	{
		cout << "║";
		if (i < right_queue.r)
			cout << right_queue.array[i].getColor() << setw(4) << " ▒▒ " << "\033[0m";
		else
			cout << "    ";
	}
	cout << "║" << endl;

	cout << "╚════";
	for (int i = 0; i < SIZE; i++)
		cout << "╩════";
	cout << "╗    ╔";
	for (int i = 0; i < SIZE; i++)
		cout << "════╩";
	cout << "════╝" << endl;

	cout << spaces << "║";

	if (current_signal == BOTTOM)
		cout << "\033[1;32m" << " ▄▄ " << "\033[0m";
	else
		cout << "\033[1;31m" << " ▄▄ " << "\033[0m";
	cout << "║" << endl;

	for (int i = 0; i < SIZE; i++)
	{
		cout << spaces << "╠════╣" << endl;
		cout << spaces;
		cout << "║";
		if (i < bottom_queue.r)
			cout << bottom_queue.array[i].getColor() << setw(4) << " ▒▒ " << "\033[0m" << "║" << endl;
		else
			cout << "    ║" << endl;
	}
	cout << spaces << "╚════╝" << endl;
	cout << endl;
}
//===============================================
//complete the missing functions/methods
template <typename T> Queue<T>::Queue(int size) {
	// creating an array dynamically
	array = new T[size];
	// Initializing variables 
	r = 0;
	n = 0;
	N = size;
}
template <typename T> Queue<T>::~Queue() {
	// removing elements from queue until queue becomes empty
	while (!isEmpty()) dequeue();

}
template <typename T> void Queue<T>::enqueue(T a) {
	if (isFull())
	{
		throw QueueFull();
	}
	// adding element to the available space in the array or queue
	array[r] = a;
	//increasing index where a new element will be added
	r++;
	// Increasing number of elements entered into the queue
	n++;
}
template <typename T> T Queue<T>::dequeue() {
	if (isEmpty())
	{
		throw QueueEmpty();
	}
	// Setting the element to be dequeue to a "temporary" variable 
	T y = array[0];
	// Moving each element forward
	for (int i = 0;i < r - 1;i++) {
		array[i] = array[i + 1];
	}
	//decreasing index where a new element will be added
	r--;
	// decreasing number of elements entered into the queue
	n = n - 1;
	return y;
}
template <typename T> bool Queue<T>::isEmpty() {
	// checking if number of elements entered = 0
	return n == 0;
}
template <typename T> bool Queue<T>::isFull() {
	// checking if number of elements entered = queue size
	return n == N;
}