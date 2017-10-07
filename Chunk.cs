using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class chunk
{
    private const float eps2 = 1e-3F;
    private  node root;
    private delcell[] dellist;
    private int delcount;
    private int vbocount;
    private int ebocount;
    private int[] vertstate;
    private Vector3[] newverts;
    private Vector3[,] retverts;
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
    private int sphere_obj_calc(spade sp) { return 0; }
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
        if (now.isleaf) return;
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
        vertstate = new int[4];
        newverts = new Vector3[4];
        retverts = new Vector3[3, 4];
        VBO = new Vector3[maxvbocount];
        EBO = new int[maxebocount];
    }
    public void Query(spade sp) { query(sp, root); }
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
                int newcount  = sphere_obj_calc(sp);
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
    public void Draw() { draw(root); }
}
