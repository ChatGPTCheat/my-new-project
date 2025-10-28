#include <algorithm>
#include <arpa/inet.h>
#include <cctype>
#include <chrono>
#include <codecvt>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <malloc.h>
#include <map>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;
int driverchoose;

class c_driver {
private:
int has_upper = 0;
int has_lower = 0;
int has_symbol = 0;
int has_digit = 0;
int fd;
pid_t pid;

typedef struct _COPY_MEMORY {
pid_t pid;
uintptr_t addr;
void* buffer;
size_t size;
} COPY_MEMORY, *PCOPY_MEMORY;

typedef struct _MODULE_BASE {
pid_t pid;
char* name;
uintptr_t base;
} MODULE_BASE, *PMODULE_BASE;

enum OPERATIONS {
OP_INIT_KEY = 0x800,
OP_READ_MEM = 0x801,
OP_WRITE_MEM = 0x802,
OP_MODULE_BASE = 0x803,
OP_HIDE_PROCESS = 0x804,
};

int symbol_file(const char *filename) {
int length = strlen(filename);
for (int i = 0; i < length; i++) {
if (islower(filename[i])) {
has_lower = 1;
} else if (isupper(filename[i])) {
has_upper = 1;
} else if (ispunct(filename[i])) {
has_symbol = 1;
} else if (isdigit(filename[i])) {
has_digit = 1;
}
}
return has_lower && !has_upper && !has_symbol && !has_digit;
}

void createDriverNode(char *path, int major_number, int minor_number) {
string command = "mknod " + string(path) + " c " + to_string(major_number) + " " + to_string(minor_number);
system(command.c_str());
}

void removeDeviceNode(char* path) {
unlink(path);
}

char* execCom(const char* shell)
{
FILE *fp = popen(shell, "r");
if (fp == NULL)
{
perror("popen失败了");
return NULL;
}
char buffer[256];
char *result = (char *)malloc(1500);
result[0] = '\0';
while (fgets(buffer, sizeof(buffer), fp) != NULL)
{
strcat(result, buffer);
}
pclose(fp);
return result;
}

int findFirstMatchingPath(const char *path, regex_t *regex, char *result)
{
DIR *dir;
struct dirent *entry;
if ((dir = opendir(path)) != NULL)
{
while ((entry = readdir(dir)) != NULL)
{
char fullpath[1024];
snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
if (entry->d_type == DT_LNK)
{
char linkpath[1024];
ssize_t len = readlink(fullpath, linkpath, sizeof(linkpath) - 1);
if (len != -1)
{
linkpath[len] = '\0';
if (regexec(regex, linkpath, 0, NULL, 0) == 0)
{
strcpy(result, fullpath);
closedir(dir);
return 1;
}
}
else
{
perror("readlink");
}
}
}
closedir(dir);
}
else
{
perror("Unable to open directory");
}
return 0;
}

int getMEN(char *path)
{
FILE *file = fopen(path, "r");
int zero, neko;
if (file == NULL)
{
return 0;
}
char line[256];
 while (fgets(line, sizeof(line), file))
{
if (sscanf(line, "%d:%d", &neko, &zero) == 2 && zero == 0)
{
fclose(file);
return neko;
}
}
fclose(file);
return 0;
}

int has_digit1(char *str)
{
int i, len;
len = strlen(str);
for (i = 0; i < len; i++)
{
if (isdigit((unsigned char)str[i]))
{
return 1;
}
}
return 0;
}

bool StrAlpha(const char *str){
for(int i = 0;i<6;i++){
if(!isalpha(str[i])){
return false;
}
}
return true;
}

string deviceName, deviceNumber, devName;
char* 自研驱动() {
FILE* file = fopen("/proc/devices", "r");
if (file == nullptr) {
cerr << "无法打开/proc/devices文件\nFailed to open /proc/devices" << endl;
exit(0);
}
char line[80];
while (fgets(line, sizeof(line), file) != nullptr) {
istringstream iss(line);
iss >> deviceNumber >> deviceName;
if (deviceName.length() != 30) continue;
devName = "/dev/" + deviceName;
unlink(devName.c_str());
system(("mknod " + devName + " c " + deviceNumber + " 0").c_str());
}
fclose(file);
return strdup(devName.c_str());
}

char* 自研驱动2号(const char* directory_path) {
DIR* dir = opendir(directory_path);
if (dir == NULL) 
{
perror("无法打开目录");
return NULL;
}
struct dirent* entry;
while ((entry = readdir(dir)) != NULL)
{
if (strlen(entry->d_name) == 30)
{
closedir(dir);
return strdup(entry->d_name); 
}
}
closedir(dir);
return NULL;
}

char *GT驱动() {
const char *dev_path = "/dev";
DIR *dir = opendir(dev_path);
if (dir == NULL){
printf("无法打开/dev目录\n");
return NULL;
}
char *files[] = { "wanbai", "CheckMe", "Ckanri", "lanran","video188"};
struct dirent *entry;
char *file_path = NULL;
while ((entry = readdir(dir)) != NULL) {
if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
continue;
}
size_t path_length = strlen(dev_path) + strlen(entry->d_name) + 2;
file_path = (char *)malloc(path_length);
snprintf(file_path, path_length, "%s/%s", dev_path, entry->d_name);
for (int i = 0; i < 5; i++) {
if (strcmp(entry->d_name, files[i]) == 0) {
printf("驱动文件：%s\n", file_path);
closedir(dir);
return file_path;
}
}
struct stat file_info;
if (stat(file_path, &file_info) < 0) {
free(file_path);
file_path = NULL;
continue;
}
if (strstr(entry->d_name, "gpiochip") != NULL) {
free(file_path);
file_path = NULL;
continue;
}
if ((S_ISCHR(file_info.st_mode) || S_ISBLK(file_info.st_mode))
&& strchr(entry->d_name, '_') == NULL && strchr(entry->d_name, '-') == NULL && strchr(entry->d_name, ':') == NULL) {
if (strcmp(entry->d_name, "stdin") == 0 || strcmp(entry->d_name, "stdout") == 0
|| strcmp(entry->d_name, "stderr") == 0) {
free(file_path);
file_path = NULL;
continue;
}
size_t file_name_length = strlen(entry->d_name);
time_t current_time;
time(&current_time);
int current_year = localtime(&current_time)->tm_year + 1900;
int file_year = localtime(&file_info.st_ctime)->tm_year + 1900;
if (file_year <= 1980) {
free(file_path);
file_path = NULL;
continue;
}
time_t atime = file_info.st_atime;
time_t ctime = file_info.st_ctime;
if ((atime == ctime)) {
if ((file_info.st_mode & S_IFMT) == 8192 && file_info.st_size == 0
&& file_info.st_gid == 0 && file_info.st_uid == 0 && file_name_length <= 9) {
printf("驱动文件：%s\n", file_path);
closedir(dir);
return file_path;
}
}
}
free(file_path);
file_path = NULL;
}
closedir(dir);
return NULL;
}

int QX驱动(){
DIR *dir;
struct dirent *ptr;
struct stat info;
dir = opendir("/proc");
ssize_t len;
char path[256];
char buffer[256];
char fd_path[256];
char fd_buffer[256];
char dev_path[256];
char data_path[128];
int ID;
int PPID;
auto start = std::chrono::high_resolution_clock::now();
while((ptr = readdir(dir)) != NULL){
if(ptr->d_type == DT_DIR){
sprintf(buffer,"/proc/%d/exe",atoi(ptr->d_name));
len = readlink(buffer, path, sizeof(path) - 1);
if (len != -1) path[len] = '\0';
char* stres = strrchr(path,'(deleted)');
if(stres != NULL){
sscanf(path, "/data/%s", &data_path);
if(StrAlpha(data_path)){
sscanf(buffer,"/proc/%d/exe",&PPID);
for(int i = 3;i<5;i++){
sprintf(fd_path,"/proc/%d/fd/%d",PPID,i);
len = readlink(fd_path, fd_buffer, sizeof(fd_buffer) - 1);
char* stress = strrchr(fd_buffer,'(deleted)');
if(stress != NULL){
int fd_file = open(fd_path, O_RDONLY);
if (fd_file == -1) {
close(fd_file);
return EXIT_FAILURE;
}
if (fstat(fd_file, &info) == -1) {
close(fd_file);
return 0;
}
sscanf(fd_buffer,"%s (deleted)", dev_path);
if(access(dev_path,F_OK) == 0){
ID = open(dev_path,O_RDWR);
if(ID != -1){
if(unlink(dev_path) == 0) {
auto end = std::chrono::high_resolution_clock::now();
float elapsed_ns = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
return ID;
}
}
} else {
mode_t mode = S_IFCHR | 0666;
dev_t dev = makedev(major(info.st_rdev), minor(info.st_rdev));
if(mknod(dev_path, mode, dev) != -1){
}
ID = open(dev_path,O_RDWR);
if(ID != -1){
if(unlink(dev_path) != -1){
auto end = std::chrono::high_resolution_clock::now();
float elapsed_ns = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
return ID;
}
}
}
close(fd_file);
break;
}
}
break;
}
}
}
}
closedir(dir);
return -1;
}

public:
c_driver() : fd(-1), pid(0) {
int driverchoose;
char* deviceName;
deviceName = 自研驱动();
fd = open(deviceName, O_RDWR);
if(fd==-1)
{
printf("自研驱动读取失败 正在读取QX驱动\n");
fd = QX驱动();
if(fd==-1)
{
printf("QX读取失败 正在读取自研驱动2号\n");
char *Fname = 自研驱动2号("/dev/");
sprintf(deviceName,"/dev/%s",Fname);
fd = open(deviceName, O_RDWR);
if(fd==-1)
{
printf("自研驱动2号读取失败 正在读取GT驱动\n");
deviceName = GT驱动();
fd = open(deviceName, O_RDWR);
if(fd==-1)
{
printf("GT驱动读取失败 请刷入其他驱动\n");
free(deviceName);
exit(0);
}
}
}
}
unlink(devName.c_str());
free(deviceName);
}
~c_driver()
{
if (fd > 0)
close(fd);
}

void 初始化进程(pid_t pid)
{
this->pid = pid;
}

bool read(uintptr_t addr, void *buffer, size_t size)
{
COPY_MEMORY cm;
cm.pid = this->pid;
cm.addr = addr;
cm.buffer = buffer;
cm.size = size;
if (ioctl(fd, OP_READ_MEM, &cm) != 0)
{
return false;
}
return true;
}

bool write(uintptr_t addr, void *buffer, size_t size)
{
COPY_MEMORY cm;
cm.pid = this->pid;
cm.addr = addr;
cm.buffer = buffer;
cm.size = size;
if (ioctl(fd, OP_WRITE_MEM, &cm) != 0)
{
return false;
}
return true;
}

template < typename T > T read(uintptr_t addr)
{
T value;
this->read(addr, &value, sizeof(T));
return value;
}

template < typename T > bool write(uintptr_t addr, T value)
{
return this->write(addr, &value, sizeof(T));
}

uintptr_t 读取指针(uintptr_t 地址){
uintptr_t value;
if (地址 < 0xFFFFFFFF) {
this->read(地址,&value,4);
} else {
this->read(地址,&value,8);
value = value & 0xFFFFFFFFFFFF;
}
return value;
}

bool 写入指针(uintptr_t 地址, uintptr_t 指针数值) {
uintptr_t value;
return this->write(地址, &指针数值, sizeof(value));
}

int 读取整数(uintptr_t 地址){
int value;
this->read(地址,&value,4);
return value;
}

bool 写入整数(uintptr_t 地址, int 整数数值) {
int value;
return this->write(地址, &整数数值, sizeof(value));
}

float 读取浮点数(uintptr_t 地址){
float value;
this->read(地址,&value,4);
return value;
}

bool 写入浮点数(uintptr_t 地址, float 浮点数值) {
float value;
return this->write(地址, &浮点数值, sizeof(value));
}

char 读取字符类(uintptr_t 地址){
char value;
this->read(地址,&value,sizeof(value));
return value;
}

bool 写入字符类(uintptr_t 地址, char 字符数值) {
char value;
return this->write(地址, &字符数值, sizeof(value));
}

uint8_t 读取短字符(uintptr_t 地址){
uint8_t value;
this->read(地址,&value,sizeof(value));
return value;
}

bool 写入短字符(uintptr_t 地址, uint8_t 短字符数值) {
uint8_t value;
return this->write(地址, &短字符数值, sizeof(value));
}

uintptr_t 获取基址头(char* name) {
MODULE_BASE mb;
char buf[0x100];
strcpy(buf,name);
mb.pid = this->pid;
mb.name = buf;
if (ioctl(fd, OP_MODULE_BASE, &mb) != 0) {
return 0;
}
return mb.base;
}

pid_t 获取进程ID(char *name)
{
FILE *fp;
pid_t pid;
char cmd[0x100] = "pidof ";
strcat(cmd, name);
fp = popen(cmd, "r");
fscanf(fp, "%d", &pid);
pclose(fp);
return pid;
}

void 隐藏进程() {
ioctl(fd, OP_HIDE_PROCESS);
}

};

static c_driver *驱动 = new c_driver();