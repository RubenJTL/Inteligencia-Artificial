#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <iomanip>

////BOOST
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
using namespace boost::numeric::ublas;


typedef matrix<int> matriz;
typedef double Num;
typedef matrix<Num> Capa;
typedef matrix<Capa> Capas;


///Operaciones
Num Error(Capa,Capa);
Num EA(Capa,Capa,int,int);
Capa DelthaO(Capa,Capa);
Capa DelthaH(Capa,Capa,Capa);
Num EP(Capa,Capa,int,int);
void ChangeW(Capa &,Capa,Capa,Num);
//void ChangeW(Capa &,)

///SIGMOIDE
Num Sigmoide(Num);
Capa Sigmoide(Capa,int);

///CREATS
Capa creatInput(Capa,int);
Capa creatOutput(Capa,int);
Capa creatLayer(int,int);
Capas creatHide(int,int,int);
Capa readF(char*,int ,int );

///ESCRITURA DE ARCHIVO
void Guardar_Pesos(char* stri,Capa WH,Capa WO);

////TEST


int main()
{
	matriz Confusion(3,4);
	srand(time(NULL));
	int contador=0;
	Num error, umbral=0.00001, errorant, alpha=1;
	///NOMBRES DE ARCHIVOS DE LECTURA
	char* inputs= (char *)"irisN.txt";
	char* deseado=(char *)"irisO.txt";
	char* inputsT= (char *)"testIrisN.txt";
	char* deseadoT=(char *)"testIrisI.txt";
	
/*
	char* inputs= (char *)"XORinput.txt";
	char* deseado=(char *)"XORout.txt";
	char* inputsT= (char *)"XORinput.txt";
	char* deseadoT=(char *)"XORout.txt";
*/


/*	char* inputs= (char *)"XOR2.txt";
	char* deseado=(char *)"XORO2.txt";
	char* inputsT= (char *)"XOR2.txt";
	char* deseadoT=(char *)"XORO2.txt";
*/	

	Capa m =readF(inputs,4,90), n =readF(deseado,3,90),mT =readF(inputsT,4,60), nT =readF(deseadoT,3,60);
	//Capa m =readF(inputs,3,8), n =readF(deseado,1,8),mT =readF(inputsT,3,8), nT =readF(deseadoT,1,8);
//	Capa m =readF(inputs,2,4), n =readF(deseado,1,4),mT =readF(inputsT,2,4), nT =readF(deseadoT,1,4);

	///INICIAR CAPAS
    Capa Input,Sd,WH,SH,WO,SO,InputTest,OutputTest,delthaO,delthaH;
   /* WH=creatLayer(4,8);
    //H2=creatLayer(8,3);
	WO=creatLayer(8,3);
    */
	WH=creatLayer(m.size2(),8);
    //H2=creatLayer(8,3);
	WO=creatLayer(8,n.size2());
	Num errorT=0;
	errorant=1;
	Num rangoalpha=0.09;
    do{
	//for(unsigned iteraciones=0;iteraciones<1000;++iteraciones){
		errorT=0;
		for(unsigned posicion=0;posicion<m.size1();++posicion){	
			Input=creatInput(m,posicion);
			Sd=creatOutput(n,posicion);
			SH=Sigmoide(prod(Input,WH),0);
			SO=Sigmoide(prod(SH,WO),1);
			error=Error(SO,Sd);
			delthaO=DelthaO(SO,Sd);
			delthaH=DelthaH(SH,delthaO,WO);
			ChangeW(WO,delthaO,SH,alpha);
			ChangeW(WH,delthaH,Input,alpha);
			errorT+=error;
		}
		if(errorant-(errorT/m.size2())>=rangoalpha){
			alpha=alpha*0.9;
			rangoalpha=rangoalpha*0.9;
		}
		//cout<<n.size2();
		errorant=errorT/m.size2();
		printf("Error %.20f  alpha  %.20f   rango de alpha %.20f\n",errorT/m.size2(),alpha,rangoalpha);

	//}
	}while(errorT/m.size2()>umbral);
	cout<<endl;
	/*for(unsigned posicion=0;posicion<mT.size1();++posicion){	
		Input=creatInput(mT,posicion);
		Sd=creatOutput(nT,posicion);
		SH=Sigmoide(prod(Input,WH),0);
		SO=Sigmoide(prod(SH,WO),1);
		

		for(unsigned i=0;i<SO.size2();++i){
			SO(0,i)=round(SO(0,i));
		}
		for(unsigned i=0;i<SO.size2();++i){
			cout<<SO(0,i)<<endl;
		}
	}*/

	for(unsigned posicion=0;posicion<mT.size1();++posicion){	
		Input=creatInput(mT,posicion);
		Sd=creatOutput(nT,posicion);
		SH=Sigmoide(prod(Input,WH),0);
		SO=Sigmoide(prod(SH,WO),1);
		for(unsigned i=0;i<SO.size2();++i){
			SO(0,i)=round(SO(0,i));
		}
		if(Sd(0,0)==1){
			if(SO(0,0)==1 and SO(0,1)==0 and SO(0,2)==0) Confusion(0,0)++;
			else if(SO(0,0)==0 and SO(0,1)==1 and SO(0,2)==0) Confusion(0,1)++;
			else if(SO(0,0)==0 and SO(0,1)==0 and SO(0,2)==1) Confusion(0,2)++;
			else Confusion(0,3)++;
		}
		if(Sd(0,1)==1){
			if(SO(0,0)==1 and SO(0,1)==0 and SO(0,2)==0) Confusion(1,0)++;
			else if(SO(0,0)==0 and SO(0,1)==1 and SO(0,2)==0) Confusion(1,1)++;
			else if(SO(0,0)==0 and SO(0,1)==0 and SO(0,2)==1) Confusion(1,2)++;
			else Confusion(1,3)++;
		}
		if(Sd(0,2)==1){
			if(SO(0,0)==1 and SO(0,1)==0 and SO(0,2)==0) Confusion(2,0)++;
			else if(SO(0,0)==0 and SO(0,1)==1 and SO(0,2)==0) Confusion(2,1)++;
			else if(SO(0,0)==0 and SO(0,1)==0 and SO(0,2)==1) Confusion(2,2)++;
			else Confusion(2,3)++;
		}
		
	}	
		printf("             \t   SETOSA   \t VERSICOLOR\t VIRGINICA \t REZAGADOS \n");
		printf("  SETOSA     \t     %d     \t    %d     \t    %d     \t    %d     \n",Confusion(0,0)*5,Confusion(0,1)*5,Confusion(0,2)*5,Confusion(0,3))*5;
		printf("  VERSICOLOR \t     %d     \t    %d     \t    %d     \t    %d     \n",Confusion(1,0)*5,Confusion(1,1)*5,Confusion(1,2)*5,Confusion(1,3)*5);
		printf("  VIRGINICA  \t     %d     \t    %d     \t    %d     \t    %d     \n",Confusion(2,0)*5,Confusion(2,1)*5,Confusion(2,2)*5,Confusion(2,3)*5);
	
}


void Guardar_Pesos(char* stri,Capa WH,Capa WO){
	
	ofstream myfile (stri);
	myfile <<WH.size1()<<"\t"<<WH.size2()<<"\n";
    for(unsigned i=0;i<WH.size1();++i){
    	for(unsigned j=0;j<WH.size2();++j){
    		myfile << setprecision(20) << WH(i,j)<<"\t";		
    	}
    	myfile <<"\n";
    }
    myfile <<WO.size1()<<"\t"<<WO.size2()<<"\n";
    for(unsigned i=0;i<WO.size1();++i){
    	for(unsigned j=0;j<WO.size2();++j){
    		myfile << setprecision(20) << WO(i,j)<<"\t";		
    	}
    	myfile <<"\n";
    }
    
    myfile.close();   
}

void TEST(){

}


///Operaciones
Num Error(Capa So,Capa Sd){
	Num sum=0;
	for(unsigned i=0;i<So.size2();++i){
		sum+=pow(So(0,i)-Sd(0,i),2);
	}
	return (sum/(2.0*So.size2()));
}

Num EA(Capa So,Capa Sd,int j,int boole){
	if(boole==1){
	return(So(0,j)-Sd(0,j));
	}
	else{
		Num sum=0;
		for(unsigned i=0;i<Sd.size2();++i){
			sum+=So(0,i)*Sd(j,i);
		}
		return sum;
	}
}

Capa DelthaO(Capa So,Capa Sd){
	Capa D(1,So.size2());
	for(unsigned i=0;i<D.size2();++i){
		D(0,i)=EA(So,Sd,i,1)*So(0,i)*(1-So(0,i));
	}
	return D;
}

Capa DelthaH(Capa SH,Capa deltha,Capa W){
	Capa D(1,SH.size2()-1);
	for(unsigned i=0;i<D.size2();++i){
		D(0,i)=EA(deltha,W,i,0)*SH(0,i+1)*(1-SH(0,i+1));
	}
	return D;
}

Num EP(Capa deltha,Capa So,int j,int i){
	return (deltha(0,j)*So(0,i));
}

void ChangeW(Capa &W,Capa deltha,Capa SA,Num alpha){
	for(unsigned i=0;i<W.size2();++i){
		for(unsigned j=0;j<W.size1();++j){
			W(j,i)=W(j,i)-alpha*EP(deltha,SA,i,j);
		}
	}
}


///CREATS
Capa creatInput(Capa m,int posicion){
	Capa  I(1,m.size2()+1);
	I(0,0)=1.0;
	for(unsigned i=1;i<I.size2();++i){
		I(0,i)=m(posicion,i-1);
	}
	return I;
}

Capa creatOutput(Capa output,int posicion){
	Capa O(1,output.size2());
	for(unsigned i=0;i<O.size2();++i){
		O(0,i)=output(posicion,i);
	}
	return O;
}

Capa creatLayer(int inputs,int outputs){	
	Capa  I(inputs+1,outputs);
	for(unsigned i=0;i<I.size1();++i){	
		for(unsigned j=0;j<I.size2();++j){
			I(i,j)=((Num) (rand())) / ((Num )(RAND_MAX));
		}
	}
	return I;
}

///LECTURA DE ARCHIVO
Capa readF(char* file,int caracteristicas, int neuronas){
	Capa m (neuronas, caracteristicas);
	Num val;
    ifstream f(file);
    for (unsigned i = 0; i < m.size1 (); ++ i){
        for (unsigned j = 0; j < m.size2 (); ++ j){
            f>>m(i,j);
        }
    }
    return m;
}

///SIGMOIDE
Num Sigmoide(Num T){
	return ((1)/(1+exp(-T)));
}
Capa Sigmoide(Capa neta,int boole){
	if(boole==0){
		Capa S(neta.size1(),neta.size2()+1);
		for(unsigned i=0;i<S.size1();++i){
			for(unsigned j=0;j<S.size2();++j){
				if(j==0) S(i,j)=1.0;
				else S(i,j)=Sigmoide(neta(i,j-1));
			}
		}
		return S;
	}
	else {
		Capa S(neta.size1(),neta.size2());
		for(unsigned i=0;i<S.size1();++i){
			for(unsigned j=0;j<S.size2();++j){
				S(i,j)=Sigmoide(neta(i,j));
			}
		}
		return S;	
	}
}
