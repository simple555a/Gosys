
#include<memory.h>
#include<stdio.h>
#include<stdlib.h>
#include <f8.h>
#include <f8_kserver.h>
#include<string>
#include<f8_fbdar.h>
#include <f8_kproxy.h>
using namespace std;
#pragma comment(lib,"libkserver.lib")
#pragma comment(lib,"libfbdar.lib")
#pragma comment(lib,"librtk.lib")
#pragma comment(lib,"libkproxy.lib")
#pragma comment(lib,"libutils.lib")
#pragma comment(lib,"librtkshell.lib")
int main(int c,char **v)
{
	kproxy_init(-1);
	kproxy_shell_ex("online localhost",2000,0);

	return 0;

}