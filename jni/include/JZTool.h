
#ifndef NATIVE_JZTOOL_H
#define NATIVE_JZTOOL_H
#pragma once
#include <asm-generic/fcntl.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <unistd.h>
#include <cstdlib>
#include <linux/ioctl.h>
#include <cstring>
typedef uint64_t ptr64;
#define IOCTL_CMD_HELLO _IO('k', 0)
enum ACTION
{
	SETPID = '\x00',
	VTOP = '\x01',
	RPMEM = '\x02',
	GETBASE = '\x03'
};

enum SIGN
{ DONE = '\x11', ERROR = '\x10' };

#define MAX_THREAD 3

typedef struct Data
{
	char action;
	char sign;
	size_t size;
	uint64_t Addr;
	char Info[512];
	pid_t pid;
	long baseaddr;
};

typedef struct ShareData
{
	Data data[MAX_THREAD];
};

extern thread_local int thread_index;

class JZReadTool
{
  public:
	ptr64 Module1;
	int Pid;
	ShareData *Sharedata;

	int fdDrv = -1;
#define TARGET_DIR "/data/system/Ormod"
	inline char link_drive()
	{
		char filePath[40];
		DIR *dir = opendir(TARGET_DIR);
		struct dirent *ent = nullptr;
		while ((ent = readdir(dir)) != nullptr)
		{
			snprintf(filePath, sizeof(filePath), "/proc/%s", ent->d_name);
		}
		fdDrv = open(filePath, O_RDWR);
		if (fdDrv < 0)
		{
			printf("连接ERROR\n");
			return ERROR;
		}
		sleep(1);
		Sharedata =
			(struct ShareData *)mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fdDrv,
									 0);
		if (Sharedata == MAP_FAILED)
		{
			printf("映射ERROR\n");
			munmap(Sharedata, PAGE_SIZE);
			return ERROR;
		}
		
		if (ioctl(fdDrv, IOCTL_CMD_HELLO) < 0) {
            printf("IC通讯ERROR\n");
            return ERROR;
        }
		return DONE;
	}

	inline char SetPid(pid_t pid)
	{
		memset(Sharedata->data[0].Info, 0, sizeof(Sharedata->data[0].Info));
		Sharedata->data[0].action = SETPID;
		Sharedata->data[0].Addr = pid;
		read(fdDrv, NULL, 0);
		return Sharedata->data[0].sign;
	}

	inline char virt_to_phys(ptr64 vaddr)
	{
		Sharedata->data[thread_index].sign = ERROR;
		Sharedata->data[thread_index].action = VTOP;
		Sharedata->data[thread_index].Addr = vaddr;
		read(fdDrv, NULL, thread_index);
		return Sharedata->data[thread_index].sign;
	}
	inline long get_module_base_optimized(pid_t pid, const char *name)
	{
		Sharedata->data[0].action = GETBASE;
		Sharedata->data[0].pid = pid;
		strcpy(Sharedata->data[0].Info, name);
		read(fdDrv, NULL, 0);
		if (Sharedata->data[0].sign == ERROR)
		{
			return 0;
		}
		return Sharedata->data[0].baseaddr;
	}

	inline char read_phys(ptr64 phys, size_t size)
	{
	    memset(Sharedata->data[thread_index].Info, 0, strlen(Sharedata->data[thread_index].Info));
		Sharedata->data[thread_index].size = size;
		Sharedata->data[thread_index].action = RPMEM;
		Sharedata->data[thread_index].Addr = phys;
		read(fdDrv, NULL, thread_index);
		return Sharedata->data[thread_index].sign;
	}

	inline char write_phys(uint64_t phys, void *buf)
	{
		memcpy(Sharedata->data[thread_index].Info, buf, strlen((char *)buf));
		Sharedata->data[thread_index].size = strlen((char *)buf);
		Sharedata->data[thread_index].Addr = phys;
		write(fdDrv, NULL, thread_index);
		return Sharedata->data[thread_index].sign;
	}

	inline void unlink_drive()
	{
		munmap(Sharedata, PAGE_SIZE);
		close(fdDrv);
		fdDrv = -1;
	}

	template < typename T > T Read(ptr64 vaddr)
	{
		T data;
		char sign;
		sign = virt_to_phys(vaddr);
		if (sign == ERROR)
		{
			return
			{
			};
		}
		sign = read_phys(*(ptr64 *) Sharedata->data[thread_index].Info, sizeof(T));
		if (sign == ERROR)
		{
			return
			{
			};
		}
		memcpy(&data, Sharedata->data[thread_index].Info, sizeof(T));
		return data;
	}

	template < typename T > void Read(ptr64 vaddr, T * data, size_t size = sizeof(T))
	{
		char sign;
		sign = virt_to_phys(vaddr);
		if (sign == ERROR)
		{
			return;
		}
		sign = read_phys(*(ptr64 *) Sharedata->data[thread_index].Info, size);
		if (sign == ERROR)
		{
			return;
		}
		memcpy(data, Sharedata->data[thread_index].Info, size);
		return;
	}

	template < typename T > void Write(ptr64 vaddr, T value)
	{
		char sign;
		sign = virt_to_phys(vaddr);
		if (sign == ERROR)
		{
			return;
		}
		sign = write_phys(*(ptr64 *) Sharedata->data[thread_index].Info, &value, sizeof(T));
		return;
	}

	inline int readb(int *c, int num)
	{
		++*c;
		return num;
	}

	template < typename...s > inline ptr64 GetPointer(ptr64 addr, s...args)
	{
		ptr64 add = 0;
		int con = 0;
		int arr[] = { (readb(&con, args))... };
		Read < ptr64 > (addr + arr[0], &add);
		for (int f = 1; f < con; f++)
		{
			if (f == con - 1)
			{
				add += arr[f];
				return add;
			}
			Read < ptr64 > (add + arr[f], &add);
		}
		return add;
	}
	void ReadPid(const char *PackageName)
	{
		FILE *fp;
		pid_t pid;
		char cmd[0x100] = "pidof ";
		strcat(cmd, PackageName);
		fp = popen(cmd, "r");
		fscanf(fp, "%d", &pid);
		pclose(fp);
		Pid = pid;
		return;
	}
};

#endif