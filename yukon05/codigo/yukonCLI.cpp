#include "yukonCLI.h"

YUKON_CLI::YUKON_CLI()
	: YUKON()
{
	
}

int YUKON_CLI::run(){
	
	int estado = 1;
	string comando;
	string mensagem = "Jogo começou";
	
	do {	//logica principal
	

		cout << mensagem << endl;
		imprimeCartas();
		cout << endl << ">> ";
		cin >> comando;
		system("clear");
		switch( interpretar(comando) ){
			case 0:
				estado=1;
				mensagem = "Movendo...";
				break;
			case 1:
				estado=2;
				mensagem = "Erro de interpretação.";
				break;
			case 3:
				estado=0;
				cout << "Fechando jogo..." << endl;
				break;
			case 4:
				estado=1;
				mensagem = "Gerando novo jogo.";
				break;
		}
		if (verificaFimJogo()){
			cout << "Você ganhou !!!\tFim do jogo.\tEnter para fechar." << endl;
			cin.ignore();
			while(cin.get()!='\n'){
				cout << endl;
			}
			estado=0;
		}
	} while (estado != 0);
	
	return 0;
}



void YUKON_CLI::imprimeCartas(){
	bool ok;
	
	//impressão dos número de colunas
	cout << "\t";
	for (int cnt=0;cnt<7;cnt++){
		cout << "__" << cnt << "__" << "\t";
	}
	cout << endl;
	
	int i,j;
	MONTE * monte;
	
	for(i=0;i<getMaxMonteTam();i++){
		cout << "  " << i; // numeracao de linhas
		if (i<10)
			cout << " ";
		cout << "|" << "\t";
		for(j=0;j<=6;j++){
			monte = getMonte(j);
			if(monte->getTamanho() >= i+1)
				cout /*<< i*/ << *monte->getCarta(i,ok) << "\t";
			else
				cout << "\t";
		}
		cout << "| " << i << endl;
	}
	
/*	// montes em horizontal
	cout << "---\n";
	for(j=0;j<6;j++){
		monte = getMonte(j);
		cout << monte->getTamanho();
		for(i=0;i<monte->getTamanho();i++){
			cout << " |" << i <<  *monte->getCarta(i,ok);
		}
		cout << endl;
	}
*/
	cout << "---\n";
	FUNDACAO * fundacao = getFundacao();
	if(fundacao->getMonte(0) == 0)
		cout << "[♡,  ] ";
	else
		cout << "[♡, " << fundacao->getMonte(0) << "] ";
	if(fundacao->getMonte(1) == 0)
		cout << "[♢,  ] ";
	else
		cout << "[♢, " << fundacao->getMonte(1) << "] ";
	if(fundacao->getMonte(2) == 0)
		cout << "[♠,  ] ";
	else
		cout << "[♠, " << fundacao->getMonte(2) << "] ";
	if(fundacao->getMonte(3) == 0)
		cout << "[♣  ] ";
	else
		cout << "[♣, " << fundacao->getMonte(3) << "] ";
	
	cout << endl;
}

/*
 * m *over
 * ajuda/sobre
 */

//interpretacao e execucao de comando
//forma <pilha_origem>,<posicao_origem>,<pilha_destino>, em numero
int YUKON_CLI::interpretar(string cmd){
	
	bool ok;
	
	if (cmd == ""){
		if (verificaFimJogo())
			return 3;
		return 2;
	}
	
	if(cmd=="novo"||cmd=="novojogo"){
		YUKON::novoJogo();
		return 4;
	}
	
	if (cmd=="fechar")
		return 3;
	
	int origemp;	//pilha de origem
	int origemc;	//carta de origem
	int destino;	//pilha de destino
	
	unsigned int posvirg1 = cmd.find(",");	//posicao da 1a virgula
	unsigned int posvirg2 = cmd.rfind(",");	//posicao da 2a (ultima) virgula
	if ((posvirg1 != string::npos)&&(posvirg2 != string::npos)){	//se foi obtido posicao das duas virgulas, faca:
		origemp = atoi((cmd.substr(0,posvirg1)).data());
		origemc = atoi((cmd.substr(posvirg1+1,posvirg2-posvirg1)).data());
		destino = atoi((cmd.substr(posvirg2+1,cmd.size()-posvirg2)).data());
		
	}
	else{
		cout << "formato de comando incorreto!" << endl;
		return 1;
	}
	
	if(destino==8)
		cout <<"Movendo carta "<<origemc<<" da pilha "<<origemp<<" para a fundamento " << endl;
	else
		cout <<"Movendo carta "<<origemc<<" da pilha "<<origemp<<" para a pilha "<<destino << endl;
	
	if (origemp>7||origemp<0||destino>7||destino<0) {
		cout << "origem ou destino e incorreto!" << endl;
		return 1;
	}
	
	MONTE * monteOrigem = getMonte(origemp);
	
	if (monteOrigem->getTamanho()<0) // ??????????????? 0 é vazio ou tem 1 elemento ??????????????????
		cout << "monte nao possui nenhuma carta!" << endl;
	if(origemp == destino)
		cout<< "Coluna de origem igual coluna de destino" << endl;
	else
		if (destino==7){
			fundacao.receberCarta(monteOrigem, ok);
			if(!ok)
				cout << "Esse movimento não é possível." <<  endl;
		} else {
			MONTE * monteDestino = getMonte(destino);
			monteDestino->receberCartas(monteOrigem, origemc, ok);
			if(!ok)
				cout << "Esse movimento nao é possivel."<<endl;
		}
		
		/*cout << "pos1=" << posvirg1 << endl;
		 * cout << "pos2=" << posvirg2 << endl;*/
		return 0;
}
