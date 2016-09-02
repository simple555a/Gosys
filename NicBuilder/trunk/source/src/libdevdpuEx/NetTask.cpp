//= UPDATE RECORD
//= Update for    Version     Update by     Date         Update(function names modified)                           
//= (19-2)       v1.0.0.19    Zander Sun    2013-9-24    1.新增：init_net_tag()、get_net_tn()、check_net_by_tag()
//=                                                      2.check_connect():增加利用网络状态标签值检查网络并决定是否断开连接的函数调用。
//=                                                      3.CNetTask():增加网络状态标签名的初始化。
//= (22-2)       v1.0.0.22    Zander Sun    2014-10-10   too many functions were modified
//= (22-3)       v1.0.0.22    Zander Sun    2013-10-10   check_conneck()
//= (23-1)       v1.0.0.23    Zander Sun    2014-10-13   delete clear_packet_buf() where it was called

#include "precomp.h"
#include "ONetPctl.h"
#include "NetTask.h"
#include "ONetPctlSend.h"

#define SET_T1_LINK_TIME 5

CNetTask::CNetTask( ONet_DB *db,
				   __bool isUseTimer ,		//是否采用定时器处理网络连接
				   CONetPctlDepository * pctls)
{
	for (__i32 i = 0; i < 4; i++)
	{
		_isOnline[i] = __false;
		_isUsed[i]  = __true;
		// Zander, 2013-8-30
		m_is_linked[i] = __false;
		m_ping_count[i] = 0;
		memset(&tn_net[i], 0, sizeof(TAG_NAME)); // Zander, 2013-9-24
		
		//=start= Zander Sun
		_islinkStartAck[i] = __false;
		_islinkStopAck[i] = __false;

		_islinkT1Ack[i]=__false;    //GM 2015.05.06
		_islinkDataAck[i]=__false;    //GM 2015.05.06

		_conCount[i] =0;
		//=end= 2014-10-10 

		// test
		recv_num[i] = 0;
		send_num[i] = 0;
	}
	_conTimeout.set(0,50000);
	_sendOutTime.set(0,500000);
	_recvOutTime.set(0,500000);
	_refreshTime = 400;
	memset(_recvBuffer,0,MAX_SIZE_BUF);
	_pctls = pctls;
	_curNum =0xffff;
	_oldNum = 0xffff;//WP
	_curPeer = NULL;
	_isConntChanged = __false;
	_isEnbleTimer =isUseTimer;
	_dbPtr = db;
	m_link_error_count = 0; //Zander 2013-7-24
	m_st_num = 0; // Zander 2013-8-21
}
  CNetTask::~CNetTask()
 {
	 _curNum =0xffff;
	 _curPeer = NULL;
 }
void   CNetTask::create_reactor(ACE_Reactor *rct)
{

	//采用上层应用的reactor
	if (rct)
	{
		reactor(rct);
	}
	else
	{
		reactor(ACE_Reactor::instance());
	}
}

  __i32 CNetTask::set_cont_outtime(__u32 tm)//ms单位
  {
	 _conTimeout.set(0,tm* 1000);

	  return RET_OK;

  }
  __i32 CNetTask::set_send_outtime(__u32 tm)//ms单位
  {
  	 _sendOutTime.set(0,tm* 1000);
	  return RET_OK;
  }
  __i32 CNetTask::set_recv_outtime(__u32 tm)//ms单位
  {
     _recvOutTime.set(0,tm* 1000);
	  return RET_OK;
  }
  __i32  CNetTask::set_onet_client_addr(__u8 addr)
  {
	  _clientAddr = addr;
	  return RET_OK;

  }
  __i32  CNetTask::set_onet_dpu_addr(__u8 addr)
  {
	_dpuAddr = addr;
	  return RET_OK;
  }

  __i32 CNetTask::set_refresh_time(__u16 tm)
{
	_refreshTime = tm;
	return RET_OK;
}

 //初始化网络配置信息:主控制器+备控制器“192.168.1.25;192.168.1.26”
 //隐含的备网信息为“192.168.2.25;192.168.2.26”
 __i32  CNetTask::init_net_config(char * paras_)
 {
	 CArgs args;
	 __i32 i = 0;
	 parse_arg_ex(paras_,&args,";");
	 if (args.argc > 2)
	 {
		 ACE_DEBUG((LM_DEBUG,"more than two addresses specified, rest ignored\n"));
		 args.argc = 2;
	 }
	 else if(args.argc == 1)
	 {
		 _isUsed[0]= __true;
		 _isUsed[2]= __true;
		 _isUsed[1]= __false;
 		 _isUsed[3]= __false;
	 }
	 else if(args.argc == 2)
	 {
		 _isUsed[0]= __true;
		 _isUsed[2]= __true;
		 _isUsed[1]= __true;
		 _isUsed[3]= __true;
	 }
	 else // =0
	 {
		 _isUsed[0]= __false;
		 _isUsed[2]= __false;
		 _isUsed[1]= __false;
		 _isUsed[3]= __false;
	 }
	 for (i = 0; i < args.argc; i++)
	 {
		 ACE_INET_Addr & addr_ = _netAddr[i];
		 if (addr_.set(SERV_PORT,args.argv[i])!= 0)
			{
				ACE_DEBUG((LM_DEBUG,"%s is not a valid internet address\n",args.argv[i]));
				return RET_Fail;

			}
		 //把备用网默认为主网+1；如主网为"192.168.1.155",备网为"192.168.2.155"
		 __u32 ipAddr = addr_.get_ip_address();
		 if (_netAddr[i+2].set(SERV_PORT,ipAddr +0x100))
			{
				ACE_DEBUG((LM_DEBUG,"%s is not a valid internet address\n",_netAddr[i+2].get_host_addr()));
				return RET_Fail;
			}
	 }
	 return RET_OK;
 }



 __i32  CNetTask::connect_all()
 {
	 __i32 i =0;
	 ACE_Time_Value outtime_ping(0,5000);

	 for(i = 0; i < 4; i++)
	 { 
		  //=start= Zander Sun
		 if ((_isOnline[i] == __true || _isUsed[i] == __false)/* && (_dbPtr->_staticAttr[i%2].dpuStatus&0x0001)*/)
		 {
			 continue;
		 }

		 if(_connector.connect(_peer[i],_netAddr[i],&_conTimeout) == 0)
			{				
				__u32 sendbuf = 512;
				__i32 ret =ACE_OS::setsockopt (_peer[i].get_handle(), 
					SOL_SOCKET, 
					SO_SNDBUF, 
					(const char *)&sendbuf, 
					sizeof (sendbuf));

					_isOnline[i] = __true;
					m_is_linked[i] = __true;

					// it must register the socket stream class at the first connect
					if(((ACE_SOCK_Stream*)(&_peer[i]))->get_handle() != ACE_INVALID_HANDLE)
						reactor()->register_handler(_peer[i].get_handle(),this,ACE_Event_Handler::READ_MASK);
			}
		 else
			{
				//每个连接成功要置在线状态
				//	ACE_DEBUG((LM_DEBUG,"%s is not connected\n",_netAddr[i].get_host_addr()));
		
				_isOnline[i] = __false;
			}
		 //=end= 20014-10-10
	 }
     
	 // Zander 2013-8-29
	 if (!(_isOnline[0]||_isOnline[1]||_isOnline[2]||_isOnline[3]))
	 {
		 return __false;
	 }

	 return __true;
 }
 __i32  CNetTask::connect_net(int i)
 {
	 if(_isOnline[i])
	 {
		 return RET_OK;
	 }

	 if(_connector.connect(_peer[i],_netAddr[i],&_conTimeout) == 0)
	 {
		 _isOnline[i] = __true;

		 // Zander 2014-10-10
		 if(((ACE_SOCK_Stream*)(&_peer[i]))->get_handle() != ACE_INVALID_HANDLE)
		 {
			 reactor()->register_handler(_peer[i].get_handle(),this,ACE_Event_Handler::READ_MASK);
		 }
	
	 }
	 else
	 {
		 _isOnline[i] = __false;
	 }
	 	 return RET_OK;
 }
 __i32  CNetTask::disconnect_net(int i)
 {
	 if(i == 0xffff) return RET_OK;
	 int ret = -1;
	 if (i == 0 || i == 1) // 2015-5-6
	 {
		 // test
		 printf("stop i = %d\n", i);
		 send_stop_link(ONet_Const::LINK_NET_0, i); // 2015-5-6   GM
	 }
	 else if(i == 2 || i == 3)
	 {
		 // test
		 printf("stop i = %d\n", i);
		 send_stop_link(ONet_Const::LINK_NET_1, i);
	 }
	 _isOnline[i] = __false;
	 _islinkStartAck[i] = __false;

	 _islinkT1Ack[i]=__false;    //2015.05.06   GM
	 _islinkDataAck[i]=__false;  //2015.05.06   GM

	 ret =reactor()->remove_handler(_peer[i].get_handle(),ACE_Event_Handler::READ_MASK|ACE_Event_Handler::DONT_CALL);
	 _peer[i].close_writer();
	 return _peer[i].close();

 }
 __i32  CNetTask::disconnect_all()
 {
	 __i32 i =0;

	 for(i = 0; i < 4; i++)
	 {	 
		 //if(_isOnline[i] == __true)
		 //{
			// _isOnline[i] = __false;
			// _peer[i].close_writer();
			// _peer[i].close();
		 //}
		 // 2015-5-27 Zander
		 disconnect_net(i);

	 }
	 return RET_OK;
 }
 //检查当前连接是否在线，不在线则在四个连接中来回切换,优先级为0.2.1.3
 //=start= Zander
 __bool  CNetTask::check_connect()
 {
	 __i32 i = 0;
	 __i32 oldNum = _curNum;
	 ACE_SOCK_Stream *oldpeer = _curPeer;



	 // 4 Nets all disconnect
	 if(!_isOnline[0] && !_isOnline[1] && !_isOnline[2] && !_isOnline[3] )
	 {
		 //	ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] all connected is failed\n"));
		 //控制器所有网线都不在，那就是什么控制器都无效了
	/*	 _dbPtr->_staticAttr[0].dpuStatus &= ~0x0001;
		 _dbPtr->_staticAttr[1].dpuStatus &= ~0x0001;	*/	

		 //2015.3.18 GM 加入如下代码
		 m_is_dpu_exist = __false;
		 //if (_isLinked == __true)
		 //{
	     _isLinked = __false;
			 
		 return __false;
		// } 
	 }else
	 {
		 m_is_dpu_exist = __true;
	 }

	 // Connecting priority:
	 //      first: less IP, Net A
	 //      second: greater IP, Net A
	 //      third: less IP, Net B
	 //      fouth: greater IP, Net B
	 if(_curNum<4)
	 {
		 if(_isOnline[_curNum] == __false)
		 {
			 if((_curNum == 0)||(_curNum == 2))
			 {_curNum = _isOnline[0]?
			 0:_isOnline[2]?
			 2:_isOnline[1]?
			 1:_isOnline[3]?
			 3:0xFFFF;}
			 if((_curNum == 1)||(_curNum == 3))
			 {_curNum = _isOnline[1]?
			 1:_isOnline[3]?
			 3:_isOnline[0]?
			 0:_isOnline[2]?
			 2:0xFFFF;}
		 }else
		 {
			 if (_dpuAddr == 20)
			 {
				 printf("dpuStatus[0] = %d\n", _dbPtr->_staticAttr[0].dpuStatus);
				 printf("dpuStatus[1] = %d\n", _dbPtr->_staticAttr[1].dpuStatus);
			 }

			 // 若当前连接与主备不一致，则需更改当前连接
			 //=start= Zander   for (22-3)	
			 if (_dbPtr->_staticAttr[0].dpuStatus & 0x01)    // 小IP为主
			 {
				 if (_curNum != 0 && _curNum != 2)
				 {
					 _curNum = _isOnline[0]?0 : _isOnline[2]?2 : 0xFFFF;
				 }
			 }else if(_dbPtr->_staticAttr[1].dpuStatus & 0x01) // 大IP为主
			 {
				 if (_curNum !=1 && _curNum != 3)
				 {
					 _curNum = _isOnline[1]?1 : _isOnline[3]?3 : 0xFFFF;
				 }
			 }
			 //=end= 2014-10-10
		 }
	 }
	 else
	 {
		 _curNum = _isOnline[0]?
			 0:_isOnline[2]?
			 2:_isOnline[1]?
			 1:_isOnline[3]?
			 3:0xFFFF;		
	 }
	
	 if (_curNum > 3)
	 {
		 return __false;
	 }

	 _curPeer = &_peer[_curNum];
	  __u8   Link_Net_Tmp; 

	 for (i=0; i!=4; i++)
	 {
		 if (i==0||i==1)
		 {
			Link_Net_Tmp=ONet_Const::LINK_NET_0;
		 }
		 else
		 {
			Link_Net_Tmp=ONet_Const::LINK_NET_1;
		 }
		 if (_isOnline[i] && _isUsed[i])
		 {
			// printf("isStartAck[%d] = %d\n",i, _islinkStartAck[i]);
			 if (!_islinkStartAck[i])
			 {
				 // 2015-5-6

					send_start_link(Link_Net_Tmp, i);
				 //if (i==0 || i==1)
				 //{
					// send_start_link(ONet_Const::LINK_NET_0, i);
					// send_t1_link(ONet_Const::LINK_NET_0,i); //? if t1 link failed, do what??how to discover it failed
				 //} 
				 //else
				 //{
					// send_start_link(ONet_Const::LINK_NET_1, i);
					// send_t1_link(ONet_Const::LINK_NET_1,i); //? if t1 link failed, do what??how to discover it failed
				 //}				 

				 
				 
				 //if ( i == _curNum )
				 //{
					// send_read_link(i);//? if read link failed, do what??how to discover it failed
					// // correct time link

				 //}
			 } 
			 else
			 {
				 if (i == _curNum)
				 {
					 // 2015-5-6
					 if (!_islinkT1Ack[i])
					 {
						 send_t1_link(Link_Net_Tmp,i);

					 } 
					 else
					 {
						 if (!_islinkDataAck[i])
						 {
							 // if ( i == _curNum )
							 // {
							 send_read_link(i);//? if read link failed, do what??how to discover it failed
							 // correct time link
							 // }
						 }else{
							 send_test_link(Link_Net_Tmp, i);   // Zander 2015-5-27
						 } 

						 // connecting net changed between two cycle of this thread
						 // = connecting nets in last cycle and this cycle are different; ??? 
						 if (_oldNum != _curNum)
						 {
							 // Zander  2015-5-15
							 if (_islinkDataAck[_oldNum] == __true)
							 {
								 //if (oldNum == 0 || oldNum == 1) // 2015-5-6
								 //{
									// send_stop_link(ONet_Const::LINK_NET_0, oldNum); // 2015-5-6
								 //}
								 //else if(oldNum == 2 || oldNum == 3)
								 //{
									// send_stop_link(ONet_Const::LINK_NET_1, oldNum);
								 //}
								 ////send_stop_link(ONet_Const::LINK_NET_0, oldNum);
								 //_islinkDataAck[oldNum]=__false;
								 //_islinkT1Ack[oldNum] = __false;
								 disconnect_net(_oldNum); // 2015-5-27   GM  
							 }
							// send_read_link(_curNum);


						 }
					 }
				 }else{
					 // Zander  2015-5-27
					 send_test_link(Link_Net_Tmp,i);

				 }
					
	


				 //send_num[i]++;
				 //printf("send num %d = %d\n", i, send_num[i]);
					
				 //if (i==0 || i==1)
				 //{
					// send_test_link(ONet_Const::LINK_NET_0,i);   //? if test link failed, do what??how to discover it failed
				 //} 
				 //else
				 //{
				 //	send_test_link(ONet_Const::LINK_NET_1,i);   //? if test link failed, do what??how to discover it failed
				 //}		

				// _dbPtr->_netStatus[i] = __true;

				 // test
				 //send_num[i]++;
				 //printf("send num %d = %d\n", i, send_num[i]);

				 //// connecting net changed between two cycle of this thread
				 //// = connecting nets in last cycle and this cycle are different; ??? 
				 //if (_oldNum != _curNum)
				 //{
					// send_read_link(_curNum);
					// 
				 //}
			 }
		 }

	 }

	 _oldNum = _curNum;

	 return __true;
 }
 //=end= 2014-10-10

 __bool   CNetTask::is_enbled()
 {
	 return _isEnbled;
 }
 __bool	  CNetTask::is_linked()
 {
	 return _isLinked;
 }
 //同步执行协议解析、赋值功能
 __i32  CNetTask::do_recv_packet(__i32 IP_i)      // Zander 2014-10-10  add "IP_i"
 {

	 __u16 dataLen_ = 0;	//每一帧内数据区长度
	 __u8  info_code_ = 0;
	 CONetBase * pctl_ptr_= NULL;
	 __i32 packHeaderLen_ = RTK_FIELD_OFFSET(FD_PACKET_ONet,info_code);//帧头长度,到（不含）info_code止
	 __i32 len = 0;
	 __u8 temp_area_1_ = 0x0;
	 memset(_recvBuffer,0,MAX_SIZE_BUF);
	 //先接收帧头
	 __u8 * ptrRead_ = _recvBuffer;

	 //=start=  Zander
	 ACE_SOCK_Stream *local_curPeer = NULL;

	 if (IP_i > 3)
	 {
		 return RET_Fail;
	 }
	 local_curPeer = &_peer[IP_i];  

	 if (!local_curPeer)
	 {
		 return RET_Fail;
	 }
	 //=end=  2014-10-10

	 // Zander 2014-10-13
	 // if connecting net of dpu changed, clear the buffer.
	 //utils_debug("_curNum = %d   _oldNum = %d", _curNum, _oldNum)
	 //if (_curNum != _oldNum)
	 //{
		// pctl_ptr_->clear_packet_buff();
	 //}

	//2015.05.07 zander   将_curPeer更改为local_curPeer
	 if(local_curPeer->get_handle() == ACE_INVALID_HANDLE)
	 {

		 return RET_Fail;
	 }
	if ((len = local_curPeer->recv_n(_recvBuffer,packHeaderLen_,&_recvOutTime)) < 0)
	{
		return __false;
	}
	//printf("recv[%d] len = %d\n", IP_i, len);
 
	 if(*ptrRead_ == SYNC_HEADER)
	 {
		 //判断是否为一般数据帧
		 temp_area_1_ = *(__u8*)(ptrRead_ +RTK_FIELD_OFFSET(FD_PACKET_ONet,control_area_1));
		 switch (temp_area_1_ & FRAME_FORMAT_MASK )
			{
		 case T1_TIMEOUT_FRAME:

			//printf("prase T1_TIMEOUT_FRAME\n");
			 //2015.05.06 GM 
			 if(temp_area_1_ & T1_ACK_MASK)
			 {
				 //			 ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] DPU link T1 is  ack\n"));
				 _islinkT1Ack[IP_i]=__true;    //2015.05.06   GM
			 }
			 break;
		 case T2_TIMEOUT_FRAME:
			//printf("prase T2_TIMEOUT_FRAME\n");
			 break;
		 case LINK_FRAME:
			 //printf("prase LINK_FRAME\n");
			 if( temp_area_1_ & LINK_START_ACK)
			 {
				// ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] %s DPU link start is  ack\n",_netAddr[_curNum].get_host_addr()));
				 _islinkStartAck[IP_i] = __true; // Zander 2014-10-10  add "IP_i"
				
			 }
			 else if( temp_area_1_ & LINK_STOP_ACK)
			 {
		//		 ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] DPU link stop is  ack\n"));
				  _islinkStopAck[IP_i] = __true;  // Zander 2014-10-10  add "IP_i"
			 }
	//		 else if(temp_area_1_ & T1_ACK_MASK)
	//		 {
	////			 ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] DPU link T1 is  ack\n"));
	//				_islinkT1Ack[IP_i]=__true;    //2015.05.06   GM
	//		 }
			 else if(temp_area_1_ & LINK_TEST_ACK)
			 {
				 //temp_area_1_ = temp_area_1_;
				 // test
	/*			 recv_num[IP_i]++;
				 printf("recv num %d = %d\n", IP_i, recv_num[IP_i]);*/


	//			 ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] DPU link Test is  ack\n"));
				 //_islinkStartAck[IP_i] = __true; // Zander 2015-5-4^  add "IP_i"
			 }
			 else
			 {
		//		 ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] LINK_FRAME control_area_1: 0x%x\n",temp_area_1_));
			 }

			 break;
			 //一般数据
		 case GENERAL_DATA_FRAME:
             //printf("prase GENERAL_DATA_FRAME\n");
			 // Zander, 2015-5-15 
			 //if (!(_islinkStartAck[IP_i] && _islinkT1Ack[IP_i]))
			 //{
				//printf("stop1!!!!!!!!!");
				// return __false;
			 //}
			 dataLen_ = *(__u8*)(ptrRead_+RTK_FIELD_OFFSET(FD_PACKET_ONet,length_low)) + 0x100 * (*(__u8*)(ptrRead_+RTK_FIELD_OFFSET(FD_PACKET_ONet,length_high))& LENGTH_HIGH_MASK);
			 // VV
			 //printf("dataLen_ = %x\n", dataLen_);


			 if(local_curPeer->recv_n(_recvBuffer + packHeaderLen_ ,dataLen_,&_recvOutTime) < 0)
			 {
				 ACE_DEBUG((LM_DEBUG,"%D,[ %P|%t]  recv failed socket 0x%x ,len :%d\n",local_curPeer,dataLen_));
				 return __false;
			 };
			 info_code_ = *(ptrRead_+RTK_FIELD_OFFSET(FD_PACKET_ONet,info_code))& ~MULTI_PACKET;
			 pctl_ptr_ = _pctls->get_pctl(info_code_);


			 if(!(pctl_ptr_))
				{
			//		ACE_DEBUG((LM_DEBUG,"this pctl :0x%x is not insisted now\n",info_code_));
					//不支持的协议不处理，但不代表网络中断
					return RET_OK;
				}
			
			 //2015.05.18 GM
			 
		
			if(info_code_==0x15)
			{
                 //printf("接收到15H的信息！");
				 handle_15H_event(_recvBuffer+RTK_FIELD_OFFSET(FD_PACKET_ONet,info_code),dataLen_,IP_i);
			 }else
			{
                // Zander, 2015-5-27
				//2015.07.31   GM   增加对0x31H信息码的处理,如果发过来的一帧数据的第十位是0x31H信息码，则表明是控制器进行了DI变位上传
				if ((IP_i != _curNum)&&(info_code_!=0x31))
				{
					//printf("disconnect:%d\n",IP_i);
					disconnect_net(IP_i);
				}else{
					//数据加锁在独自的ONET_DB结构里
					//ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] %s recv pctl id:0x%x is bengin\n",_netAddr[_curNum].get_host_addr(),info_code_));

					//记录接受两次数据的时间差

					//RevTime1=GetTickCount();
					////Sleep(500);
					//DWORD dwTimeGap = RevTime1 - RevTime2; // 单位：毫秒
					//printf("时间差：%d\n",dwTimeGap);
					//RevTime2=RevTime1;

					//普通数据帧只处理从info_code(包含)开始的信息

					pctl_ptr_->parse_packet(_recvBuffer+RTK_FIELD_OFFSET(FD_PACKET_ONet,info_code),dataLen_,IP_i); //关联相关的类进行处理

					//	 ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] %s recv pctl id:0x%x is end\n",_netAddr[_curNum].get_host_addr(),info_code_));

					_islinkDataAck[IP_i]=__true;
					//printf("IP_i = %d, dataLen = %d\n", IP_i, dataLen_);
					
				}
			 }
			}
	 }
	 else
	 {
		 //不是需要的帧也不代表网络中断
			// return RET_OK;
	 }

	_conCount[IP_i] = 0;  // Zander 2014-10-10  add "IP_i"


	 return RET_OK;

 }

 //2015.05.18 GM
 //普通数据帧只处理从info_code(包含)开始的信息
 __i32 CNetTask::handle_15H_event(
	 __u8 *recvBuff_,
	 __u32 recvLen_,
	 __u8 curNetNum_)
 {
	 __u8 * ptr_ = recvBuff_;
	 ACE_DEBUG((LM_DEBUG, "DPU返回错误信息: 0x%x----",*(ptr_ + 1)));
	 switch (*(ptr_ + 1)) {
	case ERROR_UNKNOW_SFDCS: ACE_DEBUG((LM_DEBUG, "包头不是0x68H\n")); break;
	case ERROR_NOBUILDLINK: ACE_DEBUG((LM_DEBUG, "没有建立握手连接，就发出了请求或其他数据请求\n"));
		// 2015-5-27
		_islinkStartAck[curNetNum_]=__false;  
		_islinkT1Ack[curNetNum_]=__false;
		_islinkDataAck[curNetNum_]=__false;
		break;
	case ERROR_SUBSC_INVALID: ACE_DEBUG((LM_DEBUG, "申请订购定时器失败\n")); break;
	case ERROR_ALREADYHASTIMER: ACE_DEBUG((LM_DEBUG, "无用\n")); break;
	case ERROR_TIMEROVERFLOW: ACE_DEBUG((LM_DEBUG, "申请的定时器小于1\n")); break;
	case ERROR_HASNOTIMER: ACE_DEBUG((LM_DEBUG, "目前无用\n")); break;
	case ERROR_ALREADYLINK: ACE_DEBUG((LM_DEBUG, "已经建立了连接，又重新申请连接\n")); break;
	case ERROR_INVALIDPACKET: ACE_DEBUG((LM_DEBUG, "报头不是0x68H\n")); break;
	case ERROR_LENTOOSHORT: ACE_DEBUG((LM_DEBUG, "报头中长度小于等于0\n")); break;
	case ERROR_IODEAD: ACE_DEBUG((LM_DEBUG, "访问的IO已死\n")); break;
	case ERROR_IOADDR: ACE_DEBUG((LM_DEBUG, "IO地址错\n")); break;
	case ERROR_UNKNOWNDATATYPE: ACE_DEBUG((LM_DEBUG, "下行数据的数据类型，DPU没有定义\n")); break;
	case ERROR_MAKEFRAME: ACE_DEBUG((LM_DEBUG, "组祯出现了错误\n")); break;
	case ERROR_NOVALIDIO: ACE_DEBUG((LM_DEBUG, "请求数据中IO地址都找不到\n")); break;
	case ERROR_INVALID_TARGET: ACE_DEBUG((LM_DEBUG, "DPU地址不符合定义\n")); break;
	case ERROR_INVALID_SRC: ACE_DEBUG((LM_DEBUG, "HMI节点地址不符合定义\n")); break;
	case ERROR_REGISTER_DEV: ACE_DEBUG((LM_DEBUG, "注册通信设备地址出错\n")); break;
	case ERROR_TARGET_NOTCORRECT: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_DPU_STATUS_NORUN: ACE_DEBUG((LM_DEBUG, "DPU状态不是运行态\n")); break;
	case ERROR_DEVNO_INVALID: ACE_DEBUG((LM_DEBUG, "在节点中设备序号无效\n")); break;
	case ERROR_NODE_OVERLIMIT: ACE_DEBUG((LM_DEBUG, "请求DPU或IO的数目太多，超过了限制\n")); break;
	case ERROR_NO_PROTOCOL: ACE_DEBUG((LM_DEBUG, "协议没有初始化\n")); break;
	case ERROR_NODE_HASNOADDR: ACE_DEBUG((LM_DEBUG, "请求中出现了无效的DPU或IO地址\n")); break;
	case ERROR_NODE_INVALID: ACE_DEBUG((LM_DEBUG, "返回无效的地址\n")); break;
	case SYS_MEM_ALLOC_ERR: ACE_DEBUG((LM_DEBUG, "系统分配内存出错\n")); break;
	case SYS_QERR_QFULL: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_RIGHT_NOWRITE: ACE_DEBUG((LM_DEBUG, "没有写权限\n")); break;
	case ERROR_OPEN_FILENAME: ACE_DEBUG((LM_DEBUG, "打开文件错\n")); break;
	case ERROR_NOEVENT_ITEM: ACE_DEBUG((LM_DEBUG, "读事件缓冲区错（空）\n")); break;
	case ERROR_NOLOG_ITEM: ACE_DEBUG((LM_DEBUG, "读LOG缓冲区错（空）\n")); break;
	case ERROR_OVERLIMIT: ACE_DEBUG((LM_DEBUG, "\n")); break;
	case ERROR_NO_LBDATA: ACE_DEBUG((LM_DEBUG, "没有录波文件\n")); break;
	case ERROR_SETTING_INVALID: ACE_DEBUG((LM_DEBUG, "定值区无效（指读取NVRAM失败、定值区无效）\n")); break;
	case ERROR_SETTING_AREA: ACE_DEBUG((LM_DEBUG, "定值区号错\n")); break;
	case ERROR_SETTING_NUM: ACE_DEBUG((LM_DEBUG, "定值个数错\n")); break;
	case ERROR_SETTING_OVERFLOW: ACE_DEBUG((LM_DEBUG, "定值越限\n")); break;
	case ERROR_SETTING_CRC: ACE_DEBUG((LM_DEBUG, "定值crc校验错\n")); break;
	case ERROR_SETTING_TYPE: ACE_DEBUG((LM_DEBUG, "定值类型错\n")); break;
	case ERROR_SETTING_STATE: ACE_DEBUG((LM_DEBUG, "操作状态错\n")); break;
	case ERROR_SETTING_WFILE: ACE_DEBUG((LM_DEBUG, "往配置文件写定值区号错\n")); break;
	 }


	 return RET_OK;
 }




 //=start= Zander
 __i32  CNetTask::do_send_packet(CONetBase * pBase_, __i32 IP_i)  // Zander 2014-10-10  add "IP_i"
 {
	 ACE_GUARD_RETURN(ACE_Thread_Mutex,lockobj,_net_mutex,RET_Fail);
	 ACE_SOCK_Stream *local_curPeer = NULL;

	 if (IP_i > 3)
	 {
		 return RET_Fail;
	 }
	 local_curPeer = &_peer[IP_i];



	 __u8 *p = pBase_->_packetbuff;
	 if (!(p))
	 {
		 return RET_Fail; 
	 }
	 if ((!(local_curPeer)))
	 {
		 return RET_Fail;
	 }
	 if(local_curPeer->get_handle() == ACE_INVALID_HANDLE )
	 {
		 return RET_Fail;
	 }
	 size_t len = -1;
	 int ret =-2;
	 ret= local_curPeer->send_n(pBase_->_packetbuff,pBase_->get_packet_length(),0,&_sendOutTime,&len);
	 if ( ret < 0)
	 {
		 //	 ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] do_send_packet failed\n"));
		 return RET_Fail;
	 }
	 if(len !=pBase_->get_packet_length())
	 {
		 //	ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] do_send_packet failed\n"));
		 return RET_Fail;
	 }

	 return  RET_OK;
 }
 
 // Called when input events occur (e.g., connection or data).
int  CNetTask::handle_input (ACE_HANDLE fd)
 {
	//ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] handle_input() is workeds\n"));
	__i32 ret = RET_OK;

	//记录接受两次数据的时间差

	//RevTime1=GetTickCount();

	////Sleep(500);

	//DWORD dwTimeGap = RevTime1 - RevTime2; // 单位：毫秒
	//printf("时间差：%d\n",dwTimeGap);
	//RevTime2=RevTime1;


	for (int i=0; i<4; i++)
	{
		//printf("recv[%d]\n", i);
		if (fd == ((ACE_SOCK_Stream*)(&_peer[i]))->get_handle())
		{
			//printf("handle input IP_i = %d\n", i);
			//printf("recv[%d] success\n", i);
			do_recv_packet(i);



		}
	}

	return ret;

 }


///用来定时来处理定时发送测试帧，利用ACE事件模式比开线程加sleep方法实现要节省线程资源。
/**
* Called when timer expires.  @a current_time represents the current
* time that the Event_Handler was selected for timeout
* dispatching and @a act is the asynchronous completion token that
* was passed in when <schedule_timer> was invoked.
*/
int CNetTask:: handle_timeout (const ACE_Time_Value &current_time,
							const void *act)
{
	int ret =  -1;
	ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] timer is workeds\n"));

	return RET_OK;

}
//=end= 2014-10-10

 /// Called when an exceptional events occur (e.g., SIGURG).
int  CNetTask::handle_exception (ACE_HANDLE fd)
 {
//	ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] handle_exception()\n"));

	 return 0;
 }



 /// Called when a handle_*() method returns -1 or when the
 /// remove_handler() method is called on an ACE_Reactor.  The
 /// @a close_mask indicates which event has triggered the
 /// handle_close() method callback on a particular @a handle.
int  CNetTask::handle_close (ACE_HANDLE handle,
	 ACE_Reactor_Mask close_mask)
 {

	//ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] %s handle_closed()\n",_netAddr[_curNum].get_host_addr()));

	//2015 GM 
	 int ret;
	 for(int i=0;i<4;i++)
	 {
		disconnect_net(i);
		ret =this->reactor()->remove_handler(_peer[i].get_handle(),close_mask |ACE_Event_Handler::DONT_CALL);
	 }
	 
	
	// int ret =this->reactor()->remove_handler(_curPeer->get_handle(),close_mask |ACE_Event_Handler::DONT_CALL);

	//disconnect_net(_curNum);
	 return 0;
 }

__i32  CNetTask::send_start_link(__u8 nLinkNet, __i32 IP_i)  // Zander 2014-10-10  add "IP_i"
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex,lockobj,_net_mutex,RET_Fail);
	CONetPctl_Link * pctl_ = dynamic_cast<CONetPctl_Link *>(_pctls->get_pctl(0xcd));

	if (IP_i > 3)
	{
		return RET_Fail;
	}


	if ((pctl_))
	{
		//发送链接开始请求帧 
		pctl_->reset_frame();
		pctl_->set_pctl_header(SYNC_HEADER);
		pctl_->set_pctl_dest_addr(_dpuAddr);
		pctl_->set_pctl_source_addr(_clientAddr);
		pctl_->set_protocol_type( ONet_Const::ONET_REAL_TRANSFER_PROTOCOL);
		pctl_->set_link(ONet_Const::FRAM_STYLE_LINK,0,nLinkNet,ONet_Const::LINK_STYLE_START);
		pctl_->create_frame();
		pctl_->create_packet();
		//	pctl_->debug_procotol_data();
		if(!((__i32)(do_send_packet(pctl_, IP_i))))  // Zander 2014-10-10  add "IP_i"
		{
			//ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] DPU  %s Link req is send\n",_netAddr[_curNum].get_host_addr()));
		}

	}
	else
	{
		return RET_Fail;
	}
	return RET_OK;
}
__i32  CNetTask::send_stop_link(__u8 nLinkNet, __i32 IP_i)  // Zander 2014-10-10  add "IP_i"
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex,lockobj,_net_mutex,RET_Fail);
	if (IP_i > 3)
	{
		return RET_Fail;
	}

	CONetPctl_Link * pctl_ = dynamic_cast<CONetPctl_Link *>(_pctls->get_pctl(0xcd));

	if ((pctl_))
	{
		//发送链接开始请求帧
		pctl_->reset_frame();



		pctl_->set_pctl_header(SYNC_HEADER);
		pctl_->set_pctl_dest_addr(_dpuAddr);
		pctl_->set_pctl_source_addr(_clientAddr);
		pctl_->set_protocol_type( ONet_Const::ONET_REAL_TRANSFER_PROTOCOL);
		pctl_->set_link(ONet_Const::FRAM_STYLE_LINK,0,nLinkNet,ONet_Const::LINK_STYLE_STOP);
		pctl_->create_frame();
		pctl_->create_packet();
			pctl_->debug_procotol_data();
		if(!((__i32)(do_send_packet(pctl_, IP_i))))  // Zander 2014-10-10  add "IP_i"
		{
			//ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] DPU  %s Link req is send\n",_netAddr[_curNum].get_host_addr()));
		}
	}
	else
	{
		return RET_Fail;
	}
	return RET_OK;
}
// Zander 2014-10-10  add "IP_i"
//GM 2015.05.06 add "__u8 nLinkNet" 参数
__i32  CNetTask::send_t1_link(__u8 nLinkNet,__i32 IP_i)  
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex,lockobj,_net_mutex,RET_Fail);
	if (IP_i > 3)
	{
		return RET_Fail;
	}

	CONetPctl_Link * pctl_ = dynamic_cast<CONetPctl_Link *>(_pctls->get_pctl(0xcd));
	if ((pctl_))
	{
		//发送T1时间设定帧
		pctl_->reset_frame();
		pctl_->set_pctl_header(SYNC_HEADER);
		pctl_->set_pctl_dest_addr(_dpuAddr);
		pctl_->set_pctl_source_addr(_clientAddr);
		pctl_->set_protocol_type( ONet_Const::ONET_REAL_TRANSFER_PROTOCOL);
		//pctl_->set_link(ONet_Const::FRAM_STYLE_T1,SET_T1_LINK_TIME,ONet_Const::LINK_NET_0);
		pctl_->set_link(ONet_Const::FRAM_STYLE_T1,SET_T1_LINK_TIME,nLinkNet);
		pctl_->create_frame();
		pctl_->create_packet();
		pctl_->debug_procotol_data();

		//GM 2015.04.30
		if(!((__i32)(do_send_packet(pctl_, IP_i))))  // Zander 2014-10-10  add "IP_i"
		{
			//		ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] DPU T1 req is send\n"));

		}

	}
	else
	{
		return RET_Fail;
	}
	return RET_OK;
}
 // Zander 2014-10-10  add "IP_i"
//GM 2015.05.06 add "__u8 nLinkNet" 参数
__i32  CNetTask::send_test_link(__u8 nLinkNet,__i32 IP_i)  // Zander 2014-10-10  add "IP_i"
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex,lockobj,_net_mutex,RET_Fail);
	if (IP_i > 3)
	{
		return RET_Fail;
	}
	CONetPctl_Link * pctl_ = dynamic_cast<CONetPctl_Link *>(_pctls->get_pctl(0xcd));
	if ((pctl_))
	{
		pctl_->reset_frame();
		pctl_->set_pctl_header(SYNC_HEADER);
		pctl_->set_pctl_dest_addr(_dpuAddr);
		pctl_->set_pctl_source_addr(_clientAddr);
		pctl_->set_protocol_type( ONet_Const::ONET_REAL_TRANSFER_PROTOCOL);
		//pctl_->set_link(ONet_Const::FRAM_STYLE_LINK,0,ONet_Const::LINK_NET_0,ONet_Const::LINK_STYLE_TEST);
		pctl_->set_link(ONet_Const::FRAM_STYLE_LINK,0,nLinkNet,ONet_Const::LINK_STYLE_TEST);
		pctl_->create_frame();
		pctl_->create_packet();
		//pctl_->debug_procotol_data();
		
		do_send_packet(pctl_, IP_i);  // Zander 2014-10-10  add "IP_i"

	}
	else
	{

	}
	return RET_OK;
}
__i32  CNetTask::send_read_link(__i32 IP_i)  // Zander 2014-10-10  add "IP_i"
{
	 //ACE_GUARD_RETURN(ACE_Thread_Mutex,lockobj,_net_mutex,RET_Fail);

	if (IP_i > 3)
	{
		return RET_Fail;
	}
	CONetPctl_10H_11H_12H * pctl_ = dynamic_cast<CONetPctl_10H_11H_12H *>(_pctls->get_pctl(0x10));
	if ((pctl_))
	{
		pctl_->set_pctl_dest_addr(_dpuAddr);
		pctl_->set_pctl_source_addr(_clientAddr);
		pctl_->set_call_style(ONet_Const::CMD_DOWN_SUBSCRIBE);
		pctl_->set_call_periods(_refreshTime);
		pctl_->create_frame();
		pctl_->create_packet();
		pctl_->debug_procotol_data();
		do_send_packet(pctl_, IP_i);  // Zander 2014-10-10  add "IP_i"

	}
	else
	{

	}
	return RET_OK;

}
__i32  CNetTask::send_set_vpt_value(__u8 valTaype_,__u8 ioAddr_,__u16 ioCh_,__u8 subCh_, pmc_value_t setValue_)
{
	 //ACE_GUARD_RETURN(ACE_Thread_Mutex,lockobj,_vpt_mutex,RET_Fail);
	CONetPctl_33H_34H_35H_36H_3A * pctl_ = dynamic_cast<CONetPctl_33H_34H_35H_36H_3A *>(_pctls->get_pctl(0x3A));
	if ((pctl_))
	{
		pctl_->set_pctl_dest_addr(_dpuAddr);
		pctl_->set_pctl_source_addr(_clientAddr);
		pctl_->set_control_vpt(_dpuAddr,ioAddr_,valTaype_,ioCh_,subCh_,setValue_);
		pctl_->create_frame();
		pctl_->create_packet();
		pctl_->debug_procotol_data();
		if(!((__i32)(do_send_packet(pctl_, _curNum))))  // Zander 2014-10-10  add "_curNum"
		{
			ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] %s write vpt but reslut unkown\n",_netAddr[_curNum].get_host_addr()));
		}
	}
	else
	{

	}
	return RET_OK;
}
//控制器对时
__i32  CNetTask::send_settime_link(__i32 IP_i)  // Zander 2014-10-10  add "IP_i"
{
	if (IP_i > 3)
	{
		return RET_Fail;
	}
	CONetPctl_78H * pctl_ = dynamic_cast<CONetPctl_78H *>(_pctls->get_pctl(0x78));
	if ((pctl_))
	{
		pctl_->set_pctl_dest_addr(_dpuAddr);
		pctl_->set_pctl_source_addr(_clientAddr);
		pctl_->create_frame();
		pctl_->create_packet();
		pctl_->debug_procotol_data();

		if(!((__i32)(do_send_packet(pctl_, IP_i))))  // Zander 2014-10-10  add "IP_i"
		{
			//ACE_DEBUG((LM_DEBUG,"%D,[%P|%t] %s settime  but reslut unkown\n",_netAddr[_curNum].get_host_addr()));
		}

	}
	else
	{

	}
	return RET_OK;

}



//采用定时器发送链接测试帧,1s发送一次，用定时器可以避免开线程来发送链接测试帧
//定时器使用时不能修改系统时间,这是使用定时器的一个缺陷，
//在此次应用上还是用独立线程
//但已经独立线程的接口
int  CNetTask::open(void * args)		
{
	ACE_Time_Value delay_tv(1);
	int ret = -1;
	
	ACE_Time_Value tv(0,200000);
	if ( _isEnbleTimer== __true)
	{
		ret = reactor()->schedule_timer(this,NULL,delay_tv,tv);	//定时器检查网络连接线程
	}
	else
	{
		//一个控制器一个线程，由于在多控制器下线程较多，重要性也较低，所以线程优先级较低
		// Zander 201-10-10   OLD:ret = activate(0|0|0,1,0);
		ret = activate(0, 2, 0, 0, -1, 0, 0, 0, 0, m_thread_ids);
	}

	return RET_OK;
}
int CNetTask::close()		/**< @brief 关闭 */
{
	_StopHandle.signal();
	ACE_OS::sleep(1);

	if ( _isEnbleTimer== __true)
	{

		reactor()->cancel_timer(this);	//关闭定时器
	}

	// Zander 2014-10-10
	for(int i = 0; i < 4 ; i++)
	{    
		if(((ACE_SOCK_Stream*)(&_peer[i]))->get_handle() != ACE_INVALID_HANDLE )
			reactor()->remove_handler(((ACE_SOCK_Stream*)(&_peer[i]))->get_handle(),ACE_Event_Handler::READ_MASK| ACE_Event_Handler::DONT_CALL);
	}

	//关闭所有的连接
	disconnect_all();

	return RET_OK;


}

//=start= Zander
int  CNetTask::svc()			
{
	//ACE_DEBUG((LM_DEBUG,ACE_TEXT("%D,[%P|%t],CNetTask::svc enter!! \n")));
	//1秒钟测试一次帧
		ACE_Time_Value tv(1,0);
	//处理连接线程
		if (ACE_Thread::self() == m_thread_ids[0])
	{
		while(true)
		{
			if (_StopHandle.wait(&tv, 0) == -1)
			{
				//2015.05.08 GM  
				if (check_connect())
				{
					printf("************_curNum = %d,dpuaddr=%d************\n", _curNum,_dpuAddr);
				} 
				else
				{
					printf("check_connect()return FALSE！！！ _curNum = %d,dpuaddr=%d\n", _curNum,_dpuAddr);
				}



				//check_connect();

				//printf("_curNum = %d,dpuaddr=%d\n", _curNum,_dpuAddr);

				m_st_num++;      // 

			}else
			{
				break;
			}
		}

	} 
	else if (ACE_Thread::self() == m_thread_ids[1])
	{
		while(true)
		{
			if (_StopHandle.wait(&tv, 0) == -1)
			{
				//
				for (int i=0; i!=4; i++)
				{
					if ( (_isOnline[i] == __false) && (_isUsed[i] == __true) )
					{
						connect_net(i);
					} 
					else if ((_isOnline[i] == __true) && (_isUsed[i] == __true)&&(_islinkStartAck[i]==__true))
					{
						_conCount[i]++;
						if (_conCount[i] > 3)
						{
							//GM 2015.05.04
							disconnect_net(i);
							//_dbPtr->_netStatus[i] = __false;
							_conCount[i] = 0;
						}

						
					}
				}

			}else
			{
				break;
			}
		}
	}

	ACE_DEBUG((LM_DEBUG,ACE_TEXT("CNetTask::svc quit!! \n")));

	return RET_OK;
}
//=end= 2014-10-10

//=start= Zander
void CNetTask::clearIOChannelNum()
{
	if (_dbPtr)
	{
		__i32 i=0;
		for (i=0; i<IOCARD_MAX_SIZE; i++)
		{
			if(_dbPtr->ioCards[i].channels)
			{
				ACE_ASSERT(_dbPtr->ioCards[i].ioCurNum > 0);
				_dbPtr->ioCards[i].ioCurNum = DPU_EEROR_IO_NUM;
				
			}
		}
	}
}
//=end= 2013-7-24

// 读取实时库中网络设备中的网络状态标签名
__bool CNetTask::init_net_tag()
{
	RTK_CURSOR hNode;
	RTK_CURSOR hGroup;
	RTK_CURSOR hTag;
	PCRTK_TAG  pTag;
	//TAG_NAME tn;
	//CNodeName nodeKey("@LOCAL");

	hNode = open_local_node();
	if(hNode){
		hGroup = cursor_open_first_subitem(hNode);
		while(!cursor_is_end(hGroup)){
			hTag = cursor_open_first_subitem(hGroup);
			while(!cursor_is_end(hTag)){
				pTag = (PCRTK_TAG)cursor_get_item(hTag);
				// 取网络状态标签名
				get_net_tn(pTag);
				cursor_move_next(hTag);
			}
			close_handle(hTag);
			cursor_move_next(hGroup);
		}
		close_handle(hGroup);
	}

	return __true;
	
}

// 判断标签地址是否为控制器IP地址，是则取出该标签名
__bool CNetTask::get_net_tn(PCRTK_TAG pTag)
{
	int idx;
	string str;

	if (!pTag->s.Device.Data[0])
	{
		return __false;
	}
	
	// 空地址
	if (!pTag->s.Address[0])
	{
		return __false;
	}

	// skip blanks and Tabs
	str = pTag->s.Address;
	idx = str.find_first_not_of("\t\n");
	if(idx == string::npos) {
		return __false;
	}else if(idx != 0){
		str=str.substr(idx);
	}

	for (int i=0; i!=4; i++)
	{
		// 
		if (!strcmpi(str.c_str(), _netAddr[i].get_host_addr()))
		{						
			tn_net[i].node = pTag->node;
			tn_net[i].sname.group = pTag->group;
			tn_net[i].sname.tag = pTag->key;
			break;
		}
	}

	return __true;
}

// 若多次发现网络状态标签值为on, 则断开网络标签值对应的控制器网口连接
__bool CNetTask::check_net_by_tag()
{
	PRTK_TAG pTag = NULL;
	
	for (int i=0; i!=4; i++)
	{
		pTag = query_tag_f(&tn_net[i]);
		if (!pTag)
		{
			continue;
		}

		// 添加了网络标签，但没添加网络IO设备的情况
		// Zander, 2014-2-27
		if (!(pTag->d.Value.Flags & TF_Valid))
		{
			continue;
		}

		// 网络标签值为on,则该控制器网口曾经连接上过
		if (pTag->d.Value.Value.b && m_is_linked[i])
		{
			if (m_ping_count[i] >= PING_FAILED_NUM)
			{
				m_ping_count[i] = 0;
				//disconnect_net(i);
				//断开网络
				_isOnline[i] = __false;
				_peer[i].close_writer();
				_peer[i].close();

			}else
			{
				m_ping_count[i]++;
			}
		}else
		{
			m_ping_count[i] = 0;
		}
	}
	
	return __true;
}


	/** @} */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       