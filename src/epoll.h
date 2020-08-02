#pragma once
#include <sys/epoll.h>
#include "eventloop.h"
#include <malloc.h>
#include <memory>

typedef struct aeApiState {

    // epoll_event 实例描述符
    int epfd;

    // 事件槽
    struct epoll_event* events;

} aeApiState;

struct epoll
{
    int epfd;
    struct epoll_event* events;
    epoll()
    {
        events = malloc(sizeof(struct epoll_event)* m_eventloop->setsize);
        epfd = epoll_create(1024);
    }

    ~epoll()
    {
        free(events);
        close(epfd);
    }

    int add_event(int fd, int mask)
    {
        struct epoll_event ee;
		int op = m_eventloop->events[fd]->mask == AE_NONE ?
            EPOLL_CTL_ADD : EPOLL_CTL_MOD;

		ee.events = 0;
		mask |= m_eventloop->events[fd]->mask; /* Merge old events */
		if (mask & AE_READABLE) ee.events |= EPOLLIN;
		if (mask & AE_WRITABLE) ee.events |= EPOLLOUT;
		ee.data.u64 = 0; 
		ee.data.fd = fd;

		if (epoll_ctl(epfd, op, fd, &ee) == -1) return -1;

		return 0;
    }

    void del_event(int fd,int delmask)
    {
        struct epoll_event ee;

        int mask = m_eventloop->events[fd]->mask & (~delmask);

        ee.events = 0;
        if (mask & AE_READABLE) ee.events |= EPOLLIN;
        if (mask & AE_WRITABLE) ee.events |= EPOLLOUT;
        ee.data.u64 = 0; /* avoid valgrind warning */
        ee.data.fd = fd;
        if (mask != AE_NONE) {
            epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ee);
        }
        else {
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ee);
        }
    }

    int poll(struct timeval* tvp)
    {
		int retval, numevents = 0;

		// 等待时间
		retval = epoll_wait(epfd,events, m_eventloop->setsize,
			tvp ? (tvp->tv_sec * 1000 + tvp->tv_usec / 1000) : -1);

		// 有至少一个事件就绪？
		if (retval > 0) {
			int j;

			// 为已就绪事件设置相应的模式
			// 并加入到 eventLoop 的 fired 数组中
			numevents = retval;
			for (j = 0; j < numevents; j++) {
				int mask = 0;
				struct epoll_event* e = state->events + j;

				if (e->events & EPOLLIN) mask |= AE_READABLE;
				if (e->events & EPOLLOUT) mask |= AE_WRITABLE;
				if (e->events & EPOLLERR) mask |= AE_WRITABLE;
				if (e->events & EPOLLHUP) mask |= AE_WRITABLE;

                m_eventloop->fired[j]->fd = e->data.fd;
                m_eventloop->fired[j]->mask = mask;
			}
		}
		// 返回已就绪事件个数
		return numevents;
    }
};
