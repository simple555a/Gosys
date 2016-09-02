//---------------------------------------------------------------------------

#include "precomp.h"
#pragma hdrstop

#pragma package(smart_init)

#include <list>

typedef std::list<TDBLoader::WORK_ITEM> _WORK_QUEUE;

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TDBLoader::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TDBLoader::TDBLoader(bool CreateSuspended)
    : TThread(CreateSuspended)
{
    m_bExiting = false;
    m_hAlert = CreateEvent(0, TRUE, 0, 0);
    m_Works = (void *)(new _WORK_QUEUE);
}

__fastcall TDBLoader::~TDBLoader()
{
    if(m_Works){
        delete (_WORK_QUEUE*)m_Works;
    }
}

//---------------------------------------------------------------------------
void __fastcall TDBLoader::Execute()
{
    //---- Place thread code here ----
    while(!m_bExiting){
        switch(WaitForSingleObject(m_hAlert, 500)){
        case WAIT_TIMEOUT:
            do_works();
            break;
        case WAIT_OBJECT_0:
            m_bExiting = true;
        break;
        }
    }
}

void TDBLoader::do_works()
{
    if(!m_Works) return;
    _WORK_QUEUE::iterator it;
    it = ((_WORK_QUEUE*)m_Works)->begin();
    while(it != ((_WORK_QUEUE*)m_Works)->end()){
        it->proc(it->param);
        it = ((_WORK_QUEUE*)m_Works)->erase(it);
    }
}

void TDBLoader::queue_work_item(WORKER_PROC proc, void * param)
{
    if(!m_Works) return;
    WORK_ITEM wi;
    wi.proc = proc;
    wi.param = param;
    ((_WORK_QUEUE*)m_Works)->insert(((_WORK_QUEUE*)m_Works)->end(), wi);
}
//---------------------------------------------------------------------------
