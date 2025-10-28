#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <mutex>
#include <regex.h>
class c_driver {
private:
  int has_upper = 0;
  int has_lower = 0;
  int has_symbol = 0;
  int has_digit = 0;
     std::mutex mutex[8];
  int fd;
  pid_t pid;

  typedef struct _COPY_MEMORY {
    pid_t pid;
    uintptr_t addr;
    void *buffer;
    size_t size;
  } COPY_MEMORY, *PCOPY_MEMORY;

  typedef struct _MODULE_BASE {
    pid_t pid;
    char *name;
    uintptr_t base;
  } MODULE_BASE, *PMODULE_BASE;


  enum OPERATIONS {
    OP_INIT_KEY = 0x800,
    OP_READ_MEM = 0x801,
    OP_WRITE_MEM = 0x802,
    OP_MODULE_BASE = 0x803
  };

  int symbol_file(const char *filename) {
    // 判断文件名是否含小写并且不含大写不含数字不含符号
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
  
  
  char *driver_path() {
    struct dirent *de;
    DIR *dr = opendir("/proc");
    char *device_path = NULL;

    if (dr == NULL) {
      printf("Could not open /proc directory");
      return NULL;
    }



    while ((de = readdir(dr)) != NULL) {
      if (strlen(de->d_name) != 6 || strcmp(de->d_name, "NVTSPI") == 0 || strcmp(de->d_name, "ccci_log") == 0 || strcmp(de->d_name, "aputag") == 0 || strcmp(de->d_name, "asound") == 0 || strcmp(de->d_name, "clkdbg") == 0 || strcmp(de->d_name, "crypto") == 0 || strcmp(de->d_name, "modules") == 0 || strcmp(de->d_name, "mounts") == 0 || strcmp(de->d_name, "pidmap") == 0 || strcmp(de->d_name, "phoenix") == 0 || strcmp(de->d_name, "uptime") == 0 || strcmp(de->d_name, "vmstat") == 0) {
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
  
   char *find_driver_path() {
    // 打开目录
		const char *dev_path = "/dev";
		DIR *dir = opendir(dev_path);
		if (dir == NULL){
			//printf("无法打开/dev目录\n");
			return NULL;
		}

		char *files[] = { "wanbai", "CheckMe", "Ckanri", "lanran","video188"};
		struct dirent *entry;
		char *file_path = NULL;
		while ((entry = readdir(dir)) != NULL) {
			// 跳过当前目录和上级目录
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

			// 获取文件stat结构
			struct stat file_info;
			if (stat(file_path, &file_info) < 0) {
				free(file_path);
				file_path = NULL;
				continue;
			}

			// 跳过gpio接口
			if (strstr(entry->d_name, "gpiochip") != NULL) {
				free(file_path);
				file_path = NULL;
				continue;
			}

			// 检查是否为驱动文件
			if ((S_ISCHR(file_info.st_mode) || S_ISBLK(file_info.st_mode))
				&& strchr(entry->d_name, '_') == NULL && strchr(entry->d_name, '-') == NULL && strchr(entry->d_name, ':') == NULL) {
				// 过滤标准输入输出
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
				//跳过1980年前的文件
				if (file_year <= 1980) {
					free(file_path);
					file_path = NULL;
					continue;
				}
				
				time_t atime = file_info.st_atime;
				time_t ctime = file_info.st_ctime;
				// 检查最近访问时间和修改时间是否一致并且文件名是否是symbol文件
				if ((atime == ctime)/* && symbol_file(entry->d_name)*/) {
					//检查mode权限类型是否为S_IFREG(普通文件)和大小还有gid和uid是否为0(root)并且文件名称长度在7位或7位以下
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
 
  
  
  
public:
  // 构造函数：初始化驱动
  c_driver() {
 int isll=1;
    printf("1: rt dev 2: rt proc\n");
    scanf("%d", &isll); 
    char *device_name;
    if(isll==1)
  device_name= find_driver_path();
  else
  device_name= driver_path();
    if (!device_name) {
      fprintf(stderr, "File of driver not found \n");
      exit(EXIT_FAILURE);
    }

    fd = open(device_name, O_RDWR);
    free(device_name);

    if (fd == -1) {
   //   perror("[-] 链接驱动失败");
     exit(EXIT_FAILURE);
    }
  }

	~c_driver() {
		//wont be called
		if (fd > 0)
			close(fd);
	}

	void initialize(pid_t pid) { 
	        std::lock_guard<std::mutex> lock(mutex[0]);
	this->pid = pid;
	 }

  bool init_key(char *key) {
        //  std::lock_guard<std::mutex> lock(mutex[0]);
    char buf[0x100];
    strcpy(buf, key);
    if (ioctl(fd, OP_INIT_KEY, buf) != 0) {
      return false;
    }
    return true;
  }

  bool read(uintptr_t addr, void *buffer, size_t size) {
          std::lock_guard<std::mutex> lock(mutex[1]);
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

  bool write(uintptr_t addr, void *buffer, size_t size) {
          std::lock_guard<std::mutex> lock(mutex[2]);
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

  template <typename T> T read(uintptr_t addr) {
          std::lock_guard<std::mutex> lock(mutex[3]);
    T res;
    if (this->read(addr, &res, sizeof(T)))
      return res;
    return {};
  }

  template <typename T> bool write(uintptr_t addr, T value) {
         std::lock_guard<std::mutex> lock(mutex[4]);
    return this->write(addr, &value, sizeof(T));
  }

  uintptr_t get_module_base(char *name) {
          std::lock_guard<std::mutex> lock(mutex[5]);
    MODULE_BASE mb;
    char buf[0x100];
    strcpy(buf, name);
    mb.pid = this->pid;
    mb.name = buf;

    if (ioctl(fd, OP_MODULE_BASE, &mb) != 0) {
      return 0;
    }
    return mb.base;
  }

};
static c_driver *driver = new c_driver();
