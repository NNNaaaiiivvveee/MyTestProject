using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class chunk
{
    private const float eps2 = 1e-3F;
    private  node root;// uninit!!!!!!!!!!
    private delcell[] dellist;
    private int delcount;
    private int vbocount;
    private int ebocount;
    private int[] vertstate;
    private Vector3[] newverts;
    private Vector3[,] retverts;
    private void intswap(int a,int b)
    {
        int c = vertstate[a];
        vertstate[a] = vertstate[b];
        vertstate[b] = c;
    }
    private Vector3 sphere_calc(spade sp, Vector3 In, Vector3 Out)
    {
        Vector3 io = Out - In;
        Vector3 ci = In -sp.pos;
        float io2 = Vector3.Dot(io, io);
        float ioci = Vector3.Dot(io, ci) * 2;
        float ci2 = Vector3.Dot(ci,ci) - sp.radius * sp.radius;
        float ans = 1, f, df;
        while (true)
        {
            f = io2 * ans * ans + ioci * ans + ci2;
            if (f <= eps2) break;
            df = 2 * io2 * ans + ioci;
            ans = ans - f / df;
        }
        Vector3 ret = new Vector3();
        ret.x = In.x + io.x * ans;
        ret.y = In.y + io.y * ans;
        ret.z = In.z + io.z * ans;
        return ret;
    }
    private int sphere_obj_calc(spade sp,node now)//hello world
    {
        bool del = false;
        int swpcount = 0;
        int outp = -1,onp=3,inp=7;
        float r2 = sp.radius * sp.radius,dist2;
        Vector3 gap;
        for(int i=0;i<4;i++)
        {
            gap = sp.pos - now.verts[i];
            dist2 = Vector3.Dot(gap,gap);
            if(dist2<r2-eps2)
            {
                vertstate[--inp] = i;
                if (inp <= onp) { del = true;break; }
                swpcount ^= (6 - inp);
            }
            else if (dist2 >= r2 - eps2 && dist2 <= r2 + eps2)
            {
                vertstate[++onp] = i;
                if (inp <= onp) { del = true; break; }
                swpcount ^= (7 - inp);
            }
    		else
		    {
			    vertstate[++outp] = i;
			    swpcount ^= (onp + 4 - inp);
		    }
        }
        if (del) return -1;
        int ret = 0;
        switch (inp)
	    {
	    case 6: //1in
	    {
                switch (onp)
                {
                    case 3: //0on
                        {
                            if (swpcount==1) intswap(0,1);
                            ret = 3;
                                for (int i = 0; i < 3; i++) retverts[0, i] = now.verts[vertstate[i]];
                                retverts[0, 3] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[0]]);
                                for (int i = 0; i < 3; i++) { retverts[1, i] = retverts[0, i + 1];}
                                retverts[1, 3] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[1]]);
                                for(int i = 0; i < 3; i++) { retverts[2, i] = retverts[1, i + 1]; }
                                retverts[2, 3] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[2]]);
                            break;
                        }
                    case 4: //1on
                        {
                            if (swpcount==1) intswap(0,1);
                            ret = 2;
                                retverts[0, 0] = now.verts[vertstate[0]];
                                retverts[0, 1] = now.verts[vertstate[1]];
                                retverts[0, 2] = now.verts[vertstate[4]];
                                retverts[0, 3] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[0]]);
                                for(int i = 0; i < 3; i++) { retverts[1, i] = retverts[0, i + i]; }
                                retverts[1, 3] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[1]]);
                            break;
                        }
                    case 5: //2on
                        {
                            if (swpcount==1) intswap(4,5);
                            ret = 1;
                                retverts[0, 0] = now.verts[vertstate[0]];
                                retverts[0, 1] = now.verts[vertstate[4]];
                                retverts[0, 2] = now.verts[vertstate[5]];
                                retverts[0, 3] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[0]]);
                            break;
                        }
                }
                break;
         }
	    case 5: //2in
	    {
                if (swpcount==1) intswap(5,6);
                if (onp == 4) //1 on
                {
                    ret = 1;
                        retverts[0, 0] = now.verts[vertstate[0]];
                        retverts[0, 1] = now.verts[vertstate[4]];
                        retverts[0, 2] = sphere_calc(sp, now.verts[vertstate[5]], now.verts[vertstate[0]]);
                        retverts[0, 3] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[0]]);
                    break;
                }
                //0 on
                ret = 3;
                    newverts[0] = sphere_calc(sp, now.verts[vertstate[5]], now.verts[vertstate[0]]);
                    newverts[1] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[0]]);
                    newverts[2] = sphere_calc(sp, now.verts[vertstate[5]], now.verts[vertstate[1]]);
                    newverts[3] = sphere_calc(sp, now.verts[vertstate[6]], now.verts[vertstate[1]]);
                    retverts[0, 0] = newverts[3];
                    retverts[0, 1] = newverts[2];
                    retverts[0, 2] = now.verts[vertstate[1]];
                    retverts[0, 3] = newverts[1];
                    for (int i = 0; i < 3; i++) { retverts[1, i] = retverts[0, i + 1]; }
                    retverts[1, 3] = newverts[0];
                    for(int i = 0; i < 3; i++) { retverts[2, i] = retverts[1, i + 1]; }
                    retverts[2, 3] = now.verts[vertstate[0]];
                break;
         }
	    case 4: //3in
	    {
                if (swpcount==1) intswap(4,5);
                ret = 1;
                for(int i=0;i<3;i++)
                    {
                        retverts[0,i+1] = sphere_calc(sp, now.verts[vertstate[i+4]], now.verts[vertstate[0]]);
                    }
                    retverts[0, 0] = now.verts[vertstate[0]];
                break;
            }
        }
        return ret;
    }
    private void update(node now)
    {
        int soncount = 0;
        for (int i = 0; i < 3; i++)
        {
            if (now.son[i] != null) soncount++;
        }
        if(soncount==0)
        {
            node fa = now.fa;
            fa.isleaf = true;
            for(int j=0;j<3;j++)
            {
                if(now==fa.son[j])
                {
                    fa.son[j] = null;
                    break;
                }
            }
            update(fa);
            return;
        }
        if(soncount==1)
        {
            node fa = now.fa; node son = now.son[0];
            for(int j=1;j<3;j++)
            {
                if(now.son[j]!=null)
                {
                    son = now.son[j];
                    break;
                }
            }
            for(int j=0;j<3;j++)
            {
                if(now==fa.son[j])
                {
                    fa.son[j] = son;
                    break;
                }
            }
        }
    }
    private void query(spade sp, node now)
    {
        int outcount = 0;
        float R2 = sp.radius * sp.radius;
        Vector3 gap;
        for (int i = 0; i < 4; i++)
        {
            gap = sp.pos - now.verts[i];
            if (Vector3.Dot(gap, gap) > R2 + eps2)
            {
                outcount++;
            }
        }
        MonoBehaviour.print(outcount);
        if (outcount == 4) return;
        if (outcount == 0)
        {
            dellist[delcount].nd = now;
            dellist[delcount].state = delcell.STA.DEL;
            delcount++;
            return;
        }
        if (now.isleaf)
        {
            dellist[delcount] = new delcell();
            dellist[delcount].nd = now;
            dellist[delcount].state = delcell.STA.CUT;
            delcount++;
            return;
        }
        for (int i = 0; i < 3; i++)
        {
            if (now.son[i] != null) query(sp, now.son[i]);
        }
    }
    private void draw(node now)
    {
        if(now.isleaf)
        {
            int st = vbocount;
            for (int i = 0; i < 4; i++)
            {
                VBO[vbocount] = now.verts[i];
                vbocount++;
            }
            EBO[ebocount++] = st; EBO[ebocount++] = st + 1; EBO[ebocount++] = st + 2;//tri1
            EBO[ebocount++] = st + 1; EBO[ebocount++] = st; EBO[ebocount++] = st + 3;//tri1
            EBO[ebocount++] = st + 2; EBO[ebocount++] = st; EBO[ebocount++] = st + 3;//tri1
            EBO[ebocount++] = st + 2; EBO[ebocount++] = st + 1; EBO[ebocount++] = st + 3;//tri1
            return;
        }
        for (int i = 0; i < 3; i++)
        {
            if (now.son[i] != null) draw(now.son[i]);
        }
    }
    public int Vbocount
    {
        get { return vbocount; }
    }
    public int Ebocount
    {
        get { return ebocount; }
    }
    public Vector3[] VBO;
    public int[] EBO;
    public chunk(int maxdellistcount,int maxvbocount,int maxebocount)
    {
        dellist = new global::delcell[maxdellistcount];
        delcount = vbocount = ebocount = 0;
        vertstate = new int[7];
        newverts = new Vector3[4];
        retverts = new Vector3[3, 4];
        VBO = new Vector3[maxvbocount];
        EBO = new int[maxebocount];
        retverts[0, 0] = new Vector3(0,0,10);
        retverts[0, 1] = new Vector3(0,10,0);
        retverts[0, 2] = new Vector3(0,0,0);
        retverts[0, 3] = new Vector3(10,0,0);
        root = new node(null, retverts, 0);
    }
    public void Query(spade sp)
    {
        delcount = 0;
        query(sp, root);
    }
    public void process(spade sp)
    {
        node now;
        for(int i=0;i<delcount;i++)
        {
            now = dellist[i].nd;
            if(dellist[i].state==delcell.STA.DEL)
            {
                now.fa.isleaf = true;
                for (int j = 0; j < 3; j++)
                {
                    if(now==now.fa.son[j])
                    {
                        now.fa.son[j] = null;
                        break;
                    }
                }
                update(now.fa);
            }
            else
            {
                int newcount  = sphere_obj_calc(sp,now);
                if(newcount!=0)
                {
                    now.isleaf = false;
                    for(int j=0;j<newcount;j++)
                    {
                        now.son[j] = new global::node(now,retverts,j);
                    }
                }
            }
            dellist[i].nd = null;
        }
        delcount = 0;
    }
    public void Draw()
    {
        vbocount = ebocount = 0;
        draw(root);
    }
}
