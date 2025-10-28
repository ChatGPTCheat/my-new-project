#pragma once
#define USTOOL_H
#include <string>
#define PI 3.141592653589793238

float matrix[16] = {0};
double Widtih, Higtih;

typedef char UTF8;
typedef unsigned short UTF16;

struct Actors {
    uint64_t Enc_1, Enc_2;
    uint64_t Enc_3, Enc_4;
};

struct Chunk {
    uint32_t val_1, val_2, val_3, val_4;
    uint32_t val_5, val_6, val_7, val_8;
};

uint64_t DecryptActorsArray(uint64_t uLevel, int Actors_Offset, int EncryptedActors_Offset) {
    if (uLevel < 0x10000000)
        return 0;
 
    if (driver->read<uint64_t>(uLevel + Actors_Offset) > 0)
		return uLevel + Actors_Offset;
 
    if (driver->read<uint64_t>(uLevel + EncryptedActors_Offset) > 0)
		return uLevel + EncryptedActors_Offset;
 
    auto AActors = driver->read<Actors>(uLevel + EncryptedActors_Offset + 0x10);
 
    if (AActors.Enc_1 > 0) {
        auto Enc = driver->read<Chunk>(AActors.Enc_1 + 0x80);
        return (((driver->read<uint8_t>(AActors.Enc_1 + Enc.val_1)
            | (driver->read<uint8_t>(AActors.Enc_1 + Enc.val_2) << 8))
            | (driver->read<uint8_t>(AActors.Enc_1 + Enc.val_3) << 0x10)) & 0xFFFFFF
            | ((uint64_t)driver->read<uint8_t>(AActors.Enc_1 + Enc.val_4) << 0x18)
            | ((uint64_t)driver->read<uint8_t>(AActors.Enc_1 + Enc.val_5) << 0x20)) & 0xFFFF00FFFFFFFFFF
            | ((uint64_t)driver->read<uint8_t>(AActors.Enc_1 + Enc.val_6) << 0x28)
            | ((uint64_t)driver->read<uint8_t>(AActors.Enc_1 + Enc.val_7) << 0x30)
            | ((uint64_t)driver->read<uint8_t>(AActors.Enc_1 + Enc.val_8) << 0x38);
    }
    else if (AActors.Enc_2 > 0) {
        auto Lost_Actors = driver->read<uint64_t>(AActors.Enc_2);
        if (Lost_Actors > 0) {
            return (uint16_t)(Lost_Actors - 0x400) & 0xFF00
                | (uint8_t)(Lost_Actors - 0x04)
                | (Lost_Actors + 0xFC0000) & 0xFF0000
                | (Lost_Actors - 0x4000000) & 0xFF000000
                | (Lost_Actors + 0xFC00000000) & 0xFF00000000
                | (Lost_Actors + 0xFC0000000000) & 0xFF0000000000
                | (Lost_Actors + 0xFC000000000000) & 0xFF000000000000
                | (Lost_Actors - 0x400000000000000) & 0xFF00000000000000;
        }
    }
    else if (AActors.Enc_3 > 0) {
        auto Lost_Actors = driver->read<uint64_t>(AActors.Enc_3);
        if (Lost_Actors > 0) {
            return (Lost_Actors >> 0x38) | (Lost_Actors << (64 - 0x38));
		}
    }
    else if (AActors.Enc_4 > 0) {
        auto Lost_Actors = driver->read<uint64_t>(AActors.Enc_4);
        if (Lost_Actors > 0) {
            return Lost_Actors ^ 0xCDCD00;
		}
    }
    return 0;
}

void GetDistance(Vec3 Object, Vec3 Self, float *Distance)
{
	float DistanceX = pow(Object.x - Self.x, 2);
    float DistanceY = pow(Object.y - Self.y, 2);
    float DistanceZ = pow(Object.z - Self.z, 2);
    *Distance = sqrt(DistanceX + DistanceY + DistanceZ) * 0.01f;
}
/*
void WorldToScreen(float *bscreen, Vec3 *obj)
{
    float camear = matrix[3] * obj->x + matrix[7] * obj->y + matrix[11] * obj->z + matrix[15];
    *bscreen = Higtih - (matrix[1] * obj->x + matrix[5] * obj->y + matrix[9] * obj->z + matrix[13]) / camear * Higtih;
}

void WorldToScreen(Vec2 *bscreen, Vec3 *obj)
{
    float camear = matrix[3] * obj->x + matrix[7] * obj->y + matrix[11] * obj->z + matrix[15];
    bscreen->x = Widtih + (matrix[0] * obj->x + matrix[4] * obj->y + matrix[8] * obj->z + matrix[12]) / camear * Widtih;
    bscreen->y = Higtih - (matrix[1] * obj->x + matrix[5] * obj->y + matrix[9] * obj->z + matrix[13]) / camear * Higtih;
}

void WorldToScreen(Vec2 *bscreen, Vec3 obj)
{
    float camear = matrix[3] * obj.x + matrix[7] * obj.y + matrix[11] * obj.z + matrix[15];
    bscreen->x = Widtih + (matrix[0] * obj.x + matrix[4] * obj.y + matrix[8] * obj.z + matrix[12]) / camear * Widtih;
    bscreen->y = Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * obj.z + matrix[13]) / camear * Higtih;
}

Vec2 WorldToScreen(Vec3 obj, float a[16], float b)	
{
	Vec2 bscreen;
    float camear = matrix[3] * obj.x + matrix[7] * obj.y + matrix[11] * obj.z + matrix[15];
    bscreen.x = Widtih + (matrix[0] * obj.x + matrix[4] * obj.y + matrix[8] * obj.z + matrix[12]) / camear * Widtih;
    bscreen.y = Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * obj.z + matrix[13]) / camear * Higtih;
	return bscreen;
}

void WorldToScreen(Vec2 *bscreen, float *camea, Vec3 obj)
{
    float camear = matrix[3] * obj.x + matrix[7] * obj.y + matrix[11] * obj.z + matrix[15];
    *camea = camear;
    bscreen->x = Widtih + (matrix[0] * obj.x + matrix[4] * obj.y + matrix[8] * obj.z + matrix[12]) / camear * Widtih;
    bscreen->y = Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * obj.z + matrix[13]) / camear * Higtih;
}

void WorldToScreen(Vec2 *bscreen, float *camea, float *w, Vec3 obj)
{
    float camear = matrix[3] * obj.x + matrix[7] * obj.y + matrix[11] * obj.z + matrix[15];
    *camea = camear;
    bscreen->x = Widtih + (matrix[0] * obj.x + matrix[4] * obj.y + matrix[8] * obj.z + matrix[12]) / camear * Widtih;
    bscreen->y = Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * obj.z + matrix[13]) / camear * Higtih;
	float bscreenZ = Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * (obj.z + 165) + matrix[13]) / camear * Higtih;
    float bscreenz = bscreen->y - bscreenZ;
    *w = (bscreen->y - bscreenZ) / 2;
}

void WorldToScreen(Vec4 *bscreen, float *camea, Vec3 obj)
{
    float camear = matrix[3] * obj.x + matrix[7] * obj.y + matrix[11] * obj.z + matrix[15];
    *camea = camear;
    bscreen->x = Widtih + (matrix[0] * obj.x + matrix[4] * obj.y + matrix[8] * obj.z + matrix[12]) / camear * Widtih;
    bscreen->y = Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * obj.z + matrix[13]) / camear * Higtih;
    float bscreenZ = Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * (obj.z + 165) + matrix[13]) / camear * Higtih;
    bscreen->z = bscreen->y - bscreenZ;
    bscreen->w = (bscreen->y - bscreenZ) / 2;
}

*/

// Вспомогательная функция для расчёта "camear"
inline float CalcCamear(Vec3 obj) {
    return matrix[3] * obj.x + matrix[7] * obj.y + matrix[11] * obj.z + matrix[15];
}

// Вспомогательная функция для x и y
inline float CalcScreenX(Vec3 obj, float camear) {
    return Widtih + (matrix[0] * obj.x + matrix[4] * obj.y + matrix[8] * obj.z + matrix[12]) / camear * Widtih;
}
inline float CalcScreenY(Vec3 obj, float camear) {
    return Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * obj.z + matrix[13]) / camear * Higtih;
}
inline float CalcScreenYPlusZ(Vec3 obj, float camear, float zadd) {
    return Higtih - (matrix[1] * obj.x + matrix[5] * obj.y + matrix[9] * (obj.z + zadd) + matrix[13]) / camear * Higtih;
}

void WorldToScreen(float *bscreen, Vec3 *obj)
{
    float camear = CalcCamear(*obj);
    *bscreen = CalcScreenY(*obj, camear);
}

void WorldToScreen(Vec2 *bscreen, Vec3 *obj)
{
    float camear = CalcCamear(*obj);
    bscreen->x = CalcScreenX(*obj, camear);
    bscreen->y = CalcScreenY(*obj, camear);
}

void WorldToScreen(Vec2 *bscreen, Vec3 obj)
{
    float camear = CalcCamear(obj);
    bscreen->x = CalcScreenX(obj, camear);
    bscreen->y = CalcScreenY(obj, camear);
}

Vec2 WorldToScreen(Vec3 obj, float a[16], float b) 
{
    Vec2 bscreen;
    float camear = CalcCamear(obj);
    bscreen.x = CalcScreenX(obj, camear);
    bscreen.y = CalcScreenY(obj, camear);
    return bscreen;
}

void WorldToScreen(Vec2 *bscreen, float *camea, Vec3 obj)
{
    float camear = CalcCamear(obj);
    *camea = camear;
    bscreen->x = CalcScreenX(obj, camear);
    bscreen->y = CalcScreenY(obj, camear);
}

void WorldToScreen(Vec2 *bscreen, float *camea, float *w, Vec3 obj)
{
    float camear = CalcCamear(obj);
    *camea = camear;
    bscreen->x = CalcScreenX(obj, camear);
    bscreen->y = CalcScreenY(obj, camear);
    float bscreenZ = CalcScreenYPlusZ(obj, camear, 165);
    float bscreenz = bscreen->y - bscreenZ;
    *w = bscreenz / 2;
}

void WorldToScreen(Vec4 *bscreen, float *camea, Vec3 obj)
{
    float camear = CalcCamear(obj);
    *camea = camear;
    bscreen->x = CalcScreenX(obj, camear);
    bscreen->y = CalcScreenY(obj, camear);
    float bscreenZ = CalcScreenYPlusZ(obj, camear, 165);
    bscreen->z = bscreen->y - bscreenZ;
    bscreen->w = bscreen->z / 2;
}



Vec2 rotateCoord(float angle, float objRadar_x, float objRadar_y)
{
    Vec2 radarCoordinate;
    float s = sin(angle * PI / 180);
    float c = cos(angle * PI / 180);
    radarCoordinate.x = objRadar_x * c + objRadar_y * s;
    radarCoordinate.y = -objRadar_x * s + objRadar_y * c;
    return radarCoordinate;
}

Vec3 MarixToVector(FMatrix matrix)
{
    return Vec3(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}

FMatrix MatrixMulti(FMatrix m1, FMatrix m2)
{
    FMatrix matrix = FMatrix();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
            }
        }
    }
    return matrix;
}

FMatrix TransformToMatrix(FTransform transform)
{
    FMatrix matrix;
    matrix.M[3][0] = transform.Translation.x;
    matrix.M[3][1] = transform.Translation.y;
    matrix.M[3][2] = transform.Translation.z;
    float x2 = transform.Rotation.x + transform.Rotation.x;
    float y2 = transform.Rotation.y + transform.Rotation.y;
    float z2 = transform.Rotation.z + transform.Rotation.z;
    float xx2 = transform.Rotation.x * x2;
    float yy2 = transform.Rotation.y * y2;
    float zz2 = transform.Rotation.z * z2;
    matrix.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.x;
    matrix.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.y;
    matrix.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.z;
    float yz2 = transform.Rotation.y * z2;
    float wx2 = transform.Rotation.w * x2;
    matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.z;
    matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.y;
    float xy2 = transform.Rotation.x * y2;
    float wz2 = transform.Rotation.w * z2;
    matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.y;
    matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.x;
    float xz2 = transform.Rotation.x * z2;
    float wy2 = transform.Rotation.w * y2;
    matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.z;
    matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.x;
    matrix.M[0][3] = 0;
    matrix.M[1][3] = 0;
    matrix.M[2][3] = 0;
    matrix.M[3][3] = 1;
    return matrix;
}

void BezierCurve(ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, int segments, vector<ImVec2>& points) {
	for (int i = 0; i <= segments; ++i) {
        float t = (float)i / (float)segments;
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;
        ImVec2 p = ImVec2(uuu * p1.x, uuu * p1.y); //first term
        p.x += 3 * uu * t * p2.x; 
		p.y += 3 * uu * t * p2.y; //second term
        p.x += 3 * u * tt * p3.x;
		p.y += 3 * u * tt * p3.y; //third term
        p.x += ttt * p4.x;
		p.y += ttt * p4.y; //fourth term
        points.push_back(p);
    }
}

void ExplosionRange(Vec3 Obj, ImColor color, float Range, float thickn) {
    Vec3 l1, l2, l3, l4, l5, l6, l7, l8;
    Vec2 lw1, lw2, lw3, lw4, lw5, lw6, lw7, lw8;
    l1 = Vec3(Obj.x - Range, Obj.y - Range, Obj.z);
    l2 = Vec3(Obj.x, Obj.y - Range, Obj.z);
    l3 = Vec3(Obj.x + Range, Obj.y - Range, Obj.z);
    l4 = Vec3(Obj.x - Range, Obj.y, Obj.z);
    l5 = Vec3(Obj.x + Range, Obj.y, Obj.z);
    l6 = Vec3(Obj.x - Range, Obj.y + Range, Obj.z);
    l7 = Vec3(Obj.x, Obj.y + Range, Obj.z);
    l8 = Vec3(Obj.x + Range, Obj.y + Range, Obj.z);
    WorldToScreen(&lw1, l1);WorldToScreen(&lw2, l2);
    WorldToScreen(&lw3, l3);WorldToScreen(&lw4, l4);
    WorldToScreen(&lw5, l5);WorldToScreen(&lw6, l6);
    WorldToScreen(&lw7, l7);WorldToScreen(&lw8, l8);
	
	// 绘制曲线
	ImGui::GetBackgroundDrawList()->AddBezierCurve({lw4.x, lw4.y}, {lw1.x, lw1.y}, {lw2.x, lw2.y}, {lw2.x, lw2.y}, color, thickn);
    ImGui::GetBackgroundDrawList()->AddBezierCurve({lw2.x, lw2.y}, {lw3.x, lw3.y}, {lw5.x, lw5.y}, {lw5.x, lw5.y}, color, thickn);
    ImGui::GetBackgroundDrawList()->AddBezierCurve({lw5.x, lw5.y}, {lw8.x, lw8.y}, {lw7.x, lw7.y}, {lw7.x, lw7.y}, color, thickn);
    ImGui::GetBackgroundDrawList()->AddBezierCurve({lw7.x, lw7.y}, {lw6.x, lw6.y}, {lw4.x, lw4.y}, {lw4.x, lw4.y}, color, thickn);
}

void ExplosionRangeFilled(Vec3 Obj, ImColor color, float Range, int num_segments) {
    Vec3 l1, l2, l3, l4, l5, l6, l7, l8;
    Vec2 lw1, lw2, lw3, lw4, lw5, lw6, lw7, lw8;
    l1 = Vec3(Obj.x - Range, Obj.y - Range, Obj.z);
    l2 = Vec3(Obj.x, Obj.y - Range, Obj.z);
    l3 = Vec3(Obj.x + Range, Obj.y - Range, Obj.z);
    l4 = Vec3(Obj.x - Range, Obj.y, Obj.z);
    l5 = Vec3(Obj.x + Range, Obj.y, Obj.z);
    l6 = Vec3(Obj.x - Range, Obj.y + Range, Obj.z);
    l7 = Vec3(Obj.x, Obj.y + Range, Obj.z);
    l8 = Vec3(Obj.x + Range, Obj.y + Range, Obj.z);
    WorldToScreen(&lw1, l1);WorldToScreen(&lw2, l2);
    WorldToScreen(&lw3, l3);WorldToScreen(&lw4, l4);
    WorldToScreen(&lw5, l5);WorldToScreen(&lw6, l6);
    WorldToScreen(&lw7, l7);WorldToScreen(&lw8, l8);
	
	vector<ImVec2> circle_points;

	// 生成曲线上的点
	BezierCurve({ lw4.x, lw4.y }, { lw1.x, lw1.y }, { lw2.x, lw2.y }, { lw2.x, lw2.y }, num_segments, circle_points);
	BezierCurve({ lw2.x, lw2.y }, { lw3.x, lw3.y }, { lw5.x, lw5.y }, { lw5.x, lw5.y }, num_segments, circle_points);
	BezierCurve({ lw5.x, lw5.y }, { lw8.x, lw8.y }, { lw7.x, lw7.y }, { lw7.x, lw7.y }, num_segments, circle_points);
	BezierCurve({ lw7.x, lw7.y }, { lw6.x, lw6.y }, { lw4.x, lw4.y }, { lw4.x, lw4.y }, num_segments, circle_points);
    
	// 绘制填充
	ImGui::GetBackgroundDrawList()->AddConvexPolyFilled(circle_points.data(), (int)circle_points.size(), color);
}






char *dzdz(int dzid)
{
switch (dzid)
    {
        
        case 8203:
        return "边跑边挥拳";
        break;
        
        case 1032:
        case 1033:
        return "探头";
        break;
        
        case 1040:
        case 1041:
        return "蹲着探头";
        break;
        
        case 1544:
        return "[站立]探头开镜";
        break;
        
        case 544:
        return "[趴下]开镜";
        break;
        
        case 1545:
        return "[行走]探头开镜";
        break;
        
        case 1672:
        return "[站立]探头开镜开火";
        break;
        
        case 1673:
        return "[行走]探头开镜开火";
        break;
        
        case 1160:
        return "[站立]探头开火";
        break;
        
        case 1161:
        return "[行走]探头开火";
        break;
        
        case 1168:
        return "[蹲下]探头开火";
        break;
        
        case 1169:
        return "[蹲行]探头开火";
        break;
        
        case 1552:
        return "[蹲下]探头开镜";
        break;
        
        case 1553:
        return "[蹲行]探头开镜";
        break;
        
        case 1680:
        return "[蹲下]探头开火开镜";
        break;
        
        case 1681:
        return "[蹲行]探头开火开镜";
        break;
        
        
        case 329:
        case 328:
        case 288:
      	case 272:
      	case 273:
      	case 264:
      	case 265:
    	return "换弹";
        break;
	
        case 136:
            return "开火";
        break;
        
		case 9:
            return "行走";
        break;
        
        case 524296:
            return "开车";
        break;
        
		case 11:
            return "奔跑";
        break;
        
        case 16777224:
            return "翻墙";
        break;        
        
        case 8:
            return "站立";
        break;  
        
		case 72:	
		case 73:
		case 75:
            return "跳跃";
        break;
        
        case 16:
            return "蹲着";
        break;
        
        case 32:
            return "趴着";
        break;
        
        case 16392:
        case 17416:
        case 16400:
        case 17424:
        case 16416:
        case 16456:
        case 16393:
        case 16457:
            return "拿着投掷物";
        break;
        
        
        case 520:
            return "开镜";
        break;
        
        case 65545:
        return "边走边打药";
        break;
        
        
        case 137:
        return "跑着开枪";
        break;
        
        case 200:
        case 201:
        return "跳枪";
        break;
        
        case 144:
        case 145:
        return "蹲着开枪";
        break;
        
        case 160:
        return "趴着开火";
        break;
        
        case 521:
        return "边开镜边走";
        break;
        
        case 8267:
        return "跳起来挥拳";
        break;
        
        case 8208:
        return "蹲下挥拳";
        break;
        
        case 8211:
        return "边蹲着跑边挥拳";
        break;
    
        case 35:
            return "爬行";
        break;
        
	    case 17:
        return "蹲行";
        break;
        
        case 19:
        return "蹲跑";
        break;
		
		case 268435464:
		return "跳舞";
		break;
		
		case 131072:
		case 524303:
		case 524304:
		case 524289:
            return "倒地";
        break;
        
		case 8200:
            return "挥拳";
        break;
        
        
		case 2056:
		case 2057:
		case 2059:
		case 2065:
		case 2067:
		case 2081:
		case 2080:
		case 2083:
		case 2121:
		case 2120:
		case 2123:
            return "切枪";
        break;
		
		case 4194304:
            return "游泳[未行走]";
        break;
        
        	case 4194307:
            return "游泳[已行走]";
        break;
		
		case 65544:
		case 65552:
		
		case 65568:
		return "打药";  
		break;
		
				
        default:		
            return "未知";
        break;
    }
    return nullptr;
} 

