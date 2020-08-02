#pragma once
#include <functional>
typedef std::function<void(int, int)> file_event_proc;
typedef std::function<int(int)> time_event_proc;
typedef std::function<void()> event_end_proc;
typedef std::function<void()> before_proc;

/*
 * �¼�ִ��״̬
 */
 // �ɹ�
#define AE_OK 0
// ����
#define AE_ERR -1

/*
 * �ļ��¼�״̬
 */
 // δ����
#define AE_NONE 0
// �ɶ�
#define AE_READABLE 1
// ��д
#define AE_WRITABLE 2

/*
 * ʱ�䴦������ִ�� flags
 */
 // �ļ��¼�
#define AE_FILE_EVENTS 1
// ʱ���¼�
#define AE_TIME_EVENTS 2
// �����¼�
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
// ��������Ҳ�����еȴ�
#define AE_DONT_WAIT 4

/*
 * ����ʱ���¼��Ƿ�Ҫ����ִ�е� flag
 */
#define AE_NOMORE -1

 /* Macros */
#define AE_NOTUSED(V) ((void) V)