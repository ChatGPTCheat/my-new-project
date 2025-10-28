#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <string.h>
#include <jni.h>
#include <draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <cassert>
#include <string>
#include <iostream>
#include <sys/mman.h>
#include <dlfcn.h>
#include <math.h>
#include <array>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <sys/inotify.h>
#include <内核读写.hpp>
#include <VecTool.h>
#include <timer.h>
#include <UeTools.h>
#include <draw.h>
#include "src/Tools/shou.hpp"
#include <string>
#include <unordered_map>
#include <DrawTools.h>
// Define commonly used types
typedef unsigned long long Pointer;
typedef uintptr_t Ptr;
static bool is_verified = false;
bool isConditionTrue;
#include <mutex>

// ...

std::mutex mtx; // Mutex for synchronization
int 头, 胸部, 盆骨, 
    左肩, 右肩,
    左肘, 右肘, 
    左手腕, 右手腕, 
    左大腿, 右大腿,
    左膝盖, 右膝盖, 
    左脚腕, 右脚腕;

void getUTF8(UTF8* buf, long namepy);
FTransform getBone(uintptr_t addr);

// Global variables
bool GetWorld;
float camera;
uintptr_t libUE4;
TempData TempRead;
TempData TempUes;
TempData* tempRead = &TempUes;

uintptr_t Uworld, Uleve, Gname, Objaddr, TestArrayaddr, Arrayaddr, Matrix, oneself, Bone, Human, Mesh,ipad;
int Count, AddrCount, AddrCount1, MyTeam,livecount;

// PID retrieval function


pid_t get_name_pid(char* name) {
    FILE* fp;
    pid_t pid;
    char cmd[0x100] = "pidof ";

    strcat(cmd, name);
    fp = popen(cmd, "r");
    fscanf(fp, "%d", &pid);
    pclose(fp);
    return pid;
}






// Class name cache
std::unordered_map<int, std::string> ClassCache;

// Function to get class name from code
const char* veshi(int codex) {
    if (ClassCache.find(codex) == ClassCache.end()) {
        char Name[128] = "";
        uintptr_t ye = driver->read<uintptr_t>(Gname + (codex / 0x4000) * 0x8);
        uintptr_t xu = driver->read<uintptr_t>(ye + (codex % 0x4000) * 0x8);
        driver->read(xu + 0xC, &Name, 32);
        ClassCache[codex] = Name;
    }
    return ClassCache[codex].c_str();
}

// 
// Global variables for PlayerController and actor
Ptr PlayerController;




Ptr actor;


void pubg2(){

    
    
    libUE4 = driver->get_module_base("libUE4.so");
    // Wait for libUE4.so to load
    
    while (!libUE4) {
        libUE4 = driver->get_module_base("libUE4.so");
        sleep(1);
    }

}
void pubg() {
    // Initialize FPS timer
    /*

    // Array of game package names
    const char* game_package_names[] = {"com.tencent.ig", "com.rekoo.pubgm", "com.pubg.krmobile","com.vng.pubgmobile"};
    pid_t pid = -1;

    // Find PID of game process
    for (int i = 0; i < 5; i++) {
        pid = get_name_pid((char*)game_package_names[i]);
        if (pid > 0) {
            driver->initialize(pid);
            break;
        }
    }
    */
    
    
    /*
    const char* game_package_names[] = {"com.tencent.ig", "com.pubg.krmobile", "com.rekoo.pubgm","com.vng.pubgmobile"};
    pid_t pid = -1;

    // Find PID of game process
    for (int i = 0; i < 5; i++) {
        pid = get_name_pid((char*)game_package_names[i]);
    if (pid > 0 || pid >99999)
	{
		driver->initialize(pid);
	}
    }
    
    */
    char Name[128];
	pid_t pid = get_name_pid((char*)"com.tencent.ig");
	pid_t pid2 = get_name_pid ((char*)"com.rekoo.pubgm");
	pid_t pid3 = get_name_pid ((char*)"com.vng.pubgmobile");
	pid_t pid4 = get_name_pid ((char*)"com.pubg.krmobile");
	pid_t pid5 = get_name_pid ((char*)"com.pubg.imobile");
	
	if (pid > 0) {		
		driver->initialize(pid);
	} else if (pid2 > 0) {
		driver->initialize(pid2);
	} else if (pid3 > 0) {
		driver->initialize(pid3);
	} else if (pid4 > 0) {
		driver->initialize(pid4);
	} else if (pid5 > 0) {
		driver->initialize(pid5);
	} else {
		printf("game not started");
		exit(0);
	}
    
    
    
    

    // Check if game is running
    if (pid <= 0) {
        printf("PUBG Not started ");
        exit(0);
    }

    // Initialize matrix
    memset(matrix, 0, 16);
//driver->hide_process();
    // Get base address of libUE4.so
    
 //   libUE4 = driver->getModuleBase(pid,"libUE4.so");
    // Wait for libUE4.so to load
    

    
  //  Matrix = driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0xD3E30A0) + 0x20) + 0x270; 






    // Set screen dimensions
    if (screen_x > screen_y) {
        Higtih = screen_y / 2;
        Widtih = screen_x / 2;
    } else {
        Higtih = screen_x / 2;
        Widtih = screen_y / 2;
    }
std::vector<uintptr_t> uworlds;
std::vector<uintptr_t> uleves;
std::vector<uintptr_t> arrayAddrs;
std::vector<int> counts;
    // Main game loop
   for (;;) {
        // Reset counter
       // usleep(3600);
        TempRead.mPlayerArray.Count = AddrCount;
        TempRead.mwuziArray.Count = AddrCount1;
        *tempRead = TempRead;

        AddrCount = 0;
        AddrCount1 = 0;
        Ue4gg = libUE4;
        arraygg = Arrayaddr;



	
	Uworld = driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0xEA79A90) + 0x810) + 0x78);
		Gname = driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0xe47e8f0) + 0x110);//😭
    	Uleve = driver->read<uintptr_t>(Uworld + 0x30);    // Uleve

/*
0xDD879E0
        // Get addresses
Gname = driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0xcca7020) + 0x110);
        Uworld = driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 +  0xD3E9A70) + 0x30) + 0x440);

     Uleve = driver->read<uintptr_t>(Uworld + 0x20);    // Uleve

        */
        auto chlen = DecryptActorsArray(Uleve, 0xA0, 0x448);
        if (chlen == 0) {
            chlen = DecryptActorsArray(Uleve, 0xA0, 0x488);
        }

        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        
        Arrayaddr = driver->read<uintptr_t>(chlen);

        if (Arrayaddr >= 0x10000000000) {
            continue;
        }

        
        Count = driver->read<uintptr_t>(chlen + 0x8);

        	if(Count <=0 || Count >=2000){
	if(Count <=0 || Count >=2000){
		  std::chrono::microseconds sleepDuration(7000);
          std::this_thread::sleep_for(sleepDuration);
		    continue;
		}
}
    uworlds.push_back(Uworld);
    uleves.push_back(Uleve);
    arrayAddrs.push_back(Arrayaddr);
    counts.push_back(Count);

	Matrix = driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0xEA4DC40) + 0x20) + 0x270;	
		oneself = driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0xEA79A90) + 0x810) + 0x78) + 0x38)+ 0x78)+0x30) + 0x27a8); 
		//if (!oneself) { usleep(1000); continue; }
		
		
		MyTeam = driver->read<int>(oneself + 0x938);		


/*
        // Get matrix address
	Matrix = driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0xD3E30A0) + 0x20) + 0x270; 
       
  
oneself = driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0xD46DD68) + 0x8) + 0x48) + 0x20);

        MyTeam = driver->read<int>(oneself + 0x938);
*/
        // Read player data
       // TempRead.MyWeapon = driver->read<int>(driver->read<uintptr_t>(driver->read<uintptr_t>(oneself + 0x2328) + 0x500) + 0x178);
        
            TempRead.MyWeapon = driver->read<int>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(oneself + 0x2488) + 0x558) + 0x11a0) + 0x178);
        
        
        
        driver->read(driver->read<uintptr_t>(oneself + 0x1b0) + 0x1c0, &TempRead.MyPos, 12);

        driver->read(Matrix, &matrix, 16 * 4);
        driver->read(Matrix, &TempRead.matrix, 16 * 4);

      //  TempRead.Fov = driver->read<float>(driver->read<uintptr_t>(driver->read<uintptr_t>(oneself + 0xCF67840) + 0x108) + 0x4d4);
      
      		TempRead.Fov = driver->read<float>(driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + 0x49e0) + 0x4f0) + 0x4e0);   // 自身FOV   
		TempRead.IsAiming = driver->read<int>(oneself + 0x1071);
        TempRead.IsFiring = driver->read<int>(oneself + 0x1708);
      
      //  TempRead.IsAiming = driver->read<int>(oneself + 0x1051);
      //  TempRead.IsFiring = driver->read<int>(oneself + 0x1640);
 livecount = driver->read<int>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(Uworld + 0x20) + 0x20) + 0x1d8) + 0xA54);

bool isConditionTrue = driver->read<int>(Uworld + 0x8) == 8;



        // Loop through actors
        for (int i = 0; i < Count; i++) {
            // Get actor address
            Objaddr = driver->read<uintptr_t>(Arrayaddr + 8 * i);
            
if (oneself == Objaddr || Objaddr <= 0x10000000 || Objaddr % 4 != 0 || Objaddr >= 0x10000000000)
continue;




            // Check actor address
            uintptr_t object = driver->read<uintptr_t>(Objaddr + 0x1b0);
            if (object <= 0xffff || object == 0 || object <= 0x10000000 || object % 4 != 0 || object >= 0x10000000000) {
                continue;
            }

            // Check if object is an item
            if (driver->read<float>(Objaddr + 0x2a68) != 479.5) {
                // Handle item
                auto* wuzis = &TempRead.mwuziArray.mwuzi[AddrCount1];

                int oid = driver->read<int>(Objaddr + 0x18);
                auto Name = veshi(oid);

                // Read position
                driver->read(object + 0x1c0, &wuzis->Pos, 12);
                if (wuzis->Pos.x == 0 || wuzis->Pos.y == 0 || wuzis->Pos.z == 0) {
                    continue;
                }

                // Calculate distance
                GetDistance(wuzis->Pos, TempRead.MyPos, &wuzis->Distance);

                // Save item name
                strcpy(wuzis->wuziName, Name);

                // Convert to screen coordinates
                WorldToScreen(&wuzis->ScreenPos, &camera, &wuzis->w, wuzis->Pos);

                // Increment item counter
                AddrCount1++;
                continue;
            }

            // Check if object is a player
            if (driver->read<int>(Objaddr + 0x2a68) == 1 ){
                continue;
            }

	if (driver->read<int>(Objaddr + 0xddc) == 1 || driver->read<int>(Objaddr + 0xddc) == 3){
				continue;
			}


            // Handle player
            auto* Players = &TempRead.mPlayerArray.mPlayer[AddrCount];

            driver->read(object + 0x1c0, &Players->Pos, 12);
            if (Players->Pos.x == 0 || Players->Pos.y == 0 || Players->Pos.z == 0) {
                continue;
            }

            // Check player state
		
			int State = driver->read<int>(Objaddr + 0xfa0);//状态 
        	if (State == 262144 || State == 262152 || State == 1048576 || State == 1048592)
           		continue;
        
            // Read player data
            Players->TeamID = driver->read<int>(Objaddr + 0x938);
            if (Players->TeamID == MyTeam || Players->TeamID < 1) {
                continue;
            }

            Players->MaxHealth1 = driver->read<float>(Objaddr + 0xdb0);
            float MinHealth = driver->read<float>(Objaddr + 0xdb8);
            float MaxHealth = driver->read<float>(Objaddr + 0xdbc);
            Players->Health = (MinHealth / MaxHealth) * 100;
            if (Players->Health > 100) {
                continue;
            }

            // aim pred
            /*
           if (driver->read<uintptr_t>(Objaddr + 0x1c0)) {
                driver->read(driver->read<uintptr_t>(Objaddr + 0x1c0) + 0x1330, &Players->Predict, sizeof(Players->Predict));
            } else {
                driver->read(object + 0x260, &Players->Predict, sizeof(Players->Predict));
            }

    */
				
			long int CurrentVehicle = driver->read<uintptr_t>(Objaddr + 0xdf8); //车辆向量一层
			if (CurrentVehicle) {
            	driver->read(CurrentVehicle + 0xb0, &Players->Predict, sizeof(Players->Predict)); // 载具向量
      	 	} else {
            	driver->read(object + 0x260, &Players->Predict, sizeof(Players->Predict)); // 敌人向量
        	}        	

/*
	if (driver->read<uintptr_t>(Objaddr + 0x1c0)) {
            	driver->read(driver->read<uintptr_t>(Objaddr + 0x1c0) + 0x1330, &Players->Predict, sizeof(Players->Predict)); // 载具向量
      	 	} else {
            	driver->read(driver->read<uintptr_t>(Objaddr + 0x1b0) + 0x260, &Players->Predict, sizeof(Players->Predict)); // 敌人向量
        	}              	

*/





            // Calculate distance
            GetDistance(Players->Pos, TempRead.MyPos, &Players->Distance);

            // Convert to screen coordinates
            WorldToScreen(&Players->ScreenPos, &camera, &Players->w, Players->Pos);

            // Save camera value
            Players->camera = camera;

            // Check if player is a bot
	
		//Players->IsBot = driver->read<int>(Objaddr + 0x9e9 );   // ==65537 ? true : false;
			

//Players->IsBot = driver->read<int>(Objaddr + 0x996) == 65537 ? true : false;

/*
		long int bAi1 = driver->read<int>(Objaddr + 0x9e9);
            if (bAi1==16842753 or bAi1==16843009 or bAi1==16843008) {
            Players->IsBot = 1;
            } else {
            Players->IsBot = 0;}
			
*/
/*

bool bAi = driver->read<bool>(Objaddr +0x9e9);

if(bAi)
{
Players->IsBot = 1;
}else {
Players->IsBot = 0;
}
*/
long int 人机 = driver->read<int>(Objaddr + 0x9e9);
            if (人机==16842753 or 人机==16843009 or 人机==16843008) {
            Players->IsBot = 1;
            } else {
            Players->IsBot = 0;}


/*
int isBotFlag = driver->read<int>(Objaddr +  0xab4) ? 1 : 0;




long int renji = driver->read<int>(Objaddr + 0x9e9);


int renjiBot = (renji == 16842753 || renji == 16843009 || renji == 16843008) ? 1 : 0;


Players->IsBot = (isBotFlag || renjiBot) ? 1 : 0;


if (Players->IsBot) Players->IsBot = 0; else Players->IsBot = 1;



*/

/*

if (Players->IsBot = driver->read<int>(Objaddr + 0x996)==1) {
    
} else {
   
}

*/



            // Check bot drawing flag
            if (DrawIo[9] && Players->IsBot == 1) {
                continue;
            }

//0x742A70,0x30,0x440,0x2B58,0x388,0x188

            

            long int rw = driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(Objaddr + 0x340) + 0x78) + 0x388);
            Players->drb = driver->read<int>(rw + 0xf0);
            Players->drt = driver->read<int>(rw + 0x1fc);
            Players->drj = driver->read<int>(rw + 0x234);
            
	


	Players->scwq = driver->read<int>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(Objaddr + 0x2528) + 0x558) + 0x1250) + 0x178);
			Players->dqzd = driver->read<int>(driver->read<uintptr_t>(driver->read<uintptr_t>(Objaddr + 0x2528)+ 0x558)+0xE88);
			Players->zdmax = driver->read<int>(driver->read<uintptr_t>(driver->read<uintptr_t>(Objaddr + 0x2528)+ 0x558)+0xF08);

            // Read skeleton data
            Players->Dzid = driver->read<uintptr_t>(Objaddr + 0xF88);

            // Read player name
            getUTF8(Players->PlayerName, driver->read<uintptr_t>(Objaddr + 0x900));

            // Get Mesh address
            Mesh = driver->read<uintptr_t>(Objaddr + 0x4a8);


	if (driver->read<int>(Objaddr + 0x9c0)&&MaxHealth>=1000)
            {
             continue;
            }
            
            // Get bone count
            int Bonecount = driver->read<int>(Mesh + 0x7f8 + 0x8);

            // Check Mesh address
            if (Mesh <= 0xffff) {
                continue;
            }

            // Get Human address
            Human = Mesh + 0x1b0;

            // Get bone address
            Bone = driver->read<uintptr_t>(Mesh + 0x8a8) + 0x30;

            // Check bone address
            if (Bone <= 0xffff) {
                continue;
            }

				
if (MyTeam <= 100) {
    头 = 5;
    胸部 = 4;
    盆骨 = 1;
    左肩 = 11;
    右肩 = 32;
    左肘 = 12;
    右肘 = 33;
    左手腕 = 63;
    右手腕 = 62;
    左大腿 = 52;
    右大腿 = 56;
    左膝盖 = 53;
    右膝盖 = 57;
    左脚腕 = 54;
    右脚腕 = 58;
}
if (MaxHealth == 1000){
    头 = 5;
    胸部 = 4;
    盆骨 = 1;
    左肩 = 13;
    右肩 = 34;
    左肘 = 14;
    右肘 = 35;
    左手腕 = 16;
    右手腕 = 37;
    左大腿 = 54;
    右大腿 = 58;
    左膝盖 = 55;
    右膝盖 = 59;
    左脚腕 = 56;
    右脚腕 = 60;
}
if (MyTeam == 996) {
    头 = 5;
}


			FTransform meshtrans = getBone(Human);
            FMatrix c2wMatrix = TransformToMatrix(meshtrans);		
			
            /* 头部 */
            FTransform headtrans = getBone(Bone + 头 * 48);
            FMatrix boneMatrix = TransformToMatrix(headtrans);
            Players->Head.Pos = MarixToVector(MatrixMulti(boneMatrix, c2wMatrix));
            Players->Head.Pos.z += 7; 		
            Players->Head.ScreenPos = WorldToScreen(Players->Head.Pos, matrix, camera);

            if((Players->Head.Pos.x -Players->Pos.x )>300)
                    continue;
            if((Players->Head.Pos.y -Players->Pos.y )>300)
                    continue;
            if((Players->Head.Pos.z -Players->Pos.z )>300)
                    continue;
            /* 胸部 */
            FTransform chesttrans = getBone(Bone + 胸部 * 48);
            FMatrix boneMatrix1 = TransformToMatrix(chesttrans);
            Players->Chest.Pos = MarixToVector(MatrixMulti(boneMatrix1, c2wMatrix));
            Players->Chest.ScreenPos = WorldToScreen(Players->Chest.Pos, matrix, camera);
            
			/* 盆骨 */
            FTransform pelvistrans = getBone(Bone + 盆骨 * 48);
            FMatrix boneMatrix2 = TransformToMatrix(pelvistrans);
            Players->Pelvis.Pos = MarixToVector(MatrixMulti(boneMatrix2, c2wMatrix));
            Players->Pelvis.Pos.z -= 5; /* 脖子长度 */   			

            Players->Pelvis.ScreenPos = WorldToScreen(Players->Pelvis.Pos, matrix, camera);
			
            /* 左肩膀 */
            FTransform lshtrans = getBone(Bone + 左肩 * 48);
            FMatrix boneMatrix3 = TransformToMatrix(lshtrans);
            Players->Left_Shoulder.Pos = MarixToVector(MatrixMulti(boneMatrix3, c2wMatrix));
            Players->Left_Shoulder.ScreenPos = WorldToScreen(Players->Left_Shoulder.Pos, matrix, camera);
			
            /* 右肩膀 */
            FTransform rshtrans = getBone(Bone + 右肩 * 48);
            FMatrix boneMatrix4 = TransformToMatrix(rshtrans);
            Players->Right_Shoulder.Pos = MarixToVector(MatrixMulti(boneMatrix4, c2wMatrix));
            Players->Right_Shoulder.ScreenPos = WorldToScreen(Players->Right_Shoulder.Pos, matrix, camera);
			
            /* 左手肘 */
            FTransform lelbtrans = getBone(Bone + 左肘 * 48);
            FMatrix boneMatrix5 = TransformToMatrix(lelbtrans);
            Players->Left_Elbow.Pos = MarixToVector(MatrixMulti(boneMatrix5, c2wMatrix));
            Players->Left_Elbow.ScreenPos = WorldToScreen(Players->Left_Elbow.Pos, matrix, camera);
			
            /* 右手肘 */
            FTransform relbtrans = getBone(Bone + 右肘 * 48);
            FMatrix boneMatrix6 = TransformToMatrix(relbtrans);
            Players->Right_Elbow.Pos = MarixToVector(MatrixMulti(boneMatrix6, c2wMatrix));
            Players->Right_Elbow.ScreenPos = WorldToScreen(Players->Right_Elbow.Pos, matrix, camera);
			
            /* 左手腕 */
            FTransform lwtrans = getBone(Bone + 左手腕 * 48);
            FMatrix boneMatrix7 = TransformToMatrix(lwtrans);
            Players->Left_Wrist.Pos = MarixToVector(MatrixMulti(boneMatrix7, c2wMatrix));
            Players->Left_Wrist.ScreenPos = WorldToScreen(Players->Left_Wrist.Pos, matrix, camera);
			
            /* 右手腕 */
            FTransform rwtrans = getBone(Bone + 右手腕 * 48);
            FMatrix boneMatrix8 = TransformToMatrix(rwtrans);
            Players->Right_Wrist.Pos = MarixToVector(MatrixMulti(boneMatrix8, c2wMatrix));
            Players->Right_Wrist.ScreenPos = WorldToScreen(Players->Right_Wrist.Pos, matrix, camera);
			
            /* 左大腿 */
            FTransform Llshtrans = getBone(Bone + 左大腿 * 48);
            FMatrix boneMatrix9 = TransformToMatrix(Llshtrans);
            Players->Left_Thigh.Pos = MarixToVector(MatrixMulti(boneMatrix9, c2wMatrix));
			if (abs(Players->Left_Thigh.Pos.x-Players->Pelvis.Pos.x)>20||abs(Players->Left_Thigh.Pos.y-Players->Pelvis.Pos.y)>20||abs(Players->Left_Thigh.Pos.z-Players->Pelvis.Pos.z)>20){
			Players->Left_Thigh.Pos.x=Players->Pelvis.Pos.x*2- Players->Right_Thigh.Pos.x;
			Players->Left_Thigh.Pos.y=Players->Pelvis.Pos.y*2- Players->Right_Thigh.Pos.y;
			Players->Left_Thigh.Pos.z=(Players->Pelvis.Pos.z-3)*2- Players->Right_Thigh.Pos.z;
			}
            Players->Left_Thigh.ScreenPos = WorldToScreen(Players->Left_Thigh.Pos, matrix, camera);
            
            /* 右大腿 */
            FTransform Lrshtrans = getBone(Bone + 右大腿 * 48);
            FMatrix boneMatrix10 = TransformToMatrix(Lrshtrans);
            Players->Right_Thigh.Pos = MarixToVector(MatrixMulti(boneMatrix10, c2wMatrix));
            Players->Right_Thigh.ScreenPos = WorldToScreen(Players->Right_Thigh.Pos, matrix, camera);
			
            
            /* 左膝盖 */
            FTransform Llelbtrans = getBone(Bone + 左膝盖 * 48);
            FMatrix boneMatrix11 = TransformToMatrix(Llelbtrans);
            Players->Left_Knee.Pos = MarixToVector(MatrixMulti(boneMatrix11, c2wMatrix));
            Players->Left_Knee.ScreenPos = WorldToScreen(Players->Left_Knee.Pos, matrix, camera);
			
            /* 右膝盖 */
            FTransform Lrelbtrans = getBone(Bone + 右膝盖 * 48);
            FMatrix boneMatrix12 = TransformToMatrix(Lrelbtrans);
            Players->Right_Knee.Pos = MarixToVector(MatrixMulti(boneMatrix12, c2wMatrix));
            Players->Right_Knee.ScreenPos = WorldToScreen(Players->Right_Knee.Pos, matrix, camera);
			
            /* 左脚腕 */
            FTransform Llwtrans = getBone(Bone + 左脚腕 * 48);
            FMatrix boneMatrix13 = TransformToMatrix(Llwtrans);
            Players->Left_Ankle.Pos = MarixToVector(MatrixMulti(boneMatrix13, c2wMatrix));
            Players->Left_Ankle.ScreenPos = WorldToScreen(Players->Left_Ankle.Pos, matrix, camera);
			
            /* 右脚腕 */
            FTransform Lrwtrans = getBone(Bone + 右脚腕 * 48);
            FMatrix boneMatrix14 = TransformToMatrix(Lrwtrans);
            Players->Right_Ankle.Pos = MarixToVector(MatrixMulti(boneMatrix14, c2wMatrix));
            Players->Right_Ankle.ScreenPos = WorldToScreen(Players->Right_Ankle.Pos, matrix, camera);
			

            AddrCount++;
        }

        // Wait for a short time
        }
        uworlds.clear();
uleves.clear();
arrayAddrs.clear();
counts.clear();	
}









// Function to convert UTF-16 to UTF-8
void getUTF8(UTF8* buf, long namepy) {
    UTF16 buf16[16] = {0};
    driver->read(namepy, buf16, 28);
    UTF16* pTempUTF16 = buf16;
    UTF8* pTempUTF8 = buf;
    UTF8* pUTF8End = pTempUTF8 + 32;
    while (pTempUTF16 < pTempUTF16 + 28) {
        if (*pTempUTF16 <= 0x007F && pTempUTF8 + 1 < pUTF8End) {
            *pTempUTF8++ = (UTF8)*pTempUTF16;
        } else if (*pTempUTF16 >= 0x0080 && *pTempUTF16 <= 0x07FF && pTempUTF8 + 2 < pUTF8End) {
            *pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        } else if (*pTempUTF16 >= 0x0800 && *pTempUTF16 <= 0xFFFF && pTempUTF8 + 3 < pUTF8End) {
            *pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
            *pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        } else {
            break;
        }
        pTempUTF16++;
    }
}

// Function to get FTransform from address
FTransform getBone(uintptr_t addr) {
    FTransform transform;
    driver->read(addr, reinterpret_cast<void*>(&transform), 4 * 11);
    return transform;
}





