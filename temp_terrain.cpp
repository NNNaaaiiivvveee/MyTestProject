#include"ex_terrain_base.h"

/////////////sphere_obj_calc need
static vec3 tempvertsvec[4];//present node's verts
static int tempvertsi[4];//present node's verts' index
static int vertstate[7];//ertstate used to calc new verts 0to3out 4to7onin
static vec3 newvertsvec[4];//new verts
static int newvertsi[4];//new verts index
static vec3 retvertsvec[3][4];//new objects
static int retvertsi[3][4];//new objects index
/////////////////////////////////////////////////

int terrain_lod_level;//change

terrain_lod terrain_LOD[MAX_TERRAIN_LOD_LEVEL];

int prossess_count;

obj_tobe_process obj_prossess_queue[MAX_PROSSESS_COUNT];

inline float get_maxdist(int deep)
{
	int l = 0, r = terrain_lod_level - 1, mid;
	float maxdist;
	while (l <= r)///bugs?
	{
		mid = (l + r) >> 1;
		if (terrain_LOD[mid].l <= deep&&deep <= terrain_LOD[mid].r)
		{
			maxdist = terrain_LOD[mid].maxidst;
			break;
		}
		if (deep < terrain_LOD[mid].l) { r = mid; }
		else { l = mid + 1; }
	}
	return maxdist;
}

vec3 sphere_calc(const sphere_collider & sp, const vec3 & in, const vec3 & out)
{
	vec3 io = out - in;
	vec3 ci = in - sp.center;
	float io2 = io * io;
	float ioci = io * ci * 2;
	float ci2 = ci * ci - sp.radius * sp.radius;
	float ans = 1,f,df;
	while (1)
	{
		f = io2 * ans * ans + ioci * ans + ci2;
		if (f <= eps2) break;
		df = 2 * io2 * ans + ioci;
		ans = ans - f / df;
	}
	vec3 ret;
	ret.x = in.x + io.x * ans;
	ret.y = in.y + io.y * ans;
	ret.z = in.z + io.z * ans;
	return ret;
}

int Terrain_Chunk::sphere_obj_calc(const sphere_collider & sp)
{
	int i, out = -1, on = 3, in = 7;
	bool del = false, swpcount = false;
	float r2 = sp.radius*sp.radius, dist2;
	for (i = 0;i<4;i++)
	{
		dist2 = vec3_dist2(sp.center, tempvertsvec[i]);
		if (dist2<r2 - eps2)
		{
			vertstate[--in] = i;
			if (in <= on) { del = true;break; }
			swpcount ^= (6 - in);
		}
		else if (dist2 >= r2 - eps2 && dist2 <= r2 + eps2)
		{
			vertstate[++on] = i;
			if (in <= on) { del = true;break; }
			swpcount ^= (7 - in);
		}
		else
		{
			vertstate[++out] = i;
			swpcount ^= (on + 4 - in);
		}
	}
	if (del) return -1;
	int ret = 0;
	switch (in)
	{
	case 6: //1in
	{
		switch (on)
		{
		case 3: //0on
		{
			if (swpcount) intswap(vertstate[0],vertstate[1]);
			//1 2 3 1'
			retvertsvec[0][0] = tempvertsvec[vertstate[0]];
			retvertsi[0][0] = vertregist(retvertsvec[0][0]);
			retvertsvec[0][1] = tempvertsvec[vertstate[1]];
			retvertsi[0][1] = vertregist(retvertsvec[0][1]);
			retvertsvec[0][2] = tempvertsvec[vertstate[2]];
			retvertsi[0][2] = vertregist(retvertsvec[0][2]);
			retvertsvec[0][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[0][0]);
			retvertsi[0][3] = vertregist(retvertsvec[0][3]);
			//1' 2 3 2'
			retvertsvec[1][0] = retvertsvec[0][3];
			retvertsi[1][0] = vertregist(retvertsvec[1][0]);
			retvertsvec[1][1] = retvertsvec[0][1];
			retvertsi[1][1] = vertregist(retvertsvec[1][1]);
			retvertsvec[1][2] = retvertsvec[0][2];
			retvertsi[1][2] = vertregist(retvertsvec[1][2]);
			retvertsvec[1][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[1][1]);
			retvertsi[1][3] = vertregist(retvertsvec[1][3]);
			//3' 1' 3 2'
			retvertsvec[2][0] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[1][2]);
			retvertsi[2][0] = vertregist(retvertsvec[2][0]);
			retvertsvec[2][1] = retvertsvec[1][0];
			retvertsi[2][1] = vertregist(retvertsvec[2][1]);
			retvertsvec[2][2] = retvertsvec[1][2];
			retvertsi[2][2] = vertregist(retvertsvec[2][2]);
			retvertsvec[2][3] = retvertsvec[1][3];
			retvertsi[2][3] = vertregist(retvertsvec[2][3]);
			ret = 3;
			break;
		}
		case 4: //1on
		{
			if (swpcount) intswap(vertstate[0], vertstate[1]);
			//1 2 on 1'
			retvertsvec[0][0] = tempvertsvec[vertstate[0]];
			retvertsi[0][0] = vertregist(retvertsvec[0][0]);
			retvertsvec[0][1] = tempvertsvec[vertstate[1]];
			retvertsi[0][1] = vertregist(retvertsvec[0][1]);
			retvertsvec[0][2] = tempvertsvec[vertstate[4]];
			retvertsi[0][2] = vertregist(retvertsvec[0][2]);
			retvertsvec[0][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[0][0]);
			retvertsi[0][3] = vertregist(retvertsvec[0][3]);
			//2 on 1' 2'
			retvertsvec[1][0] = retvertsvec[0][1];
			retvertsi[1][0] = vertregist(retvertsvec[1][0]);
			retvertsvec[1][1] = retvertsvec[0][2];
			retvertsi[1][1] = vertregist(retvertsvec[1][1]);
			retvertsvec[1][2] = retvertsvec[0][3];
			retvertsi[1][2] = vertregist(retvertsvec[1][2]);
			retvertsvec[1][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[1][0]);
			retvertsi[1][3] = vertregist(retvertsvec[1][3]);
			ret = 2;
			break;
		}
		case 5: //2on
		{
			if (swpcount) intswap(vertstate[4], vertstate[5]);
			retvertsvec[0][0] = tempvertsvec[vertstate[0]];
			retvertsi[0][0] = vertregist(retvertsvec[0][0]);
			retvertsvec[0][1] = tempvertsvec[vertstate[4]];
			retvertsi[0][1] = vertregist(retvertsvec[0][1]);
			retvertsvec[0][2] = tempvertsvec[vertstate[5]];
			retvertsi[0][2] = vertregist(retvertsvec[0][2]);
			retvertsvec[0][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[0][0]);
			retvertsi[0][3] = vertregist(retvertsvec[0][3]);
			ret = 1;
			break;
		}
		}
		break;
	}
	case 5: //2in
	{
		if (swpcount) intswap(vertstate[5], vertstate[6]);
		if (on == 4) //1 on
		{
			retvertsvec[0][0] = tempvertsvec[vertstate[0]];
			retvertsi[0][0] = vertregist(retvertsvec[0][0]);
			retvertsvec[0][1] = tempvertsvec[vertstate[4]];
			retvertsi[0][1] = vertregist(retvertsvec[0][1]);
			retvertsvec[0][2] = sphere_calc(sp, tempvertsvec[vertstate[5]], retvertsvec[0][0]);
			retvertsi[0][2] = vertregist(retvertsvec[0][2]);
			retvertsvec[0][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[0][0]);
			retvertsi[0][3] = vertregist(retvertsvec[0][3]);
			ret = 1;
			break;
		}
		//0 on
		//1 2 1in1 1in2
		retvertsvec[0][0] = tempvertsvec[vertstate[0]];
		retvertsi[0][0] = vertregist(retvertsvec[0][0]);
		retvertsvec[0][1] = tempvertsvec[vertstate[1]];
		retvertsi[0][1] = vertregist(retvertsvec[0][1]);
		retvertsvec[0][2] = sphere_calc(sp, tempvertsvec[vertstate[5]], retvertsvec[0][0]);
		retvertsi[0][2] = vertregist(retvertsvec[0][2]);
		retvertsvec[0][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[0][0]);
		retvertsi[0][3] = vertregist(retvertsvec[0][3]);
		//2 1in1 1in2 2in2
		retvertsvec[1][0] = retvertsvec[0][1];
		retvertsi[1][0] = vertregist(retvertsvec[1][0]);
		retvertsvec[1][1] = retvertsvec[0][2];
		retvertsi[1][1] = vertregist(retvertsvec[1][1]);
		retvertsvec[1][2] = retvertsvec[0][3];
		retvertsi[1][2] = vertregist(retvertsvec[1][2]);
		retvertsvec[1][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[1][0]);
		retvertsi[1][3] = vertregist(retvertsvec[1][3]);
		//2 1in1 2in2 2in1
		retvertsvec[2][0] = retvertsvec[1][0];
		retvertsi[2][0] = vertregist(retvertsvec[2][0]);
		retvertsvec[2][1] = retvertsvec[1][1];
		retvertsi[2][1] = vertregist(retvertsvec[2][1]);
		retvertsvec[2][2] = retvertsvec[1][3];
		retvertsi[2][2] = vertregist(retvertsvec[2][2]);
		retvertsvec[2][3] = sphere_calc(sp, tempvertsvec[vertstate[5]], retvertsvec[2][0]);
		retvertsi[2][3] = vertregist(retvertsvec[2][3]);
		ret = 3;
		break;
	}
	case 4: //3in
	{
		if (swpcount)intswap(vertstate[4], vertstate[5]);
		retvertsvec[0][0] = tempvertsvec[vertstate[0]];
		retvertsi[0][0] = vertregist(retvertsvec[0][0]);
		retvertsvec[0][1] = sphere_calc(sp, tempvertsvec[vertstate[4]], retvertsvec[0][0]);
		retvertsi[0][1] = vertregist(retvertsvec[0][1]);
		retvertsvec[0][2] = sphere_calc(sp, tempvertsvec[vertstate[5]], retvertsvec[0][0]);
		retvertsi[0][2] = vertregist(retvertsvec[0][2]);
		retvertsvec[0][3] = sphere_calc(sp, tempvertsvec[vertstate[6]], retvertsvec[0][0]);
		retvertsi[0][3] = vertregist(retvertsvec[0][3]);
		ret = 1;
		break;
	}
	}
	return ret;
}//to be polish

void Terrain_Chunk::layertree_del(tetrahedron_node * now)
{
	if (now->isleaf)
	{
		//gen drop
		//del verts
		//del box
		free(now);
		return;
	}
	if (now->ch[0]) layertree_del(now->ch[0]);
	if (now->ch[1]) layertree_del(now->ch[1]);
	if (now->ch[2]) layertree_del(now->ch[2]);
	//del verts
	free(now);
}

void  Terrain_Chunk::layertree_update(tetrahedron_node*now)
{
	int chcount = 0,i;
	tetrahedron_node* ch;
	for (i = 0;i < 3;i++)
	{
		if (now->ch[i] != NULL)
		{
			chcount++;
			ch = now->ch[i];
		}
	}
	if (!chcount)
	{
		for (i = 0;i < 3;i++)
		{
			if (now == now->fa->ch[i])
			{
				now->fa->ch[i] = NULL;
				break;
			}
		}//unlock father and son
		layertree_update(now->fa);
		//del verts
		free(now);
		return;
	}
	if (chcount == 1)
	{
		for (i = 0;i < 3;i++)
		{
			if (now == now->fa->ch[i])
			{
				now->fa->ch[i] = ch;
				break;
			}
		}//link father and myson
		//del verts
		free(now);
		return;
	}
}

inline void Terrain_Chunk::get_verts_attrib(tetrahedron_node*now)
{
	tempvertsi[0] = now->tetrahedron_obj[0];
	tempvertsvec[0] = (*vbo)[tempvertsi[0]].pos;//this will be changed when release
	tempvertsi[1] = now->tetrahedron_obj[1];
	tempvertsvec[1] = (*vbo)[tempvertsi[1]].pos;//this will be changed when release
	tempvertsi[2] = now->tetrahedron_obj[2];
	tempvertsvec[2] = (*vbo)[tempvertsi[2]].pos;//this will be changed when release
	tempvertsi[3] = now->tetrahedron_obj[3];
	tempvertsvec[3] = (*vbo)[tempvertsi[3]].pos;//this will be changed when release
}

void Terrain_Chunk::draw_tetrahedron_node(const vec3 & pos, const vec3 & dir, tetrahedron_node * now, bool test, int deep)
{
	deep++;
	get_verts_attrib(now);
	if (!tempvertsi[0]) //a vert is inf away
	{
		if (test)
		{
			int st = 0;
			st += (((tempvertsvec[1] - pos)*dir) > 0);
			st += (((tempvertsvec[2] - pos)*dir) > 0);
			st += (((tempvertsvec[3] - pos)*dir) > 0);
			if (!st) return;
			test = !(st == 3);
		}
		if (now->isleaf)//opengldraw
		{
			return;
		}
		/////////////////deep test
		float maxdist = get_maxdist(deep);
		bool toofar = true;
		for (int i = 1;i <= 3;i++)
		{
			if (vec3_dist2(pos, tempvertsvec[i]) < maxdist)
			{
				toofar = false;
				break;
			}
		}
		if (toofar)
		{
			//opengldraw
			return;
		}
		/////////////test end
	}
	else
	{
		if (test)
		{
			int st = 0;
			st += (((tempvertsvec[0] - pos)*dir) > 0);
			st += (((tempvertsvec[1] - pos)*dir) > 0);
			st += (((tempvertsvec[2] - pos)*dir) > 0);
			st += (((tempvertsvec[3] - pos)*dir) > 0);
			if (!st) return;
			test = !(st == 4);
		}
		if (now->isleaf)//opengldraw
		{
			return;
		}
		/////////////////deep test
		float maxdist = get_maxdist(deep);
		bool toofar = true;
		for (int i = 0;i <= 3;i++)
		{
			if (vec3_dist2(pos, tempvertsvec[i]) < maxdist)
			{
				toofar = false;
				break;
			}
		}
		if (toofar)
		{
			//opengldraw
			return;
		}
		/////////////test end
	}
	if (now->ch[0]) draw_tetrahedron_node(pos, dir, now->ch[0], test, deep);
	if (now->ch[1]) draw_tetrahedron_node(pos, dir, now->ch[1], test, deep);
	if (now->ch[2]) draw_tetrahedron_node(pos, dir, now->ch[2], test, deep);
}

float Terrain_Chunk::query_tetrahedron_node(const sphere_collider & sp, tetrahedron_node * now)
{
	if (sp.hardness < now->hardness) return 0.0f;
	get_verts_attrib(now);
	if (!tempvertsi[0])//-inf
	{
		bool cross = false;
		for (int i = 1;i <= 3;i++)
		{
			if (vec3_dist(tempvertsvec[i], sp.center) < sp.expand_radius)
			{
				cross = true;
				break;
			}
		}
		if (!cross) return 0.0f;
	}
	else
	{
		int counti = 0;
		for (int i = 0;i <= 3;i++)
		{
			if (vec3_dist(tempvertsvec[i], sp.center) > sp.expand_radius)
			{
				counti++;
			}
		}
		if (!counti)
		{
			obj_prossess_queue[prossess_count].isdel = true;
			obj_prossess_queue[prossess_count++].obj = now;
			return hardness_calc();
		}
		if (counti == 4) return 0.0f;
	}
	if (now->isleaf)
	{
		obj_prossess_queue[prossess_count].isdel = false;
		obj_prossess_queue[prossess_count++].obj = now;
		return hardness_calc();
	}
	if (now->ch[0]) query_tetrahedron_node(sp, now->ch[0]);
	if (now->ch[1]) query_tetrahedron_node(sp, now->ch[1]);
	if (now->ch[2]) query_tetrahedron_node(sp, now->ch[2]);
	return hardness_calc();
}

int Terrain_Chunk::dig_tetrahedron_node(const sphere_collider & sp)
{
	int i, j;
	for (i = 0;i <= prossess_count;i++)
	{
		tetrahedron_node* now = obj_prossess_queue[i].obj;
		if (obj_prossess_queue[i].isdel)
		{
			for (j = 0;j < 3;j++)
			{
				if (now == now->fa->ch[j])
				{
					now->fa->ch[j] = NULL;
					break;
				}
			}//unlock father and son
			layertree_update(now->fa);
			layertree_del(now);
		}
		else
		{
			get_verts_attrib(now);
			int newcount = sphere_obj_calc(sp);
			if (newcount >= 1)
			{
				now->isleaf = false;
				//del box
				switch (newcount)// caution! if now is -inf hardness should be recalculate
				{
					case 1 :
					{
						now->ch[0] = &tetrahedron_node(retvertsi[0],now,now->hardness);
						break;
					}
					case 2 :
					{
						now->ch[0] = &tetrahedron_node(retvertsi[0], now, now->hardness);
						now->ch[1] = &tetrahedron_node(retvertsi[1], now, now->hardness);
						break;
					}
					case 3 :
					{
						now->ch[0] = &tetrahedron_node(retvertsi[0], now, now->hardness);
						now->ch[1] = &tetrahedron_node(retvertsi[1], now, now->hardness);
						now->ch[2] = &tetrahedron_node(retvertsi[2], now, now->hardness);
						break;
					}
				}
			}
		}
		obj_prossess_queue[i].obj = NULL;
	}
	prossess_count = 0;
}

int Terrain_Chunk:: VBOM_alloc(VBO_manager & VBOM)
{
	if (!VBOM[0]) return 0;
	return VBOM[VBOM[0]--];
}

int Terrain_Chunk:: VBOM_free(VBO_manager & VBOM, int id)
{
	VBOM[++VBOM[0]] = id;
	return VBOM[0];
}

int Terrain_Chunk:: VBOM_obj_alloc(VBO_manager & VBOM, int objcount)
{
	
}
