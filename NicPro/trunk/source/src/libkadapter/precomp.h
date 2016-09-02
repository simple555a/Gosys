#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <f8.h>
#include <f8_kadapter.h>

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#pragma warning(disable : 4786)

#include <object.h>

using namespace std;

#define _online(a) (a->flags & ADAPTER_F_ONLINE)

void IKAdapter_fire_event(IKAdapter *_this, f8_int e, f8_int p1, f8_int p2);

void * create_msg_queue(IKAdapter * a);
void destroy_msg_queue(void * _q);
__bool IKAdapter_begin_transfer(IKAdapter * adapter);
__bool IKAdapter_end_transfer(IKAdapter * adapter);