
// Abstract : test all of 6 functions of ECCIP automatically ,p(x)=x^233+x^74+1 
// Modification History:
//    Date           By           Version     Change Description                                           
// ===============================================================================================
// Nov.24,2017		ghan			1.0   
// ===============================================================================================

//notes:


#include "systemc.h"                                        
#include "stdio.h"       
#include "time.h"  

#define m  255    
#define size 233


//************************SetValueToFile function****************************************************************	                        
                                                                                                                        

void  SetValueToFile(FILE *pFile,sc_biguint<256> temp)                                                                 
{                                                                                                                       
	                                                                                                                
	sc_uint<8>  p[32];   
	
	for (int j = 0; j < 32; j++)
	{
		p[j] = temp.range(8 * j + 7, 8 * j);
	}
	//************* make sure ouput order****************

	for(int i=0;i<32;i++)                                                                                          
	{                                                                                                               
		
		if(i%4==0)
				fprintf(pFile,"%2.2x",(unsigned int)p[i+3]); 
		else if(i%4==1)
				fprintf(pFile,"%2.2x", (unsigned int)p[i+1]);
		else if(i%4==2)
				fprintf(pFile,"%2.2x", (unsigned int)p[i-1]);
		else if(i%4==3)
				fprintf(pFile,"%2.2x\n", (unsigned int)p[i-3]);
		
				
	}                                                                                                                                                                                                                               	                                                                                                                
}  

//****************************************************************************************	

//*************************************GetValueFromFile2048 function**********************************  

sc_biguint<256> GetValueFromFile(FILE *pFile)                                                                          
{                                                                                                                       
                                                                                                                        
	sc_biguint<256>	temp;                                                                                   
	unsigned int 		array[8];                                                                        
		                                                                                                        
	for(int j=0; j<8; j++)                                                                                   
	{                                                                                                               
	  	fscanf(pFile, "%lX", array+j);                                                                          
	  	temp.range(32*j+31,32*j)=array[j];                                                                         
	 }                                                                                                              
	                                                                                                                
	 return temp;                                                                                                   
}                       
                                                                                                                        
//****************************************************************************************	

//****************************************************************************************	

sc_biguint<m+1> Inverse( const sc_biguint<m+1> x, const sc_biguint<m+1> p )                                                           
{                                                                                                                       
	sc_biguint<m+1>G0,G1,G2;                                                                                       
	sc_biguint<m+1>V0,V1,V2;                                                                                       
	G0=p;	G1=x;                                                                                                   
	V0=0;	V1=1;                                                                                                   
                                                                                                                        
	for(int i=1;;i++)                                                                                               
	{                                                                                                               
		G2=G0 % G1;//gcd£®x,p£©                                                                                             
		V2=V0+(G0/G1)*V1;//ti=t(i-2)+q(i-1)t(i-1)                                                                                       
		if (G2==1)                                                                                              
		{    
			if(i%2==1)
			{
				return p-V2;                                                                                      
				break;                                                                                          
			}
			else
			{				
				return V2;
				break;
			}
		}                                                                                                       
		else                                                                                                    
		{	                                                                                                
			G0=G1;                                                                                          
			G1=G2;                                                                                          
			V0=V1;                                                                                          
			V1=V2;                                                                                          
		}                                                                                                       
	                                                                                                                
			                                                                                                
	}                                                                                                               
        //  return V2;                                                                                                    
                                                                                                                        
}                                                                                                                       

//************************ Integer y/x mod p function **********************************************                            

                                                                                                                       
sc_biguint<m+1> int_inverse(  sc_biguint<m+1> y,  sc_biguint<m+1> x,  sc_biguint<m+1> p )             
{ 
	
	sc_biguint<m+1> result;  
  result=(y*Inverse(x,p))%p;
  
  return result;
 
          
}



//************************ Inverse y/x mod p function **********************************************                              
                                                                                                                        
sc_biguint<m+1> mod_inverse(  sc_biguint<m+1> y,  sc_biguint<m+1> x,  sc_biguint<m+1> p )             
{                                                                                                                       
                                          
	
	sc_biguint<m+1>	A;
	sc_biguint<m+1>	B;
	sc_biguint<m+1>	U;
	sc_biguint<m+1>	V;
	sc_biguint<m+1>	Inverse;
	
	sc_biguint<m+1>	temp;
	
   
	A=x;
	B=p;
	U=y;
	V=0;
	
	for(;;)
	{
		if(A!=B)
		{
			if(A[0]==0)
			{
				A=A/2;
				if(U[0]==0)
					U=U/2;
				else
					U=(U^p)/2;
			}
			else	if(B[0]==0)
				{
					B=B/2;
					if(V[0]==0)
						V=V/2;
					else
						V=(V^p)/2;
				}
			else	if(A>B)
				{
					A=(A^B)/2;
					U=U^V;
					if(U[0]==0)
						U=U/2;
					else
						U=(U^p)/2;
				}
			else
			{
				B=(A^B)/2;
				V=U^V;
				if(V[0]==0)
					V=V/2;
				else
					V=(V^p)/2;
			}
		}
		else
		{
			Inverse=U;
			break;
		}
	}
	return Inverse;
}
				
				
			
//****************************************************************************************	  
                    	                        
//************************ abmodp function **********************************************                              
                                                                                                                        
sc_biguint<m+1> abmodp(sc_biguint<m+1> a,  sc_biguint<m+1> b,  sc_biguint<m+1> p )             
{                                                                                                                       
        
	sc_biguint<m+1>	c;
	
	sc_biguint<size>	constant;

	for(int w=0;w<size;w++)
		constant[w]=1;
	
	if(a[0]==1)
		c=b;
	else
		c=0;
	for(int i=1;i<size;i++)
	{
		
		if((b*2)<constant) 
			b=b*2;
		else
			b=(b*2)^p;
			
		if(a[i]==1)
			c=c^b;
	}
	
	
	return c;
}

//****************************************************************************************


         

//****************************************************************************************


//************************ doublePointX function **********************************************                              
                                                                                                                        
sc_biguint<m+1> doublePointX(sc_biguint<m+1> x1,  sc_biguint<m+1> y1,  sc_biguint<m+1> p,sc_biguint<m+1> a )             
{                                                                                                                       
       
	sc_biguint<m+1>	Lan;
	
	Lan=x1^(mod_inverse(y1,x1,p));    
	
	sc_biguint<m+1>	x3;
	
	x3=(abmodp(Lan,Lan,p))^Lan^a;
	
	return x3;
	
	
}
//****************************************************************************************




//************************ doublePointY function **********************************************                              
                                                                                                                        
sc_biguint<m+1> doublePointY(sc_biguint<m+1> x1,  sc_biguint<m+1> y1,  sc_biguint<m+1> p,sc_biguint<m+1> a )             
{                                                                                                                       
       
	sc_biguint<m+1>	Lan;
	
	Lan=x1^(mod_inverse(y1,x1,p));    
	
	sc_biguint<m+1>	x3;
	
	x3=(abmodp(Lan,Lan,p))^Lan^a;
	
	sc_biguint<m+1> y3;

	
	  
	y3=(abmodp(x1,x1,p))^(abmodp(Lan,x3,p))^x3;
	
	return y3;

}
//****************************************************************************************		

//************************ pointAddX function **********************************************                              
                                                                                                                        
sc_biguint<m+1> pointAddX(   sc_biguint<m+1> x1,  sc_biguint<m+1> y1, 
			     sc_biguint<m+1> x2,  sc_biguint<m+1> y2,
			     sc_biguint<m+1> p,sc_biguint<m+1> a )             
{                                                                                                                       
       
	sc_biguint<m+1>	Lan;
	
	Lan=mod_inverse((y1^y2),(x1^x2),p);    
	
	sc_biguint<m+1>	x3;
	
	x3=(abmodp(Lan,Lan,p))^Lan^x1^x2^a;
	
	return x3;
	
}
//****************************************************************************************

//************************ pointAddY function **********************************************                              
                                                                                                                        
sc_biguint<m+1> pointAddY(sc_biguint<m+1> x1,  sc_biguint<m+1> y1, 
			     sc_biguint<m+1> x2,  sc_biguint<m+1> y2,
			     sc_biguint<m+1> p,sc_biguint<m+1> a )             
{                                                                                                                       
       
	sc_biguint<m+1>	Lan;
	
	Lan=mod_inverse((y1^y2),(x1^x2),p);    
	
	sc_biguint<m+1>	x3;
	
	x3=(abmodp(Lan,Lan,p))^Lan^x1^x2^a;
	
	sc_biguint<m+1> y3;
	
	//cout<<"x3+x2"<<endl;
	//for(int i=0;i<8;i++)
	//  cout<<(x2^x3).range(31+32*i,32*i).to_string(SC_HEX)<<endl;
	//
	//cout<<"x1*y1"<<endl;
	//for(int i=0;i<8;i++)
	//  cout<<abmodp(Lan,(x2^x3),p).range(31+32*i,32*i).to_string(SC_HEX)<<endl;
	
	y3=(abmodp(Lan,(x2^x3),p))^x3^y2;
	
	return y3;
	
}
//****************************************************************************************

//************************k_length function***************************************************                        
                   
int scan_length(const sc_biguint<m+1> k)
{
	int length=0;
	for(int i=m-1;i>=0;i--)
	{
		if(k[i]==1)
		{
			length=i+1;
			break;
		}
	}
	return length;
}
//**********************************************************************************************   

//************************kPx function***************************************************                        
//¥”◊ÛµΩ”“…®√Ë∑®                   
sc_biguint<m+1> kPx(sc_biguint<m+1> k,  sc_biguint<m+1> Px, sc_biguint<m+1> Py , 
		   sc_biguint<m+1> p,sc_biguint<m+1> a)             
{                                                                                                                       
    	sc_biguint<m+1>	 Rx[m+1];
		sc_biguint<m+1>	 Ry[m+1];
	
	Rx[0]=Px;
	Ry[0]=Py;
                           
	int kLength;
	kLength=scan_length(k);

	sc_biguint<m+1> R0;

	for(int i=0;i<kLength-1;i++)
	{
		
		Rx[i+1]=doublePointX(Rx[i],Ry[i],p,a);   
		Ry[i+1]=doublePointY(Rx[i],Ry[i],p,a);  

	
		if(k[kLength-i-2]==1)		                          
		{                                                         
			R0=Rx[i+1];
			Rx[i+1]=pointAddX(Rx[i+1],Ry[i+1],Px,Py,p,a);     
			Ry[i+1]=pointAddY(R0,Ry[i+1],Px,Py,p,a);     
			    
		}                                                         

	}

	sc_biguint<m+1>	 kPx;
	
	kPx=Rx[kLength-1];
	
	return kPx;
}

//**********************************************************************************************    
 
//************************kPy function***************************************************                        
                   
sc_biguint<m+1> kPy(sc_biguint<m+1> k,  sc_biguint<m+1> Px, sc_biguint<m+1> Py , 
		   sc_biguint<m+1> p,sc_biguint<m+1> a)             
{                                                                                                                       
      	sc_biguint<m+1>	 Rx[m+1];
	sc_biguint<m+1>	 Ry[m+1];
	
	Rx[0]=Px;
	Ry[0]=Py;
                           
	int kLength;
	kLength=scan_length(k);

	sc_biguint<m+1> R0;

	for(int i=0;i<kLength-1;i++)
	{
		
		Rx[i+1]=doublePointX(Rx[i],Ry[i],p,a);   
		Ry[i+1]=doublePointY(Rx[i],Ry[i],p,a);  

	
		if(k[kLength-i-2]==1)		                          
		{                                                         
			R0=Rx[i+1];
			Rx[i+1]=pointAddX(Rx[i+1],Ry[i+1],Px,Py,p,a);     
			Ry[i+1]=pointAddY(R0,Ry[i+1],Px,Py,p,a);     
			    
		}                                                         
	}

	sc_biguint<m+1>	 kPy;
	
	kPy=Ry[kLength-1];
	
	return kPy; 
	
}
//**********************************************************************************************    


//****************************************************************************************

void main()
{
   sc_biguint<m+1>	c_end_flag;                                                                                          
   sc_biguint<m+1>	rtl_end_flag;   

   int index=0;
   
   
   int func_id;
   
   func_id=5;//kp
   //func_id=1;//PA
   //func_id=2;//DP
   //func_id=3;//field_inverse
   //func_id=4;//abmodp
   //func_id=5;//integer_inverse

for(;;)
{	
for(;;)//read rtl_end_flag and c_end_flag
  {
  	//read rtl_end_flag=2?
  	//read c_end_flag=2?
  	
//************************ read rtl_end_flag ****************************************************                            
                                                                                                                      
	//sc_biguint<m+1>	rtl_end_flag;                                                                                          
	                                                                                                                
	FILE *fprtl;                                                                                                      
	if((fprtl=fopen("../../sim/data/rtl_end_flag.dat","r"))==NULL)                                                                          
	                                                                                                                
	{                                                                                                               
		printf("The x file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	rtl_end_flag=GetValueFromFile(fprtl);
	
	fclose(fprtl);                                                                                                    
                                                                                                                      
//****************************************************************************************     

//************************ read c_end_flag ****************************************************                            
                                                                                                                      
	//sc_biguint<m+1>	c_end_flag;                                                                                          
	                                                                                                                
	FILE *fpc;                                                                                                      
	if((fpc=fopen("../../sim/data/c_end_flag.dat","r"))==NULL)                                                                          
	                                                                                                                
	{                                                                                                               
		printf("The x file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	c_end_flag=GetValueFromFile(fpc);
	
	fclose(fpc);                                                                                                    
                                                                                                                      
//****************************************************************************************


       if((rtl_end_flag==2)&&(c_end_flag==2))
          break;

	   
}//match the second "for"

       srand((unsigned)time(NULL));//set random seed(system time)  
      
    //**************** generating a 256-bit  k ********************************************	                
                                                                                                                      
	sc_biguint<256>	k;	
	                                                                                                                
	for(int ik=0;ik<32;ik++)                                                                                       
	{                                                                                                               
		
		if((ik==31)||(ik==30)||(ik==29))                                                                                             
			k.range(8*ik+7,8*ik)=(rand() & 0x00); 	  	                                        
		else                                                                                                    
			k.range(8*ik+7,8*ik)=(rand()&0xff); 	
			
		//if(ik==0)                                                                                             
		//	k.range(8*ik+7,8*ik)=(rand() & 0xff); 	                                         
		//else                                                                                                    
		//	k.range(8*ik+7,8*ik)=(rand()&0x00); 	   	                                        
	}   
   //**************************************************************************************
   
                        
//**************************output 256-bit k.dat ******************
	
	FILE *fk;                                                                                                  
	if((fk=fopen("../../sim/data/k.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fk,k);  	                                                                        
	                                                                                                                
	fclose(fk);     
//*********************************************************************************                              

 //**************** generating a 256-bit  x1 ********************************************	                
                                                                                                                      
	sc_biguint<256>	x1;	
	                                                                                                                
	for(int ix1=0;ix1<32;ix1++)                                                                                       
	{                                                                                                               
		
		if((ix1==31)||(ix1==30)||(ix1==29))                                                                                             
			x1.range(8*ix1+7,8*ix1)=(rand() & 0x00); 	  	                                        
		else                                                                                                    
			x1.range(8*ix1+7,8*ix1)=(rand()&0xff); 	
		   	                                        
	}   
   //**************************************************************************************
   
                        
//**************************output 256-bit x1.dat ******************
	
	FILE *fx1;                                                                                                  
	if((fx1=fopen("../../sim/data/x1.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fx1,x1);  	                                                                        
	                                                                                                                
	fclose(fx1);     
//*********************************************************************************      

 //**************** generating a 256-bit  y1 ********************************************	                
                                                                                                                      
	sc_biguint<256>	y1;	
	                                                                                                                
	for(int iy1=0;iy1<32;iy1++)                                                                                       
	{                                                                                                               
		
		if((iy1==31)||(iy1==30)||(iy1==29))                                                                                             
			y1.range(8*iy1+7,8*iy1)=(rand() & 0x00); 	  	                                        
		else                                                                                                    
			y1.range(8*iy1+7,8*iy1)=(rand()&0xff); 	
		   	                                        
	}   
   //**************************************************************************************
   
                        
//**************************output 256-bit y1.dat ******************
	
	FILE *fy1;                                                                                                  
	if((fy1=fopen("../../sim/data/y1.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fy1,y1);  	                                                                        
	                                                                                                                
	fclose(fy1);     
//*********************************************************************************    

//**************** generating a 256-bit  x2 ********************************************	                
                                                                                                                      
	sc_biguint<256>	x2;	
	                                                                                                                
	for(int ix2=0;ix2<32;ix2++)                                                                                       
	{                                                                                                               
		
		if((ix2==31)||(ix2==30)||(ix2==29))                                                                                             
			x2.range(8*ix2+7,8*ix2)=(rand() & 0x00); 	  	                                        
		else                                                                                                    
			x2.range(8*ix2+7,8*ix2)=(rand()&0xff); 	
		   	                                        
	}   
   //**************************************************************************************
   
                        
//**************************output 256-bit x2.dat ******************
	
	FILE *fx2;                                                                                                  
	if((fx2=fopen("../../sim/data/x2.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fx2,x2);  	                                                                        
	                                                                                                                
	fclose(fx2);     
//*********************************************************************************      

 //**************** generating a 256-bit  y2 ********************************************	                
                                                                                                                      
	sc_biguint<256>	y2;	
	                                                                                                                
	for(int iy2=0;iy2<32;iy2++)                                                                                       
	{                                                                                                               
		
		if((iy2==31)||(iy2==30)||(iy2==29))                                                                                             
			y2.range(8*iy2+7,8*iy2)=(rand() & 0x00); 	  	                                        
		else                                                                                                    
			y2.range(8*iy2+7,8*iy2)=(rand()&0xff); 	
		   	                                        
	}   
   //**************************************************************************************
   
                        
//**************************output 256-bit y2.dat ******************
	
	FILE *fy2;                                                                                                  
	if((fy2=fopen("../../sim/data/y2.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fy2,y2);  	                                                                        
	                                                                                                                
	fclose(fy2);     
//*********************************************************************************   

//************************ read p ****************************************************                            
                                                                                                                      
	sc_biguint<m+1>	p;                                                                                          
	                                                                                                                
	FILE *fpp;                                                                                                      
	if((fpp=fopen("../../sim/data/p.dat","r"))==NULL)                                                                          
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	p=GetValueFromFile(fpp);                                                               
                                                                                                                      
	fclose(fpp);                                                                                                    
                                                                                                                      
//****************************************************************************************      
	
//************************ read a ****************************************************                            
                                                                                                                      
	sc_biguint<m+1>	a;                                                                                          
	                                                                                                                
	FILE *fpa;                                                                                                      
	if((fpa=fopen("../../sim/data/a.dat","r"))==NULL)                                                                          
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	a=GetValueFromFile(fpa);                                                               
                                                                                                                      
	fclose(fpa);                                                                                                    
                                                                                                                      
//****************************************************************************************  

//************************ read px ****************************************************                            
                                                                                                                      
	sc_biguint<m+1>	px;                                                                                          
	                                                                                                                
	FILE *fpx;                                                                                                      
	if((fpx=fopen("../../sim/data/px.dat","r"))==NULL)                                                                          
	                                                                                                                
	{                                                                                                               
		printf("The x file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	px=GetValueFromFile(fpx);
	
	fclose(fpx);                                                                                                    
 //************************ read py ****************************************************                            
                                                                                                                      
	sc_biguint<m+1>	py;                                                                                          
	                                                                                                                
	FILE *fpy;                                                                                                      
	if((fpy=fopen("../../sim/data/py.dat","r"))==NULL)                                                                          
	                                                                                                                
	{                                                                                                               
		printf("The x file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	py=GetValueFromFile(fpy);
	
	fclose(fpy);                                                                                                                                                    


 if(func_id==0)
 {

//**************************************************************************************** 

   
	   
     sc_biguint<m+1>	kp_x;   
     
     kp_x=kPx(k,px,py,p,a);
     
     sc_biguint<m+1>	kp_y;   
     
     kp_y=kPy(k,px,py,p,a);
  
  //**************************output 256-bit kp_x.dat ******************
	
	FILE *fkpx;                                                                                                  
	if((fkpx=fopen("../../sim/data/kp_x.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fkpx,kp_x);  	                                                                        
	                                                                                                                
	fclose(fkpx);     
//*********************************************************************************  

//**************************output 256-bit kp_y.dat ******************
	
	FILE *fkpy;                                                                                                  
	if((fkpy=fopen("../../sim/data/kp_y.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fkpy,kp_y);  	                                                                        
	                                                                                                                
	fclose(fkpy);     
//********************************************************************************* 
}

if(func_id==1)
 {

//**************************************************************************************** 

   
	   
     sc_biguint<m+1>	pa_x;   
     
     pa_x=pointAddX(x1,y1,x2,y2,p,a);
     
     sc_biguint<m+1>	pa_y;   
     
     pa_y=pointAddY(x1,y1,x2,y2,p,a);
  
  //**************************output 256-bit pa_x.dat ******************
	
	FILE *fpax;                                                                                                  
	if((fpax=fopen("../../sim/data/pa_x.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fpax,pa_x);  	                                                                        
	                                                                                                                
	fclose(fpax);     
//*********************************************************************************  

//**************************output 256-bit pa_y.dat ******************
	
	FILE *fpay;                                                                                                  
	if((fpay=fopen("../../sim/data/pa_y.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fpay,pa_y);  	                                                                        
	                                                                                                                
	fclose(fpay);     
//********************************************************************************* 
}


if(func_id==2)
 {

//**************************************************************************************** 

   
	   
     sc_biguint<m+1>	dp_x;   
     
     dp_x=doublePointX(x1,y1,p,a);
     
     sc_biguint<m+1>	dp_y;   
     
     dp_y=doublePointY(x1,y1,p,a);
  
  //**************************output 256-bit dp_x.dat ******************
	
	FILE *fdpx;                                                                                                  
	if((fdpx=fopen("../../sim/data/dp_x.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fdpx,dp_x);  	                                                                        
	                                                                                                                
	fclose(fdpx);     
//*********************************************************************************  

//**************************output 256-bit dp_y.dat ******************
	
	FILE *fdpy;                                                                                                  
	if((fdpy=fopen("../../sim/data/dp_y.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fdpy,dp_y);  	                                                                        
	                                                                                                                
	fclose(fdpy);     
//********************************************************************************* 
}

if(func_id==3)
 {

//**************************************************************************************** 

   
	   
     sc_biguint<m+1>	f_inverse;   
     
     f_inverse=mod_inverse(y1,x1,p);
     
     
      
  //**************************output 256-bit abmodp.dat ******************
	
	FILE *finverse;                                                                                                  
	if((finverse=fopen("../../sim/data/f_inverse.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(finverse,f_inverse);  	                                                                        
	                                                                                                                
	fclose(finverse);     
//*********************************************************************************  

}

if(func_id==4)
 {

//**************************************************************************************** 

   
	   
     sc_biguint<m+1>	abmodp_v;   
     
     abmodp_v=abmodp(x1,y1,p);
     
      
  //**************************output 256-bit abmodp.dat ******************
	
	FILE *fabmodp;                                                                                                  
	if((fabmodp=fopen("../../sim/data/abmodp.dat","w"))==NULL)                                             
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fabmodp,abmodp_v);  	                                                                        
	                                                                                                                
	fclose(fabmodp);     
//*********************************************************************************  

}

if(func_id==5)
 {

//**************************************************************************************** 
	   
     sc_biguint<m+1>	i_inverse;   
     
     i_inverse=int_inverse(y1,x1,p);
     
          
  //**************************output 256-bit i_inverse.dat ******************
	
	FILE *fiinverse;                                                                                                  
	if((fiinverse=fopen("../../sim/data/i_inverse.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fiinverse,i_inverse);  	                                                                        
	                                                                                                                
	fclose(fiinverse);     
//*********************************************************************************  

}


   /////////////////////  set c_end_flag=1    ///////////////////////
	
	c_end_flag=1;
	
        //**************************output c_end_flag.dat ******************
	
	FILE *fcend;                                                                                                  
	if((fcend=fopen("../../sim/data/c_end_flag.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fcend,c_end_flag);  	                                                                        
	                                                                                                                
	fclose(fcend);     
//*********************************************************************************  
 index=index+1;
	 cout<<"compute times="<<index<<endl;

   ////////////////////// read rtl_end_flag=1? ///////////////////////
   for(;;)
   {
   	//************************ read rtl_end_flag ****************************************************                            
                                                                                                                                                                                                             
	                                                                                                                
	FILE *frend;                                                                                                      
	if((frend=fopen("../../sim/data/rtl_end_flag.dat","r"))==NULL)                                                                          
	                                                                                                                
	{                                                                                                               
		printf("The x file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	rtl_end_flag=GetValueFromFile(frend);
	
	fclose(frend);                                                                                                    
                                                                                                                      
       //****************************************************************************************     
       
       if(rtl_end_flag==1)
         break;
    }
    
       /////////////////////  set c_end_flag=2    ///////////////////////
	
	c_end_flag=2;
	
        //**************************output c_end_flag.dat ******************
	
	FILE *fcend2;                                                                                                  
	if((fcend2=fopen("../../sim/data/c_end_flag.dat","w"))==NULL)                                                                     
	                                                                                                                
	{                                                                                                               
		printf("The file can not be opened!:\n");                                                               
		exit(0);                                                                                                
	}                                                                                                               
	                                                                                                                
	SetValueToFile(fcend2,c_end_flag);  	                                                                        
	                                                                                                                
	fclose(fcend2);     
//*********************************************************************************

}//match the first "for"


}
