#include <functional>
#include <time.h>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

typedef function<void(int, int)> file_event_proc;
typedef function<int(int)> time_event_proc;
typedef function<void()> event_end_proc;
typedef function<void()> before_proc;

struct file_event
{
	int mask;
	file_event_proc rproc;
	file_event_proc wproc;
};

struct time_event
{
	long long id;
	long when_sec;
	long when_ms;
	time_event_proc time_proc;
	event_end_proc end_proc;
	std::shared_ptr<time_event> next;
};

struct fired_event
{
	int fd;
	int mask;
};

class eventloop
{
private:
	int maxfd;
	int setsize;
	long long time_event_next_id;
	time_t last_time;
	std::vector<std::shared_ptr<file_event>> events;
	std::vector<std::shared_ptr<fired_event>> fired;
	std::shared_ptr<time_event> time_event_head;
	bool stop;
	before_proc before_sleep;
public:
	eventloop() :maxfd(-1), setsize(10), time_event_next_id(0), last_time(time(NULL)),
		stop(true), before_sleep(NULL), time_event_head(new time_event)
	{
		for (int i = 0; i < setsize; ++i)
		{
			events.push_back(make_shared<file_event>());
			fired.push_back(make_shared<fired_event>());
		}
	}
	~eventloop()
	{

	}
	int create_file_event(int fd, int mask, file_event_proc proc);
	void run();
	int process_event(int flags = 0);
	void set_before_sleep(before_proc bs);
};

int eventloop::create_file_event(int fd, int mask, file_event_proc proc)
{
    events[fd]->mask = mask;
    events[fd]->wproc = proc;
    events[fd]->rproc = proc;
    if (fd > maxfd)
    {
        maxfd = fd;
    }
    return 0;
}

void eventloop::run()
{
    this->stop = false;
    int num = 10;
    while (!this->stop && num--)
    {
        
        if (this->before_sleep != NULL)
        {
            before_sleep();
        }
        process_event();
    }
}

int eventloop::process_event(int flags)
{
    //这个就绪的fd应该是通过多路复用设置的，这里简化了一下
	fired[0]->fd = 1;
	fired[0]->mask = 1;
    //Sleep(100);
    //这里多路复用可以拿到num，我没写所以直接用写死的值了。
    for (int i = 0; i < setsize; ++i)
    {
        int fd = fired[i]->fd;
        int mask = fired[i]->mask;
        if (fired[i]->mask != 0)
        {
            events[fired[i]->fd]->rproc(fd,mask);
        }
    }
    return 0;
}

void before()
{
	cout << "before" << endl;
}

void demo_1(int fd, int mask)
{
	cout << fd << "和" << mask << endl;
}

void eventloop::set_before_sleep(before_proc bs)
{
    this->before_sleep = bs;
}

int main()
{
	eventloop* loop = new eventloop();
	loop->set_before_sleep(before);
    loop->create_file_event(1, 1, demo_1);
	loop->run();
	delete loop;
}
