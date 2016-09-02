#include "precomp.h"

__uint CRecorder::run()
{
	hislog_key_t nowTime, lastTime;
	CInMemoryBuffer * buffer;
	int color = 0;
	
	notify(EVT_INIT_COMPLETE);
	
	lastTime = hislogGetCurrentTime();

	buffer = bufferMgr.getCurrentBuffer();
	buffer->startRecord(lastTime);
	
	while(!m_bStopRequestPending){
		nowTime = hislogGetCurrentTime();
		if(nowTime <= lastTime){
			rtk_sleep(hislogRecordInterval);
			continue;
		}else{
			buffer->archive(nowTime, color);
		}
		/*
			we commit a buffer in either of the two cases:
			*) we've accumulated enough data in the in-memory
			buffer
			*) the user has adjusted system time so that we're
			now in the past of what we've recorded.
		*/
		if(nowTime - buffer->m_startTime >= hislogArchiveTimeSpan-1){
			buffer->stopRecord(nowTime);
			buffer = bufferMgr.switchToAlternateBuffer();
			if(buffer->m_state != bs_ready){
				// TODO: should handle this
				assert(0);
			}
			buffer->startRecord(nowTime + 1);
		}
		
		rtk_sleep(hislogRecordInterval);
		color++;
		if(color == hislogRecordColors){
			color = 0;
		}
		lastTime = nowTime;
	}

	//buffer->stopRecord(nowTime);
	//buffer->flushToDisk();

	return 0;
}

