int sphere_obj_calc(spherecollider &sp)
{
  int i,out=-1,on=3,in=7;
  bool del=false,swpcount=false;
  float r2=sp.r*sp.r,dist2;
  for(i=0;i<4;i++)
  {
    dist2=dist2(sp.center,tempverts[i];
    if(dist2<r2-eps)
    {
      vertstate[--in]=i;
      if(in<=on){del=true;break;}
      swpcount ^= 6-in;
     }
     else if(dist2>=r2-eps&&dist2<=r2+eps)
     {
       vertstate[++on]=i;
       if(in<=on){del=true;break;}
       swpcount^=7-in;
     }
     else
     {
       vertstate[++out]=i;
       swpcount^=(on+4-in);
     }
   }
   if(del) return -1;
   int ret=0;
   switch(in)
   {
     case 6: //1in
     {
       switch(on)
       {
         case 3: //0on
         {
           if(swpcuont)
           {
             vertstate[0]^=vertstate[1];
             vertstate[1]^=vertstate[0];
             vertstate[0]^=vertstate[1];
           }
           //1 2 3 1'
           for(j=0;j<3;j++)
           {
             retvertvec[0][j]=tempverts[vertstate[j]];
             retverti[0][j]=vertregist(retvertvec[0][j]);
           }
           retvertvec[0][3]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[0][0]);
           retverti[0][3]=vertregist(retvertvec[0][3]);
           //1' 2 3 2'
           retvertvec[1][0]=retvertvec[0][3];
           retverti[1][0]=vertregist(retvertvec[1][0]);
           retvertvec[1][1]=retvertvec[0][1];
           retverti[1][1]=vertregist(retvertvec[1][1]);
           retvertvec[1][2]=retvertvec[0][2];
           retverti[1][2]=vertregist(retvertvec[1][2]);
           retvertvec[1][3]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[1][1]);
           retverti[1][3]=vertregist(retvertvec[1][3]);
           //3' 1' 3 2'
           retvertvec[2][0]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[1][2]);
           retverti[2][0]=vertregist(retvertvec[2][0]);
           retvertvec[2][1]=retvertvec[1][0];
           retverti[2][1]=vertregist(retvertvec[2][1]);
           retvertvec[2][2]=retvertvec[1][2];
           retverti[2][2]=vertregist(retvertvec[2][2]);
           retvertvec[2][3]=retvertvec[1][3];
           retverti[2][3]=vertregist(retvertvec[2][3]);
           ret=3;
           break;
         }
         case 4: //1on
         {
           if(swpcuont)
           {
             vertstate[0]^=vertstate[1];
             vertstate[1]^=vertstate[0];
             vertstate[0]^=vertstate[1];
           }
           //1 2 on 1'
           retvertvec[0][0]=tempverts[vertstate[0]];
           retverti[0][0]=vertregist(retvertvec[0][0]);
           retvertvec[0][1]=tempverts[vertstate[1]];
           retverti[0][1]=vertregist(retvertvec[0][1]);
           retvertvec[0][2]=tempverts[vertstate[4]];
           retverti[0][2]=vertregist(retvertvec[0][2]);
           retvertvec[0][3]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[0][0]);
           retverti[0][3]=vertregist(retvertvec[0][3]);
           //2 on 1' 2'
           retvertvec[1][0]=retvertvec[0][1];
           retverti[1][0]=vertregist(retvertvec[1][0]);
           retvertvec[1][1]=retvertvec[0][2];
           retverti[1][1]=vertregist(retvertvec[1][1]);
           retvertvec[1][2]=retvertvec[0][3];
           retverti[1][2]=vertregist(retvertvec[1][2]);
           retvertvec[1][3]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[1][0]);
           retverti[1][3]=vertregist(retvertvec[1][3]);
           ret=2;
           break;
         }
         case 5: //2on
         {
           if(swpcount)
           {
             vertstate[4]^=vertstate[5];
             vertstate[5]^=vertstate[4];
             vertstate[4]^=vertstate[5];
           }
           retvertvec[0][0]=tempverts[vertstate[0]];
           retverti[0][0]=vertregist(retvertvec[0][0]);
           retvertvec[0][1]=tempverts[vertstate[4]];
           retverti[0][1]=vertregist(retvertvec[0][1]);
           retvertvec[0][2]=tempverts[vertstate[5]];
           retverti[0][2]=vertregist(retvertvec[0][2]);
           retvertvec[0][3]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[0][0]);
           retverti[0][3]=vertregist(retvertvec[0][3]);
           ret = 1;
           break;
         }
       }
       break;
     }
     case 5: //2in
     {
       if(swpcount)
       {
           vertstate[5]^=vertstate[6];
           vertstate[6]^=vertstate[5];
           vertstate[5]^=vertstate[6];
       }
       if(on==4) //1 on
       {
         retvertvec[0][0]=tempverts[vertstate[0]];
         retverti[0][0]=vertregist(retvertvec[0][0]);
         retvertvec[0][1]=tempverts[vertstate[4]];
         retverti[0][1]=vertregist(retvertvec[0][1]);
         retvertvec[0][2]=sphere_calc(sp,tempverts[vertstate[5]],retvertvec[0][0]);
         retverti[0][2]=vertregist(retvertvec[0][2]);
         retvertvec[0][3]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[0][0]);
         retverti[0][3]=vertregist(retvertvec[0][3]);
         ret = 1;
         break;
       }
       //0 on
       //1 2 1in1 1in2
       retvertvec[0][0]=tempverts[vertstate[0]];
       retverti[0][0]=vertregist(retvertvec[0][0]);
       retvertvec[0][1]=tempverts[vertstate[1]];
       retverti[0][1]=vertregist(retvertvec[0][1]);
       retvertvec[0][2]=sphere_calc(sp,tempverts[vertstate[5]],retvertvec[0][0]);
       retverti[0][2]=vertregist(retvertvec[0][2]);
       retvertvec[0][3]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[0][0]);
       retverti[0][3]=vertregist(retvertvec[0][3]);
       //2 1in1 1in2 2in2
       retvertvec[1][0]=retvertvec[0][1];
       retverti[1][0]=vertregist(retvertvec[1][0]);
       retvertvec[1][1]=retvertvec[0][2];
       retverti[1][1]=vertregist(retvertvec[1][1]);
       retvertvec[1][2]=retvertvec[0][3]
       retverti[1][2]=vertregist(retvertvec[1][2]);
       retvertvec[1][3]=sphere_calc(sp,tempverts[vertstate[6]],retvertvec[1][0]);
       retverti[1][3]=vertregist(retvertvec[1][3]);
       //2 1in1 2in2 2in1
       retvertvec[2][0]=retvertvec[1][0];
       retverti[2][0]=vertregist(retvertvec[2][0]);
       retvertvec[2][1]=retvertvec[1][1];
       retverti[2][1]=vertregist(retvertvec[2][1]);
       retvertvec[2][2]=retvertvec[1][3]
       retverti[2][2]=vertregist(retvertvec[2][2]);
       retvertvec[2][3]=sphere_calc(sp,tempverts[vertstate[5]],retvertvec[2][0]);
       retverti[2][3]=vertregist(retvertvec[2][3]);
       ret = 3;
       break;
     }
     case 4: //3in
     {
       if(swpcount)
       {
         vertstate[4]^=vertstate[5];
         vertstate[5]^=vertstate[4];p
         vertstate[4]^=vertstate[5];
       }
       retvertvec[0][0]=tempverts[vertstate[0]];
       retverti[0][0]=vertregist(retvertvec[0][0]);
       for(j=1;j<=3;j++)
       {
         retvertvec[0][j] = sphere_calc(sp,tempverts[vertstate[j+3]],retvertvec[0][0]);
         retverti[0][j]=vertregist(retvertvec[0][j]);
       }
       ret=1;
       break;
     }
   }
   return ret;
}
