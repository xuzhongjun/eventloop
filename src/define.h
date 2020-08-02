#pragma once
#include <functional>
typedef std::function<void(int, int)> file_event_proc;
typedef std::function<int(int)> time_event_proc;
typedef std::function<void()> event_end_proc;
typedef std::function<void()> before_proc;

/*
 * 事件执行状态
 */
 // 成功
#define AE_OK 0
// 出错
#define AE_ERR -1

/*
 * 文件事件状态
 */
 // 未设置
#define AE_NONE 0
// 可读
#define AE_READABLE 1
// 可写
#define AE_WRITABLE 2

/*
 * 时间处理器的执行 flags
 */
 // 文件事件
#define AE_FILE_EVENTS 1
// 时间事件
#define AE_TIME_EVENTS 2
// 所有事件
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
// 不阻塞，也不进行等待
#define AE_DONT_WAIT 4

/*
 * 决定时间事件是否要持续执行的 flag
 */
#define AE_NOMORE -1

 /* Macros */
#define AE_NOTUSED(V) ((void) V)