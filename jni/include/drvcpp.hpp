#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
//#include "过缺页.h"
#include <sys/utsname.h> 
//#include "数组解密.h"

#include <sys/ptrace.h>
#include <stdint.h>

class c_driver {
	private:
	int has_upper = 0;
	int has_lower = 0;
	int has_symbol = 0;
	int has_digit = 0;
	int fd=-1;
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

	struct process {
    	pid_t process_pid;
		char process_comm[15];
	};

	enum OPERATIONS {
    	OP_INIT_KEY = 0x800,
    	OP_READ_MEM = 0x801,
    	OP_WRITE_MEM = 0x802,
    	OP_MODULE_BASE = 0x803,
    	OP_HIDE_PROCESS = 0x804,
    	OP_PID_HIDE_PROCESS = 0x805,
    	OP_GET_PROCESS_PID = 0x806
	};
#define 驱动路径 "/dev/"


int symbol_file(const char *filename) {
		//判断文件名是否含小写并且不含大写不含数字不含符号
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
char* execCom(const char* shell)
{
    FILE *fp = popen(shell, "r");

    if (fp == NULL)
    {
        perror("popen failed");
        return NULL;
    }

    char buffer[256];
    char *result = (char *)malloc(1000); // allocate memory for the result string
    result[0] = '\0';                  // initialize as an empty string

    // Read and append output of the first command to result
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        strcat(result, buffer);
    }
    pclose(fp);
    return result;
}

// 修复createDriverNode和removeDeviceNode函数
void createDriverNode(char *path, int major_number, int minor_number)
{
    std::string command = "mknod " + std::string(path) + " c " + std::to_string(major_number) + " " + std::to_string(minor_number);
    system(command.c_str());
   // printf("[-] 创建 %s", path);
}

// 删除驱动节点
// 新的函数，用于删除设备节点
void removeDeviceNode(char* path) {
        // printf("%s\n",path);
        if (unlink(path) == 0) {
       //     printf("[-] 已删除设备节点\n");
             // cerr << "已删除设备节点：" << devicePath << endl;
        } else {
            
        }
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
	
public:
  char *driver_path() {
    struct dirent *de;
    DIR *dr = opendir("/proc");
    char *device_path = NULL;

    if (dr == NULL) {
      printf("Could not open /proc directory");
      return NULL;
    }

    while ((de = readdir(dr)) != NULL) {
      if (strlen(de->d_name) != 6 || strcmp(de->d_name, "aputag") == 0 || strcmp(de->d_name, "asound") == 0 || strcmp(de->d_name, "clkdbg") == 0 || strcmp(de->d_name, "crypto") == 0 || strcmp(de->d_name, "driver") == 0 || strcmp(de->d_name, "mounts") == 0 || strcmp(de->d_name, "pidmap") == 0) {
        continue;
      }
      int is_valid = 1;
      for (int i = 0; i < 6; i++) {
        if (!isalnum(de->d_name[i])) {
          is_valid = 0;
          break;
        }
      }
        if (is_valid) {
            device_path = (char*)malloc(11 + strlen(de->d_name));
            sprintf(device_path, "/proc/%s", de->d_name);
            struct stat sb;
            if (stat(device_path, &sb) == 0 && S_ISREG(sb.st_mode)) {
                break;
            } else {
                free(device_path);
                device_path = NULL;
            }
        }
    }
    puts(device_path);
    closedir(dr);
    return device_path;
  }	
  char* qx10()
	{
    	const char* command = "dir=$(ls -l /proc/*/exe 2>/dev/null | grep -E '/data/[^/]* \\(deleted\\)' | sed 's/ /\\n/g' | grep '/proc' | sed 's/\\/[^/]*$//g');if [[ \"$dir\" ]]; then sbwj=$(head -n 1 \"$dir/comm\");open_file=\"\";for file in \"$dir\"/fd/*; do link=$(readlink \"$file\");if [[ \"$link\" == \"/dev/$sbwj (deleted)\" ]]; then open_file=\"$file\";break;fi;done;if [[ -n \"$open_file\" ]]; then nhjd=$(echo \"$open_file\");sbid=$(ls -L -l \"$nhjd\" | sed 's/\\([^,]*\\).*/\\1/' | sed 's/.*root //');echo \"/dev/$sbwj\";rm -rf \"/dev/$sbwj\";mknod \"/dev/$sbwj\" c \"$sbid\" 0;fi;fi;";
    	FILE* file = popen(command, "r");
    	if (file == NULL) {
        	return NULL;
    	}
    	static char result[512];
    	if (fgets(result, sizeof(result), file) == NULL) {
			return NULL;
		}
    	pclose(file);
        result[strlen(result)-1] = '\0';
		return result;
    }
	
  
    c_driver(){
		printf("1.QX\n2.GT\n");
	printf("\nDriver selection(1/2)：");
int qd;
scanf("%d",&qd);

	if(qd==1){	
	char *dev_path3 = qx10();
		if (dev_path3 != NULL) {
			fd = open(dev_path3, O_RDWR);
			if (fd>0){
				printf("\nHidden driver：%s\n\n", dev_path3);
				unlink(dev_path3);
			}
		}
	}
	if (qd==2){
    char *device_name = driver_path();
    fd = open(device_name, O_RDWR);
    if (fd == -1) {
      printf("[-] open driver failed\n");
      free(device_name);
      exit(0);
    }
    free(device_name);
				
			}
	}
~c_driver(){
if (fd > 0)
close(fd);
}

	void initialize(pid_t pid)
	{
		this->pid = pid;
	}

    bool initKey(const char* key) {
        if (ioctl(fd, OP_INIT_KEY, key) != 0) {
            return false;
        }
        return true;
    }

    bool read(uintptr_t addr, void* buffer, size_t size) {
        COPY_MEMORY cm;
        cm.pid = this->pid;
        cm.addr = addr;
        cm.buffer = buffer;
        cm.size = size;

        if (ioctl(fd, OP_READ_MEM, &cm) != 0) {
            return false;
        }
        return true;
    }

    bool write(uintptr_t addr, void* buffer, size_t size) {
        COPY_MEMORY cm;
        cm.pid = this->pid;
        cm.addr = addr;
        cm.buffer = buffer;
        cm.size = size;

        if (ioctl(fd, OP_WRITE_MEM, &cm) != 0) {
            return false;
        }
        return true;
    }

template <typename T>
    T read(uintptr_t addr) {
        T res;
        if (this->read(addr, &res, sizeof(T)))
            return res;
        return {};
        
    }
    
    template <typename T>
    bool write(uintptr_t addr, T value) {
        return this->write(addr, &value, sizeof(T));
    }



    
    
    
    
    
    
    
    
    
    
template <class T> T WriteAddress(long int addr, T value)
    {
    char lj[128];
    sprintf(lj, "/proc/%d/mem", pid);
    long int handle = open(lj, O_RDWR | O_SYNC);
    pwrite64(handle, &value, sizeof(T), addr);
    close(handle);
    return 0;
    }
    

   uintptr_t getModuleBase(char* name) {
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
    
    void hide_process() {
	    ioctl(fd, OP_HIDE_PROCESS);
	}
	
};
static c_driver* driver = new c_driver();