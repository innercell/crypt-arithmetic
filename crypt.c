/*	
	Tiago Roberti Sampaio
	www.trsampaio.com
	http://www.trsampaio.com/2014/09/01/solucao-de-cripto-aritmetica-com-algoritmos-geneticos/
	Funcionamento do Programa

DONALD
gERALD
------
ROBERT

COCA
COLA
----
OASIS


Testes
Tx crossover 40  60  90
Pop inicial 100 300 500
Tx Mutacao    5  15  30
Crossover aleatorio (torneio simples 3)

1. A populacao inicial é criada (novo_individuo())
	1.1 novo_individuo():nova_lista() Uma lista com todos os valores de entradas (SENDMORY) é criada
	1.2 novo_individuo():atribui_valor() Um valor disponível na lista acima é escolhido aleatoriamente, logo após marcado como indisponível
	1.3 novo_individuo():criar_parcelas() Os valores SENDMORY são movidos para um vetor auxiliar para mais tarde concatená-los
	1.4 novo_individuo():concatena_parcelas() Os valores SENDMORY são concatenados como SEND MORE MONEY (Ex: S*1000+E*100+N*10+D)
	1.5 novo_individuo():fitness() A combinacao SENDMORY é avaliada, na forma: (SEND+MORE)-MONEY
	1.6 Uma possível mutacao do novo individuo é calculada (mutacao(int taxa_mutacao))
	1.6.1 Os valores já utilizados são marcados na nova_lista()
	1.6.2 Um número é sorteado para determinar se a mutacao vai ocorrer ou não
	1.6.3 Um gene (SENDMORY) é escolhido aleatoriamente para ser mutado, e uma valor disponivel é escolhido aleatoriamente para substituir
	1.7 Caso ocorra mutacao o individuo é reorganizado. (criar_parcelas(),concatena_parcelas(),fitness())
2. N reproducões vão ocorrer de acordo com a taxa de crossover (reproduzir())
	2.1 Dois pais são escolhidos aleatoriamente tal que pai1!=pai2 e a geracao do pai1 ou do pai2 não ultrapasse o limite estipulado
	2.2 Dois filhos serão gerados, cada um com uma certa quantidade genetica de cada pai (tam_divisao_crossover)
	2.3 reproduzir():verificar_gene_invalido() Após um filho ser criado, a sequência genética do mesmo é avaliada em busca de repeticões, e corrigida
	2.3.1 verificar_gene_invalido():nova_lista() Os valores já utilizados são marcados
	2.3.2 Caso um valor tente ser marcado pela segunda vez ele é substituido por um gene diferente, do pai1 ou pai2.
	2.3.3 Caso essa correcao genética caia em loop, o gene invalido (repetido) é mutado para um aleatório
	2.4 Caso a combinacao de dois pais diferentes resulte em um filho igual ao pai, então um dos genes do filho será mutado aleatoriamente
3. A selecao_natural(int alcance_selecao_natural) será executada
	3.1 A populacao atual é copiada para uma matriz auxiliar
	3.2 Toda a populacao é ordenada pela ordem crescente do resultado da funcao fitness()
	3.3 Os N (alcance_selecao_natural) melhores individuos são separados, e os demais são mortos.
	3.4 Uma nova populacao pós-selecao natural é criada.
4. Voltar ao Passo 2, enquanto o resultado da funcao fitness!=0 ou o numero máximo de ciclos (Nascimento->Reproducao->Morte(Selecao Natural)) for atingido.

Obs: Dependendo da forma que a correcão de genes repetidos é feita ira cair em um loop eterno; Atualmente essa correcão é feita somente de duas formas, lendo a sequencia da esquerda para a direita e da direita para e esquerda; e quando cai em loop eterno os "Pais Incompativeis" são armazenados e exibidos ao fim do programa.

*/

#include <stdio.h>
#include <stdlib.h>
#define pop 10000 //Populacao Maxima
#define popinicial 100 //Populacao Inicial
#define taxa_crossover 50 //Taxa de Reproducao(crossover) em %
int tam_divisao_crossover=4; //Quantia genetica de cada Pai
int num_geracoes=10000;
int taxa_mutacao=3; //Taxa de Mutacao em %
int alcance_selecao_natural=50; //Alcance da selecao natural (Quantos melhores individuos serão selecionados)
int max_ciclo=333; //Limite de vezes que o programa rodara
//Variáveis do sistema
int populacao[pop][13]; //[0] -> parcela 1, [1] -> parcela 2, [2] -> parcela 3, [3] -> res_fitness, [4]S,[5]E,[6]N,[7]D,[8]M,[9]O,[10]R,[11]Y,[12] Geracao
int indiv=0; //index da populacao
int letras[8]; //SENDMORY
int p1[4]; //parcela 1 send
int p2[4]; //parcela 2 more
int p3[5]; //parcela 3 money
int val_disp[10]; //valores de entrada disponiveis
int resultado_encontrado=0;
int stopnow=0;
int num_loop_eterno=0;
int max_loop_genetico=30;
int agpi[100][2];
int iagpi=0; //incrementador do agpi
//Construtores
void nova_lista()
{
	val_disp[0]=0;
	val_disp[1]=1;
	val_disp[2]=2;
	val_disp[3]=3;
	val_disp[4]=4;
	val_disp[5]=5;
	val_disp[6]=6;
	val_disp[7]=7;
	val_disp[8]=8;
	val_disp[9]=9;
}
void gravar_agpi(int pai1,int pai2)
{
	int mor=populacao[pai1][1]/10;
	agpi[iagpi][0]=(populacao[pai1][0]*10000)+(mor*10)+populacao[pai1][11]; //SEND + MOR + Y
	mor=populacao[pai2][1]/10;
	agpi[iagpi][1]=(populacao[pai2][0]*10000)+(mor*10)+populacao[pai2][11]; //SEND + MOR + Y
	iagpi++;

}
void atribui_valor()
{	
	int i,rnd;
	
	for(i=0;i<8;i++)
	{
		do{	
		rnd=rand()%10; //printf("\nnovo num randomico gerado! %d\n",rnd);
		//if(val_disp[rnd]==777) printf("Ops! numero randomico %d invalido! valor indisponivel",rnd);
		}while(val_disp[rnd]==777);//777=valor indisponivel
		//printf("val_disp[%d]=%d\n",rnd,val_disp[rnd]);
		letras[i]=val_disp[rnd];
		val_disp[rnd]=777;
		populacao[indiv][i+4]=letras[i];
	}		
}

void concatena_parcelas()
{
	populacao[indiv][0]=(p1[0]*1000)+(p1[1]*100)+(p1[2]*10)+p1[3]; //concatena parcela 1
	populacao[indiv][1]=(p2[0]*1000)+(p2[1]*100)+(p2[2]*10)+p2[3]; //concatena parcela 2
	populacao[indiv][2]=(p3[0]*10000)+(p3[1]*1000)+(p3[2]*100)+(p3[3]*10)+p3[4]; //concatena parcela 3

	//debug
	//printf("Individuo n%d p1=%d p2=%d p3=%d\n",indiv,populacao[indiv][0],populacao[indiv][1],populacao[indiv][2]);
}

void fitness()
{
	populacao[indiv][3]=populacao[indiv][2]-(populacao[indiv][0]+populacao[indiv][1]); //(send+more)-money
	if(populacao[indiv][3]<0) populacao[indiv][3]*=-1;
	if(populacao[indiv][3]==0) resultado_encontrado=1;	
	

}

int mutacao(int taxa_mutacao)
{
	nova_lista();
	int i;
	//Marcando na lista os valores já usados
	for(i=0;i<8;i++)
	{
		int index=populacao[indiv][i+4];
		if(val_disp[index]!=777)
		{
			val_disp[index]=777;
		}
	}
	int rnd_mut=rand()%100; //printf("rnd_mut:%d\n",rnd_mut); 
	if(rnd_mut<taxa_mutacao) //Mutacao aceita!
	{	
		
		int rnd_gene=rand()%8; //printf("rnd_gene:%d gene de valor:%d \n",rnd_gene,populacao[indiv][4+rnd_gene]);//escolher um dos genes a ser mutado
		int rnd_val;
		do{ rnd_val=rand()%10; }while(val_disp[rnd_val]==777);
		//printf("------------------Mutacao do indiv n%d: subst %d por %d\n",indiv,populacao[indiv][4+rnd_gene],val_disp[rnd_val]);
		populacao[indiv][rnd_gene+4]=val_disp[rnd_val];
		
		
		return 1;
	}else{
		return 0;
	}
	 
}

void criar_parcelas()
{
	p1[0]=populacao[indiv][4];//S
	p1[1]=populacao[indiv][5];//E
	p1[2]=populacao[indiv][6];//N
	p1[3]=populacao[indiv][7];//D

	p2[0]=populacao[indiv][8];//M
	p2[1]=populacao[indiv][9];//O
	p2[2]=populacao[indiv][10];//R
	p2[3]=populacao[indiv][5];//E

	p3[0]=populacao[indiv][8];//M
	p3[1]=populacao[indiv][9];//O
	p3[2]=populacao[indiv][6];//N
	p3[3]=populacao[indiv][5];//E
	p3[4]=populacao[indiv][11];//Y

}

void novo_individuo()
{
	nova_lista();
	atribui_valor();

	
	criar_parcelas();
	concatena_parcelas();
	fitness();
	populacao[indiv][12]=0; //Geracao 0
	printf("Individuo n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);
	if(mutacao(taxa_mutacao))
	{
		criar_parcelas();
		concatena_parcelas();
		fitness();	
		printf("IndividuoM n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);		
	}

	indiv++;
}



int verificar_gene_invalido(int pai1,int pai2,int CrescDecr) //quando atraves de mutacao ou cross over as letras (genes) diferentes recebem o mesmo valor
{
	nova_lista();
	int i;
	int erro=0;
	if(CrescDecr==0)
	{
		for(i=0;i<8;i++)
		{
			int index=populacao[indiv][i+4];
			if(val_disp[index]!=777)
			{
				val_disp[index]=777;
			}else{
				erro=1;
				//printf("Erro genetico!(Cresc)");
				if(populacao[pai1][i+4]!=populacao[indiv][i+4]) //Pegar gene diferente do pai1
				{	
					//printf(".");
					//printf("#Indiv n%d: subst %d por %d(pai1) (SENDMORY:%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][i+4],populacao[pai1][i+4],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);			
					populacao[indiv][i+4]=populacao[pai1][i+4];
					val_disp[index]=777;
				}else if(populacao[pai2][i+4]!=populacao[indiv][i+4]){ //Pegar gene diferente do pai2
					//printf("#Indiv n%d: subst %d por %d(pai2) (SENDMORY:%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][i+4],populacao[pai2][i+4],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);	
					//printf(".");
					populacao[indiv][i+4]=populacao[pai2][i+4];
					val_disp[index]=777;
				}
			}
		}
	}else if(CrescDecr==1){
		for(i=7;i>=0;i--)
		{
			int index=populacao[indiv][i+4];
			if(val_disp[index]!=777)
			{
				val_disp[index]=777;
			}else{
				erro=1;
				//printf("Erro genetico!(Decr)");
				if(populacao[pai1][i+4]!=populacao[indiv][i+4]) //Pegar gene diferente do pai1
				{	
					//printf("#Indiv n%d: subst %d por %d(pai1) (SENDMORY:%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][i+4],populacao[pai1][i+4],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);			
					populacao[indiv][i+4]=populacao[pai1][i+4];
					val_disp[index]=777;
				}else if(populacao[pai2][i+4]!=populacao[indiv][i+4]){ //Pegar gene diferente do pai2
					//printf("#Indiv n%d: subst %d por %d(pai2) (SENDMORY:%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][i+4],populacao[pai2][i+4],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);	
					populacao[indiv][i+4]=populacao[pai2][i+4];
					val_disp[index]=777;
				}
			}
		}

	}	
	return erro;

}


void reproduzir() 
{
	//Criar filho 1
	int pai1,pai2,mut;
	do{	
		pai1=rand()%(indiv-1); //Indiv -> individuo atual
		pai2;
		do{ pai2=rand()%(indiv-1); }while(pai1==pai2);
	
		if((populacao[pai1][0]==populacao[pai2][0])&&(populacao[pai1][1]==populacao[pai2][1])&&(populacao[pai1][2]==populacao[pai2][2]))
		{
			//printf("pai1 == pai2\n");
			//taxa_mutacao++;
			pai1=rand()%(indiv-1); //Indiv -> individuo atual
			pai2;
			do{ pai2=rand()%(indiv-1); }while(pai1==pai2);
			
		}
	}while( (populacao[pai1][12]>num_geracoes)||(populacao[pai2][12]>num_geracoes) );  
	
	int i;	
	for(i=0;i<tam_divisao_crossover;i++) populacao[indiv][i+4]=populacao[pai1][i+4];	
	for(i=tam_divisao_crossover;i<8;i++) populacao[indiv][i+4]=populacao[pai2][i+4];
	
	int repetir=0;
	int aux;
	int decr;
	int k;
	int loop=0;
	//Verificar e corrigir genes repetidos
	do{	
		loop++;
		//printf("Iteracao n%d \n",loop);  
		if(loop==max_loop_genetico) break;
		k=0;
		do{ verificar_gene_invalido(pai1,pai2,0); k++;}while(k<max_loop_genetico);
		k=0;
		do{ verificar_gene_invalido(pai1,pai2,1); k++;}while(k<max_loop_genetico);
	}while((verificar_gene_invalido(pai1,pai2,0)!=0)&&(verificar_gene_invalido(pai1,pai2,1)!=0));
	
	if(loop==max_loop_genetico)
	{
		 printf("Loop Eterno - requer mutacao\n");num_loop_eterno++;
		 gravar_agpi(pai1,pai2);
			//Mutacao do valor repetido
			nova_lista();
			int i;
			//Marcando na lista os valores já usados
			for(i=0;i<8;i++)
			{
				int index=populacao[indiv][i+4];
				if(val_disp[index]!=777)
				{
					val_disp[index]=777;
				}else if(val_disp[index]==777){
					//Valor ja utilizado, marcar para mutacao
					mut=i;printf("valor repetido = %d",index);
				}	
			}			
		
			int rnd_gene=mut; //valor para ser mutado
			int rnd_val;
			do{ rnd_val=rand()%10; }while(val_disp[rnd_val]==777);
			printf("------------------MutacaoAnormal do indiv n%d: subst %d por %d\n",indiv,populacao[indiv][4+rnd_gene],val_disp[rnd_val]);
			populacao[indiv][rnd_gene+4]=val_disp[rnd_val];	


		criar_parcelas();
		concatena_parcelas();
		fitness();	
		printf("FilhoM n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);		
		
	}
	

	criar_parcelas();loop++; //printf("Iteracao n%d \n",loop);
	concatena_parcelas();
	fitness();
//INICIO PAI==FILHHO	
	//Elimando problema pai==filho
	if( ((populacao[indiv][0]==populacao[pai1][0])&&(populacao[indiv][1]==populacao[pai1][1])&&(populacao[indiv][2]==populacao[pai1][2])) ||
            ((populacao[indiv][0]==populacao[pai2][0])&&(populacao[indiv][1]==populacao[pai2][1])&&(populacao[indiv][2]==populacao[pai2][2])) )
	{
		//Se o filho for igual ao pai entao vou criar o filho novamente
		//Se o filho for igual ao pai entao vou criar o filho novamente
		printf("PAI == FILHO ");
		if(mutacao(100))
		{
		criar_parcelas();
		concatena_parcelas();
		fitness();	
		printf("FilhoM n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);		
		}
		/*
		for(i=0;i<tam_divisao_crossover;i++) populacao[indiv][i+4]=populacao[pai1][i+4];	
		for(i=tam_divisao_crossover;i<8;i++) populacao[indiv][i+4]=populacao[pai2][i+4];
		
		loop=0;
		//Verificar e corrigir genes repetidos
		do{	
			loop++;
			//printf("Iteracao n%d \n",loop);  
			if(loop==5) break;
			k=0;
			do{ verificar_gene_invalido(pai1,pai2,1); k++;}while(k<max_loop_genetico);
			k=0;
			do{ verificar_gene_invalido(pai1,pai2,0); k++;}while(k<max_loop_genetico);
		}while((verificar_gene_invalido(pai1,pai2,0)!=0)&&(verificar_gene_invalido(pai1,pai2,1)!=0));

	

		if(loop==5)
		{
		 printf("Loop Eterno - requer mutacao\n");num_loop_eterno++;
		}
		criar_parcelas();loop++; //printf("Iteracao n%d \n",loop);
		concatena_parcelas();
		fitness();*/
	}
//FIM PAI==FILHO

	if(populacao[pai1][12]>=populacao[pai2][12])
	{
		populacao[indiv][12]=populacao[pai1][12]+1; //Somar a geracao do pai
	}else{
		populacao[indiv][12]=populacao[pai2][12]+1;
	}
	printf("Filho n%dG%d(%dx%d) (%d+%d)-%d=%d (SENDMORY:%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],pai1,pai2,populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);
//printf("indiv n%d, E=%d\n",indiv,populacao[indiv][5]);
	if(mutacao(taxa_mutacao))
	{
		criar_parcelas();
		concatena_parcelas();
		fitness();	
		printf("FilhoM n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);		
	}	

	indiv++;

	//Criar filho 2
	for(i=0;i<tam_divisao_crossover;i++) populacao[indiv][i+4]=populacao[pai2][i+4];	
	for(i=tam_divisao_crossover;i<8;i++) populacao[indiv][i+4]=populacao[pai1][i+4];
	repetir=0;
	
	//Verificar e corrigir genes repetidos
	loop=0;
	do{
		loop++;
		//printf("Iteracao n%d \n",loop); 
		if(loop==max_loop_genetico) break;
		k=0;
		do{ verificar_gene_invalido(pai1,pai2,1); k++;}while(k<max_loop_genetico);
		k=0;
		do{ verificar_gene_invalido(pai1,pai2,0); k++;}while(k<max_loop_genetico);
	}while((verificar_gene_invalido(pai1,pai2,0)!=0)&&(verificar_gene_invalido(pai1,pai2,1)!=0));

	if(loop==max_loop_genetico)
	{
		 printf("Loop Eterno - requer mutacao\n");num_loop_eterno++;
		 gravar_agpi(pai1,pai2);
			//Mutacao do valor repetido
			nova_lista();
			int i;
			//Marcando na lista os valores já usados
			for(i=0;i<8;i++)
			{
				int index=populacao[indiv][i+4];
				if(val_disp[index]!=777)
				{
					val_disp[index]=777;
				}else if(val_disp[index]==777){
					//Valor ja utilizado, marcar para mutacao
					mut=i; printf("valor repetido = %d",index);
				}	
			}			
		
			int rnd_gene=mut; //valor para ser mutado
			int rnd_val;
			do{ rnd_val=rand()%10; }while(val_disp[rnd_val]==777);
			printf("------------------MutacaoAnormal do indiv n%d: subst %d por %d\n",indiv,populacao[indiv][4+rnd_gene],val_disp[rnd_val]);
			populacao[indiv][rnd_gene+4]=val_disp[rnd_val];	


		criar_parcelas();
		concatena_parcelas();
		fitness();	
		printf("FilhoM n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);		
		
	}

	criar_parcelas();
	concatena_parcelas();
	fitness();

//INICIO PAI==FILHHO	
	//Elimando problema pai==filho
	if( ((populacao[indiv][0]==populacao[pai1][0])&&(populacao[indiv][1]==populacao[pai1][1])&&(populacao[indiv][2]==populacao[pai1][2])) ||
            ((populacao[indiv][0]==populacao[pai2][0])&&(populacao[indiv][1]==populacao[pai2][1])&&(populacao[indiv][2]==populacao[pai2][2])) )
	{
		//Se o filho for igual ao pai entao vou criar o filho novamente
		printf("PAI == FILHO ");
		if(mutacao(100))
		{
		criar_parcelas();
		concatena_parcelas();
		fitness();	
		printf("FilhoM n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);		
		}
		/*for(i=0;i<tam_divisao_crossover;i++) populacao[indiv][i+4]=populacao[pai2][i+4];	
		for(i=tam_divisao_crossover;i<8;i++) populacao[indiv][i+4]=populacao[pai1][i+4];
		
	
		loop=0;
		//Verificar e corrigir genes repetidos
		do{	
			loop++;
			//printf("Iteracao n%d \n",loop);  
			if(loop==5) break;
			k=0;
			do{ verificar_gene_invalido(pai1,pai2,0); k++;}while(k<max_loop_genetico);
			k=0;
			do{ verificar_gene_invalido(pai1,pai2,1); k++;}while(k<max_loop_genetico);
		}while((verificar_gene_invalido(pai1,pai2,0)!=0)&&(verificar_gene_invalido(pai1,pai2,1)!=0));

	

		if(loop==5)
		{
		 printf("Loop Eterno - requer mutacao\n");num_loop_eterno++;
		}
		criar_parcelas();loop++; //printf("Iteracao n%d \n",loop);
		concatena_parcelas();
		fitness();*/
	}
//FIM PAI==FILHO


	if(populacao[pai1][12]>=populacao[pai2][12])
	{
		populacao[indiv][12]=populacao[pai1][12]+1; //Somar a geracao do pai
	}else{
		populacao[indiv][12]=populacao[pai2][12]+1;
	}
	printf("Filho n%dG%d(%dx%d) (%d+%d)-%d=%d (SENDMORY:%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],pai1,pai2,populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);
//printf("indiv n%d, E=%d\n",indiv,populacao[indiv][5]);
	if(mutacao(taxa_mutacao))
	{
		criar_parcelas();
		concatena_parcelas();
		fitness();	
		printf("FilhoM n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",indiv,populacao[indiv][12],populacao[indiv][0],populacao[indiv][1],populacao[indiv][2],populacao[indiv][3],populacao[indiv][4],populacao[indiv][5],populacao[indiv][6],populacao[indiv][7],populacao[indiv][8],populacao[indiv][9],populacao[indiv][10],populacao[indiv][11]);		
	}
	indiv++;
	
}

void selecao_natural(int alcance_selecao_natural) //Vou selecionar os N melhores individuos e matar os outros
{
	int n=alcance_selecao_natural;
	//int n=(alcance_selecao_natural*pop)/100;
	//Ordernar n individuos pelo valor de fitness populacao[indiv][3]
	int mat[pop][13];
	int aux[1][13];
	int a,b,i;
	//Copiar a matriz populacao p/ a matriz mat
	for(a=0;a<indiv;a++)
	{	
		for(i=0;i<13;i++) mat[a][i]=populacao[a][i];
	}
	printf("%d individuos na populacao\n",indiv);
	for(a=0;a<indiv;a++)
	{
		for(b=0;b<indiv-1;b++)
		{
			if(mat[b+1][3]<mat[b][3])
			{
				//Salvo o valor menor numa auxiliar
				for(i=0;i<13;i++) aux[0][i]=mat[b][i];
				//Transfiro o valor maior para o menor				
				for(i=0;i<13;i++) 
				{
					mat[b][i]=mat[b+1][i];
					mat[b+1][i]=aux[0][i];//Transfiro a auxiliar para o valor maior					
				}
				
						
			}

		}
	}
	
	for(a=0;a<n;a++) //Copiar para a populacao os N melhores inviduos
	{
		for(i=0;i<13;i++) populacao[a][i]=mat[a][i];
	}
	for(a=n;a<indiv;a++) //Matando os individuos fracos
	{
		for(i=0;i<13;i++) populacao[a][i]=0;
	}
	printf("**********NOVA POPULACAO APOS SELECAO NATURAL***********\n");	
	indiv=n;
	for(a=0;a<indiv;a++)
	{	
		printf("Individuo n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",a,populacao[a][12],populacao[a][0],populacao[a][1],populacao[a][2],populacao[a][3],populacao[a][4],populacao[a][5],populacao[a][6],populacao[a][7],populacao[a][8],populacao[a][9],populacao[a][10],populacao[a][11]);		
	}
}

int main()
{
	srand(time(NULL));
	int ciclo=0;
	int a;
	printf("*******************POPULACAO INICIAL********************\n");
	while(indiv<popinicial) novo_individuo();
	do{
		printf("\n-----------------------CICLO n%d------------------------|Indiv %d|TxMutacao %d|\n",ciclo,indiv,taxa_mutacao);ciclo++;
		printf("*******************POPULACAO ATUAL********************\n");
		for(a=0;a<indiv;a++)printf("Individuo n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",a,populacao[a][12],populacao[a][0],populacao[a][1],populacao[a][2],populacao[a][3],populacao[a][4],populacao[a][5],populacao[a][6],populacao[a][7],populacao[a][8],populacao[a][9],populacao[a][10],populacao[a][11]);
		printf("***************************FILHOS************************\n");
		while(indiv<popinicial+(popinicial*taxa_crossover)/100) reproduzir();
		selecao_natural(alcance_selecao_natural);
	}while((ciclo<max_ciclo)&&(resultado_encontrado!=1)&&(stopnow!=1));

	if(resultado_encontrado==1)
	{
		a=0;
		printf("\nResultado ENCONTRADO em %d Ciclos é:\n",ciclo);
		printf("Individuo n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",a,populacao[a][12],populacao[a][0],populacao[a][1],populacao[a][2],populacao[a][3],populacao[a][4],populacao[a][5],populacao[a][6],populacao[a][7],populacao[a][8],populacao[a][9],populacao[a][10],populacao[a][11]);		
	}else{
		printf("\nResultado aproximado em %d Ciclos é:\n",ciclo);
		a=0;
		printf("Individuo n%dG%d (%d+%d)-%d=%d (SENDMORY=%d%d%d%d%d%d%d%d)\n",a,populacao[a][12],populacao[a][0],populacao[a][1],populacao[a][2],populacao[a][3],populacao[a][4],populacao[a][5],populacao[a][6],populacao[a][7],populacao[a][8],populacao[a][9],populacao[a][10],populacao[a][11]);		
	}
	if(stopnow==1)
	{
		printf("\nPrograma não aceita mais reproducoes! Geracao limite atingida!\n");
	}
	printf("\nAGPI(Anomalias Geneticas de Pais Incompativeis): %d\n",num_loop_eterno);
	printf("Deseja ver os pais incompativeis?(Y/N) ");
	char resp;	
	scanf("%c",&resp);
	if((resp=='Y')||(resp=='y')) for(a=0;a<iagpi;a++) printf("|AGPI n%d|Pai1 %d|Pai2 %d|\n",a,agpi[a][0],agpi[a][1]);
    system("pause");
}
