#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

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
    void *buffer;
    size_t size;
  } COPY_MEMORY, *PCOPY_MEMORY;

  typedef struct _MODULE_BASE {
    pid_t pid;
    char *name;
    uintptr_t base;
  } MODULE_BASE, *PMODULE_BASE;

  struct process {
    pid_t process_pid;
    char *process_comm;
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
      if (strlen(de->d_name) != 6 || strcmp(de->d_name, "NVISPI") == 0 || strcmp(de->d_name, "aputag") == 0 || strcmp(de->d_name, "asound") == 0 || strcmp(de->d_name, "clkdbg") == 0 || strcmp(de->d_name, "crypto") == 0 || strcmp(de->d_name, "driver") == 0 || strcmp(de->d_name, "mounts") == 0 || strcmp(de->d_name, "pidmap") == 0) {
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

public:
  c_driver() {
    char *device_name = driver_path();
    fd = open(device_name, O_RDWR);

    if (fd == -1) {
      printf("[-] open driver failed\n");
      free(device_name);
      exit(0);
    }
    free(device_name);
  }

  ~c_driver() {
    // wont be called
    if (fd > 0)
      close(fd);
  }

  void initialize(pid_t pid) { this->pid = pid; }

  bool init_key(char *key) {
    char buf[0x100];
    strcpy(buf, key);
    if (ioctl(fd, OP_INIT_KEY, buf) != 0) {
      return false;
    }
    return true;
  }

  bool read(uintptr_t addr, void *buffer, size_t size) {
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
    T res;
    if (this->read(addr, &res, sizeof(T)))
      return res;
    return {};
  }

  template <typename T> bool write(uintptr_t addr, T value) {
    return this->write(addr, &value, sizeof(T));
  }

  uintptr_t get_module_base(char *name) {
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

  void hide_process() { ioctl(fd, OP_HIDE_PROCESS); }

  void hide_pid_process(unsigned int &pid) {
    ioctl(fd, OP_PID_HIDE_PROCESS, pid);
  }
  int kernel_getpid(char *PackageName) {
    struct process pc;
    strcpy(pc.process_comm, PackageName);
    if (ioctl(fd, OP_GET_PROCESS_PID, &pc) != 0) {
      return 0;
    }
    int pid = pc.process_pid;
    if (pid > 0) {
      this->pid = pid;
    } else {
      return 0;
    }
    return pid;
  }
};

static c_driver *driver = new c_driver();
