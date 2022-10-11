#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

// Trabalho realizado por:

// Cauê Scotti 00338597


// -----------------------------------------------------------------------------------------------------------

typedef struct tiles{       //Estrutura que representa o tabuleiro de jogo

    int *p_casa;    //Aponta para o valor da casa (NULL se zero)
    int *p_cor;     //Aponta para a cor da casa

} Matriz;


void Inclui_Elemento(int matriz[4][8], int *p_mat);
int Movimento_Matriz (int mat[4][8], int *pos_mat, int mov);
int Compara_Anterior (int mat[4][8]);
int Confere_Matriz (Matriz casas[4][4]);
void Atualiza_aux(int *p_mat, int matriz[4][8], Matriz casas[4][4], int *pos_valores,int cores[12], int menu);

int Confere_Pontuacao (int pontos);
void Salva_Pontuacao (int pontos);
void Salva_Tabuleiro (Matriz casas[4][4], int pontos, int mov);
void Carrega_Tabuleiro (int *pos_mat, int *pontos, int *mov);

int Menu_Inicial(void);
void Print_Fundo (void);
void Print_Base (void);
void Print_Pontuacoes (void);
void Print_Matriz (Matriz casas[4][4], int pontos, int mov);
int Print_Caixa (int menu);

int main(){

//  Dados que serão usados para regular as repetições e para definir os possíveis valores da matriz

    int i, j, k;
    int jogo = 1, opcao, teste, comando;
    int pontuacao = 0, movimentos = 0;
    int novo_jogo, salva_jogo, compara, testa_pontuacao;

    int val[11];
    for (teste = 1; teste <= 11; teste++)
        val[teste-1] = pow(2, teste);
    int *pont;
    pont = &val[0];

    int cor[12] = {8, 7, 6, 4, 5, 1, 3, 2, 11, 9, 12, 14};

//  Dados auxiliares para implementar todas as funções (matriz auxiliar)

    int mat_aux[4][8] = {0};    // Das colunas 1 a 4, salva a matriz atual. Das colunas 5 a 8 a matriz anterior
    int *pont_maux;
    pont_maux = &mat_aux[0][0];

//  Variável do struct, contendo a matriz de ponteiros

    Matriz var[4][4];
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++)
            var[i][j].p_casa = NULL;
    }


    _setcursortype(0);

//  O Desenvolvimento do programa começa a partir daqui

    while (jogo){   // Repete até que seja solicitado para sair

        for (i = 0; i < 4; i++){        // Quando o jogo começar, a matriz auxiliar,
            for (j = 0; j < 8; j++)     //a matriz principal, a pontuação e os movi-
               mat_aux[i][j] = 0;       //mentos devem sem iguais a zero ou NULL.
        }


        for (i = 0; i < 4; i++){
            for (j = 0; j < 4; j++)
                var[i][j].p_casa = NULL;
        }

        pontuacao = 0;
        movimentos = 0;

        // Impressão da tela inicial
        Print_Fundo ();
        opcao = Menu_Inicial();

        // A partir da opção selecionada na tela inicial, inicia um novo jogo ou o jogo salvo é carregado

        if (opcao == 49 || opcao == 50){ // Novo Jogo ou carregar um jogo salvo
            system("cls");
            Inclui_Elemento (mat_aux, pont_maux);
            Inclui_Elemento (mat_aux, pont_maux);
            Atualiza_aux (pont_maux, mat_aux, var, pont, cor, 1);

            if (opcao == 50){           // Carrega o tabuleiro salvo (2 no iniciar)
                Carrega_Tabuleiro (pont_maux, &pontuacao, &movimentos);
                Atualiza_aux (pont_maux, mat_aux, var, pont, cor, 2);
                Atualiza_aux (pont_maux, mat_aux, var, pont, cor, 1);
            }

            Print_Fundo ();         // Imprime a base do jogo para que os valores
            Print_Base();           //da matriz sejam impressos em seguida


            do {    // Enquanto ainda for possível realizar movimentos, o loop continua

                Print_Matriz(var, pontuacao, movimentos);
                comando = getch ();     // getch() recebe um caracter digitado. Como "comando" é um inteiro,
                                        //o valor retornado é o referente à tabela ASCII. Por isso os ifs a
                                        //seguir verificam valores numéricos

                if (comando == 110){            // Encerrar o jogo atual e começar um novo jogo (N = 110 em ASCII)

                    novo_jogo = Print_Caixa (0);// Confirmação

                    if (novo_jogo == 49){   //1 em ASCII é 49
                        for (i = 0; i < 4; i++){
                            for (j = 0; j < 4; j++)
                                mat_aux[i][j] = 0;
                        }
                        Inclui_Elemento (mat_aux, pont_maux);
                        Inclui_Elemento (mat_aux, pont_maux);
                        Atualiza_aux (pont_maux, mat_aux, var, pont, cor, 1);
                        movimentos = 0;
                        pontuacao = 0;
                    }
                    // Print do novo tabuleiro de jogo
                    Print_Fundo ();
                    Print_Base ();
                    Print_Matriz(var, pontuacao, movimentos);
                }

                else if (comando == 115){       // Salva o jogo atual (S = 115 em ASCII)
                    salva_jogo = Print_Caixa (1); // Confirmação
                    if (salva_jogo == 49)
                        Salva_Tabuleiro (var, pontuacao, movimentos);
                    // Print do tabuleiro de jogo para apagar a caixa de "Print_Caixa"
                    Print_Fundo ();
                    Print_Base ();
                    Print_Matriz(var, pontuacao, movimentos);
                }

                else if (comando == 27){         // Termina o jogo (ESC = 27 em ASCII)
                    teste = 0;
                    Print_Caixa (2);
                    Sleep (2000);
                    Print_Fundo ();
                }

                else if (comando == 8){          // Desfazer (BACKSPACE = 8 em ASCII)
                    Atualiza_aux (pont_maux, mat_aux, var, pont, cor, 3);
                    movimentos--;
                    pontuacao -= k;
                }

                else{                           // Realiza um movimento (Setas)

                    comando = getch();          // As setas retornam dois valores pela tabela ASCII. Sendo assim,
                                                //getch é utilizado novamente para que seja passado apenas um valor para o comando,
                                                //evitando repetições indevidas.

                    Atualiza_aux (pont_maux, mat_aux, var, pont, cor, 2);
                    k = Movimento_Matriz (mat_aux, pont_maux, comando); //Recebe o comando e realiza o movimento
                    pontuacao += k;
                    compara = Compara_Anterior (mat_aux);       // Verifica se o movimento é válido
                    if (compara == 1){                          // Se for válido, adiciona mais um valor
                        movimentos++;
                        Inclui_Elemento (mat_aux, pont_maux);
                    }
                    Atualiza_aux (pont_maux, mat_aux, var, pont, cor, 1);
                    teste = Confere_Matriz(var);                // Confere se é possível continuar o jogo (1 se sim, 0 se não)
                    if (teste == 0){
                        Print_Matriz(var, pontuacao, movimentos);
                        i = Print_Caixa (2);
                        Sleep (2000);
                        testa_pontuacao = Confere_Pontuacao (pontuacao); // Confere a pontuação adquirida e,
                        if (testa_pontuacao == 1){                       //se for maior que alguma das 10
                            Salva_Pontuacao (pontuacao);                 //maiores, salva no arquivo
                        }
                    }
                }
            } while (teste);    // O jogo termina quando "Confere_Matriz" retorna 0
        }

        else{       // Se for selecionado "sair" na tela inicial, o loop "jogo" para e fecha a janela de comandos
            jogo = 0;
            system ("taskkill /IM cb_console_runner.exe");
        }
    }

    return 0;
}

int Menu_Inicial(void){     // Realiza a impressão do menu inicial
    int i, j;

    // Linha 1 (Logo 2048)
    textbackground (12);    //Fundo vermelho
    textcolor (0);          //Letra preta
    for (j = 26; j < 90; j += 16){
        if (j == 26 || j == 42 || j == 74){
            gotoxy (j, 2);
            printf ("              ");
        }
        else if (j == 58){
            gotoxy (j, 2);
            printf ("  ");
            gotoxy (j + 12, 2);
            printf ("  ");
        }
    }

    // Linhas 2 e 3 (Logo 2048)
    for (i = 3; i < 5; i++){
        for (j = 26; j < 90; j += 16){
            if (j > 26){
                gotoxy (j, i);
                printf ("  ");
                gotoxy (j + 12, i);
                printf ("  ");
            }
            else {
                gotoxy (j + 12, i);
                printf("  ");
            }
        }
    }

    // Linha 4 (Logo 2048)
    for (j = 26; j < 90; j += 16){
        if (j == 26 || j == 74 || j == 58){
            gotoxy (j, 5);
            printf ("              ");
        }
        else if (j == 42){
            gotoxy (j, 5);
            printf ("  ");
            gotoxy (j + 12, 5);
            printf ("  ");
        }
    }

    // Linhas 5 e 6 (Logo 2048)
    for (i = 6; i < 8; i++){
        for (j = 26; j < 90; j += 16){
            if (j == 42 || j == 74){
                gotoxy (j, i);
                printf ("  ");
                gotoxy (j + 12, i);
                printf ("  ");
            }
            else if (j == 26){
                gotoxy (j, i);
                printf("  ");
            }
            else {
                gotoxy (j + 12, i);
                printf ("  ");
            }
        }
    }

    // Linha 7 (Logo 2048)
    for (j = 26; j < 90; j += 16){
        if (j == 26 || j == 42 || j == 74){
            gotoxy (j, 8);
            printf ("              ");
        }
        else if (j == 58){
            gotoxy (j + 12, 8);
            printf ("  ");
        }
    }

    // Caixa
    for (i = 14; i < 26; i++){
        if (i == 14 || i == 25){
            for (j = 26; j < 90; j++){
                gotoxy (j, i);
                printf(" ");
            }
        }
        else{
            gotoxy (26, i);
            printf ("  ");
            gotoxy (88, i);
            printf ("  ");
        }
    }

    // Escrita dentro da caixa
    textbackground (15);
    gotoxy (40, 16);
    printf ("1 - Novo Jogo");
    gotoxy (40, 19);
    printf ("2 - Carregar Jogo");
    gotoxy (40, 22);
    printf ("3 - Sair");

    gotoxy (1, 1);

    i = getch();

    return i;
}


void Inclui_Elemento (int mat[4][8], int *pos_mat){     // Inclui aleatoriamente os valores 2 ou 4 na matriz auxiliar

    int i, j, ind, tr = 1;
    int min = 0, max = 3, max_ind = 9;
    int sort[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 4};
    srand(time(NULL));
    while (tr){
      i = min + (rand() % (max - min + 1));        //Sorteia uma linha e uma coluna
      j = min + (rand() % (max - min + 1));
      ind = min + (rand() % (max_ind - min + 1));
      if (mat[i][j] == 0){                         // Se a posição da linha e da coluna sorteada tiver valor igual a zero,
        *(pos_mat + (i * 8) + j) = sort[ind];      //adiciona esse valor à matriz auxiliar. Caso contrário, sorteia de novo
        tr = 0;
      }
    }
    return;
}


int Movimento_Matriz(int mat[4][8], int *pos_mat, int mov){     // Realiza o movimento das casas

     /*A ideia do movimento é passar todas as linhas ou colunas (dependendo do movimento) para um vetor, deixar essas linhas ou
    colunas na forma desejada e após inserir na matriz principal*/

	int i, j, pos, quant, pontos = 0;
	int linha[4], linha2[4];   //Os vetores linha serão usados para armazenar e alterar os valores das linhas ou colunas e adicioná-las a matriz principal

	//Os movimentos em todas as direções estão definidos a seguir:

	if (mov == 77){
		for (i = 0; i < 4; i++){    //Comando de repetição para percorrer todas as linhas da matriz

            /* A ideia da estrutura a seguir é primeiro verificar os elementos diferentes de 0 da linha analisada, jogá-los para a direita
            e a seguir preencher o vetor linha com 0. Para tal, a variável 'pos' armazena o lugar vago mais a direita e 'quant' o índice onde
            os zeros começarão a ser incluídos para completar o vetor 'linha' */

			pos = 3;
			quant = 0;


            //Verifica todos os elementos da linha. Se != de 0, armazena em linha[pos] e executa pos-- para alcançar a casa anterior.
            //Se == 0, aumenta 'quant' para anexar 0 nos espaços em branco.

			for (j = 3; j >= 0; j--){
				if (mat[i][j] != 0){
					linha[pos] = mat[i][j];
					pos--;
				}
				else
					quant++;
			}

			//Utilizando a variável 'quant' para preencher 'linha' com 0

			for (j = quant - 1; j >= 0; j--)
				linha[j] = 0;

            //O trecho a seguir verifica o vetor 'linha' formado anteriormente e agora confere se existem valores iguais um ao lado do outro
            //Se sim, soma-os e altera o vetor

			for (j = 3; j > 0; j--){
				if (linha[j] == linha[j-1]){
					linha[j] = linha[j] + linha[j-1];
					pontos += linha[j];
					linha[j-1] = 0;
				}
			}

			//Se ocorrer alguma soma, ficam espaços em branco no vetor 'linha'.
			//Então, todo o processo é realizado novamente (agora com o valores já somados) e o resultado é armazenado em 'linha2'

			pos = 3;
			quant = 0;
			for (j = 3; j >= 0; j--){
				if (linha[j] != 0){
					linha2[pos] = linha[j];
					pos--;
				}
				else
					quant++;
			}
			for (j = quant - 1; j >= 0; j--)
				linha2[j] = 0;


			//Por fim, os valores de 'linha2' são copiados para a linha da matriz principal


			for (j = 0; j < 4; j++)
				*(pos_mat + (i * 8) + j) = linha2[j];
		}
	}

	/*O movimento a seguir é igual ao de cima, só que começando pela coluna mais a esquerda. Sendo assim, os valores de variáveis e comandos
	de repetição foram alterados para tal*/

	else if (mov == 75){
		for (i = 0; i < 4; i++){
			pos = 0;
			quant = 4;
			for (j = 0; j < 4; j++){
				if (mat[i][j] != 0){
					linha[pos] = mat[i][j];
					pos++;
				}
				else
					quant--;
			}

			for (j = quant; j < 4; j++)
				linha[j] = 0;

			for (j = 0; j < 3; j++){
				if (linha[j] == linha[j+1]){
					linha[j] = linha[j] + linha[j+1];
					pontos += linha[j];
					linha[j+1] = 0;
				}
			}

			pos = 0;
			quant = 4;
			for (j = 0; j < 4; j++){
				if (linha[j] != 0){
					linha2[pos] = linha[j];
					pos++;
				}
				else
					quant--;
			}
			for (j = quant; j < 4; j++)
				linha2[j] = 0;



			for (j = 0; j < 4; j++)
				*(pos_mat + (i * 8) + j) = linha2[j];
		}
	}

	/*O movimento a seguir é igual ao de cima, só que analizando colunas ao invés de linhas, começando por baixo. Sendo assim, os valores de
	 variáveis e comandos de repetição foram alterados para tal*/

	else if (mov == 80){
		for (j = 0; j < 4; j++){
			pos = 3;
			quant = 0;
			for (i = 3; i >= 0; i--){
				if (mat[i][j] != 0){
					linha[pos] = mat[i][j];
					pos--;
				}
				else
					quant++;
			}
			for (i = quant - 1; i >= 0; i--)
				linha[i] = 0;

			for (i = 3; i > 0; i--){
				if (linha[i] == linha[i-1]){
					linha[i] = linha[i] + linha[i-1];
					pontos += linha[i];
					linha[i-1] = 0;
				}
			}

			pos = 3;
			quant = 0;
			for (i = 3; i >= 0; i--){
				if (linha[i] != 0){
					linha2[pos] = linha[i];
					pos--;
				}
				else
					quant++;
			}
			for (i = quant - 1; i >= 0; i--)
				linha2[i] = 0;



			for (i = 0; i < 4; i++)
				*(pos_mat + (i * 8) + j) = linha2[i];
		}
	}

	/*O movimento a seguir é igual ao de cima, só que analizando colunas ao invés de linhas, começando por cima. Sendo assim, os valores de
	variáveis e comandos de repetição foram alterados para tal*/

	else if (mov == 72){
		for (j = 0; j < 4; j++){
			pos = 0;
			quant = 4;
			for (i = 0; i < 4; i++){
				if (mat[i][j] != 0){
					linha[pos] = mat[i][j];
					pos++;
				}
				else
					quant--;
			}

			for (i = quant; i < 4; i++)
				linha[i] = 0;

			for (i = 0; i < 3; i++){
				if (linha[i] == linha[i+1]){
					linha[i] = linha[i] + linha[i+1];
					pontos += linha[i];
					linha[i+1] = 0;
				}
			}

			pos = 0;
			quant = 4;
			for (i = 0; i < 4; i++){
				if (linha[i] != 0){
					linha2[pos] = linha[i];
					pos++;
				}
				else
					quant--;
			}
			for (i = quant; i < 4; i++)
				linha2[i] = 0;

			for (i = 0; i < 4; i++)
				*(pos_mat + (i * 8) + j) = linha2[i];
		}
	}

	return pontos;
}


int Compara_Anterior (int mat[4][8]){       // Verifica se o movimento é valido

    int i, j, comp = 0;

    for (i = 0; i < 4; i ++){
        for (j = 0; j < 4; j++){
            if (mat[i][j] != mat[i][j + 4])     // Compara as colunas 1 a 4 da matriz auxiliar com as colunas 5 a 8
                comp = 1;                       // Se algum valor for diferente, então o movimento é valido
        }
    }

    return comp;
}


int Confere_Matriz (Matriz casas[4][4]){    // Confere se o jogo foi terminado

    int i, j, teste = 0;

    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            if (casas[i][j].p_casa == NULL)     // Se ainda existir um espaço vago, então é possível continuar o jogo
                teste = 1;
        }
    }

    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            if (casas[i][j].p_casa != NULL){
                if (*(casas[i][j].p_casa) == 2048)    // Se o valor 2048 for atingido, o jogo encerra
                    teste = 0;
            }
        }
    }

    return teste;
}


void Atualiza_aux(int *pos_mat, int mat[4][8], Matriz casas[4][4], int *pos_valores,int cores[12], int menu){   // Atualiza os valores da matriz auxiliar e da matriz principal

    int i, j;

    if (menu == 1){                           // Passa os valores da matriz auxiliar para a matriz principal
        int num;
        for (i = 0; i < 4; i++){
            for (j = 0; j < 4; j++){
                if (mat[i][j] == 0){
                    casas[i][j].p_casa = NULL;
                    casas[i][j].p_cor = &(cores[0]);
                }
                else if (mat[i][j] == 8){
                    casas[i][j].p_casa = pos_valores + 2;
                    casas[i][j].p_cor = &(cores[3]);
                }
                else if (mat[i][j] == 64){
                    casas[i][j].p_casa = pos_valores + 5;
                    casas[i][j].p_cor = &(cores[6]);
                }
                else if (mat[i][j] == 128){
                    casas[i][j].p_casa = pos_valores + 6;
                    casas[i][j].p_cor = &(cores[7]);
                }
                else{
                    num = log10(mat[i][j]) / log10(2);              // Como os possíveis valores são pontências de 2, o índice do vetor "val" - 1
                    casas[i][j].p_casa = pos_valores + (num - 1);   //retorna a potencia de 2^(indice - 1). Logo, é possível usar log de base 2
                    casas[i][j].p_cor = &(cores[num]);              //para obter o índice desejado
                }
            }
        }
    }

    else if (menu == 2){                        // Passa o estado atual do jogo para as colunas 5 a 8 da matriz auxiliar
        for (i = 0; i < 4; i++){
            for (j = 0; j < 4; j++)
                *(pos_mat + (i * 8) + (j + 4)) = *(pos_mat + (i * 8) + j);
        }
    }


    else if (menu == 3){                        // Desfaz a jogada, passando os valores das colunas 5 a 8 da matriz
        int num;                                //auxiliar para as colunas 1 a 4 da mesma e para a matriz de ponteiros
        for (i = 0; i < 4; i++){                //de struct
            for (j = 0; j < 4; j++){
                mat[i][j] = mat[i][j + 4];
                if (mat[i][j + 4] == 0){
                    casas[i][j].p_casa = NULL;
                    casas[i][j].p_cor = &cores[0];
                }
                else if (mat[i][j + 4] == 8){
                    casas[i][j].p_casa = pos_valores + 2;
                    casas[i][j].p_cor = &(cores[3]);
                }
                else if (mat[i][j + 4] == 64){
                    casas[i][j].p_casa = pos_valores + 5;
                    casas[i][j].p_cor = &(cores[6]);
                }
                else if (mat[i][j + 4] == 128){
                    casas[i][j].p_casa = pos_valores + 6;
                    casas[i][j].p_cor = &(cores[7]);
                }

                else{
                    num = log10(mat[i][j + 4]) / log10(2);
                    casas[i][j].p_casa = pos_valores + (num - 1);
                    casas[i][j].p_cor = &(cores[num]);
                }
            }
        }
    }

    return;
}


int Confere_Pontuacao (int pontos){     // Confere se a pontuação ficou entre as 10 melhores já registradas

    int teste = 0;
    int cont = 0, num;
    char linha[30];
    char *p;

    FILE *arq = fopen ("pontuacoes.txt", "r");

    if (arq == NULL){       // Se o arquivo não existir, então salva de qualquer forma
        teste = 1;
        return teste;
    }

    else{
        while (cont < 10 && !feof(arq)){
            fgets (linha, sizeof(linha), arq);
            fprintf (arq, "%s", linha);
            p = strtok (linha, "|");        // Utiliza a tokenização para obter os valores das pontuações, que ficam após o
            p = strtok (NULL, "\0");        //caracter '|' e o terminador
            num = atoi (p);
            if (pontos > num || feof (arq)) // Compara a pontuação atual com as pontuações do arquivo (retorna 1 se for maior)
                teste = 1;
            cont++;
        }
    }

    fclose(arq);

    return teste;
}


void Salva_Pontuacao (int pontos){      // Salva a pontuação no arquivo texto

    int i = 0, j;
    char nome[20];
    int teste;
    int num, cont = 0;
    char linha [30], linha2[30];
    char *p;


    fflush(stdin);                                                                  // Solicita ao usuário o nome para salvar a pontuação
    gotoxy (50, 15);                                                                //e o padroniza, para que o arquivo mantenha o mesmo
    printf("Digite o nome para salvar a pontuacao: ");                              //tamanho para cada linha
    gotoxy (58, 16);
    fgets (nome, 20, stdin);
    while (isalpha(nome[i]) != 0 || isdigit(nome[i]) != 0 || nome[i] == ' ')
       i++;
    char nome_real[21];
    for (j = 0; j < 20; j++){
        if (j < i)
            nome_real[j] = nome[j];
        else
            nome_real[j] = ' ';
    }
    nome_real[20] = '\0';


    FILE *arq = fopen ("pontuacoes.txt", "r");      // Arquivo principal
    FILE *arq2 = fopen ("pontuacoes_aux.txt", "w"); // Arquivo auxiliar

    if (arq == NULL){   // Se não existir um arquivo principal, o cria e salva a pontuação atual
        fclose (arq);
        arq = fopen ("pontuacoes.txt", "w");
        fprintf(arq, "%s |%d\n", nome_real, pontos);
    }

    else{
        teste = 1;
        while (cont < 10 && !feof(arq)){    // Como cont deve ser menor que 10, serão escritas 10 linha no máximo no arquivo auxiliar
            fgets (linha, sizeof(linha), arq);
            for (i = 0; i <29; i++){    // Copia o vetor linha em linha2 para salvar no arquivo
                linha2[i] = linha[i];
            }
            linha2[29] = '\0';
            p = strtok (linha, "|");    // Confere os valores salvos e compara-os com a pontuação atual, como em "Confere_Pontuação"
            p = strtok (NULL, "\0");
            num = atoi (p);
            if (pontos > num && teste){
                fprintf(arq2, "%s |%d\n", nome_real, pontos);   // Se a pontuação atual for maior que a salva, escreve o nome informado
                teste = 0;                                      //pelo usuário e a pontuação no arquivo auxiliar
                cont++;
            }
            fprintf (arq2, "%s", linha2);   // Escreve a linha analisada no arquivo auxiliar
            cont++;
        }
        fclose (arq);
        fclose (arq2);

        arq = fopen ("pontuacoes.txt", "w");
        arq2 = fopen ("pontuacoes_aux.txt", "r");

        fgets (linha, 30, arq2);            // Por fim, passa todos os valores do arquivo auxiliar para o arquivo principal
        while (!feof(arq2)){
            fprintf (arq, "%s", linha);
            fgets (linha, 30, arq2);
        }
    }

    fclose (arq);
    fclose (arq2);

    return;
}


void Salva_Tabuleiro (Matriz casas[4][4], int pontos, int mov){     //Salva o jogo atual em um arquivo binário

    int i, j;
    int tiles[16];

    for (i = 0; i < 4; i++){                                    // Passa todos os valores apontados pela matriz de ponteiros
        for (j = 0; j < 4; j++){                                //para um vetor "tiles"
            if (casas[i][j].p_casa == NULL)
                tiles[(i * 4) + j] = 0;
            else
                tiles[(i * 4) + j] = *(casas[i][j].p_casa);
        }
    }

    FILE *arq = fopen ("ultimo_tabuleiro.bin", "w");
    fwrite (&tiles, sizeof(int), 16, arq);      //Salva todos os valores de "tiles" no arquivo binário
    fwrite (&pontos, sizeof(int), 1, arq);      //Salva a pontuação no arquivo
    fwrite (&mov, sizeof(int), 1, arq);         //Salva a quantidade de movimentos no arquivo
    fclose (arq);

    return;
}


void Carrega_Tabuleiro (int *pos_mat, int *pontos, int *mov){       //Carrega o jogo salvo

    int i, j;

    FILE *arq = fopen ("ultimo_tabuleiro.bin", "r");
    for (i = 0; i < 4; i++){                                        // Passa primeiramente os valores de cada casa salvos
        for (j = 0; j < 4; j++)                                     //para a matriz auxiliar
            fread (pos_mat + (i * 8) + j, sizeof(int), 1, arq);
    }
    fread (pontos, sizeof(int), 1, arq);    // Passa a pontuação salva para a pontuação atual
    fread (mov, sizeof(int), 1, arq);       // Passa a quantidade de movimentos salva para a quantidade de movimentos atual
    fclose (arq);

    return;
}


void Print_Fundo (void){        // Imprime a cor de fundo do jogo

    int i, j;

    textbackground (15);

    for (i = 0; i < 48; i++){       // Para que o fundo seja impresso, uma cor é selecionada
        for (j = 0; j < 120; j++){  //e vários caracteres ' ' são impressos, deixando assim
            gotoxy (j, i);          //apenas a cor selecionada sem nenhum outro tipo de caractere
            printf(" ");
        }
    }

    return;
}


void Print_Base (void){     // Imprime os elementos fixos na tela de jogo

    int i, j;

    textcolor (8);
    textbackground(15);

    gotoxy(30, 1);
    printf("2048");

    //Imprime as instruções laterais
    gotoxy (68, 15);
    printf("Melhores pontuacoes:");
    Print_Pontuacoes ();
    gotoxy (68, 24);
    printf ("Setas - Movimento");
    gotoxy (68, 25);
    printf ("N - Novo jogo");
    gotoxy (68, 26);
    printf ("S - Salvar jogo");
    gotoxy (68, 27);
    printf ("BACKSPACE - Desfazer");
    gotoxy (68, 28);
    printf ("ESC - Sair");

    //Imprime as escritas "Pontuacao:" e "Movimentos:" no canto superior direito
    gotoxy (68, 4);
    printf ("Pontuacao:");
    gotoxy (68, 5);
    printf ("Movimentos:");

    textbackground (0);
    // Linhas horizontais do tabuleiro
    for (i = 4; i < 29; i += 6){
        for (j = 8; j < 57; j++){
            gotoxy (j, i);
            printf(" ");
        }
    }
    // Linhas verticais do tabuleiro
    for (i = 4; i < 29; i++){
        for (j = 8; j < 58; j += 12){
            gotoxy (j, i);
            printf(" ");
        }
    }

    return;
}


void Print_Matriz (Matriz casas[4][4], int pontos, int mov){        // Imprime retangulos dentro do tabuleiro que contém a cor e o valor de casa casa

    int i, j;

    // Print dos valores da matriz
    textcolor(0);
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            if (casas[i][j].p_casa == NULL){                // Caso o ponteiro da matriz analisado não aponte para
                textbackground (*(casas[i][j].p_cor));      //valor nenhum, será impresso no tabuleiro um retângulo apenas
                gotoxy ((9 + (12 * j)),(5 + (6 * i)));      //com a cor, sem nenhum dígito dentro
                printf("           ");
                gotoxy ((9 + (12 * j)),(6 + (6 * i)));
                printf("           ");
                gotoxy ((9 + (12 * j)),(7 + (6 * i)));
                printf("           ");
                gotoxy ((9 + (12 * j)),(8 + (6 * i)));
                printf("           ");
                gotoxy ((9 + (12 * j)),(9 + (6 * i)));
                printf("           ");
            }
            else{
                textbackground (*(casas[i][j].p_cor));          // Caso o ponteiro da matriz analisado aponte para
                gotoxy ((9 + (12 * j)),(5 + (6 * i)));          //algum valor, será impresso no tabuleiro um retângulo
                printf("           ");                          //com a cor e o valor correspondente
                gotoxy ((9 + (12 * j)),(6 + (6 * i)));
                printf("           ");
                gotoxy ((9 + (12 * j)),(7 + (6 * i)));
                printf("  %4d     ", *(casas[i][j].p_casa));
                gotoxy ((9 + (12 * j)),(8 + (6 * i)));
                printf("           ");
                gotoxy ((9 + (12 * j)),(9 + (6 * i)));
                printf("           ");
            }
        }
    }

    // Print da pontuação e dos movimentos

    textcolor (8);
    textbackground (15);
    gotoxy (79, 4);
    printf ("%d", pontos);
    gotoxy (80, 5);
    printf ("%d", mov);

    gotoxy (1, 1);

    return;
}


int Print_Caixa (int menu){     // Imprime uma caixa que confirma opções do usuário ou informa que o jogo foi encerrado

    int i, j;
    int escolha;

    // Desenha as bordas da caixa
    textbackground (12);
    for (i = 11; i < 20; i += 8){
        for (j = 42; j < 91; j++){
            gotoxy (j, i);
            printf (" ");
        }
    }

    for (i = 11; i < 20; i++){
        for (j = 42; j < 91; j += 48){
            gotoxy (j, i);
            printf(" ");
        }
    }

    // Limpa o interior da caixa
    textbackground (15);
    for (i = 12; i < 19; i++){
        for (j = 43; j < 90; j++){
            gotoxy (j, i);
            printf(" ");
        }
    }

    if (menu == 0){         // Confirmação do novo jogo
        gotoxy(50, 14);
        printf("Deseja mesmo comecar um novo jogo?");
        gotoxy(57, 16);
        printf ("1 - Sim    2 - Nao");

        gotoxy (1, 1);

        escolha = getch ();
    }

    else if (menu == 1){    // Confirmação para salvar o jogo
        gotoxy(50, 14);
        printf("Deseja mesmo salvar o jogo atual?");
        gotoxy(57, 16);
        printf ("1 - Sim    2 - Nao");

        gotoxy (1, 1);

        escolha = getch ();
    }

    else{             // Informa que o jogo foi encerrado
        gotoxy (60, 13);
        printf ("Jogo encerrado!");
        gotoxy (1, 1);
    }


    if (menu == 1 && escolha == 49){     // Informa que o jogo foi salvo

        // Limpa a caixa
        textbackground (15);
        for (i = 12; i < 19; i++){
            for (j = 43; j < 89; j++){
                gotoxy (j, i);
                printf(" ");
            }
        }

        gotoxy(58, 14);
        printf("Jogo Salvo!");
        gotoxy (1, 1);
        Sleep (1500);

    }

    return escolha;
}


void Print_Pontuacoes(void){    // Imprime os três maiores valores salvos no canto direito da tela de jogo

    int i = 0;
    char carac;

    FILE *arq = fopen ("pontuacoes.txt", "r");

    if (arq != NULL){

        gotoxy (68, 16);
        while (i < 3 && !feof(arq)){        // A impressão é feita tal como está no arquivo. Sendo assim,
            carac = fgetc (arq);            //a função lê caractere por caractere e o imprime na tela
            printf("%c", carac);
            if (carac == '\n'){
                i++;
                gotoxy (68, 16 + i);
            }
        }
        fclose (arq);
    }

    return;
}
