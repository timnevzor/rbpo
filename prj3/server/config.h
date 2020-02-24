#pragma once

#define SYSLOG

#define ENCRYPTED_FLAG "41baaa356aeb9c536e2f64551d5461a338796ff69f24faf7"
#define THREAD_LIMIT 300
#define BUFLEN 1024
#define REQ_STRING "get flag\n"
#define PORT 7987
#define FLAG_MESSAGE string("You got the encrypted flag: ") + ENCRYPTED_FLAG + "\n"
#define CONFIG "server-pervak.toml"
//#define CONFIG "/etc/server-pervak.toml"
#define SUBNET_205 "172.16.41.0"
#define SUBNET_216 "172.16.41.64"
#define SUBNET_301 "172.16.41.128"
#define SUBNET_308 "172.16.41.192"
#define MASK "255.255.255.192" 

// standard libraries
#include <stdexcept>
#include <chrono>
#include <map>
#include <list>
#include <vector>
//#include <cstdlib>
//#include <cstring>
#include <iostream>
//#include <sstream>
#include <string>
#include <thread>
#include <atomic>
#include <ctime>

// POSIX libraries
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifdef SYSLOG
#include <syslog.h>
#endif

//toml library
#include "cpptoml.h"

using namespace std;
