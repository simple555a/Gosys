#include "precomp.h"

static const NODE_KEY nk = {"TEST_NODE"};
static const GROUP_KEY gk = {"TEST_GROUP"}; 
static RTK_CURSOR hNode, hGrp;
static CInMemoryBuffer * theDebugBuffer;
/*
  Read strTagFile, insert all the contained valid tag entries
  to the global tag-table.
  Return valid entries
*/
static int _load_tags(RTK_CURSOR hgroup, string & strTagFile)
{
	string strLine; 
	char   strLineBuf[1024];
	int    iLine;
	FILE   * fp;
	string element;
	string key;    
	RTK_TAG tte;
	char g[rtkm_group_key_length+1];
	PRTK_GROUP pgroup;
	SHORT_TAG_NAME stn;
	RTK_CURSOR	htag;
	TAG_NAME tn;

	pgroup = (PRTK_GROUP)cursor_get_item(hgroup);
	if( !pgroup ){
		utils_error("Error opening hgroup.\n");
		return 0;
	}
	stn.group = pgroup->key;

	tn.node = pgroup->node;
	tn.sname.group = pgroup->key;
	
	fp=fopen(strTagFile.data(),"rt");
	if(!fp){
		utils_error("Cannot open tag datafile:%s\n",strTagFile.data()); 	   
		return 0;
	}

	iLine=0;
	// add tags
	while(!feof(fp)){
		iLine++;		
		if( !fgets(strLineBuf,sizeof(strLineBuf),fp) ) break;
		if(strLineBuf[strlen(strLineBuf)-1]=='\n'){
			strLineBuf[strlen(strLineBuf)-1]=0;
		}

		// 2002/10/21, Jackie, group can have a description field
		if(!strnicmp(strLineBuf, ".desc ", 6)){
			strncpy(
				pgroup->description, 
				strLineBuf+6, 
				sizeof(pgroup->description)
				);
			pgroup->description[sizeof(pgroup->description) - 1] = 0;
			continue;
		}
		if(strLineBuf[0] == '#'){
			continue;
		}

		// strupr(strLineBuf);
		int err = tag_by_text(&tte, strLineBuf, &pgroup->key);
		if( err < 0 ){
			tag_trans_error( err );
			utils_error(" error in file %s, line %d.\n", 
						strTagFile.data(),iLine
				);
			continue;
		}
		// try insert it
		htag = create_tag(hgroup, &tte.key, &tte.s);
		if(!htag){
			utils_error(
				"File %s, line %d, tag=%s, duplicate key or not enough memory\n", 
				strTagFile.data(), 
				iLine,
				(char*)CTagName(tte.key)
				);
		}else{
			tn.sname.tag = tte.key;
			theDebugBuffer->addTag(&tn);
			close_handle(htag);
		}
	}
	fclose(fp);
	strncpy(g, pgroup->key.Data, rtkm_group_key_length);
	g[sizeof(g) - 1] = 0;
	__uint itemcount;
	itemcount = cursor_get_subitem_count(hgroup);
	utils_trace(
		"Group %s, refresh=%d, %d tags added\n", 
		g, 
		pgroup->period, 
		itemcount
		);
	return itemcount;
}

void loadRandomDatabase()
{
	string tfile;
	RTK_GROUP g;
	
	lock_rtdb(__true, 1000);

	hNode = create_node(&nk, 0);
	memset(&g, 0, sizeof(g));
	g.node = nk;
	g.key = gk;
	hGrp = create_group(hNode, &g);
	tfile = string(get_working_dir()) + "config/diagnostic.dat";
	_load_tags(hGrp, tfile);

	unlock_rtdb();
}

void randomTagValues(hislog_key_t time)
{
	RTK_CURSOR hTag;
	PRTK_TAG pTag;

	hTag = cursor_open_first_subitem(hGrp);
	while(!cursor_is_end(hTag)){
		pTag = (PRTK_TAG)cursor_get_item(hTag);
		if(rng_rand(0, 10000) > 9500){
			pTag->d.Value.Flags &= ~TF_Valid;
		}else{
			pTag->d.Value.Flags |= TF_Valid;
		}
		switch(get_value_type(pTag->s.Flags)){
		case dt_bool:
			pTag->d.Value.Value.b = rng_rand(0, 100) > 80? __true : __false;
			break;
		case dt_real4:
			/* a sine wave plus a white noise */
			pTag->d.Value.Value.flt = (__r4)(sin((double)time) * 50 + 50 + rng_rand(0, 100)/100.);
			break;
		}
		cursor_move_next(hTag);
	}
}

/*
	generate a lot of archives randomly
*/
void randomArchive(
	hislog_key_t t1,
	hislog_key_t t2,
	__uint step
	)
{
	hislog_key_t nowTime;
	int color = 0;
	
	nowTime = t1;
	theDebugBuffer = bufferMgr.getCurrentBuffer();

	loadRandomDatabase();
	
	theDebugBuffer->startRecord(nowTime);
	
	while(nowTime < t2){
		randomTagValues(nowTime);
		
		theDebugBuffer->archive(nowTime, color);
	
		if(nowTime - theDebugBuffer->m_startTime >= hislogArchiveTimeSpan-1){
			theDebugBuffer->stopRecord(nowTime);
			utils_debug(
				"Flushing to archive %s-%s\n",
				timeAsString(theDebugBuffer->m_startTime).c_str(),
				timeAsString(nowTime).c_str()
				);
			theDebugBuffer->flushToDisk();
			nowTime += step;
			theDebugBuffer->startRecord(nowTime);
		}else{
			nowTime += step;
		}
		
		color++;
		if(color == hislogRecordColors){
			color = 0;
		}
	}
	
	theDebugBuffer->stopRecord(nowTime);
	theDebugBuffer->flushToDisk();

	close_handle(hGrp);
	close_handle(hNode);
}


