#include <iostream>
#include "eventloop.h"
using namespace std;

void before()
{
	cout << "before" << endl;
}

void demo_1(int fd, int mask)
{
	cout << fd << "ºÍ" << mask << endl;
}

int main()
{
	LOOP->set_before_sleep(before);
	LOOP->create_file_event(1, 1, demo_1);
	LOOP->run();
}