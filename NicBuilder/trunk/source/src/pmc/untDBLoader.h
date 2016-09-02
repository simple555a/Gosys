//---------------------------------------------------------------------------

#ifndef untDBLoaderH
#define untDBLoaderH
//---------------------------------------------------------------------------
#include <Classes.hpp>

typedef void (PMC_API *WORKER_PROC)(void * param);

//---------------------------------------------------------------------------
class TDBLoader : public TThread
{
private:
    struct WORK_ITEM{
        WORKER_PROC proc;
        void        *param;
    };
    typedef void *WORK_QUEUE;
    WORK_QUEUE  m_Works;
protected:
    void __fastcall Execute();
    void    do_works();
public:
    __fastcall TDBLoader(bool CreateSuspended);
    virtual __fastcall ~TDBLoader();
    void    queue_work_item(WORKER_PROC proc, void * param);
    void    stop();
public:
    bool    m_bExiting;
    HANDLE  m_hAlert;
};
//---------------------------------------------------------------------------
#endif
