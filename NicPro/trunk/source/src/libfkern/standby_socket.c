#if defined(INCLUDE_STANDBY_SOCKET)

struct cmd_socket_t{
	f8_u32 port;
	int (*proc)(struct cmd_socket_t * sock, struct peer_cmd_t * c, int len);
	int params[4];
	RTK_LIST_ENTRY l;
};

#define MAX_CMD_SOCKS 8

struct cmd_socket_t cmdSocks[MAX_CMD_SOCKS];
RTK_LIST_ENTRY freeSocks, activeSocks;

struct cmd_socket_t * createCmdSocket(f8_u32 port)
{
	RTK_LIST_ENTRY * l;
	struct cmd_socket_t * c;
	int cookie;
	if(RtkIsListEmpty(&freeSocks)){
		return 0;
	}
	cookie=peer_lock();
	l=freeSocks.Flink;
	while(l != &freeSocks){
		c=RTK_CONTAINING_RECORD(l,struct cmd_socket_t,l);
		if(c->port == port){
			peer_unlock(cookie);
			return 0;
		}
		l=l->Flink;
	}
	/* remove from free list */
	l=RtkRemoveHeadList(&freeSocks);
	c=RTK_CONTAINING_RECORD(l,struct cmd_socket_t,l);
	/* add to active list */
	RtkInsertTailList(&activeSocks, l);
	c->port=port;
	peer_unlock(cookie);
	return c;
}

void freeCmdSocket(struct cmd_socket_t * c)
{
	int cookie;
	cookie=peer_lock();
	RtkRemoveEntryList(&c->l);
	RtkInsertHeadList(&freeSocks, &c->l);
	peer_unlock(cookie);
}

void peer_ping()
{
	struct peer_cmd_t tp;
	static int count;
	int lk;
	struct cmd_socket_t * c;
	c=createCmdSocket(0);
	if(g_kernel.status.state == KERN_S_RUNNING && get_state() == hsp_s_idle){
		memset(&tp, 0, sizeof tp);
		tp.cmd = Pr_query_status;
		tp.time = ke_get_time();
		lk = peer_lock();
		peer_write(&tp, sizeof tp);
		peer_unlock(lk);
	}	
}

#endif

