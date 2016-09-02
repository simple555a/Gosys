#include "precomp.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)


CSerialPortItems::CSerialPortItems(PCNODE_KEY key)
{
    this->key = *key;
    m_sCaption = "串口访问服务";
    m_iIcon = 11;
    m_SearchKey = "SPORTS";
    m_ClassName = "串口设置";
    m_PropertyPage = framSerialPorts;
}

void CSerialPortItems::ReloadContents()
{
    int i;
    RemoveChildren();
    CSerialPortItem *item;
    for(i=0; i<16; i++){
        item = new CSerialPortItem(&key, i+1);
        AddChild(0, item);
    }
}

CSerialPortItem::CSerialPortItem(PCNODE_KEY node, int i)
{
    m_PortNo = i;
    m_iIcon = 11;
    m_sCaption = "串口 ";
    m_sCaption += i;
    m_bExpandable = false;
    key = *node;

    if(!framSerialPort){
        framSerialPort = new TframSerialPort(0);
    }
    m_PropertyPage = framSerialPort;
    m_SearchKey = m_sCaption;
    m_ClassName = "串口设置";

    m_baud = 57600;
    m_databits = 8;
    m_stopbits = 1;
    m_checkmode = 'm';

    m_SupportedActions.insert(ca_Edit);
}

void CSerialPortItem::ReloadContents()
{
    char buf[32];
    char setting[1024];
    CArgs args;
    sprintf(buf, "PORT%d", m_PortNo);
    if(!get_config_string(&key, "ports", buf, setting, sizeof(setting), 500)){
        return;
    }
    utils_debug("Port%d on %s: %s\n", m_PortNo, (char*)CNodeName(key), setting);
    parse_arg_ex(setting, &args, ",");
    if(args.argc != 4){
        return;
    }
    m_baud = atoi(args.argv[0]);
    if( m_baud != 4800 &&
        m_baud != 9600 &&
        m_baud != 19200 &&
        m_baud != 38400 &&
        m_baud != 57600 &&
        m_baud != 115200
    ){
        m_baud = 57600;
    }

    m_checkmode = *args.argv[1];
    if(!strchr("mbeon", m_checkmode) ){
        m_checkmode = 'm';
    }

    m_databits = atoi(args.argv[2]);
    if(m_databits < 6 || m_databits>8){
        m_databits = 8;
    }

    if( !strcmp(args.argv[3], "1") ){
        m_stopbits = 1;
    }else if(!strcmp(args.argv[3], "2") ){
        m_stopbits = 2;
    }else if(!strcmp(args.argv[3], "1.5") ){
        m_stopbits = 3;
    }else{
        m_stopbits = 1;
    }
}

void CSerialPortItem::UI_UpdateView()
{
    TframSerialPort *frm;
    frm = (TframSerialPort*)m_PropertyPage;
    frm->Tag = (int)this;
    frm->UpdataView();
}

void CSerialPortItem::Save()
{
    AnsiString s;
    TframSerialPort *frm;
    frm = (TframSerialPort*)m_PropertyPage;
    frm->Tag = (int)this;
    s = frm->Validate();
    char buf[32];
    sprintf(buf, "port%d", m_PortNo);
    if(set_config_string(&key, "ports", buf, s.c_str(), 500)){
        ReloadContents();
    }
}
